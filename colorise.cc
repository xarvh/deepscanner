
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
    unsigned char a[0];
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;


typedef struct {
    Pixel min;
    Pixel max;
} Map;



Map maps[3] = {
    {
        {112,  37, 255 },
        {156, 146, 255 }
    },
    {
        {255,   0,  99 },
        {255, 154,  99 }
    },
    {
        {  7, 162, 255 },
        {101, 255, 255 }
    }
};


double mmax = 0;
double mmin = 9999999;


void pixelTransform(Pixel* p, Pixel minInput, Pixel maxInput) {

    int zeroId = 0;
    while (!p->a[zeroId]) zeroId++;
    // TODO detect black pixel!

    //if (zeroId != 1) return;

    double normalizedInputValue = (p->a[zeroId] - minInput.a[zeroId]) / (double)(maxInput.a[zeroId] - minInput.a[zeroId]);
    Map map = maps[zeroId];

    for (int i = 0; i < 3; i++) {

        int minComponentOutput = map.min.a[i];
        int maxComponentOutput = map.max.a[i];

        int v = minComponentOutput + (int)((maxComponentOutput - minComponentOutput) * normalizedInputValue);
        p->a[i] = v;
    }
}





int main(int argc, char** argv) {

    int bufferSize = 1024*1024;

    Pixel max = {0, 0, 0};
    Pixel min = {255, 255, 255};

    FILE* in = fopen(argv[1], "rb");
    Pixel bf[bufferSize];
    while (int r = fread(bf, sizeof(Pixel), bufferSize, in)) {
        for (Pixel* p = bf; p - bf < r; p++) {
            for (int i = 0; i < 3; i++) {
                if (p->a[i] && p->a[i] > max.a[i]) max.a[i] = p->a[i];
                if (p->a[i] && p->a[i] < min.a[i]) min.a[i] = p->a[i];
            }
        }
    }

    /*
    for (int i = 0; i < 3; i++) {
        printf("%d %d\n", min.a[i], max.a[i]);
    }
    */

    rewind(in);
    FILE* out = fopen(argv[2], "wb");
    while (int r = fread(bf, sizeof(Pixel), bufferSize, in)) {
        for (Pixel* p = bf; p - bf < r; p++) {
            pixelTransform(p, min, max);
        }
        fwrite(bf, sizeof(Pixel), r, out);
    }

    fclose(in);
    fclose(out);
    return 0;
}
