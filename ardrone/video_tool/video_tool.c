

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

static void start_capturing(void);
static void stop_capturing(void);




#define FORCED_WIDTH  640
#define FORCED_HEIGHT 480
#define FORCED_FORMAT V4L2_PIX_FMT_YUYV
#define FORCED_FIELD  V4L2_FIELD_ANY

static int verbose = 0;
#define pr_debug(fmt, arg...) \
	if (verbose) fprintf(stderr, fmt, ##arg)

#define CLEAR(x) memset(&(x), 0, sizeof(x))

enum io_method {
	IO_METHOD_READ,
	IO_METHOD_MMAP,
	IO_METHOD_USERPTR,
};

struct buffer {
	void   *start;
	size_t  length;
};

static char            *dev_name;
static enum io_method   io = IO_METHOD_MMAP;
static int              fd = -1;
struct buffer          *buffers;
static unsigned int     n_buffers;
static int		out_buf;
static int              force_format;
static int              frame_count = 10;

static void errno_exit(const char *s)
{
	fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
	exit(EXIT_FAILURE);
}

static int xioctl(int fh, int request, void *arg)
{
	int r;

	do {
		r = ioctl(fh, request, arg);
	} while (-1 == r && EINTR == errno);

	return r;
}


static void process_image(const void *p, int size)
{
	pr_debug("%s: called! size=%d\n", __func__,size);



	FILE * pFile;

	if(size==614400){
		printf("sendto udp camera-horizontal \n");
		pFile = fopen ("/tmp/horizontal.yuv", "wb");
		fwrite (p , 1, size, pFile);
		fclose (pFile);

		//system("/data/video/udp_broadcast_file 7070 /tmp/horizontal.yuv");
	}

	if(size==50688){
		printf("sendto udp camera-vertical \n");
		pFile = fopen ("/tmp/vertical.yuv", "wb");
		fwrite (p , 1, size, pFile);
		fclose (pFile);

		//system("/data/video/udp_broadcast_file 6969 /tmp/vertical.yuv");
	}



	//if (out_buf)
	//	fwrite(p, size, 1, stdout);



	fflush(stderr);
	fprintf(stderr, ".");
	fflush(stdout);


}

static int read_frame(void)
{
	struct v4l2_buffer buf;
	unsigned int i;

	pr_debug("%s: called!\n", __func__);

	switch (io) {
	case IO_METHOD_READ:
		if (-1 == read(fd, buffers[0].start, buffers[0].length)) {
			switch (errno) {
			case EAGAIN:
				return 0;

			case EIO:
				/* Could ignore EIO, see spec. */

				/* fall through */

			default:
				errno_exit("read");
			}
		}

		process_image(buffers[0].start, buffers[0].length);
		break;

	case IO_METHOD_MMAP:
		CLEAR(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;

		if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
			switch (errno) {
			case EAGAIN:
				return 0;

			case EIO:
				/* Could ignore EIO, see spec. */

				/* fall through */

			default:
				errno_exit("VIDIOC_DQBUF");
			}
		}

		assert(buf.index < n_buffers);

		//stop_capturing();
		process_image(buffers[buf.index].start, buf.bytesused);
		//start_capturing();


		if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
			errno_exit("VIDIOC_QBUF");
		break;

	case IO_METHOD_USERPTR:
		CLEAR(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_USERPTR;

		if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
			switch (errno) {
			case EAGAIN:
				return 0;

			case EIO:
				/* Could ignore EIO, see spec. */

				/* fall through */

			default:
				errno_exit("VIDIOC_DQBUF");
			}
		}

		for (i = 0; i < n_buffers; ++i)
			if (buf.m.userptr == (unsigned long)buffers[i].start
			    && buf.length == buffers[i].length)
				break;

		assert(i < n_buffers);

		process_image((void *)buf.m.userptr, buf.bytesused);

		if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
			errno_exit("VIDIOC_QBUF");
		break;
	}

	return 1;
}

static void mainloop(void)
{
	unsigned int count;

	pr_debug("%s: called!\n", __func__);

	count = frame_count;

	while ((count-- > 0)||(frame_count<=0)) {

		for (;;) {
			fd_set fds;
			struct timeval tv;
			int r;

			FD_ZERO(&fds);
			FD_SET(fd, &fds);

			/* Timeout. */
			tv.tv_sec = 2;
			tv.tv_usec = 0;

			r = select(fd + 1, &fds, NULL, NULL, &tv);

			if (-1 == r) {
				if (EINTR == errno)
					continue;
				errno_exit("select");
			}

			if (0 == r) {
				fprintf(stderr, "select timeout\n");
				//exit(EXIT_FAILURE);
				continue;
			}

			if (read_frame())
				break;
			/* EAGAIN - continue select loop. */
		}
	}
}

static void stop_capturing(void)
{
	enum v4l2_buf_type type;

	pr_debug("%s: called!\n", __func__);

	switch (io) {
	case IO_METHOD_READ:
		/* Nothing to do. */
		break;

	case IO_METHOD_MMAP:
	case IO_METHOD_USERPTR:
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
			errno_exit("VIDIOC_STREAMOFF");
		break;
	}
}




static void start_capturing(void)
{
	unsigned int i;
	enum v4l2_buf_type type;
	int err;

	pr_debug("%s: called!\n", __func__);

	pr_debug("\tn_buffers: %d\n", n_buffers);

	switch (io) {
	case IO_METHOD_READ:
		/* Nothing to do. */
		break;

	case IO_METHOD_MMAP:
		for (i = 0; i < n_buffers; ++i) {
			struct v4l2_buffer buf;

			pr_debug("\ti: %d\n", i);

			CLEAR(buf);
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_MMAP;
			buf.index = i;

			pr_debug("\tbuf.index: %d\n", buf.index);

			err == xioctl(fd, VIDIOC_QBUF, &buf);
			pr_debug("\terr: %d\n", err);

			if (-1 == err)
				errno_exit("VIDIOC_QBUF");

			pr_debug("\tbuffer queued!\n");
		}

		pr_debug("Before STREAMON\n");
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
			errno_exit("VIDIOC_STREAMON");
		pr_debug("After STREAMON\n");
		break;

	case IO_METHOD_USERPTR:
		for (i = 0; i < n_buffers; ++i) {
			struct v4l2_buffer buf;

			CLEAR(buf);
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_USERPTR;
			buf.index = i;
			buf.m.userptr = (unsigned long)buffers[i].start;
			buf.length = buffers[i].length;

			if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
				errno_exit("VIDIOC_QBUF");
		}
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
			errno_exit("VIDIOC_STREAMON");
		break;
	}
}

static void uninit_device(void)
{
	unsigned int i;

	pr_debug("%s: called!\n", __func__);

	switch (io) {
	case IO_METHOD_READ:
		free(buffers[0].start);
		break;

	case IO_METHOD_MMAP:
		for (i = 0; i < n_buffers; ++i)
			if (-1 == munmap(buffers[i].start, buffers[i].length))
				errno_exit("munmap");
		break;

	case IO_METHOD_USERPTR:
		for (i = 0; i < n_buffers; ++i)
			free(buffers[i].start);
		break;
	}

	free(buffers);
}

static void init_read(unsigned int buffer_size)
{
	pr_debug("%s: called!\n", __func__);

	buffers = calloc(1, sizeof(*buffers));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	buffers[0].length = buffer_size;
	buffers[0].start = malloc(buffer_size);

	if (!buffers[0].start) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}
}

static void init_mmap(void)
{
	struct v4l2_requestbuffers req;

	pr_debug("%s: called!\n", __func__);

	CLEAR(req);

	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support "
				 "memory mapping\n", dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_REQBUFS");
		}
	}
	pr_debug("\treq.count: %d\n", req.count);
	pr_debug("\treq.type: %d\n", req.type);
	pr_debug("\treq.memory: %d\n", req.memory);
	pr_debug("\n");


	if (req.count < 2) {
		fprintf(stderr, "Insufficient buffer memory on %s\n",
			 dev_name);
		exit(EXIT_FAILURE);
	}

	buffers = calloc(req.count, sizeof(*buffers));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		struct v4l2_buffer buf;

		CLEAR(buf);

		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = n_buffers;

		if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
			errno_exit("VIDIOC_QUERYBUF");

		pr_debug("\tbuf.index: %d\n", buf.index);
		pr_debug("\tbuf.type: %d\n", buf.type);
		pr_debug("\tbuf.bytesused: %d\n", buf.bytesused);
		pr_debug("\tbuf.flags: %d\n", buf.flags);
		pr_debug("\tbuf.field: %d\n", buf.field);
		pr_debug("\tbuf.timestamp.tv_sec: %ld\n", (long) buf.timestamp.tv_sec);
		pr_debug("\tbuf.timestamp.tv_usec: %ld\n", (long) buf.timestamp.tv_usec);
		pr_debug("\tbuf.timecode.type: %d\n", buf.timecode.type);
		pr_debug("\tbuf.timecode.flags: %d\n", buf.timecode.flags);
		pr_debug("\tbuf.timecode.frames: %d\n", buf.timecode.frames);
		pr_debug("\tbuf.timecode.seconds: %d\n", buf.timecode.seconds);
		pr_debug("\tbuf.timecode.minutes: %d\n", buf.timecode.minutes);
		pr_debug("\tbuf.timecode.hours: %d\n", buf.timecode.hours);
		pr_debug("\tbuf.timecode.userbits: %d,%d,%d,%d\n",
				buf.timecode.userbits[0],
				buf.timecode.userbits[1],
				buf.timecode.userbits[2],
				buf.timecode.userbits[3]);
		pr_debug("\tbuf.sequence: %d\n", buf.sequence);
		pr_debug("\tbuf.memory: %d\n", buf.memory);
		pr_debug("\tbuf.m.offset: %d\n", buf.m.offset);
		pr_debug("\tbuf.length: %d\n", buf.length);
		pr_debug("\tbuf.input: %d\n", buf.input);
		pr_debug("\n");

		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start =
			mmap(NULL /* start anywhere */,
			      buf.length,
			      PROT_READ | PROT_WRITE /* required */,
			      MAP_SHARED /* recommended */,
			      fd, buf.m.offset);

		if (MAP_FAILED == buffers[n_buffers].start)
			errno_exit("mmap");
	}
}

static void init_userp(unsigned int buffer_size)
{
	struct v4l2_requestbuffers req;

	pr_debug("%s: called!\n", __func__);

	CLEAR(req);

	req.count  = 4;
	req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_USERPTR;

	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support "
				 "user pointer i/o\n", dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_REQBUFS");
		}
	}

	buffers = calloc(4, sizeof(*buffers));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (n_buffers = 0; n_buffers < 4; ++n_buffers) {
		buffers[n_buffers].length = buffer_size;
		buffers[n_buffers].start = malloc(buffer_size);

		if (!buffers[n_buffers].start) {
			fprintf(stderr, "Out of memory\n");
			exit(EXIT_FAILURE);
		}
	}
}

static void init_device(void)
{
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	unsigned int min;

	pr_debug("%s: called!\n", __func__);

	if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s is no V4L2 device\n",
				 dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_QUERYCAP");
		}
	}

	pr_debug("\tdriver: %s\n"
		 "\tcard: %s \n"
		 "\tbus_info: %s\n",
			cap.driver, cap.card, cap.bus_info);
	pr_debug("\tversion: %u.%u.%u\n",
			(cap.version >> 16) & 0xFF,
			(cap.version >> 8) & 0xFF,
			cap.version & 0xFF);
	pr_debug("\tcapabilities: 0x%08x\n", cap.capabilities);

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "%s is no video capture device\n",
			 dev_name);
		exit(EXIT_FAILURE);
	}

	switch (io) {
	case IO_METHOD_READ:
		if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
			fprintf(stderr, "%s does not support read i/o\n",
				 dev_name);
			exit(EXIT_FAILURE);
		}
		break;

	case IO_METHOD_MMAP:
	case IO_METHOD_USERPTR:
		if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
			fprintf(stderr, "%s does not support streaming i/o\n",
				 dev_name);
			exit(EXIT_FAILURE);
		}
		break;
	}


	/* Select video input, video standard and tune here. */


	CLEAR(cropcap);

	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
		pr_debug("\tcropcap.type: %d\n", cropcap.type);
		pr_debug("\tcropcap.bounds.left: %d\n", cropcap.bounds.left);
		pr_debug("\tcropcap.bounds.top: %d\n", cropcap.bounds.top);
		pr_debug("\tcropcap.bounds.width: %d\n", cropcap.bounds.width);
		pr_debug("\tcropcap.bounds.height: %d\n", cropcap.bounds.height);

		pr_debug("\tcropcap.defrect.left: %d\n", cropcap.defrect.left);
		pr_debug("\tcropcap.defrect.top: %d\n", cropcap.defrect.top);
		pr_debug("\tcropcap.defrect.width: %d\n", cropcap.defrect.width);
		pr_debug("\tcropcap.defrect.height: %d\n", cropcap.defrect.height);

		pr_debug("\tcropcap.pixelaspect.numerator: %d\n", cropcap.pixelaspect.numerator);
		pr_debug("\tcropcap.pixelaspect.denominator: %d\n", cropcap.pixelaspect.denominator);
		pr_debug("\n");

		CLEAR(crop);
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c = cropcap.defrect; /* reset to default */

		if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
			switch (errno) {
			case EINVAL:
				/* Cropping not supported. */
				break;
			default:
				/* Errors ignored. */
				pr_debug("\tcropping not supported\n");
				break;
			}
		}
	} else {
		/* Errors ignored. */
	}


	CLEAR(fmt);

	if(strstr("p6_camif.0",cap.driver)){
		printf("camera-horizontal\n");
		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
		fmt.fmt.pix.width=640;
		fmt.fmt.pix.height=480;
	} else {
		printf("camera-vertical\n");
		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
		fmt.fmt.pix.width=176;
		fmt.fmt.pix.height=144;
	}

/*
	if (force_format) {
		fmt.fmt.pix.width       = FORCED_WIDTH;
		fmt.fmt.pix.height      = FORCED_HEIGHT;
		fmt.fmt.pix.pixelformat = FORCED_FORMAT;
		fmt.fmt.pix.field       = FORCED_FIELD;
*/
		pr_debug("\tfmt.fmt.pix.pixelformat: %c,%c,%c,%c\n",
				fmt.fmt.pix.pixelformat & 0xFF,
				(fmt.fmt.pix.pixelformat >> 8) & 0xFF,
				(fmt.fmt.pix.pixelformat >> 16) & 0xFF,
				(fmt.fmt.pix.pixelformat >> 24) & 0xFF
				);
		pr_debug("\n");

		if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
			errno_exit("VIDIOC_S_FMT");

		/* Note VIDIOC_S_FMT may change width and height. */
	//} else {
		/* Preserve original settings as set by v4l2-ctl for example */
		/*
		if (-1 == xioctl(fd, VIDIOC_G_FMT, &fmt))
			errno_exit("VIDIOC_G_FMT");

		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		pr_debug("\tfmt.fmt.pix.pixelformat: %c,%c,%c,%c\n",
				fmt.fmt.pix.pixelformat & 0xFF,
				(fmt.fmt.pix.pixelformat >> 8) & 0xFF,
				(fmt.fmt.pix.pixelformat >> 16) & 0xFF,
				(fmt.fmt.pix.pixelformat >> 24) & 0xFF
				);

	}
*/
	/* Buggy driver paranoia. */
	/*
	min = fmt.fmt.pix.width * 2;
	if (fmt.fmt.pix.bytesperline < min)
		fmt.fmt.pix.bytesperline = min;
	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min)
		fmt.fmt.pix.sizeimage = min;
		*/



	switch (io) {
	case IO_METHOD_READ:
		init_read(fmt.fmt.pix.sizeimage);
		break;

	case IO_METHOD_MMAP:
		init_mmap();
		break;

	case IO_METHOD_USERPTR:
		init_userp(fmt.fmt.pix.sizeimage);
		break;
	}
}

static void close_device(void)
{
	pr_debug("%s: called!\n", __func__);

	if (-1 == close(fd))
		errno_exit("close");

	fd = -1;
}

static void open_device(void)
{
	struct stat st;

	pr_debug("%s: called!\n", __func__);

	if (-1 == stat(dev_name, &st)) {
		fprintf(stderr, "Cannot identify '%s': %d, %s\n",
			 dev_name, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (!S_ISCHR(st.st_mode)) {
		fprintf(stderr, "%s is no device\n", dev_name);
		exit(EXIT_FAILURE);
	}

	fd = open(dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

	if (-1 == fd) {
		fprintf(stderr, "Cannot open '%s': %d, %s\n",
			 dev_name, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

static void usage(FILE *fp, int argc, char **argv)
{
	fprintf(fp,
		 "Usage: %s [options]\n\n"
		 "Version 1.3\n"
		 "Options:\n"
		 "-d | --device name   Video device name [%s]\n"
		 "-h | --help          Print this message\n"
		 "-m | --mmap          Use memory mapped buffers [default]\n"
		 "-r | --read          Use read() calls\n"
		 "-u | --userp         Use application allocated buffers\n"
		 "-o | --output        Outputs stream to stdout\n"
		 "-f | --format        Force format to 640x480 YUYV\n"
		 "-c | --count         Number of frames to grab [%i]\n"
		 "-v | --verbose       Verbose output\n"
		 "",
		 argv[0], dev_name, frame_count);
}

static const char short_options[] = "d:hmruofc:v";

static const struct option
long_options[] = {
	{ "device", required_argument, NULL, 'd' },
	{ "help",   no_argument,       NULL, 'h' },
	{ "mmap",   no_argument,       NULL, 'm' },
	{ "read",   no_argument,       NULL, 'r' },
	{ "userp",  no_argument,       NULL, 'u' },
	{ "output", no_argument,       NULL, 'o' },
	{ "format", no_argument,       NULL, 'f' },
	{ "count",  required_argument, NULL, 'c' },
	{ "verbose", no_argument,      NULL, 'v' },
	{ 0, 0, 0, 0 }
};

int main(int argc, char **argv)
{
	//system("killall -9 program.elf");

	dev_name = "/dev/video0";

	for (;;) {
		int idx;
		int c;

		c = getopt_long(argc, argv,
				short_options, long_options, &idx);

		if (-1 == c)
			break;

		switch (c) {
		case 0: /* getopt_long() flag */
			break;

		case 'd':
			dev_name = optarg;
			break;

		case 'h':
			usage(stdout, argc, argv);
			exit(EXIT_SUCCESS);

		case 'm':
			io = IO_METHOD_MMAP;
			break;

		case 'r':
			io = IO_METHOD_READ;
			break;

		case 'u':
			io = IO_METHOD_USERPTR;
			break;

		case 'o':
			out_buf++;
			break;

		case 'f':
			force_format++;
			break;

		case 'c':
			errno = 0;
			frame_count = strtol(optarg, NULL, 0);
			if (errno)
				errno_exit(optarg);
			break;

		case 'v':
			verbose = 1;
			break;

		default:
			usage(stderr, argc, argv);
			exit(EXIT_FAILURE);
		}
	}

	open_device();
	init_device();
	start_capturing();
	mainloop();
	stop_capturing();
	uninit_device();
	close_device();
	fprintf(stderr, "\n");
	return 0;
}
