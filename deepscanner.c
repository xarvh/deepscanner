/*
 MAIN.C		v0.01
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pngio.h"

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

static pngT* png;


/*=============================================================================
 * COLOR SCALEs
 */
void colorScale1StZero(double k, double* r, double* g, double* b)
{
 *r = k + .5;
 *g = k*3 - 1.5;
 *b = k*4 - 3.0;
}

void colorScale2NdZero(double k, double* r, double* g, double* b)
{
 *r = k*k;
 *g = k;
 *b = .5 + k*2;
}


void colorScale3RdZero(double k, double* r, double* g, double* b)
{
 *r = k;
 *g = k*k;
 *b = 1;
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

    rgbaT* pixel = png->p + x + y*mainPSize;
    pixel->r = R;
    pixel->g = G;
    pixel->b = B;
    }
}



/*=============================================================================
 * LOOP
 */
int main(int argc, char **argv)
{
 if(argc > 1) mainPSize = atoi(argv[1]);

 if(argc > 2) mainAlpha = atof(argv[2]);
 if(argc > 3) mainCtR = atof(argv[3]);
 if(argc > 4) mainCtI = atof(argv[4]);
 if(argc > 5) mainSize = atof(argv[5]);

 int m = 600/mainPSize;
 if(m < 1) m = 1;

 png = pngEmpty(mainPSize, mainPSize);

 display(mainCtR, mainCtI, mainSize, mainPSize);

 char name[200];
 sprintf(name, "render_%.4e_%.4e_%.4e_%.4e.png", mainAlpha, mainCtR, mainCtI, mainSize);
 pngSave(name, png);
}



//EOF =========================================================================
