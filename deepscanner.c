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
static double mainAlpha = -1.0;
static double mainCtR = 0.;
static double mainCtI = 0.;
static double mainSize = 1.;
static unsigned mainPSize = 100;



/*=============================================================================
 * COLOR SCALEs
 */
void colorScale1StZero(double k, double* r, double* g, double* b)
{
 *r = 3.8*k-2.80;		//k + .5;
 *g =  .7*k+ .30;		//k*3 - 1.5;
 *b = 2.5*k-1.25;		//k*4 - 3.0;
}

void colorScale2NdZero(double k, double* r, double* g, double* b)
{
 *r = 1;			//k*k;
 *g = .35 + .45*k*k;		//k;
 *b = .35 + .45*k*k;		//.5 + k*2;
}


void colorScale3RdZero(double k, double* r, double* g, double* b)
{
 *r = .15 + .15*k*k;
 *g = .35 + .45*k*k;
 *b = 1;

// *r = k;
// *g = k*k;
// *b = 1;
}

int colorScaleBoundary(double v)
{
 if(v >= 1.) return 255;
 if(v <  0.) return 0;
 return (int)(v * 256);
}




/*=============================================================================
 * DEEP SCAN
 */
void deepScan(double rc, double ic, double size, double a, int* Rp, int* Gp, int* Bp)
{
 double x, y;
 double v;
 double R, G, B;
 int k, c, cnt = 0;

 *Rp = *Gp = *Bp = 0;

 for(x = rc - size/2; x <= rc + size/2; x += size/3)
    for(y = ic - size/2; y <= ic + size/2; y += size/3)
	{
	    k = newtonNearestZero(y, -x, mainAlpha);
	    v = 1.4 - a * log(1. + newtonLength());

	    switch(k) {
		case 0: colorScale1StZero(v, &R, &G, &B); break;
		case 1: colorScale2NdZero(v, &R, &G, &B); break;
		case 2: colorScale3RdZero(v, &R, &G, &B); break;
		default: R = 1., G = 1., B = 1.; break;
	    }

	    *Rp += colorScaleBoundary(R);
	    *Gp += colorScaleBoundary(G);
	    *Bp += colorScaleBoundary(B);
	    cnt++;
	}

 *Rp /= cnt;
 *Gp /= cnt;
 *Bp /= cnt;
}




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

 unsigned total = 0, cnt = 0;

 for(int y = 0; y < screen_size; y++)
  for(int x = 0; x < screen_size; x++)
    {
	double i = i0 - y*m;	// screen Y goes from up to down!
	double r = r0 + x*m;

	if(-1 < newtonNearestZero(i, r, mainAlpha))
	    {
		total += newtonLength();
		cnt++;
	    }
    }


 double a = .8 / log(total / (double)cnt + 1.);
 int R, G, B;

 for(int y = 0; y < screen_size; y++)
  for(int x = 0; x < screen_size; x++)
    {
	double i = i0 - y*m;	// screen Y goes from up to down!
	double r = r0 + x*m;

	deepScan(r, i, size/screen_size, a, &R, &G, &B);
	ioScreenPutPixel(x, y, R, G, B);
    }
}










/*=============================================================================
 * LOOP
 */
int mainLoop()
{
 char name[256];

 display(mainCtR, mainCtI, mainSize, mainPSize);

 sprintf(name, "alpha=%.2e center=(%.2e,%.2e) size=%.2e", mainAlpha, mainCtR, mainCtI, mainSize);
 ioScreenName(name);
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

 if(ioScreenMemory(mainPSize, mainPSize, m)) return -1;

 mainLoop();
 ioScreenShot();
}



//EOF =========================================================================
