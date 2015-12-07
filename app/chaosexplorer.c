/*
 MAIN.C		v0.01
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "io.h"

// PROTOTYPES for newton.cc
int newtonNearestZero(double r, double i, double alpha);
signed newtonLength();



/*=============================================================================
 * GLOBALS
 */
#define COLOR_MED	200
#define COLOR_MIN	  0

static double mainAlpha = -1.0;
static double mainCtR = 0.;
static double mainCtI = 0.;
static double mainSize = 1.;
static unsigned mainPSize = 100;




/*=============================================================================
 * DISPLAY
 * Draws the image, point per point.
 * First two temp matrixes are filled with the zero ID and the
 * orbital length: this allows to compute the average orbital length.
 * Then the pixels are drawn according to the temp matrixes,
 * using the average orbital length to obtain smoother colors.
 */
void display(double cr, double ci, double size, int screen_size)
{
 double r0, i0, m;

 m = size / screen_size;
 r0 = cr - size/2;
 i0 = ci + size/2;

 signed char *l, length[screen_size*screen_size];
 signed char *z, zero[screen_size*screen_size];
 unsigned total = 0, cnt = 0;

 l = length;
 z = zero;
 for(int y = 0; y < screen_size; y++)
  for(int x = 0; x < screen_size; x++)
    {
	double i = i0 - y*m;	// screen Y goes from up to down!
	double r = r0 + x*m;

	// i and r are swapped, we want imaginary axis to be vertical!
	*z = newtonNearestZero(i, r, mainAlpha);
	*l = newtonLength();

	if(*z > -1) total += *l, cnt++;

	z++;
	l++;
    }


 double a = (COLOR_MED - COLOR_MIN) / log(total / (double)cnt + 1.);

 l = length;
 z = zero;
 for(int y = 0; y < screen_size; y++)
  for(int x = 0; x < screen_size; x++)
    {
	int c;

	c = (int)(COLOR_MIN + a*log(1. + *l++));
	if(c > 255) c = 255;

	switch(*z++) {
	    case 0: ioScreenPutPixel(x, y, c, 0, 0); break;
	    case 1: ioScreenPutPixel(x, y, 0, c, c); break;
	    case 2: ioScreenPutPixel(x, y, 0, 0, c); break;
	    default: ioScreenPutPixel(x, y, 255, 255, 255); break;
	}
    }
}





/*=============================================================================
 * LOOP
 */

int mainLoop()
{
 char name[256];
 int k;

 display(mainCtR, mainCtI, mainSize, mainPSize);
 sprintf(name, "alpha=%.2e center=(%.2e,%.2e) size=%.2e", mainAlpha, mainCtR, mainCtI, mainSize);
 ioScreenUpdate(name);

 do k = ioKeybRead(); while(!k);

 switch(k) {

    case '+': mainSize /= 1.2; break;	// zoom in
    case '-': mainSize *= 1.2; break;	// zoom out
    case 2:  mainCtI -= mainSize / 10.; break;
    case 8:  mainCtI += mainSize / 10.; break;
    case 6:  mainCtR += mainSize / 10.; break;
    case 4:  mainCtR -= mainSize / 10.; break;

    case 111: mainAlpha += 0.1; break;
    case 222: mainAlpha -= 0.1; break;

    default: break;
 }
}




void endprint()
{
 printf("\nTo obtain the last picture with a different resolution,\n");
 printf("copy and paste these values after the new [size] parameter:\n");
 printf("  %.4e %.4e %.4e %.4e\n", mainAlpha, mainCtR, mainCtI, mainSize);
}


int main(int argc, char **argv)
{
 int m;

 if(argc > 1) mainPSize = atoi(argv[1]);

 if(argc > 2) mainAlpha = atof(argv[2]);
 if(argc > 3) mainCtR = atof(argv[3]);
 if(argc > 4) mainCtI = atof(argv[4]);
 if(argc > 5) mainSize = atof(argv[5]);

 m = 600/mainPSize;
 if(m < 1) m = 1;

 if(ioScreenInit(mainPSize, mainPSize, m)) return -1;

 atexit(endprint);
 printf("usage:\n");
 printf("\t%s [size] [alpha] [R coordinate] [I coordinate] [area size]\n", argv[0]);
 printf("\n");
 printf("\tF1 to take a screenshot\n");
 printf("\tESC to exit\n");

 while(1) mainLoop();
}



//EOF =========================================================================
