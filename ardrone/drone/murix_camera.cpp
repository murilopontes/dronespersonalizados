/*
 * camera.cpp
 *
 *  Created on: 19/01/2014
 *      Author: mpontes
 */


#include "murix_camera.h"

#include "murix_boost_headers.h"
#include "murix_cpp_headers.h"


//mmap, munmap - map or unmap files or devices into memory
#include <sys/mman.h>

//camera
#include <linux/videodev2.h>










int camera_xioctl(int fh, int request, void *arg,const char *name)
{
	int r = ioctl(fh, request, arg);
	if(r==-1){
		printf("ioctl fd=%d name=%s \033[1;31mFAIL\033[m\r\n",fh,name);
	} else {
		printf("ioctl fd=%d name=%s \033[1;32mOK\033[m\r\n",fh,name);
	}
	return r;
}


void camera_init(camera_context_t* ctx){

	///////////////////////////////////////////////////////////////////////////
	////////////////// ------ OPEN DEVICE

	//| O_NONBLOCK
	ctx->fd = open(ctx->dev_name, O_RDWR , 0);
	if(ctx->fd<0){
		printf("error cannot open device\r\n");
	}

	////////////////////////////////////////////////////////////////
	///////////////// ---------- DEVICE CAPS ----------------------

	struct v4l2_capability cap;
	memset(&cap,0,sizeof(cap));
	camera_xioctl(ctx->fd, VIDIOC_QUERYCAP, &cap,"VIDIOC_QUERYCAP");

	//////////////////////////////////////////////////////////////////////
	///////////////////--------- SET FORMAT

	struct v4l2_format fmt;
	memset(&fmt,0,sizeof(fmt));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;
	fmt.fmt.pix.width=ctx->width;
	fmt.fmt.pix.height=ctx->height;
	camera_xioctl(ctx->fd, VIDIOC_S_FMT, &fmt,"VIDIOC_S_FMT");

	/////////////////////////////////////////////////////////////////
	/////////////// ----------------  REQUEST BUFFER ---------///////

	struct v4l2_requestbuffers req;
	memset(&req,0,sizeof(req));
	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	camera_xioctl(ctx->fd, VIDIOC_REQBUFS, &req,"VIDIOC_REQBUFS");

	/////////////////////////////////////////////////////////////////////////////
	////////////////--------------- MMAP INIT

	ctx->n_buffers=req.count;
	ctx->buffers = (camera_buffer_t*) calloc(ctx->n_buffers, sizeof(camera_buffer_t));

	for (int i = 0; i < ctx->n_buffers; ++i) {
		struct v4l2_buffer buf;
		memset(&buf,0,sizeof(buf));
		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;
		camera_xioctl(ctx->fd, VIDIOC_QUERYBUF, &buf,"VIDIOC_QUERYBUF");
		ctx->buffers[i].length = buf.length;
		ctx->buffers[i].start = mmap(NULL,buf.length,PROT_READ | PROT_WRITE ,MAP_SHARED, ctx->fd, buf.m.offset);
	}

	/////////////////////////////////////////////////////////////////////
	////////////---------------- enqueue request

	for (int i = 0; i < ctx->n_buffers; ++i) {

		struct v4l2_buffer buf;
		memset(&buf,0,sizeof(buf));

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		camera_xioctl(ctx->fd, VIDIOC_QBUF, &buf,"VIDIOC_QBUF");
	}

	/////////////////////////////////////////////////////////////////////
	////////////---------------- START CAPTURE

	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	camera_xioctl(ctx->fd, VIDIOC_STREAMON, &type,"VIDIOC_STREAMON");
}

int camera_loop_check(camera_context_t* ctx){

	fd_set fds;
	struct timeval tv;
	int r;
	FD_ZERO(&fds);
	FD_SET(ctx->fd, &fds);
	/* Timeout. */
	tv.tv_sec = 2;
	tv.tv_usec = 0;

	r = select(ctx->fd + 1, &fds, NULL, NULL, &tv);

	if (-1 == r) {
		return -1;
	}

	if (0 == r) {
		return 0;
	}
	return 1;
}

void camera_process_image(camera_context_t* ctx,const void *p, int size)
{
	//printf("camera_process_image size=%d\r\n",size);
	memcpy(ctx->buf,p,size);
}

void camera_loop_frame(camera_context_t* ctx){

	///////////////////////////////////////////////////
	////////////////////////////---------- DEQUEUE
	struct v4l2_buffer buf;
	memset(&buf,0,sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	if(camera_xioctl(ctx->fd, VIDIOC_DQBUF, &buf,"VIDIOC_DQBUF")!=-1){
		////////////////////////////////////////////////--- USE
		camera_process_image(ctx,ctx->buffers[buf.index].start, buf.bytesused);
		//////////////////////////////////////////////----RE-ENQUEUE
		camera_xioctl(ctx->fd, VIDIOC_QBUF, &buf,"VIDIOC_QBUF");
	}

}

void camera_close(camera_context_t* ctx){

	////////////////////////////////////////////////////////////////////////
	//////////////////////////-------- STOP CAPTURE

	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	camera_xioctl(ctx->fd, VIDIOC_STREAMOFF, &type,"VIDIOC_STREAMOFF");

	////////////////////////////////////////////////////////////////////////
	//////////////////////////-------- deinit mmap

	for (int i = 0; i < ctx->n_buffers; ++i){
		munmap(ctx->buffers[i].start, ctx->buffers[i].length);
	}
	free(ctx->buffers);

	////////////////////////////////////////////////////////////////////////
	//////////////////////////-------- CLOSE DEVICE

	close(ctx->fd);
}


void camera_photo(camera_context_t* ctx){
	camera_init(ctx);
	camera_loop_frame(ctx);
	camera_close(ctx);
}


uint8_t photo_h[614400];

camera_context_t camera_h(void){

	camera_context_t ctx_h;
	ctx_h.dev_name = (char*)"/dev/video0";
	ctx_h.width=640;
	ctx_h.height=480;
	ctx_h.buf = photo_h;
	ctx_h.img_size=614400;

	camera_photo(&ctx_h);

	return ctx_h;
}

uint8_t photo_v[50688];

camera_context_t camera_v(void){

	camera_context_t ctx_v;
	ctx_v.dev_name = (char*)"/dev/video1";
	ctx_v.width=176;
	ctx_v.height=144;
	ctx_v.buf = photo_v;
	ctx_v.img_size=50688;

	camera_photo(&ctx_v);

	return ctx_v;
}

