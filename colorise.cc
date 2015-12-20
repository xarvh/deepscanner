
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


typedef struct {
    unsigned char a[0];
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;



Pixel firstZeroColorScale(int value) {
    Pixel p = {255, 255, 255};
    return p;
}


Pixel secondZeroColorScale(int value) {
    Pixel p = {0, 0, 0};

    p.r = pow(2, 8 * (value / 256.));

    return p;
}


Pixel thirdZeroColorScale(int value) {
    Pixel p = {255, 255, 255};
    return p;
}


int main(int argc, char** argv) {

    int bufferSize = 1024*1024;
    Pixel bf[bufferSize];

    FILE* in = fopen(argv[1], "rb");
    FILE* out = fopen(argv[2], "wb");
    while (int r = fread(bf, sizeof(Pixel), bufferSize, in)) {
        for (Pixel* p = bf; p - bf < r; p++) {
            /*
            Pixel black = {0, 0, 0}, white = {255, 255, 255}, red = {255, 0, 0};
            int x = 0;
            if (p->r) x++;
            if (p->g) x++;
            if (p->b) x++;

            if (x == 0) *p = black;
            else if (x == 1) *p = white;
            else *p = red;
            */

            if (p->r) *p = firstZeroColorScale(p->r);
            else if (p->g) *p = secondZeroColorScale(p->g);
            else if (p->b) *p = thirdZeroColorScale(p->b);
        }
        fwrite(bf, sizeof(Pixel), r, out);
    }

    fclose(in);
    fclose(out);
    return 0;
}
