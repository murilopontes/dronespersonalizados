//============================================================================
// Name        : ardrone_free.cpp
// Author      : murilo pontes
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "all_headers_cpp.h"
#include "daemonize.h"
#include "singleton.h"

int main(int argc,char** argv) {


	singleton(argv[0]);
	system("killall -9 program.elf");
	daemonize();

	return 0;
}
