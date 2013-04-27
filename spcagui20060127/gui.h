 
#ifndef TELECOM_H
#define TELECOM_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <wait.h>
#include <limits.h>
#include <SDL/SDL.h>
#include "SDL_image.h"

#define TELECOM_X 160
#define TELECOM_Y 192

enum {
	PHOTO = 1,
	AVI,
	PAUSE,
	GRAB,
	QUIT,
	FLIPRGB,
	TOTAL_ACTIONS
};
enum {
	BRIGHT = TOTAL_ACTIONS,
	CONTRAST,
	COLOR,
	SIZE,
	PALETTE,
	END_CONTROL
};


struct button {
	int id;
	int x;
	int y;
	int toggle;
	SDL_Surface *iconrelease;
	SDL_Surface *iconpress;
};

struct potentiometre {
	int id;
	int x;
	int y;
	int value;
	int y0;
	SDL_Surface *fond;
	SDL_Surface *curseur;
};

struct control {
	int id;
	int x;
	int y;
	int nbpuce;
	int available;
	int valid;
	int courant;
	SDL_Surface *fond;
	SDL_Surface *pucevalid;
	SDL_Surface *puceactive;
	SDL_Surface *pucevide;
};
/* button if value is set to 1 oneshoot button otherwhise toggle */
void 
init_button ( int id, int value,int x,int y,
		struct button *mybutt,
		const char *release,const char *press);
void 
free_button ( struct button *mybutt);
void 
draw_button ( struct button *mybutt,SDL_Surface *Screen);
void 
process_button (int value ,struct button *mybutt,
		SDL_Surface *Screen, unsigned char *message);
/* value is between 0 to 255 */		
void 
init_potentiometre ( int id ,int value,int x,int y,
		struct potentiometre *mypot,const char *fond);	
void 
free_potentiometre (struct potentiometre *mypot);
void 
draw_potentiometre(struct potentiometre *mypot, SDL_Surface *Screen);
void 
process_potentiometre(int yp, struct potentiometre *mypot, 
		SDL_Surface *Screen, unsigned char *message);
/* control box nb is number of puce maximum is 8
	value is available with 2^ if active red available green not available grey */		
void 
init_control (int id,int value,int nb, int x, int y,
		 struct control *mycontrol,const char *myfond); 
void
free_control (struct control *mycontrol);
void 
draw_control (struct control *mycontrol,SDL_Surface *Screen);
void 
process_control (int yp,struct control *mycontrol,
		SDL_Surface *Screen,unsigned char *message);
void 
refresh_control( int value, struct control *mycontrol,
		SDL_Surface *Screen,unsigned char *message);





	
#endif /* TELECOM_H */
