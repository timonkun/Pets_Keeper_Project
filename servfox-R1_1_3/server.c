/****************************************************************************
#	 	spcaserv: TCP/IP mjpeg stream server .		            #
#This package work with the spca5xx based webcam with the raw jpeg feature. #
#All the decoding is in user space with the help of libjpeg.                #
#.                                                                          #
# 		Copyright (C) 2003 2004 2005 Michel Xhaard                  #
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include "spcaframe.h"
//#include "spcav4l.h"
#include "v4l2uvc.h"
#include "utils.h"
#include "tcputils.h"
#include "version.h"
#include "color.h"

int getpictureflag = 0;

static int debug = 1;
void grab(void);
void service(void *ir);
void sigchld_handler(int s);
struct vdIn *videoIn;

int main(int argc, char *argv[])
{

	char *videodevice = NULL;

	int grabmethod = 1;
	int format = V4L2_PIX_FMT_MJPEG;
	int fps = 15;
	int width = 352;
	int height = 288;
	char *avifilename = NULL;
	char *separateur;
	char *sizestring = NULL;
	char *fpsstring  = NULL;
	int i;
	int serv_sock, new_sock;
	pthread_t w1;
	pthread_t server_th;
	int sin_size;
	unsigned short serverport = 7070;
	struct sockaddr_in their_addr;
	struct sigaction sa;
	
	int time_delay = 0;
	int record_time = 0;
	unsigned int picture_count = 0;
	unsigned char frmrate = 0;
	int querycontrols = 0;
	int queryformats = 0;
	int enableRawStreamCapture = 0;
	int enableRawFrameCapture = 0;

	
	for (i = 1; i < argc; i++) {
		/* skip bad arguments */
		if (argv[i] == NULL || *argv[i] == 0 || *argv[i] != '-') {
			continue;
		}
		if (strcmp(argv[i], "-d") == 0) {
			if (i + 1 >= argc) {
				if (debug)
					printf
					    ("No parameter specified with -d, aborting.\n");
				exit(1);
			}
			videodevice = strdup(argv[i + 1]);
		}
		if (strcmp(argv[i], "-g") == 0) {
			/* Ask for read instead default  mmap */
			grabmethod = 0;
		}

		if (strcmp(argv[i], "-i") == 0){
		  if (i + 1 >= argc) {
		    printf("No parameter specified with -i, aborting. \n");
		    exit(1);
		  }
		  fpsstring = strdup(argv[i + 1]);
		  fps = strtoul(fpsstring, &separateur, 10);
		  printf(" interval: %d fps \n", fps);
		}

		if (strcmp(argv[i], "-s") == 0) {
			if (i + 1 >= argc) {
				if (debug)
					printf
					    ("No parameter specified with -s, aborting.\n");
				exit(1);
			}

			sizestring = strdup(argv[i + 1]);

			width = strtoul(sizestring, &separateur, 10);
			if (*separateur != 'x') {
				if (debug)
					printf
					    ("Error in size use -s widthxheight \n");
				exit(1);
			} else {
				++separateur;
				height = strtoul(separateur, &separateur, 10);
				if (*separateur != 0)
					if (debug)
						printf
						    ("hmm.. dont like that!! trying this height \n");
				if (debug)
					printf(" size width: %d height: %d \n",
					       width, height);
			}
		}
		if (strcmp(argv[i], "-o") == 0) {
	        /* set the avi filename */
	        if (i + 1 >= argc) {
	        printf("No parameter specified with -o, aborting.\n");
	        exit(1);
	        }
	        avifilename = strdup(argv[i + 1]);
	    }

	    if (strcmp(argv[i], "-O") == 0) {
		    /* get picture */
		    getpictureflag = 1;
		}
		
		if (strcmp(argv[i], "-w") == 0) {
			if (i + 1 >= argc) {
				if (debug)
					printf
					    ("No parameter specified with -w, aborting.\n");
				exit(1);
			}
			serverport = (unsigned short)atoi(argv[i + 1]);
			if (serverport < 1024) {
				if (debug)
					printf
					    ("Port should be between 1024 to 65536 set default 7070 !.\n");
				serverport = 7070;
			}
		}
		if (strcmp(argv[i], "-l") == 0) {
		    /* query list of valid video formats */
		    querycontrols = 1;
		}
		if (strcmp(argv[i], "-L") == 0) {
		    /* query list of valid video formats */
		    queryformats = 1;
		}
		if (strcmp(argv[i], "-c") == 0) {
			/* Enable raw frame capture for the first frame */
			enableRawFrameCapture = 1;
		}
		if (strcmp(argv[i], "-C") == 0) {
			/* Enable raw frame stream capture from the start*/
			enableRawFrameCapture = 2;
		}

		if (strcmp(argv[i], "-h") == 0) {
			printf("usage: servfox [-h -d -g ] \n");
			printf("-h	print this message \n");
			printf("-c	enable raw frame capturing for the first frame\n");
		    printf("-C	enable raw frame stream capturing from the start\n");
			printf("-d	/dev/videoX       use videoX device\n");
			printf("-g	use read method for grab instead mmap \n");
			printf("-i	fps 		  use specified frame interval \n");
			printf("-l	query valid controls and settings\n");
			printf("-L	query valid video formats\n");
			printf("-o	avifile  create avifile, default video.avi\n");
			printf("-O	get picture.\n");
			printf("-s	widthxheight      use specified input size \n");
			printf("-w	port      server port \n");

			exit(0);
		}
	}
	/* main code */

	printf(" %s \n", version);
	if (videodevice == NULL || *videodevice == 0) {
		videodevice = "/dev/video0";
	}
	
	if (avifilename == NULL || *avifilename == 0) {
			avifilename = "video.avi";
		}

	//memset(videoIn, 0, sizeof(struct vdIn));
	videoIn = (struct vdIn *) calloc(1, sizeof(struct vdIn));

	if ( queryformats ) {
		 /* if we're supposed to list the video formats, do that now and go out */
			check_videoIn(videoIn,(char *) videodevice);
			free(videoIn);
			exit(1);
	}

	if (init_videoIn(videoIn, videodevice, width, height, fps, format, 
	    	grabmethod, avifilename) != 0)
	{
		if (debug)
			printf(" damned encore rate !!\n");

		free(videoIn);
		exit(1);
	}
	// if(debug) printf("depth %d",videoIn->bppIn);  

	/* if we're supposed to list the controls, do that now */
    if ( querycontrols )
        enum_controls(videoIn->fd);
        
	if (enableRawStreamCapture) {
			videoIn->captureFile = fopen("stream.raw", "wb");
			if(videoIn->captureFile == NULL) {
				perror("Unable to open file for raw stream capturing");
			} 
		else{
				printf("Starting raw stream capturing to stream.raw ...\n");
			}
	}
	
	if (enableRawFrameCapture)
		videoIn->rawFrameCapture = enableRawFrameCapture;
		

	initLut();   // from luvcview

	////////////luvcview/////////////////////////////
	/* main big loop */
    while (videoIn->signalquit) {
		//
		if (uvcGrab(videoIn) < 0) {
		    printf("Error grabbing \n");
		    break;
		}
		//
	    /* if we're grabbing video, show the frame rate */
	    if (videoIn->toggleAvi)
	        printf("\rframe rate: %d ",frmrate);
		//
		
		if(getpictureflag){
		//if (videoIn->getPict) { 
			switch(videoIn->formatIn){
				case V4L2_PIX_FMT_MJPEG:
					get_picture(videoIn->tmpbuffer,videoIn->buf.bytesused);
					break;
				case V4L2_PIX_FMT_YUYV:
					printf("get picture yuv...\n");
					get_pictureYV2(videoIn->framebuffer,videoIn->width,videoIn->height);
					break;
				default:
					break;
			}
			videoIn->getPict = 0;
			printf("get picture %d!\n", picture_count++);
			record_time -= time_delay;
			if(record_time < 0) {
				printf("picture_count=%d\ntimes up, close.\n");
				exit(0);	// times up, jump out.
			}
			sleep(time_delay);
		}
    }

    /* if avifile is defined, we made a video: compute the exact fps and
       set it in the video */
    if (videoIn->avifile != NULL) {
        float fps=(videoIn->framecount/(videoIn->recordtime/1000));
        fprintf(stderr,"setting fps to %f\n",fps);
        AVI_set_video(videoIn->avifile, videoIn->width, videoIn->height,
            fps, "MJPG");
        AVI_close(videoIn->avifile);
    }
	////////////luvcview/////////////////////////////

#if 0
	pthread_create(&w1, NULL, (void *)grab, NULL);

	serv_sock = open_sock(serverport);
	signal(SIGPIPE, SIG_IGN);	/* Ignore sigpipe */

	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	syslog(LOG_ERR, "Spcaserv Listening on Port  %d\n", serverport);
	printf("Waiting .... for connection. CTrl_c to stop !!!! \n");
	while (videoIn->signalquit) {
		sin_size = sizeof(struct sockaddr_in);
		if ((new_sock =
		     accept(serv_sock, (struct sockaddr *)&their_addr,
			    &sin_size)) == -1) {
			continue;
		}
		syslog(LOG_ERR, "Got connection from %s\n",
		       inet_ntoa(their_addr.sin_addr));
		printf("Got connection from %s\n",
		       inet_ntoa(their_addr.sin_addr));
		pthread_create(&server_th, NULL, (void *)service, &new_sock);
	}
	pthread_join(w1, NULL);

	close(serv_sock);
#endif
	close_v4l2(videoIn);
	free(videoIn);
	freeLut();
	
	return 0;
}

void grab(void)
{
	int err = 0;
	for (;;) {
		//if(debug) printf("I am the GRABBER !!!!! \n");
		err = uvcGrab(videoIn); //v4lGrab(&videoIn);
		if (!videoIn->signalquit || (err < 0)) {
			if (debug)
				printf("GRABBER going out !!!!! \n");
			break;
		}
	}
}

void service(void *ir)
{
	int *id = (int *)ir;
	int frameout = 1;
	struct frame_t *headerframe;
	int ret;
	int sock;
	int ack = 0;
	unsigned char wakeup = 0;
	unsigned short bright;
	unsigned short contrast;
	struct client_t message;
	sock = *id;
	// if(debug) printf (" \n I am the server %d \n", *id);
	/* initialize video setting */
	//bright = upbright(videoIn);
	//contrast = upcontrast(videoIn);
	//bright = downbright(videoIn);
	//contrast = downcontrast(videoIn);
	for (;;) {
		memset(&message, 0, sizeof(struct client_t));
		ret =
		    read(sock, (unsigned char *)&message,
			 sizeof(struct client_t));
		//if(debug) printf("retour %s %d ret\n",message.message,ret);
		if (ret < 0) {
			if (debug)
				printf(" Client vaporished !! \n");
			break;
		}
		if (!ret)
			break;
		if (ret && (message.message[0] != 'O'))
			break;

		if (message.updobright) {
			switch (message.updobright) {
			case 1:
				//bright = upbright(videoIn);
				break;
			case 2:
				//bright = downbright(videoIn);
				break;
			}
			ack = 1;
		} else if (message.updocontrast) {
			switch (message.updocontrast) {
			case 1:
				//contrast = upcontrast(videoIn);
				break;
			case 2:
				//contrast = downcontrast(videoIn);
				break;
			}
			ack = 1;
		} else if (message.updoexposure) {
			switch (message.updoexposure) {
			case 1:
				//spcaSetAutoExpo(videoIn);
				break;
			case 2:;
				break;
			}
			ack = 1;
		} else if (message.updosize) {	//compatibility FIX chg quality factor ATM
			switch (message.updosize) {
			case 1:
				//qualityUp(videoIn);
				break;
			case 2:
				//qualityDown(videoIn);
				break;
			}
			ack = 1;
		} else if (message.fps) {
			switch (message.fps) {
			case 1:
				//timeDown(videoIn);
				break;
			case 2:
				//timeUp(videoIn);
				break;
			}
			ack = 1;
		} else if (message.sleepon) {

			ack = 1;
		} else
			ack = 0;
		while ((frameout == videoIn->frame_cour) && videoIn->signalquit)
			usleep(1000);
		if (videoIn->signalquit) {
			videoIn->framelock[frameout]++;
			headerframe =
			    (struct frame_t *)videoIn->ptframe[frameout];
			//headerframe->nbframe = framecount++;
			//if(debug) printf ("reader %d key %s width %d height %d times %dms size %d \n", sock,headerframe->header,
			//headerframe->w,headerframe->h,headerframe->deltatimes,headerframe->size);
			headerframe->acknowledge = ack;
			headerframe->bright = bright;
			headerframe->contrast = contrast;
			headerframe->wakeup = wakeup;
			/**ret =
			    write_sock(sock, (unsigned char *)headerframe,
				       sizeof(struct frame_t));

			if (!wakeup)
				ret = write_sock(sock,
					   (unsigned char *)(videoIn->ptframe[frameout] +sizeof(struct frame_t)),
					    headerframe->size);
			**/
			// ret = write_sock(sock,(unsigned char*)(videoIn->ptframe[frameout]+sizeof(struct frame_t)),headerframe->size);

			videoIn->framelock[frameout]--;
			frameout = (frameout + 1) % 4;
		} else {
			if (debug)
				printf("reader %d going out \n", *id);
			break;
		}
	}
	close_sock(sock);
	pthread_exit(NULL);
}

void sigchld_handler(int s)
{
	videoIn->signalquit = 0;
}
