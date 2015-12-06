
#include <stdio.h>
#include <string.h>


typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;



typedef struct {
    unsigned long r[256];
    unsigned long g[256];
    unsigned long b[256];
} Count;



int bufferSize = 1024*1024;



int main(int argc, char** argv) {

    FILE* f = fopen(argv[1], "rb");

    Count count;
    memset((void *)&count, 0, sizeof(count));

    Pixel bf[bufferSize];
    int r;
    while (r = fread(bf, sizeof(Pixel), bufferSize, f)) {
        for (Pixel* p = bf; p - bf < r; p++) {
            if (!p->g && !p->b) count.r[p->r]++;
            if (!p->r && !p->b) count.g[p->g]++;
            if (!p->r && !p->g) count.b[p->b]++;
        }
    }

    for (int i = 0; i < 256; i++)
        printf("%4d, %8ld, %8ld, %8ld\n", i, count.r[i], count.g[i], count.b[i]);

    return 1;
}
