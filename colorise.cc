
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



/* ===========================================================================
 * TRANSFORMS
 */
Pixel white(int value) {
    Pixel p = {255, 255, 255};
    return p;
}


Pixel StockholmRed(int value) {
    Pixel p = {0, 0, 0};

    p.r = pow(2, 8 * (value / 256.));

    if (p.r < 4) {
        p.r = 0;
        p.g = 88;
        p.b = 255;
    }

    return p;
}


typedef Pixel (* Transform)(int);
Transform transforms[] = {
    white,
    StockholmRed
};


/* ===========================================================================
 * MAIN
 */
int main(int argc, char** argv) {

    int bufferSize = 1024*1024;
    Pixel bf[bufferSize];

    if (argc < 6) {
        fprintf(stderr, "%s RedTransform BlueTransform GreenTransform inutFile outputFile\n", argv[0]);
        return -1;
    }

    Transform rt = transforms[atoi(argv[1])];
    Transform gt = transforms[atoi(argv[2])];
    Transform bt = transforms[atoi(argv[3])];

    FILE* in = fopen(argv[4], "rb");
    FILE* out = fopen(argv[5], "wb");
    while (int r = fread(bf, sizeof(Pixel), bufferSize, in)) {
        for (Pixel* p = bf; p - bf < r; p++) {
            if (p->r) *p = rt(p->r);
            else if (p->g) *p = gt(p->g);
            else if (p->b) *p = bt(p->b);
        }
        fwrite(bf, sizeof(Pixel), r, out);
    }

    fclose(in);
    fclose(out);
    return 0;
}
