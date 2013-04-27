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

#include "gui.h"
#include "config.h"

static const char curseur[] = PATHSPCA "curseur.png";
static const char pvalid[] = PATHSPCA "pvalid.png";
static const char pactif[] = PATHSPCA "pactif.png";
static const char pvide[] = PATHSPCA "pvide.png";

static void
draw_surface (int x, int y, SDL_Surface * surface, SDL_Surface * screen)
{
  SDL_Rect dest;
  dest.x = x;
  dest.y = y;
  dest.w = surface->w;
  dest.h = surface->h;
  SDL_BlitSurface (surface, NULL, screen, &dest);
  SDL_UpdateRect(screen,dest.x,dest.y,dest.w,dest.h);

}

void
init_button (int id, int value, int x, int y, struct button *mybutt,
	     const char *release, const char *press)
{				/* init pot */
  mybutt->id = id;
  mybutt->x = x;
  mybutt->y = y;
  mybutt->toggle = value;
  if (release)
    mybutt->iconrelease = IMG_Load (release);
  if (press)
    mybutt->iconpress = IMG_Load (press);
}

void
free_button (struct button *mybutt)
{
  SDL_FreeSurface (mybutt->iconrelease);
  SDL_FreeSurface (mybutt->iconpress);
}

void
draw_button (struct button *mybutt, SDL_Surface * Screen)
{
  if (mybutt->toggle)
    {
      draw_surface (mybutt->x, mybutt->y, mybutt->iconpress, Screen);
    }
  else
    {
      draw_surface (mybutt->x, mybutt->y, mybutt->iconrelease, Screen);
    }

}

void
process_button (int value, struct button *mybutt, SDL_Surface * Screen,
		unsigned char *message)
{

  mybutt->toggle = !mybutt->toggle;
  SDL_Delay (200);
  draw_button (mybutt, Screen);
//  SDL_Flip (Screen);
  message[0] = mybutt->id;
  message[1] = mybutt->toggle;

  if (!value)
    {
      SDL_Delay (500);
      mybutt->toggle = !mybutt->toggle;
      draw_button (mybutt, Screen);
   //   SDL_Flip (Screen);
    }

}

void
init_potentiometre (int id, int value, int x, int y,
		    struct potentiometre *mypot, const char *fond)
{				/* init pot */
  mypot->id = id;
  mypot->x = x;
  mypot->y = y;
  mypot->value = value;
  mypot->fond = IMG_Load (fond);
  mypot->curseur = IMG_Load (curseur);
}

void
free_potentiometre (struct potentiometre *mypot)
{
  SDL_FreeSurface (mypot->fond);
  SDL_FreeSurface (mypot->curseur);
}

void
draw_potentiometre (struct potentiometre *mypot, SDL_Surface * Screen)
{ /* add 16 center the pot shift , -6 put hot point to middle + */
  int i;
  i = (mypot->value * 128) >> 8;
  mypot->y0 = (mypot->y + 128+16) - (i);
  /* first draw bakground */
  draw_surface (mypot->x, mypot->y, mypot->fond, Screen);
  /* then cursor */
  draw_surface (mypot->x, mypot->y0-6, mypot->curseur, Screen);
}

void
process_potentiometre (int yp, struct potentiometre *mypot,
		       SDL_Surface * Screen, unsigned char *message)
{ /* add 16 center the pot shift */
  int i;
  SDL_Delay (200);
  i = 128+16 - (yp - mypot->y);
  /* CLIP the value */
  if (i < 0) i = 0;
  if (i > 127) i = 127;
  mypot->value = (i << 8) / 128;
  //printf("value %d\n",mypot->value);
  draw_potentiometre (mypot, Screen);
  message[0] = mypot->id;
  message[1] = mypot->value;

//  SDL_Flip (Screen);
}

void
init_control (int id, int value, int nb, int x, int y,
	      struct control *mycontrol, const char *myfond)
{
  unsigned char masq = 0x01;
  int i;
  mycontrol->id = id;
  mycontrol->nbpuce = nb;
  mycontrol->x = x;
  mycontrol->y = y;
  mycontrol->available = value;
  mycontrol->pucevalid = IMG_Load (pvalid);
  mycontrol->puceactive = IMG_Load (pactif);
  mycontrol->pucevide = IMG_Load (pvide);
  mycontrol->fond = IMG_Load (myfond);
  /* find a default mode */
  for (i = 0; i < 8; i++)
    {
      if ((value & masq) > 0)
	{
	  mycontrol->courant = masq;
	  //printf("courant %d\n",mycontrol->courant);
	  break;
	}
      masq = masq << 1;

    }

}

void
free_control (struct control *mycontrol)
{
  SDL_FreeSurface (mycontrol->fond);
  SDL_FreeSurface (mycontrol->pucevalid);
  SDL_FreeSurface (mycontrol->puceactive);
  SDL_FreeSurface (mycontrol->pucevide);
}

void
draw_control (struct control *mycontrol, SDL_Surface * Screen)
{
  unsigned char masq = 0x01;
  int end_h, posx, posy;
  int starty = 20;
  int i;
  /* first draw bakgraound */
  draw_surface (mycontrol->x, mycontrol->y, mycontrol->fond, Screen);

  posy = mycontrol->y + starty;
  end_h = mycontrol->fond->h + posy - 16;	// (32+16)
  posx = mycontrol->x + 3;
  for (i = 0; i < mycontrol->nbpuce; i++)
    {
      if ((masq & mycontrol->courant) > 0)
	{
	  draw_surface (posx, posy, mycontrol->puceactive, Screen);
	}
      else if ((masq & mycontrol->available) > 0)
	{
	  draw_surface (posx, posy, mycontrol->pucevalid, Screen);
	}
      else
	{
	  draw_surface (posx, posy, mycontrol->pucevide, Screen);
	}
      posy += 16;
      if (posy >= end_h)
	break;
      masq = masq << 1;

    }
}

void
process_control (int yp, struct control *mycontrol, SDL_Surface * Screen,
		 unsigned char *message)
{
  unsigned char masq = 0x01;
  unsigned char maxmasq = 0x01;

  int i, yn;
  int posy, posy1, posx;
  int starty = 20;
  /* debounce delay */
  SDL_Delay (200);
  /* initialize */
  posx = mycontrol->x + 3;
  /* find courant */
  for (i = 0; i < mycontrol->nbpuce; i++)
    {
      if ((mycontrol->courant & maxmasq) > 0)
	break;
      maxmasq = maxmasq << 1;
    }
  posy = (mycontrol->y) + starty + (i << 4);
  /* test if mouse on actif return */
  yn = (yp - (mycontrol->y + starty)) >> 4;
  //printf ("y value: %d \n",yn);
  posy1 = mycontrol->y + starty + (yn << 4);
  masq = masq << yn;
  /* if nothing todo return 0 */
  message[0] = 0;//mycontrol->id;
  message[1] = 0;//mycontrol->courant;
  if (mycontrol->courant == masq)
    return;
  /* if mouse not actif are they valid */
  if ((mycontrol->available & masq) > 0)
    {
      /* actualize */
      mycontrol->courant = masq;
      message[0] = mycontrol->id;
      message[1] = mycontrol->courant;
      draw_surface (posx, posy, mycontrol->pucevalid, Screen);

      draw_surface (posx, posy1, mycontrol->puceactive, Screen);
    }
  else
    {
      /*nothing todo go back */
      return;
    }
//  SDL_Flip (Screen);
}

/* function not on even context */
void
refresh_control (int value, struct control *mycontrol, SDL_Surface * Screen,
		 unsigned char *message)
{
  unsigned char masq = 0x01;
  int i;
  mycontrol->available = value;
  if (!(mycontrol->courant & value))
    {
      /* find a new match */
      for (i = 0; i < 8; i++)
	{
	  if ((value & masq) > 0)
	    {
	      mycontrol->courant = masq;
	      //printf("courant %d\n",mycontrol->courant);
	      break;
	    }
	  masq = masq << 1;

	}
    }
  draw_control (mycontrol, Screen);
  message[0] = mycontrol->id;
  message[1] = mycontrol->courant;
//  SDL_Flip (Screen);
}
