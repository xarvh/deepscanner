
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


Pixel black(int value) {
    Pixel p = {0, 0, 0};
    return p;
}


// 2
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


// 3
Pixel fiery(int value) {
    Pixel p = {0, 0, 0};

    value = pow(2, 8 * (value / 256.));
    int dt = 256 / 3;

    int level = value / dt;
    int shade = (value - dt*level) * 3;

    switch (level) {
        case 0: p.r = shade; break;
        case 1: p.r = 255; p.g = shade; break;
        default: p.r = p.g = 255; p.b = shade; break;
    }

    return p;
}


// 4
Pixel crayonOrange(int value) {
    Pixel p = {255, 0, 99};
    p.g = value / 2;
    return p;
}

// 5
Pixel crayonAzure(int value) {
    Pixel p = {0, 0, 255};
    p.r = value / 3;
    p.g = 128 + value / 2;
    return p;
}

// 6
Pixel crayonPurple(int value) {
    Pixel p = {0, 0, 255};
    p.r = 112 + value / 7;
    p.g = 37 + value / 2;
    return p;
}

// --
typedef Pixel (* Transform)(int);
Transform transforms[] = {
    white,
    black,
    StockholmRed,
    fiery,
    crayonOrange,
    crayonAzure,
    crayonPurple,
};


/* ===========================================================================
 * MAIN
 */
int main(int argc, char** argv) {

    int bufferSize = 1024*1024;
    Pixel bf[bufferSize];

    if (argc < 5) {
        fprintf(stderr, "%s RedTransform BlueTransform GreenTransform inutFile\n", argv[0]);
        return -1;
    }

    int ri = atoi(argv[1]);
    int gi = atoi(argv[2]);
    int bi = atoi(argv[3]);
    char* inFn = argv[4];

    FILE* in = fopen(inFn, "rb");

    char outFn[200];
    sprintf(outFn, "co%d %d %d.%s", ri, gi, bi, inFn);
    FILE* out = fopen(outFn, "wb");

    Transform rt = transforms[ri];
    Transform gt = transforms[gi];
    Transform bt = transforms[bi];

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
