/*
 NEWTON.CC		v0.00
 Newton's iteration for a particular polynomial

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

#include <complex>
#define COMPLEX std::complex<double>





/*=============================================================================
 * GLOBALS
 */
static double newtonDistance = 0.001*0.001;	//actually, a squared distance!
static signed newtonOrbitLength;



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
COMPLEX newtonOrbit(COMPLEX z, double a)
{
 COMPLEX n = 2.*(z+1.)*z*z - a;
 COMPLEX d = (3.*z + 4.)*z + a+1.;

 return n / d;
}





/*=============================================================================
 * NEAREST ZERO
 *
 * Actually searches for the first zero that gets close enough to the orbit.
 */
int newtonNearestZero(double r, double i, double alpha)
{
 COMPLEX zeroA[3];
 COMPLEX f;

 // compute the zeros, solutions of
 // z^3 + 2z^2 + (a+1)z + a
 f = alpha;
 f = sqrt(1. - 4. * f);
 zeroA[0] = -1.;		// first zero, always (-1, 0)
 zeroA[1] = -(1. + f) / 2.;	// second zero
 zeroA[2] = -(1. - f) / 2.;	// third zero

 // iterate 'z' along the orbit:
 COMPLEX z(r, i);
 for(int i = 0; i < 100; i++)
    {
	// if one of the zeros is close enough,
	// store the iteration count and return the zero ID.
	for(int k = 0; k < 3; k++)
	    {
		// compute distance
		f = z - zeroA[k];
		if(f.real()*f.real() + f.imag()*f.imag() < newtonDistance)
		    {
			newtonOrbitLength = i;
			return k;
		    }
	}

	z = newtonOrbit(z, alpha);
    }

 // no zero found
 return -1;
}



/*=============================================================================
 * LENGTH
 * Just returns the second iteration parameter.
 */
signed newtonLength()
{
 return newtonOrbitLength;
}


//EOF =========================================================================
