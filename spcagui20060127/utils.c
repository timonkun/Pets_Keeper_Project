/****************************************************************************
#	 	SpcaGui:  Spca5xx Grabber                                   #
# 		Copyright (C) 2004 Michel Xhaard                            #
#                                                                           #
# This program is free software; you can redistribute it and/or modify      #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation; either version 2 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# This program is distributed in the hope that it will be useful,           #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with this program; if not, write to the Free Software               #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA #
#                                                                           #
****************************************************************************/

#include "utils.h"

static avi_t *avi_fd = NULL;
static int framecount = 0;
static double time_start = 0;
static double time_courant = 0;

double
ms_time (void)
{
  static struct timeval tod;
  gettimeofday (&tod, NULL);
  return ((double) tod.tv_sec * 1000.0 + (double) tod.tv_usec / 1000.0);

}

int
aviOpen (struct vdIn *vd)
{
  static char filename[25];
  int error = 0;
  memset (filename, 0, sizeof (filename));
  framecount = 0;
  time_start = ms_time ();
  getFileName (filename, 0);
  if (filename)
    {
      if ((vd->formatIn == VIDEO_PALETTE_RAW_JPEG)
	  || (vd->formatIn == VIDEO_PALETTE_YUV420P))
	{

	  if ((avi_fd = AVI_open_output_file (filename)) == NULL)
	    {
	      printf ("cannot open write file ? \n");
	      error = -1;
	      goto fin;
	    }

	  switch (vd->formatIn)
	    {
	    case VIDEO_PALETTE_RAW_JPEG:
	      AVI_set_video (avi_fd, vd->hdrwidth, vd->hdrheight, 20, "MJPG");
	      break;
	    case VIDEO_PALETTE_YUV420P:
	      AVI_set_video (avi_fd, vd->hdrwidth, vd->hdrheight, 20, "I420");
	      break;
	    default:
	      printf ("FOURCC invalid try jpeg or yuv \n");
	      error = -1;
	      break;
	    }
	}
      else
	{
	  printf ("FOURCC invalid try jpeg or yuv \n");
	  error = -1;
	}
    }
  else
    {
      printf ("Could'nt Get a Filename !!\n");
      error = -1;
    }
  time_start = ms_time ();
fin:
  return error;
}

void
aviWrite (struct vdIn *vd)
{
  int error = 0;
  int sizein = 0;
  int sizeout = 0;
  sizein = vd->framesizeIn;
  if (avi_fd)
    {
      switch (vd->formatIn)
	{
	case VIDEO_PALETTE_RAW_JPEG:
	  sizeout = vd->hdrwidth * vd->hdrheight * 3;
	  outpict = (unsigned char *) realloc (outpict, sizeout);
	  create_jpeg_from_data (outpict, vd->pixTmp, 3,
				 vd->hdrwidth, vd->hdrheight, 0x22, sizein,
				 &sizeout, 0);
	  if (AVI_write_frame (avi_fd,
			       (unsigned char *) outpict, sizeout) < 0)
	    printf ("write error on avi out \n");
	  free (outpict);
	  outpict = NULL;
	  break;
	case VIDEO_PALETTE_YUV420P:
	  if (AVI_write_frame (avi_fd,
			       (unsigned char *) vd->pixTmp, sizein) < 0)
	    printf ("write error on avi out \n");
	  break;
	default:
	  printf ("FOURCC invalid try jpeg or yuv\n");
	  error = -1;
	  break;
	}
    }
  time_courant = ms_time ();
  framecount++;
}

void
aviClose (void)
{
  double tottime = 0;
  if (avi_fd)
    {
      printf ("AVI close asked \n");
      tottime = time_courant - time_start;
      if (tottime > 0)
	avi_fd->fps = (double) framecount *1000 / tottime;

      AVI_close (avi_fd);
      printf ("close avi\n");
      avi_fd = NULL;
    }

}

void
put_image_pnm (FILE * out, unsigned char *image, int w, int h)
{
  fprintf (out, "P6\n%d %d\n255\n", w, h);
  fwrite (image, sizeof (char), w * h * 3, out);
}

/* Take from 
/*
 * vidcat.c
 *
 * Copyright (C) 1998 - 2001 Rasca, Berlin
 *
 * This program is free software; you can redistribute it and/or modify
 */
void
put_image_jpeg (FILE * out, char *image, int width, int height, int quality)
{
#ifdef HAVE_LIBJPEG
  int y, x, line_width;
  JSAMPROW row_ptr[1];
  struct jpeg_compress_struct cjpeg;
  struct jpeg_error_mgr jerr;
  char *line;

  line = malloc (width * 3);
  if (!line)
    return;

  cjpeg.err = jpeg_std_error (&jerr);
  jpeg_create_compress (&cjpeg);
  cjpeg.image_width = width;
  cjpeg.image_height = height;

  cjpeg.input_components = 3;
  cjpeg.in_color_space = JCS_RGB;

  jpeg_set_defaults (&cjpeg);
  jpeg_set_quality (&cjpeg, quality, TRUE);
  cjpeg.dct_method = JDCT_FASTEST;
  jpeg_stdio_dest (&cjpeg, out);


  jpeg_start_compress (&cjpeg, TRUE);
  row_ptr[0] = line;

  line_width = width * 3;
  for (y = 0; y < height; y++)
    {
      memcpy (line, image, line_width);
      jpeg_write_scanlines (&cjpeg, row_ptr, 1);
      image += line_width;
    }

  jpeg_finish_compress (&cjpeg);
  jpeg_destroy_compress (&cjpeg);
  free (line);
#endif
}

void
getPicture (unsigned char *src, int w, int h, int format, int size)
{
  static char filename[25];
  __u32 *lpix;
  __u16 *pix;
  __u8 *dest;
  int i;
  int sizein = 0;
  int sizeout = 0;
  FILE *foutpict;
  memset (filename, 0, sizeof (filename));
  /* allocate the outpict buffer make room for rgb24 */
  sizein = size;
  sizeout = w * h * 3;
  outpict = (unsigned char *) realloc (outpict, sizeout);

  dest = (__u8 *) outpict;
#ifdef HAVE_LIBJPEG
  getFileName (filename, 2);
#else
  getFileName (filename, 1);
#endif
  foutpict = fopen (filename, "wb");
  if (format == VIDEO_PALETTE_RAW_JPEG)
    {
      create_jpeg_from_data (dest, src, 3, w, h, 0x22, sizein, &sizeout, 0);
      printf (" picture raw jpeg %s\n", filename);
      fwrite (outpict, sizeof (char), sizeout, foutpict);
      fclose (foutpict);
    }
  else
    {
      switch (format)
	{
	case VIDEO_PALETTE_RGB565:
	  pix = (__u16 *) src;
	  for (i = 0; i < (sizeout - 3); i += 3)
	    {
	      dest[i] = (*pix & 0xF800) >> 8;
	      dest[i + 1] = (*pix & 0x07E0) >> 3;
	      dest[i + 2] = (*pix & 0x001F) << 3;
	      pix++;
	    }
	  printf (" picture rgb565 %s\n", filename);
	  break;
	case VIDEO_PALETTE_RGB24:
	  for (i = 0; i < sizeout; i += 3)
	    {
	      dest[i] = src[i + 2];
	      dest[i + 1] = src[i + 1];
	      dest[i + 2] = src[i];
	    }
	  printf (" picture rgb24 %s\n", filename);
	  break;
	case VIDEO_PALETTE_RGB32:
	  lpix = (__u32 *) src;
	  for (i = 0; i < sizeout; i += 3)
	    {
	      dest[i] = (*lpix & 0x00FF0000) >> 16;
	      dest[i + 1] = (*lpix & 0x0000FF00) >> 8;
	      dest[i + 2] = (*lpix & 0x000000FF);
	      lpix++;
	    }
	  printf (" picture rgb32 %s\n", filename);
	  break;
	case VIDEO_PALETTE_YUV420P:
	  YUV420toRGB (src, dest, w, h, 1, 0);
	  printf (" picture yuv420p %s\n", filename);
	  break;

	default:
	  break;
	}
#ifdef HAVE_LIBJPEG
      put_image_jpeg (foutpict, outpict, w, h, 80);
#else
      put_image_pnm (foutpict, outpic, w, h);
#endif
      fclose (foutpict);
    }

  free (outpict);
  outpict = NULL;
}

void
getFileName (char *name, int format)
{
  char temp[80];
  char *myext[] = { "avi", "pnm", "jpg" };
  int i;
  time_t curdate;
  struct tm *tdate;
  memset (temp, '\0', sizeof (temp));
  time (&curdate);
  tdate = localtime (&curdate);
  snprintf (temp, 24, "%02d:%02d:%04d-%02d:%02d:%02d.%s\0",
	    tdate->tm_mon + 1, tdate->tm_mday, tdate->tm_year + 1900,
	    tdate->tm_hour, tdate->tm_min, tdate->tm_sec, myext[format]);

  memcpy (name, temp, strlen (temp));

}


static
swapRB (char *Buffer, int size)
{
  char temp;
  int i;
  for (i = 0; i < size; i += 3)
    {
      temp = Buffer[i];
      Buffer[i] = Buffer[i + 2];
      Buffer[i + 2] = temp;
    }

}

#define CLIP(color) (unsigned char)((color>0xFF)?0xff:((color<0)?0:color))

void
YUV420toRGB (unsigned char *src, unsigned char *dst, int width, int height,
	     int flipUV, int ColSpace)
{
  unsigned char *Y;
  unsigned char *V;
  unsigned char *U;
  int y1, y2, u, v;
  int v1, v2, u1, u2;
  unsigned char *pty1, *pty2;
  int i, j;
  unsigned char *RGB1, *RGB2;
  int r, g, b;

  //Initialization
  Y = src;
  V = Y + width * height;
  U = Y + width * height + width * height / 4;

  pty1 = Y;
  pty2 = pty1 + width;
  RGB1 = dst;
  RGB2 = RGB1 + 3 * width;
  for (j = 0; j < height; j += 2)
    {
      //printf ("process line %d\n",j);
      for (i = 0; i < width; i += 2)
	{
	  if (flipUV)
	    {
	      u = (*V++) - 128;
	      v = (*U++) - 128;
	    }
	  else
	    {
	      v = (*V++) - 128;
	      u = (*U++) - 128;
	    }
	  switch (ColSpace)
	    {
	      // M$ color space
	    case 0:
	      {
		v1 = ((v << 10) + (v << 9) + (v << 6) + (v << 5)) >> 10;	// 1.593
		u1 = ((u << 8) + (u << 7) + (u << 4)) >> 10;	//         0.390
		v2 = ((v << 9) + (v << 4)) >> 10;	//                0.515
		u2 = ((u << 11) + (u << 4)) >> 10;	//               2.015
	      }
	      break;
	      // PAL specific
	    case 1:
	      {
		v1 = ((v << 10) + (v << 7) + (v << 4)) >> 10;	//      1.1406
		u1 = ((u << 8) + (u << 7) + (u << 4) + (u << 3)) >> 10;	// 0.3984
		v2 = ((v << 9) + (v << 6) + (v << 4) + (v << 1)) >> 10;	// 0.5800
		u2 = ((u << 11) + (u << 5)) >> 10;	//              2.0312
	      }
	      break;
	      // V4l2
	    case 2:
	      {
		v1 = ((v << 10) + (v << 8) + (v << 7) + (v << 5)) >> 10;	//       1.406
		u1 = ((u << 8) + (u << 6) + (u << 5)) >> 10;	//                0.343
		v2 = ((v << 9) + (v << 7) + (v << 6) + (v << 5)) >> 10;	//        0.718
		u2 = ((u << 10) + (u << 9) + (u << 8) + (u << 4) + (u << 3)) >> 10;	// 1.773
	      }
	      break;
	    case 3:
	      {
		v1 = u1 = v2 = u2 = 0;
	      }
	      break;
	    default:
	      break;

	    }
	  //up-left

	  y1 = (*pty1++);
	  if (y1 > 0)
	    {
	      r = y1 + (v1);
	      g = y1 - (u1) - (v2);
	      b = y1 + (u2);

	      r = CLIP (r);
	      g = CLIP (g);
	      b = CLIP (b);


	    }
	  else
	    {
	      r = g = b = 0;
	    }
	  *RGB1++ = r;
	  *RGB1++ = g;
	  *RGB1++ = b;

	  //down-left


	  y2 = (*pty2++);
	  if (y2 > 0)
	    {
	      r = y2 + (v1);
	      g = y2 - (u1) - (v2);
	      b = y2 + (u2);

	      r = CLIP (r);
	      g = CLIP (g);
	      b = CLIP (b);


	    }
	  else
	    {
	      r = b = g = 0;
	    }
	  *RGB2++ = r;
	  *RGB2++ = g;
	  *RGB2++ = b;

	  //up-right
	  y1 = (*pty1++);

	  if (y1 > 0)
	    {
	      r = y1 + (v1);
	      g = y1 - (u1) - (v2);
	      b = y1 + (u2);

	      r = CLIP (r);
	      g = CLIP (g);
	      b = CLIP (b);


	    }
	  else
	    {
	      r = g = b = 0;
	    }

	  *RGB1++ = r;
	  *RGB1++ = g;
	  *RGB1++ = b;



	  //down-right
	  y2 = (*pty2++);
	  if (y2 > 0)
	    {

	      r = y2 + (v1);
	      g = y2 - (u1) - (v2);
	      b = y2 + (u2);

	      r = CLIP (r);
	      g = CLIP (g);
	      b = CLIP (b);



	    }
	  else
	    {
	      r = b = g = 0;
	    }

	  *RGB2++ = r;
	  *RGB2++ = g;
	  *RGB2++ = b;

	}
      RGB1 += 3 * width;
      RGB2 += 3 * width;
      pty1 += width;
      pty2 += width;
    }

//printf ("done YUV420 -> RGB \n");
}
