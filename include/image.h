#ifndef IMAGE_H
#define IMAGE_H

#include <IL/il.h>

typedef struct {
	ILint width;
	ILint height;
	ILubyte* data;
} image_t;

#endif
