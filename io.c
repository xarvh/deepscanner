/*
 IO.C			v0.05
 Simple Screen/Keyboard/Mouse wrapper
 SDL-based implementation.

 Copyright (c) 2005 Francesco Orsenigo <nightwalker@lombardiacom.it>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to
 the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 Boston, MA 02111-1307, USA.
*/

/*
 * FUNCTION CALLS:
 *
 * int ioScreenPutPixel(int x, int y, int r, int g, int b)
 *	Puts a pixel of color (r,g,b) at x,y.
 *
 * int ioScreenUpdate(char* name)
 *	Refreshes the screen
 *	If 'name' is not NULL, window name is also changed to 'name'.
 *
 * int ioScreenShot()
 *	Saves a .BMP screenshot of the current screen.
 *	Filename is the window caption plus a numeric counter.
 *
 * int ioScreenMemory(int w, int h, int m)
 *	Initializes a memory image of w x h virtual pixels, each of
 *	size m x m.
 *	This image is not displayed but can be saved with ioScreenShot().
 *	Returns 0 on success.
 *	On error, -1 is returned and an error message is output to stderr.
 *
 * int ioScreenInit(int w, int h, int m)
 *	Initializes a window of w x h virtual pixels, each of size m x m.
 *	Returns 0 on success.
 *	On error, -1 is returned and an error message is output to stderr.
 *
 * int ioMouseX()
 * int ioMouseY()
 *	Returns the X and Y mouse positions respectively, relative to the last
 *	button press event.
 *
 * int ioKeybRead()
 *	Waits for a keypress, and returns the key code.
 *	On ESC will exit(0).
 *	On F1 will take a screenshot.
 *	Special codes are:
 *		8 Up Arrow/Keypad 8
 *		6 Right Arrow/Keypad 6
 *		2 Down Arrow/Keypad 2
 *		4 Left Arrow/Keypad 4
 *		111 Left mouse button
 *		222 Right mouse button
 *		333 Middle mouse button
 */

#include <SDL/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>





/*=============================================================================
 * SCREEN
 *=============================================================================
 */
static SDL_Surface* hwscreen;
static SDL_Surface* swscreen;
static unsigned multi = 1;
static char screen_name[256];




/*=============================================================================
 * PUTPIXEL
 */
int ioScreenPutPixel(int x, int y, int r, int g, int b)
{
 int* bfstart;	// buffer start
 int* bfend;	// buffer end

 int* vpix;	// virtual pixel position
 int* pix;	// actual pixel
 
 bfstart = (int*)swscreen->pixels;
 bfend = bfstart + swscreen->w*swscreen->h;
 vpix = bfstart + (x + y*swscreen->w)*multi;

 for(x = 0; x < multi; x++) for(y = 0; y < multi; y++)
    {
	pix = vpix + x + y*swscreen->w;
	if(pix >= bfstart && pix < bfend)
	    *pix = (r<<16) | (g<<8) | (b<<0);
    }
}





/*=============================================================================
 * SHOT
 */
static unsigned shotID = 0;

int ioScreenShot()
{
 char name[256];
 char* c;

 // create filename
 sprintf(name, "shot%03d_%s.bmp", shotID++, screen_name);

 // replace all non printable chars with '_'
 for(c = name; *c; c++) if(*c <= ' ') *c = '_';

 // save
 if(SDL_SaveBMP(swscreen, name))
    printf("error writing %s\n", name);
 else
    printf("%s written.\n", name);
}





/*=============================================================================
 * NAME
 */
int ioScreenName(char* name)
{
 SDL_WM_SetCaption(name, name);

 strncpy(screen_name, name, sizeof(screen_name));
 screen_name[sizeof(screen_name)-1] = '\0';
}





/*=============================================================================
 * UPDATE
 */
int ioScreenUpdate(char* name)
{
 if(name) ioScreenName(name);

 // copy the virtual software screen to the actual hardware screen.
 SDL_BlitSurface(swscreen, NULL, hwscreen, NULL);

 // and request for the hardware screen update
 SDL_UpdateRect(hwscreen, 0, 0, hwscreen->w, hwscreen->h);
}





/*=============================================================================
 * VIRTUAL
 * Sets up virtual screen only.
 */
int ioScreenMemory(int w, int h, int m)
{
 int e;

 if(w*m > 4*1024 || h*m > 4*1024)
    {
	fprintf(stderr, "screen too large: %dx%d times %d?\n", w, h, m);
	return -1;
    }

 if(m < 1) m = 10;
 multi = m;
 w *= multi;
 h *= multi;

 SDL_Init(0);

 swscreen = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0xff<<16, 0xff<<8, 0xff, 0);
 if(swscreen == NULL) return -1;

 return 0;
}






/*=============================================================================
 * INIT
 */
int ioScreenInit(int w, int h, int m)
{
 int e;

 // create software surface
 if(ioScreenMemory(w, h, m))
    return -1;

 // init SDL video
 e = SDL_InitSubSystem(SDL_INIT_VIDEO);
 if(e == -1)
    {
	fprintf(stderr, "no SDL\n");
	return -1;
    }

 // create hardware surface
 hwscreen = SDL_SetVideoMode(swscreen->w, swscreen->h, 32, SDL_HWSURFACE);
 if(hwscreen == NULL)
    {
	fprintf(stderr, "no screen\n");
	return -1;
    }

 // mouse and keyboard stuff
 SDL_ShowCursor(SDL_ENABLE);
 SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY/2, SDL_DEFAULT_REPEAT_INTERVAL);

 return 0;
}





/*=============================================================================
 * MOUSE
 *=============================================================================
 */
static int mouseX, mouseY;

int ioMouseX() {return mouseX / multi;}
int ioMouseY() {return mouseY / multi;}





/*=============================================================================
 * KEYBOARD
 *=============================================================================
 */
int ioKeybRead()
{
 SDL_Event event[1];
 int k;

 if(SDL_PollEvent(event)) switch(event->type) {

    // window quit button
    case SDL_QUIT: exit(0);

    // mouse events
    case SDL_MOUSEBUTTONDOWN:
	mouseX = event->button.x;
	mouseY = event->button.y;
	switch(event->button.button) {
	    case SDL_BUTTON_LEFT: return 111;
	    case SDL_BUTTON_RIGHT: return 222;	
	    case SDL_BUTTON_MIDDLE: return 333;	
	    default: return 0;
	}

    // keyboard events
    case SDL_KEYDOWN:
	k = event->key.keysym.sym;

	if(k >= ' ' && k <= '~') return k;

	switch(k) {
	    case 27: exit(0);
	    case 13: return 13;
	    case SDLK_KP_PLUS: return '+';
	    case SDLK_KP_MINUS: return '-';
	    case SDLK_F1: ioScreenShot(); return 0;

	    case SDLK_UP:
	    case SDLK_KP8: return 8;

	    case SDLK_RIGHT:
	    case SDLK_KP6: return 6;

	    case SDLK_DOWN:
	    case SDLK_KP2: return 2;

	    case SDLK_LEFT:
	    case SDLK_KP4: return 4;

	    default: break;
	}
 }

 return 0;
}

//EOF =========================================================================
