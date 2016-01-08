
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;


int main(int argc, char** argv) {

    int scale = atoi(argv[1]);
    FILE* in = fopen(argv[2], "rb");

    // Get file size
    fseek(in, 0L, SEEK_END);
    int fileSize = ftell(in);
    rewind(in);

    int pixelCount = fileSize / sizeof(Pixel);

    // calculate length of (square) input image
    int inLength = sqrt(pixelCount);

    // scale
    int inSize = inLength * scale; // many rows
    int outSize = inLength / scale; // a single row, scaled down
    Pixel inBf[inSize];
    Pixel outBf[outSize];

    // output file
    char fn[256];
    sprintf(fn, "aa%d.%s.%d.data", scale, argv[2], outSize);
    FILE* out = fopen(fn, "wb");

    // process
    while (int r = fread(inBf, sizeof(Pixel), inSize, in)) {

        for(int i = 0; i < outSize; i++) {
            int r = 0, g = 0, b = 0;

            for(int dx = 0; dx < scale; dx++) for(int dy = 0; dy < scale; dy++) {
                Pixel* p = inBf + i * scale + dx + dy * inLength;

                r += p->r;
                g += p->g;
                b += p->b;
            }

            outBf[i].r = r / (scale * scale);
            outBf[i].g = g / (scale * scale);
            outBf[i].b = b / (scale * scale);
        }

        fwrite(outBf, sizeof(Pixel), outSize, out);
    }

    fclose(in);
    fclose(out);

    return 1;
}
