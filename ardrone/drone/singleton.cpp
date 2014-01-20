/*
 * singleton.c
 *
 *  Created on: 02/12/2013
 *      Author: mpontes
 */


#include "singleton.h"

#include "all_cpp_headers.h"
#include "all_boost_headers.h"


//
void singleton(char* program_name) {


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
