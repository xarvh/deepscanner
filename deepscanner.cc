#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex>
#define Complex std::complex<double>


/*=============================================================================
 * ORBIT
 *
 * F = (zP' - P) / P' =
 *
 *      z(3z^2 + 4z + a+1) - (z^3 + 2z^2 + (a+1)z + a)
 *   = ------------------------------------------------ =
 *                   3z^2 + 4z + a+1
 *
 *       2z^3 + 2z^2 - a       2(z + 1)z^2 - a
 *   = ------------------- = -------------------
 *       3z^2 + 4z + a+1       (3z + 4)z + a+1
 */
Complex nextOrbit(Complex z, double a)
{
 Complex n = 2.*(z+1.)*z*z - a;
 Complex d = (3.*z + 4.)*z + a+1.;

 return n / d;
}


/*=============================================================================
 * NEAREST ZERO
 *
 * Searches for the first zero that gets close enough to the orbit.
 */
void nearestZero(double r, double i, double alpha, int* orbitLength, int* closestZeroId) {
    double distanceThreshold = 1e-13;
    double squaredDistanceThreshold = distanceThreshold * distanceThreshold;

    Complex zeroA[3];

    // compute the zeros, solutions of
    // z^3 + 2z^2 + (a+1)z + a
    Complex f = alpha;
    f = sqrt(1. - 4. * f);
    zeroA[0] = -1.;		// first zero, always (-1, 0)
    zeroA[1] = -(1. + f) / 2.;	// second zero
    zeroA[2] = -(1. - f) / 2.;	// third zero

    // iterate 'z' along the orbit:
    Complex z(r, i);
    for(int i = 255; i >= 0; i--) {

         // if one of the zeros is close enough, store the iteration count and return the zero id.
         for(int id = 0; id < 3; id++) {
         	  Complex delta = z - zeroA[id];
         	  if(delta.real()*delta.real() + delta.imag()*delta.imag() < squaredDistanceThreshold) {
         	  	  *orbitLength = i;
                  *closestZeroId = id;
         	  	  return;
             }
         }

         z = nextOrbit(z, alpha);
    }

    *orbitLength = 0;
    *closestZeroId = -1;
}


void display(FILE* out, double cr, double ci, double size, double alpha, int rasterSize) {
    double m = size / rasterSize;
    double r0 = cr - size/2;
    double i0 = ci + size/2;

    for(int y = 0; y < rasterSize; y++) {

        unsigned char rowBf[rasterSize * 3];

        for(int x = 0; x < rasterSize; x++) {
            double i = i0 - y*m;	// screen Y goes from up to down!
            double r = r0 + x*m;

            int closestZeroId;
            int orbitLength;
            nearestZero(i, r, alpha, &orbitLength, &closestZeroId);

            unsigned char R = 0, G = 0, B = 0;
            switch(closestZeroId) {
                case 0: G = orbitLength; break;
                case 1: R = orbitLength; break;
                case 2: B = orbitLength; break;
            }

            rowBf[x*3 + 0] = R;
            rowBf[x*3 + 1] = G;
            rowBf[x*3 + 2] = B;
        }

        fwrite(rowBf, 1, sizeof(rowBf), out);
        printf("row %6d of %6d\n", y, rasterSize);
    }
}


int main(int argc, char **argv) {

    // default to fractal wave
    double alphaConstant = 3.00e-01;
    double centerR = 1.11e-04;
    double centerI = -8.7978e-02;
    double sizeRI = 1.1000e-04;

    unsigned rasterSize = 350;

    if(argc > 1) rasterSize = atoi(argv[1]);

    if(argc > 2) alphaConstant = atof(argv[2]);
    if(argc > 3) centerR = atof(argv[3]);
    if(argc > 4) centerI = atof(argv[4]);
    if(argc > 5) sizeRI = atof(argv[5]);

    char name[200];
    sprintf(name, "map %.4e %.4e %.4e %.4e %d.data", alphaConstant, centerR, centerI, sizeRI, rasterSize);

    FILE* out = fopen(name, "wb");
    display(out, centerR, centerI, sizeRI, alphaConstant, rasterSize);
    fclose(out);
    return 0;
}
