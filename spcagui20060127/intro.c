#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include "SFont.h"
#include "SDL_image.h"
#include "config.h"

static const char Fhelvet[] = PATHSPCA "721_helvet_or.png";
static const char Fcopper[] = PATHSPCA "24P_Copperplate_Blue.png";
static const char Fbigfont[] = PATHSPCA "BigFont.png";
static const char Farial[] = PATHSPCA "24P_Arial_NeonYellow.png";

void
intro (void)
{
  SDL_Surface *screen;
  SDL_Event sdlevent;
  Uint32 rgbmap;
  SFont_Font *Neon;
  SFont_Font *Copper;
  SFont_Font *Blue;
  SFont_Font *Sylvie;
  char tabcar[10] = "0123456789";
  char Fstring = '\0';
  char sortab[80];
  int i, j;
  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      fprintf (stderr, "Couldn't initialize SDL: %s\n", SDL_GetError ());
      exit (1);
    }

  putenv ("SDL_VIDEO_CENTERED=1");
  
  /* Initialize the display */
  screen = SDL_SetVideoMode (640, 480, 0, 0);
  if (screen == NULL)
    {
      fprintf (stderr, "Couldn't set %dx%dx%d video mode: %s\n",
	       640, 480, 16, SDL_GetError ());
      exit (1);
    }
  rgbmap = SDL_MapRGB (screen->format, 0, 0, 0);
  SDL_FillRect (screen, NULL, rgbmap);
  /* Set the window manager title bar */
  SDL_WM_SetCaption ("SpcaGui Intro", "Spca5xx");

  memset (sortab, 0, sizeof (sortab));
  Sylvie = SFont_InitFont (IMG_Load (Fhelvet));
  if (!Sylvie)
    {
      fprintf (stderr, "An error occured while loading the font.");
      exit (1);
    }
  Blue = SFont_InitFont (IMG_Load (Fcopper));
  if (!Blue)
    {
      fprintf (stderr, "An error occured while loading the font.");
      exit (1);
    }
  Copper = SFont_InitFont (IMG_Load (Fbigfont));
  if (!Copper)
    {
      fprintf (stderr, "An error occured while loading the font.");
      exit (1);
    }
  Neon = SFont_InitFont (IMG_Load (Farial));
  if (!Neon)
    {
      fprintf (stderr, "An error occured while loading the font.");
      exit (1);
    }

  for (i = 0; i < 50; i++)
    {
      SFont_Write (screen, Neon, 0, 0, "  Spca5xx Grabber!!");
      SFont_Write (screen, Copper, 0 + i, 100 + i,
		   "SpcaGui (Co) Michel Xhaard ");
      SFont_Write (screen, Blue, 50, 190, "General Public Licence @2004 ");
      SFont_Write (screen, Copper, 50 - i, 180 + i, "http://www.libsdl.org");
      SFont_Write (screen, Copper, 300 - (2 * i), 260 + (2 * i),
		   "mxhaard@magic.fr");
      SFont_Write (screen, Blue, 20, 380, "Simple DirectMedia Layer ");
       SFont_Write (screen, Blue, 20, 410, "Press any key to skip intro ! ");
      SFont_Write (screen, Neon,
		   640 - SFont_TextWidth (Neon, "Picture & Avi  !!! "),
		   480 - SFont_TextHeight (Neon), "Picture & Avi !!! ");
      SDL_UpdateRect (screen, 0, 0, 0, 0);
      if (SDL_PollEvent (&sdlevent) == 1) {
				switch (sdlevent.type) {
					case SDL_KEYDOWN:
						goto fin;
					break;
				}
	}	
				 

      SDL_Delay (50);
    }
  SDL_Delay (1000);
  for (j = 9; j >= 0; j--)
    {
      SDL_FillRect (screen, NULL, rgbmap);
      sortab[0] = tabcar[j];
      sortab[1] = Fstring;
      SFont_Write (screen, Sylvie,
		   320 - (SFont_TextWidth (Sylvie, sortab) >> 1),
		   240 - (SFont_TextHeight (Sylvie) >> 1), sortab);
      SDL_UpdateRect (screen, 0, 0, 0, 0);
      if (SDL_PollEvent (&sdlevent) == 1) {
				switch (sdlevent.type) {
					case SDL_KEYDOWN:
						goto fin;
					break;
				}
	}	
      SDL_Delay (500);
    }
fin:
  SFont_FreeFont (Copper);
  SFont_FreeFont (Neon);
  SFont_FreeFont (Blue);
  SFont_FreeFont (Sylvie);
  // Bye Bye !!
  SDL_Quit ();
}
