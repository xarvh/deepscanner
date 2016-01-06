
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




void maximiseSpread(unsigned long* map, unsigned long* count) {

    unsigned long total = 0;
    for (int i = 0; i < 256; i++) total += count[i];

    unsigned long partialTotal = 0;
    for (int i = 0; i < 256; i++) {
        map[i] = 255 - (255 * partialTotal) / total;
        partialTotal += count[i];
    }
}







int main(int argc, char** argv) {

    FILE* in = fopen(argv[1], "rb");

    Count count;
    memset((void *)&count, 0, sizeof(count));

    // initial count
    Pixel bf[bufferSize];
    int r;
    while (r = fread(bf, sizeof(Pixel), bufferSize, in)) {
        for (Pixel* p = bf; p - bf < r; p++) {
            if (!p->g && !p->b) count.r[p->r]++;
            else if (!p->r && !p->b) count.g[p->g]++;
            else if (!p->r && !p->g) count.b[p->b]++;
        }
    }

    // create translation map
    Count map;
    maximiseSpread(map.r, count.r);
    maximiseSpread(map.g, count.g);
    maximiseSpread(map.b, count.b);


    // output
    char fn[256];
    sprintf(fn, "maxed%s", argv[1]);
    FILE* out = fopen(fn, "wb");

    rewind(in);
    while (r = fread(bf, sizeof(Pixel), bufferSize, in)) {
        for (Pixel* p = bf; p - bf < r; p++) {
            if (!p->g && !p->b) p->r = map.r[p->r];
            else if (!p->r && !p->b) p->g = map.g[p->g];
            else if (!p->r && !p->g) p->b = map.b[p->b];
        }
        fwrite(bf, sizeof(Pixel), r, out);
    }

    fclose(in);
    fclose(out);

    return 1;
}
