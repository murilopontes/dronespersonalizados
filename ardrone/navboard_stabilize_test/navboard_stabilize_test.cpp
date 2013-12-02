

//Utilities library
#include <cstdlib> //	 General purpose utilities: program control, dynamic memory allocation, random numbers, sort and search
#include <csignal> //	 Functions and macro constants for signal management
#include <csetjmp> //	 Macro (and function) that saves (and jumps) to an execution context
#include <cstdarg> //	 Handling of variable length argument lists
#include <typeinfo> //	 Runtime type information utilities
//#include <typeindex> // (since C++11)	std::type_index
//#include <type_traits> // (since C++11)	Compile-time type information
#include <bitset> //	std::bitset class template
#include <functional> //	 Function objects, designed for use with the standard algorithms
#include <utility> //	 Various utility components
#include <ctime> //	 C-style time/date utilites
//#include <chrono> // (since C++11)	 C++ time utilites
#include <cstddef> //	 typedefs for types such as size_t, NULL and others
//#include <initializer_list> // (since C++11)	std::initializer_list class template
//#include <tuple> // (since C++11)	std::tuple class template
//Dynamic memory management
#include <new> //	 Low-level memory management utilities
#include <memory> //	 Higher level memory management utilities
//#include <scoped_allocator> // (since C++11)	 Nested allocator class
//Numeric limits
#include <climits> //	 limits of integral types
#include <cfloat> //	 limits of float types
//#include <cstdint> // (since C++11)	 fixed-size types and limits of other types
//#include <cinttypes> // (since C++11)	 formatting macros , intmax_t and uintmax_t math and conversions
#include <limits> //	 standardized way to query properties of fundamental types
//Error handling
#include <exception> //	 Exception handling utilities
#include <stdexcept> //	 Standard exception objects
#include <cassert> //	 Conditionally compiled macro that compares its argument to zero
//#include <system_error> // (since C++11)	 defines std::error_code, a platform-dependent error code
#include <cerrno> //	 Macro containing the last error number
//Strings library
#include <cctype> //	 functions to determine the type contained in character data
#include <cwctype> //	 functions for determining the type of wide character data
#include <cstring> //	 various narrow character string handling functions
#include <cwchar> //	 various wide and multibyte string handling functions
//#include <cuchar> // (since C++11)	 C-style Unicode character conversion functions
#include <string> //	std::basic_string class template
//Containers library
//#include <array> // (since C++11)	std::array container
#include <vector> //	std::vector container
#include <deque> //	std::deque container
#include <list> //	std::list container
//#include <forward_list> // (since C++11)	std::forward_list container
#include <set> //	std::set and std::multiset associative containers
#include <map> //	std::map and std::multimap associative containers
//#include <unordered_set> // (since C++11)	std::unordered_set and std::unordered_multiset unordered associative containers
//#include <unordered_map> // (since C++11)	std::unordered_map and std::unordered_multimap unordered associative containers
#include <stack> //	std::stack container adaptor
#include <queue> //	std::queue and std::priority_queue container adaptors
//Algorithms library
#include <algorithm> //	 Algorithms that operate on containers
//Iterators library
#include <iterator> //	 Container iterators
//Numerics library
#include <cmath> //	 Common mathematics functions
#include <complex> //	 Complex number type
#include <valarray> //	 Class for representing and manipulating arrays of values
//#include <random> // (since C++11)	 Random number generators and distributions
#include <numeric> //	 Numeric operations on values in containers
//#include <ratio> // (since C++11)	 Compile-time rational arithmetic
//#include <cfenv> // (since C++11)	 Floating-point environment access functions
//Input/output library
#include <iosfwd> //	 forward declarations of all classes in the input/output library
#include <ios> //	std::ios_base class, std::basic_ios class template and several typedefs
#include <istream> //	std::basic_istream class template and several typedefs
#include <ostream> //	std::basic_ostream class template and several typedefs
#include <iostream> //	std::basic_iostream class template and several typedefs
#include <fstream> //	std::basic_fstream, std::basic_ifstream, std::basic_ofstream class templates and several typedefs
#include <sstream> //	std::basic_stringstream, std::basic_istringstream, std::basic_ostringstream class templates and several typedefs
//#include <strstream> //	std::strstream, std::istrstream, std::ostrstream(deprecated)
#include <iomanip> //	 Helper functions to control the format or input and output
#include <streambuf> //	std::basic_streambuf class template
#include <cstdio> //	 C-style input-output functions
//Localization library
#include <locale> //	 Localization utilities
#include <clocale> //	 C localization utilities
//#include <codecvt> // (since C++11)	 Unicode conversion facilities
//Regular Expressions library
//#include <regex> // (since C++11)	 Classes, algorithms and iterators to support regular expression processing
//Atomic Operations library
//#include <atomic> // (since C++11)	 Atomic operations library
//Thread support library
//#include <thread> // (since C++11)	std::thread class and supporting functions
//#include <mutex> // (since C++11)	 mutual exclusion primitives
//#include <shared_mutex> // (since C++14)	 shared mutual exclusion primitives
//#include <future> // (since C++11)	 primitives for asynchronous computations
//#include <condition_variable> // (since C++11)	thread waiting conditions
//C compatibility headers
#include <ciso646> //	 empty header. The macros that appear in iso646.h in C are keywords in C++
//#include <ccomplex> // (since C++11)	 simply includes the header #include <complex> //
//#include <ctgmath> // (since C++11)	 simply includes the headers #include <ccomplex> // and #include <cmath> //: the overloads equivalent to the contents of the C header tgmath.h are already provided by #include <cmath> // and #include <complex> //
//#include <cstdalign> // (since C++11)	 defines one compatibility macro constant
//#include <cstdbool> // (since C++11)	 defines one compatibility macro constant

//Deprecated headers
/*
#include <assert.h> // (deprecated)	 behaves as if each name from #include <cassert> // is placed in global namespace
#include <complex.h> // (deprecated)	 behaves as if each name from #include <ccomplex> // is placed in global namespace
#include <ctype.h> // (deprecated)	 behaves as if each name from #include <cctype> // is placed in global namespace
#include <errno.h> // (deprecated)	 behaves as if each name from #include <cerrno> // is placed in global namespace
#include <fenv.h> // (deprecated)	 behaves as if each name from #include <cfenv> // is placed in global namespace
#include <float.h> // (deprecated)	 behaves as if each name from #include <cfloat> // is placed in global namespace
#include <inttypes.h> // (deprecated)	 behaves as if each name from #include <cinttypes> // is placed in global namespace
#include <iso646.h> // (deprecated)	 behaves as if each name from #include <ciso646> // is placed in global namespace
#include <limits.h> // (deprecated)	 behaves as if each name from #include <climits> // is placed in global namespace
#include <locale.h> // (deprecated)	 behaves as if each name from #include <clocale> // is placed in global namespace
#include <math.h> // (deprecated)	 behaves as if each name from #include <cmath> // is placed in global namespace
#include <setjmp.h> // (deprecated)	 behaves as if each name from #include <csetjmp> // is placed in global namespace
#include <signal.h> // (deprecated)	 behaves as if each name from #include <csignal> // is placed in global namespace
//#include <stdalign.h> // (deprecated)	 behaves as if each name from #include <cstdalign> // is placed in global namespace
#include <stdarg.h> // (deprecated)	 behaves as if each name from #include <cstdarg> // is placed in global namespace
#include <stdbool.h> // (deprecated)	 behaves as if each name from #include <cstdbool> // is placed in global namespace
#include <stddef.h> // (deprecated)	 behaves as if each name from #include <cstddef> // is placed in global namespace
#include <stdint.h> // (deprecated)	 behaves as if each name from #include <cstdint> // is placed in global namespace
#include <stdio.h> // (deprecated)	 behaves as if each name from #include <cstdio> // is placed in global namespace
#include <stdlib.h> // (deprecated)	 behaves as if each name from #include <cstdlib> // is placed in global namespace
#include <string.h> // (deprecated)	 behaves as if each name from #include <cstring> // is placed in global namespace
#include <tgmath.h> // (deprecated)	 behaves as if each name from #include <ctgmath> // is placed in global namespace
#include <time.h> // (deprecated)	 behaves as if each name from #include <ctime> // is placed in global namespace
//#include <uchar.h> // (deprecated)	 behaves as if each name from #include <cuchar> // is placed in global namespace
#include <wchar.h> // (deprecated)	 behaves as if each name from #include <cwchar> // is placed in global namespace
#include <wctype.h> // (deprecated)	 behaves as if each name from #include <cwctype> // is placed in global namespace
*/


#if 0
//C library: http://en.cppreference.com/w/c/header
#include <assert.h> //	Conditionally compiled macro that compares its argument to zero
#include <complex.h> // (since C99)	Complex number arithmetic
#include <ctype.h> //	Functions to determine the type contained in character data
#include <errno.h> //	Macros reporting error conditions
#include <fenv.h> // (since C99)	Floating-point environment
#include <float.h> //	Limits of float types
#include <inttypes.h> // (since C99)	Format conversion of integer types
#include <iso646.h> // (since C99)	 Alternative operator spellings
#include <limits.h> //	Sizes of basic types
#include <locale.h> //	Localization utilities
#include <math.h> //	Common mathematics functions
#include <setjmp.h> //	Nonlocal jumps
#include <signal.h> //	Signal handling
//#include <stdalign.h> // (since C11)	 Type alignment control
#include <stdarg.h> //	 Variable arguments
//#include <stdatomic.h> // (since C11)	Atomic types
#include <stdbool.h> // (since C99)	Macros supporting type bool
#include <stddef.h> //	 Common macro definitions
#include <stdint.h> // (since C99)	Fixed-width integer types
#include <stdio.h> //	Input/output
#include <stdlib.h> //	 General utilities: memory management, program utilities, string conversions, random numbers
//#include <stdnoreturn.h> // (since C11)	 Non-returning functions
#include <string.h> //	String handling
#include <tgmath.h> // (since C99)	Type-generic math (macros wrapping math.h and complex.h)
//#include <threads.h> // (since C11)	Thread library
#include <time.h> //	Time/date utilities
//#include <uchar.h> // (since C11)	UTF-16 and UTF-32 character utilities
#include <wchar.h> // (since C99)	Extended multibyte and wide character utilities
#include <wctype.h> // (since C99)	Wide character classification and mapping utilities
#endif



//posix API
#include <unistd.h>

//posix threads
#include <pthread.h>

//file control options
#include <fcntl.h>

//terminal I/O interfaces
#include <termios.h>


//#include "../open_ardrone_v1_lib/type.h"
//#include "../open_ardrone_v1_lib/util.h"
#include "../open_ardrone_v1_lib/gpio.h"
#include "../open_ardrone_v1_lib/udp.h"
#include "../open_ardrone_v1_lib/mot.h"


typedef struct {
  int fd;
} navboard_context_t;

int between(double value,double x, double y){
  if( value >= x && value <= y) return 1;
  else return 0;
}

double clamp(double value,double min, double max){
	if(value<min) return min;
	if(value>max) return max;
	return value;
}

double make_stable(double target,double current){

	// distance = final - initial
		double height_distance = target-current;
		double adj_h=0;
		//Fuzzy controller
		double f1=3*1;
		double f2=3*2;
		double f3=3*3;
		double f4=3*4;
		double f5=3*5;
		double f6=3*6;
		//big (sky to target)
		if( height_distance < -f5 ) adj_h-=0.03;
		//medium (sky to target)
		if( between(height_distance,-f6,-f3)) adj_h-=0.02;
		//few (sky to target)
		if( between(height_distance,-f4,-f1)) adj_h-=0.01;
		//stable area
		if( between(height_distance,-f2,f2)) adj_h+=0.0;
		//few (ground to target)
		if( between(height_distance,f1,f4)) adj_h+=0.01;
		//medium (ground to target)
		if( between(height_distance,f3,f6)) adj_h+=0.02;
		//big (ground to target)
		if( height_distance>f5) adj_h+=0.03;

	return adj_h;
}


int navboard_context_init(navboard_context_t* context){

	context->fd = open("/dev/ttyPA2", O_RDWR | O_NOCTTY | O_NDELAY);
	if (context->fd == -1)
	{
		perror("nav_Init: Unable to open /dev/ttyPA2 - ");
		return -1;
	}
	//read calls are non blocking
	fcntl(context->fd, F_SETFL, FNDELAY);

	//set port options
	struct termios options;
	//Get the current options for the port
	tcgetattr(context->fd, &options);
	//Set the baud rates to 460800
	cfsetispeed(&options, B460800);
	cfsetospeed(&options, B460800);

	//Enable the receiver and set local mode
	options.c_cflag |= (CLOCAL | CREAD);
	//clear input options
	options.c_iflag = 0;
	//clear local options
	options.c_lflag=0;
	//clear output options (raw output)
	options.c_oflag &= ~OPOST;
	//Set the new options for the port
	tcsetattr(context->fd, TCSANOW, &options);

	//set /MCLR pin -> reset navboard
	gpio_set(132,1);

	//start acquisition
	uint8_t cmd=0x01;
	write(context->fd,&cmd,1);


	udp_struct udp_vertical;
	udpClient_Init(&udp_vertical,8080);


	uint8_t buffer[128];

	double height=0;
	double speed=0;
	double motor[4]={0,0,0,0};

	mot_Init();

	for(;;){

		int count_ok=0;
		int count_rest=52;

		do {
		 //
	     int n = read(context->fd, &buffer[count_ok], count_rest);
	     if(n<0) continue;
	     count_ok+=n;
	     count_rest-=n;
		} while(count_ok<52);

       ///////////////////////////////////////////////////////////
		uint16_t *packet = (uint16_t*)&buffer;

		double ax=(packet[2] - 2068.0)/100.0;
		double ay=(packet[3] - 1976.0)/100.0;
		double az=(packet[4] - 2052.0)/100.0;
		//printf("ax=%.2f ay=%.2f az=%.2f ",ax,ay,az);
		
        double radian2degree = 57.295779513082320876798154814105f;
		double apitch =  -atan2(ax,az) * radian2degree;
		double aroll  =   atan2(ay,az) * radian2degree;
		double ayaw=0;
		

		//double gx=(packet[5] - 1664.0)/1.0;
		//double gy=(packet[6] - 1669.0)/1.0;
		//double gz=(packet[7] - 1663.0)/1.0;
		
		if(packet[17] < (400*37.5f)){
		   height=packet[17] / 37.5f;
		}

		double speed_min=0.10;
		double speed_max=0.99;

		///////////////////////////////////////////////////////////
		double adj_pitch  = make_stable(0,apitch);
		double adj_roll   = make_stable(0,aroll);
		double adj_yaw    = make_stable(0,ayaw);
		double adj_h      = make_stable(30,height);

		//
		speed+=adj_h;
		speed=clamp(speed,speed_min,speed_max);
		motor[0] = clamp(speed +adj_roll -adj_pitch +adj_yaw,speed_min,speed_max);
		motor[1] = clamp(speed -adj_roll -adj_pitch -adj_yaw,speed_min,speed_max);
		motor[2] = clamp(speed -adj_roll +adj_pitch +adj_yaw,speed_min,speed_max);
		motor[3] = clamp(speed +adj_roll +adj_pitch -adj_yaw,speed_min,speed_max);

		mot_Run(motor[0],motor[1],motor[2],motor[3]);

		printf("m=|%.2f|%.2f|%.2f|%.2f|\r\n",motor[0],motor[1],motor[2],motor[3]);




		//////////////////////////////////////////////////////////
		//printf("pitch=%.2f roll=%.2f h=%.2f\r\n",apitch,aroll,height);
		


	    //udpClient_Send(&udp_vertical, &buffer, 52);

	}

	//init nav structure
    //nav->ts = util_timestamp();
    //nav->dt = 0;
}




#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

static void daemonize(void)
{
    pid_t pid, sid;
    int fd;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS); /*Killing the Parent Process*/
    }

    /* At this point we are executing as the child process */

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory. */
    if ((chdir("/")) < 0)
    {
        exit(EXIT_FAILURE);
    }


    fd = open("/dev/null",O_RDWR, 0);

    if (fd != -1)
    {
        dup2 (fd, STDIN_FILENO);
        dup2 (fd, STDOUT_FILENO);
        dup2 (fd, STDERR_FILENO);

        if (fd > 2)
        {
            close (fd);
        }
    }

    /*resettign File Creation Mask */
    umask(027);
}




//
void verify_singleton(char* program_name) {


    //strip program_name from complete path
	char* slash = strrchr(program_name, '/');
	if (slash) {
		program_name = (slash + 1);
	}

	//format pidof command
	char cmd_str[2048];
	sprintf(cmd_str, "pidof %s", program_name);
	printf("cmd_str->%s\r\n", cmd_str);

	//execute pidof command
	char cmd_output[2048];
	FILE* pidof_fd = popen(cmd_str, "r");
	fgets(cmd_output, sizeof(cmd_output), pidof_fd);
	pclose(pidof_fd);
	printf("result of pidof ->(%s)\r\n", cmd_output);

	//
	int pids = 0;
	const char s[2] = " ";
	char *token = strtok(cmd_output, s);
	while (token != NULL) {
		printf("pid[%d]=%s\r\n", pids, token);
		token = strtok(NULL, s);
		pids++;
	}
	if (pids != 1) {
		printf("exit! this program is already running\r\n");
		exit(1);
	} else {
		printf("ok! normal workflow\r\n");
	}
}

int main(int argc, char** argv)
{

	//daemonize();

	//
	verify_singleton(argv[0]);

	//
	system("killall -9 program.elf");

	//
	int ret;
	navboard_context_t ctx;
	
	//
	ret=navboard_context_init(&ctx);

	//
	if(ret < 0) return ret;



	return 0;
}



