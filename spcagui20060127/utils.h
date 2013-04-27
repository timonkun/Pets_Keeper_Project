 
#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <string.h>
#include <fcntl.h>
#include <wait.h>
#include <time.h>
#include <limits.h>
#include <SDL/SDL.h>
#include "SDL_image.h"
#include "avilib.h"

#include "spca5xx.h"
#include "spcav4l.h"
#include "spcadecoder.h"

#ifdef HAVE_LIBJPEG
#include <jpeglib.h>
#endif

void
YUV420toRGB (unsigned char *src, unsigned char *dst, int width, int height,
	     int flipUV, int ColSpace);
void
getFileName(char* name,int format);

void 
getPicture (unsigned char* src ,int w,int h, int format, int size);

int aviOpen (struct vdIn *vd);
void aviClose (void);
void aviWrite ( struct vdIn *vd);

/* helping  buffer picture and frame avi */ 
static unsigned char *outpict = NULL;


#endif // UTILS_H
