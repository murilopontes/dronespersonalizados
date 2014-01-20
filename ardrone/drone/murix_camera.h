/*
 * camera.h
 *
 *  Created on: 19/01/2014
 *      Author: mpontes
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <stdint.h>

class camera_buffer_t  {
public:
	void   *start;
	uint32_t  length;
} ;

class camera_context_t {
public:
	char* dev_name;
	int width;
	int height;
	//////////////////////////
	int fd;
	camera_buffer_t *buffers;
	int n_buffers;
	uint8_t* buf;
	uint32_t img_size;
} ;



camera_context_t camera_v(void);
camera_context_t camera_h(void);


#endif /* CAMERA_H_ */
