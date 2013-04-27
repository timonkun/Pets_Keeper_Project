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
#define DATE "18 September 2005"
#define VERSION  "0.3.5"

#include "gui.h"
#include "spca5xx.h"
#include "spcav4l.h"
#include "utils.h"
#include "config.h"

static const char colorpot[] = PATHSPCA "colorpot.png";
static const char contrastpot[] = PATHSPCA "contrastpot.png";
static const char brightpot[] = PATHSPCA "brightpot.png";
static const char photo1[] = PATHSPCA "photo.png";
static const char pictrec[] = PATHSPCA "pictrec.png";
static const char clap1[] = PATHSPCA "clap1.png";
static const char avi2[] = PATHSPCA "avi2.png";
static const char pause1[] = PATHSPCA "pause.png";
static const char recp[] = PATHSPCA "recp.png";
//static const char play[] = PATHSPCA"play.png";
//static const char restart[] = PATHSPCA"restart.png";
static const char play[] = PATHSPCA "soundoff.png";
static const char restart[] = PATHSPCA "soundoff.png";
static const char quitter[] = PATHSPCA "quitter.png";
static const char rgb[] = PATHSPCA "rgb.png";
static const char bgr[] = PATHSPCA "bgr.png";
static const char ctrlsize[] = PATHSPCA "ctrlsize.png";
static const char ctrlpal[] = PATHSPCA "ctrlpal.png";

int fd[2];			/* File descriptor array for the pipe */
int pid;
int flags, dummy;

/* allocate the whole structure */
struct vdIn myvidIn;
struct button mybutton[TOTAL_ACTIONS];
struct potentiometre color, contrast, bright;
struct control palette, size;

void initialize (const char *videodev, int method);
void free_spcagui (void);
void telecom (void);
void processvideo (void);

int
main (int argc, char *argv[])
{
  const char *videodevice = NULL;
  int grabmethod = 1;
  int i;
  for (i = 1; i < argc; i++)
    {
      /* skip bad arguments */
      if (argv[i] == NULL || *argv[i] == 0 || *argv[i] != '-')
	{
	  continue;
	}
      if (strcmp (argv[i], "-d") == 0)
	{
	  if (i + 1 >= argc)
	    {
	      printf ("No parameter specified with -d, aborting.\n");
	      exit (1);
	    }
	  videodevice = strdup (argv[i + 1]);
	}
      if (strcmp (argv[i], "-g") == 0)
	{
	  /* Ask for mmap instead default  read */
	  grabmethod = 0;
	}
	if (strcmp (argv[i], "-h") == 0) {
			printf ("usage: spcagui [-h -d -g ] \n");
			printf ("-h	print this message \n");
			printf ("-d	/dev/videoX       use videoX device\n");
			printf ("-g	use read method for grab instead mmap \n");
			exit (0);
	}
    }
  printf ("SpcaGui version: %s date: %s\n", VERSION, DATE );
  if (videodevice == NULL || *videodevice == 0) {
	videodevice = "/dev/video0";
  }
  initialize (videodevice, grabmethod);
//  intro ();
/* Create the pipe we will use */
  if ((pipe (fd)) < 0)
    exit (0);
  if ((flags = fcntl (fd[0], F_GETFL, dummy)) < 0)
    {
      printf ("getflags failure %d\n", flags);
      exit (-1);
    }

  if (fcntl (fd[0], F_SETFL, flags | O_NONBLOCK) < 0)
    {
      printf ("setflags failure\n");
      exit (-1);
    }
  /* Fork and close appropriate descriptors */
  if ((pid = fork ()) < 0)
    exit (-1);

  if (pid == 0)
    {
      /* We are in child */
      processvideo ();
      exit (1);
    }
  else
    {
      /* We are in parent */
      telecom ();
    }

  /* Wait for exit status */
  waitpid (pid, NULL, 0);
  free_spcagui ();
  exit (1);

}

void
initialize (const char *videodev, int method)
{
  
  /* make room for init data */
  myvidIn.videodevice = NULL;
  myvidIn.cameraname = NULL;
  myvidIn.bridge = NULL;
  myvidIn.videodevice = (char *) realloc (myvidIn.videodevice, 16);
  myvidIn.cameraname = (char *) realloc (myvidIn.cameraname, 32);
  myvidIn.bridge = (char *) realloc (myvidIn.bridge, 9);
  myvidIn.grabMethod = method;	// 1 mmap 0 read
  snprintf (myvidIn.videodevice, 12, "%s", videodev);
  printf ("video device %s\n", myvidIn.videodevice);
  init_v4l (&myvidIn);
  /* initialize widget */
  init_potentiometre (COLOR, SpcaGetColors (&myvidIn), 64, 32, &color,
		      colorpot);
  init_potentiometre (CONTRAST, SpcaGetContrast (&myvidIn), 32, 32, &contrast,
		      contrastpot);
  init_potentiometre (BRIGHT, SpcaGetBrightness (&myvidIn), 0, 32, &bright,
		      brightpot);
  init_button (PHOTO, 0, 0, 0, &mybutton[PHOTO], photo1, pictrec);
  init_button (AVI, 0, 32, 0, &mybutton[AVI], clap1, avi2);
  init_button (PAUSE, 0, 64, 0, &mybutton[PAUSE], pause1, recp);
  //init_button( GRAB,0,96,0,&mybutton[GRAB],play,restart);
  init_button (GRAB, 0, 128, 160, &mybutton[GRAB], play, restart);
  init_button (QUIT, 0, 128, 0, &mybutton[QUIT], quitter, quitter);
  //init_button( FLIPRGB,0,128,160,&mybutton[FLIPRGB],rgb,bgr);
  init_button (FLIPRGB, 0, 96, 0, &mybutton[FLIPRGB], rgb, bgr);
  /* init available palette */
 
  if (myvidIn.cameratype == JPEG)
    {
      
      init_control (SIZE, myvidIn.sizenative, 8, 96, 32, &size, ctrlsize);
    }
  else
    {
      init_control (SIZE, myvidIn.sizeothers, 8, 96, 32, &size, ctrlsize);
    }
  init_control (PALETTE, myvidIn.palette, 6, 128, 32, &palette, ctrlpal);
}

void
init_video (struct spca5xx_frame *myframe)
{
  init_jpeg_decoder ();
  myframe->pictsetting.change = 01;
  myframe->pictsetting.gamma = 3;
  myframe->pictsetting.OffRed = -16;
  myframe->pictsetting.OffBlue = -16;
  myframe->pictsetting.OffGreen = -16;
  myframe->pictsetting.GRed = 256;
  myframe->pictsetting.GBlue = 256;
  myframe->pictsetting.GGreen = 256;
  myframe->pictsetting.force_rgb = 0;
  myframe->cropx1 = myframe->cropx2 = myframe->cropy1 = myframe->cropy2 = 0;

}

void
free_spcagui (void)
{
  free (myvidIn.videodevice);
  free (myvidIn.cameraname);
  free (myvidIn.bridge);
  free_potentiometre (&color);
  free_potentiometre (&contrast);
  free_potentiometre (&bright);
  free_button (&mybutton[PHOTO]);
  free_button (&mybutton[AVI]);
  free_button (&mybutton[PAUSE]);
  free_button (&mybutton[GRAB]);
  free_button (&mybutton[QUIT]);
  free_button (&mybutton[FLIPRGB]);
  free_control (&palette);
  free_control (&size);
  close_v4l (&myvidIn);
}

int
refresh_screen (struct spca5xx_frame *myframe, struct vdIn *vd,
		SDL_Surface * Screen)
{

  if ((myframe->hdrwidth != vd->hdrwidth) || (myframe->depth != vd->bppIn))
    {
      /* adjust parameters and copy data in -> out */
     // printf ("something change !! \n");
      myframe->hdrwidth = myframe->width = vd->hdrwidth;
      myframe->hdrheight = myframe->height = vd->hdrheight;
      myframe->depth = vd->bppIn;
      myframe->format = VIDEO_PALETTE_RGB24;
      return -1;
    }
  memcpy (myframe->tmpbuffer, vd->pixTmp, vd->framesizeIn);
  if (SDL_MUSTLOCK (Screen))
    {
      if (SDL_LockSurface (Screen) < 0)
	return -1;
    }
  switch (vd->formatIn)
    {
    case VIDEO_PALETTE_RAW_JPEG:
      {

	//printf("outjpeg w %d h %d\n",myframe->width,myframe->height);
	spca50x_outpicture (myframe);

      }
      break;

    case VIDEO_PALETTE_YUV420P:
      {
	// uncompressed data yuv420P decoder in module
	//printf("outyuv w %d h %d bpp %d\n",myframe->width,myframe->height,vd->bppIn >> 3);
	YUV420toRGB (myframe->tmpbuffer,
		     myframe->data, myframe->width, myframe->height, 0, 0);

      }
      break;
    case VIDEO_PALETTE_RGB565:
    case VIDEO_PALETTE_RGB32:
    case VIDEO_PALETTE_RGB24:
      {
	// uncompressed data. rgb decoder in module simple copy for display.
	//printf("outrgb w %d h %d bpp %d\n",myframe->width,myframe->height,vd->bppIn >> 3);
	memcpy (myframe->data, myframe->tmpbuffer,
		(myframe->width * myframe->height * vd->bppIn) >> 3);
      }
      break;
    default:
      break;
    }
  if (SDL_MUSTLOCK (Screen))
    {
      SDL_UnlockSurface (Screen);
    }
//SDL_Flip(Screen);
  return 0;
}

void
telecom (void)
{
  int run = 1;
  int telelock = 0;
  SDL_Surface *screen;
  int tmp;
  Uint32 video_flags, lib_flags;
  SDL_Event event;
  unsigned char message[2];
  close (fd[0]);		/* Close the read descriptor */
  lib_flags = SDL_INIT_VIDEO;
  //video_flags = (SDL_HWPALETTE | SDL_DOUBLEBUF | SDL_NOFRAME);
  video_flags = (SDL_HWPALETTE | SDL_DOUBLEBUF);
  int avi = 1;
  int pause = 1;
  int mousecolors = 0;
  int mousebright = 0;
  int mousecontrast = 0;
  int mousey = 0;
  int ycour = 0;
  if (SDL_Init (lib_flags) < 0)
    {
      fprintf (stderr, "Couldn't initialize SDL: %s\n", SDL_GetError ());
      exit (EXIT_FAILURE);
    }
  putenv ("SDL_VIDEO_WINDOW_POS=10,10");  
  screen = SDL_SetVideoMode (TELECOM_X, TELECOM_Y, 0, video_flags);

  SDL_ShowCursor (1);
  SDL_WM_SetCaption ("Control", "Control");
  draw_button (&mybutton[PHOTO], screen);
  draw_button (&mybutton[AVI], screen);
  draw_button (&mybutton[PAUSE], screen);
  draw_button (&mybutton[GRAB], screen);
  draw_button (&mybutton[QUIT], screen);
  draw_button (&mybutton[FLIPRGB], screen);
  draw_potentiometre (&color, screen);
  draw_potentiometre (&contrast, screen);
  draw_potentiometre (&bright, screen);
  draw_control (&size, screen);
  draw_control (&palette, screen);
  SDL_Flip (screen);


  while ((SDL_WaitEvent (&event) >= 0) && (run))
    {
    memset (message, 0x00, sizeof (message));
    	if (event.type == SDL_MOUSEMOTION)
	{
	int x, y;
	  SDL_GetMouseState (&x, &y);
	  if ( y > 32 && y < 192){
	  
	  	if ( x > 64 ) {
	  		if(mousecolors){
				ycour = y - mousey;
				if ((ycour > 2) || (ycour < -2 )){
					mousey = mousey + ycour;
			 		process_potentiometre (mousey, &color, screen, message);
	 			}
			}
	  
	 	 } else if (x > 32) {
		 	if(mousecontrast){
				ycour = y - mousey;
				if ((ycour > 2) || (ycour < -2 )){
					mousey = mousey + ycour;
			 		process_potentiometre (mousey, &contrast, screen, message);
				}
			}
	  	 } else {
	  		if(mousebright){
				ycour = y - mousey;
				if ((ycour > 2) || (ycour < -2 )){
					mousey = mousey + ycour;
			 		process_potentiometre (mousey, &bright, screen, message);
				}
			
	  		}	
	  
	 	}
	}
	}
    	if (event.type == SDL_MOUSEBUTTONUP)
	{
	 // printf ("reset smooth potentiometers \n");
	 mousecolors = 0;
  	 mousebright = 0;
	 mousecontrast = 0;
	}
      /* Determine if a button has been clicked */
      if (event.type == SDL_MOUSEBUTTONDOWN)
	{
	  int x, y;
	  SDL_GetMouseState (&x, &y);
	  
	  if (y > 32)
	    {
	      if (avi && pause)
		{
		  if (x > 96)
		    {
		      /* here we are in zone 3 if avi run don't send anyway */
		      if (x > 128)
			{
			  if (y > 160)
			    {

			      /* process rgb button */
			      //process_button (1, &mybutton[FLIPRGB],screen,message);
			    }
			  else if (y > 128)
			    {
			      /* process about button */
			    }
			  else
			    {
			      /* process palette control is there a change ? */
			      tmp = palette.courant;
			      process_control (y, &palette, screen, message);
			      write (fd[1], message, 2);
			      /* if yes change size setting to native */
			     
				if (palette.courant != tmp){
					if(palette.courant == 0x01){
					 refresh_control (myvidIn.sizenative, &size,
						   screen, message);
					} else {
					refresh_control (myvidIn.sizeothers, &size,
						   screen, message);
					}
				}
			    }
			}
		      else
			{
			  /* process size box */
			  process_control (y, &size, screen, message);
			}
		    }
		  else
		    {
		      /* here we are in pot area */
		      if (x > 64)
			{
			  /* Color pot */
			process_potentiometre (y, &color, screen, message);
			mousecolors = 1;
			mousey = event.button.y;
			}
		      else if (x > 32)
			{
			  /* Contrast pot */
			  process_potentiometre (y, &contrast, screen,
						 message);
			mousecontrast = 1;
			mousey = event.button.y;			
			}
		      else
			{
			  /* bright pot */
			  process_potentiometre (y, &bright, screen, message);
			  mousebright = 1;
			  mousey = event.button.y;
			}

		    }
		}
	    }
	  else
	    {
	      /* here we are in cmd area */
	      if (x > 128)
		{
		  if (avi)
		    {
		      process_button (1, &mybutton[QUIT], screen, message);
		      run = 0;
		      printf ("ByeBye !! \n");
		    }
		  else
		    {
		      printf ("Please Close avi first to quit !! \n");
		    }
		}
	      else if (x > 96)
		{
		  //process_button (1, &mybutton[GRAB],screen,message);
		  process_button (1, &mybutton[FLIPRGB], screen, message);
		}
	      else if (x > 64)
		{
		  process_button (1, &mybutton[PAUSE], screen, message);
		  pause = !pause;
		}
	      else if (x > 32)
		{
		  process_button (1, &mybutton[AVI], screen, message);
		  avi = !avi;
		}
	      else
		{
		  if (avi)
		    process_button (0, &mybutton[PHOTO], screen, message);
		}
	    }
	  
	}
	if (message[0] != 0)
	    write (fd[1], message, 2);
	  //sleep(1);
    }
  close (fd[1]);
  SDL_Quit ();
}

void
processvideo (void)
{
  SDL_Surface *screen;
  Uint32 video_flags, lib_flags;
  SDL_Event event;
  struct spca5xx_frame *myframe;
  lib_flags = SDL_INIT_VIDEO;
  video_flags = (SDL_HWPALETTE | SDL_DOUBLEBUF);
  int size = 0;
  unsigned char buf[2];

  int len;
  int run = 1;
  int change = 0;
  int pause = 0;
  int avi = 0;
  int fliprgb = 0;
  int photo = 0;

  close (fd[1]);		/* Close the write descriptor */
  if (SDL_Init (lib_flags) < 0)
    {
      fprintf (stderr, "Couldn't initialize SDL: %s\n", SDL_GetError ());
      exit (EXIT_FAILURE);
    }
     putenv ("SDL_VIDEO_CENTERED=1");  
  screen = SDL_SetVideoMode (myvidIn.hdrwidth, myvidIn.hdrheight,
			     0, video_flags);
  SDL_ShowCursor (1);
  //SDL_WM_SetCaption ("SpcaGui MX@2004", "SpcaGui");
  SDL_WM_SetCaption (myvidIn.cameraname, "SpcaGui");
  /* allocate the output frame struct */
  myframe = (struct spca5xx_frame *) malloc (sizeof (struct spca5xx_frame));
  myframe->data = screen->pixels;
  myframe->cameratype = myvidIn.cameratype;
  /* output data definition */
  myframe->width = myvidIn.hdrwidth;
  myframe->height = myvidIn.hdrheight;
  /* output format */
  myframe->format = VIDEO_PALETTE_RGB24;
  /* allocate a big frame buffer */
  size = myframe->width * myframe->height * 4;
  myframe->tmpbuffer = (unsigned char *) malloc (size);

  /* input data maybe a jpeg one */

  if (myvidIn.cameratype == JPEG)
    {
      init_video (myframe);
      myframe->method = 0;
      myvidIn.formatIn = VIDEO_PALETTE_RAW_JPEG;
    }
  else
    {
    /* myvidIn.formatIn is set by initialize don't touch 
    you should use the remote command instead */
     // myvidIn.formatIn = VIDEO_PALETTE_RGB24;
    }

  setPalette (&myvidIn);


  while (run)
    {
      if (!pause)
	{
	  SpcaGrab (&myvidIn);
	}
      if (photo)
	{
	  photo = 0;
	  getPicture (myvidIn.pixTmp, myvidIn.hdrwidth,
		      myvidIn.hdrheight, myvidIn.formatIn,
		      myvidIn.framesizeIn);
	}

      if (avi)
	{
	  aviWrite (&myvidIn);
	}

      if (refresh_screen (myframe, &myvidIn, screen) < 0)
	{
	  if ((myvidIn.formatIn == VIDEO_PALETTE_YUV420P)
	      || (myvidIn.formatIn == VIDEO_PALETTE_RAW_JPEG))
	    {
	      change = 24;	//RGB24 output
	    }
	  else
	    {
	      change = myframe->depth;
	    }
	 // printf ("setting sdl w %d h %d bpp %d\n", myframe->width,
	 //	  myframe->height, change);
	  screen =
	    SDL_SetVideoMode (myframe->width, myframe->height, change,
			      video_flags);
	  myframe->data = screen->pixels;
	}
      else
	{
	  SDL_Flip (screen);
	}
     
      if ((len = read (fd[0], buf, 2)) > 0)
	{
	 // printf ("get message id %d value %d \n", buf[0], buf[1]);
	  switch (buf[0])
	    {
	    case 1:		/* take a picture only if !avi */
	      if (!avi)
		{
		  /* set picture flag */
		  photo = 1;
		}
	      break;
	    case 2:		/* avi start stop */
	      avi = buf[1];
	      int err = 0;
	      if (avi)
		{
		  err = aviOpen (&myvidIn);
		  if (err < 0)
		    avi = 0;
		}
	      else
		{
		  aviClose ();
		}
	      break;
	    case 3:		/* pause */
	      pause = buf[1];
	      break;
	    case 4:
	      break;
	    case 5:		/* if avi run close avi first */
	      run = 0;
	      break;
	    case 6:		/* maybe flip rgb */
	      fliprgb = buf[1];
	      myvidIn.flipUV = fliprgb;
	      break;
	    case 7:
	      SpcaSetBrightness (&myvidIn, buf[1]);
	      break;
	    case 8:
	      SpcaSetContrast (&myvidIn, buf[1]);
	      break;
	    case 9:
	      SpcaSetColors (&myvidIn, buf[1]);
	      break;
	    case 10:
	      {
		switch (buf[1])
		  {
		  case 1:
		    myvidIn.hdrwidth = 640;
		    myvidIn.hdrheight = 480;
		    break;
		  case 2:
		    myvidIn.hdrwidth = 384;
		    myvidIn.hdrheight = 288;
		    break;
		  case 4:
		    myvidIn.hdrwidth = 352;
		    myvidIn.hdrheight = 288;
		    break;
		  case 8:
		    myvidIn.hdrwidth = 320;
		    myvidIn.hdrheight = 240;
		    break;
		  case 16:
		    myvidIn.hdrwidth = 192;
		    myvidIn.hdrheight = 144;
		    break;
		  case 32:
		    myvidIn.hdrwidth = 176;
		    myvidIn.hdrheight = 144;
		    break;
		  case 64:
		    myvidIn.hdrwidth = 160;
		    myvidIn.hdrheight = 120;
		    break;
		  default:
		    break;
		  }
		changeSize (&myvidIn);
	      }
	      break;
	    case 11:
	      {
		switch (buf[1])
		  {
		  case 1:
		    myvidIn.formatIn = VIDEO_PALETTE_RAW_JPEG;
		    break;
		  case 2:
		    myvidIn.formatIn = VIDEO_PALETTE_YUV420P;
		    break;
		  case 4:
		    myvidIn.formatIn = VIDEO_PALETTE_RGB565;
		    break;
		  case 8:
		    myvidIn.formatIn = VIDEO_PALETTE_RGB24;
		    break;
		  case 16:
		    myvidIn.formatIn = VIDEO_PALETTE_RGB32;
		    break;
		  default:
		    break;
		  }
		setPalette (&myvidIn);
	      }
	      break;
	    default:
	      break;
	    }
	}
      // sleep(1);
    }
  close (fd[0]);		/* Close the file descriptor, we're done */
  sleep (1);
  free (myframe->tmpbuffer);
  free (myframe);
  SDL_Quit ();
}
