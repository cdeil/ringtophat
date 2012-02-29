#include "ringtophat.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void CreateTestImage(int **img, int w, int h, int mode)
{
    int x, y;

    switch (mode) {
        case 0:
            for (y = 0; y < h; y++) {
                for (x = 0; x < w; x++) {
                    img[y][x] = 0;
                }
            }
            break;
        case 1:
            for (y = 0; y < h; y++) {
                for (x = 0; x < w; x++) {
                    img[y][x] = 1;
                }
            }
            break;
        case 2:
            for (y = 0; y < h; y++) {
                for (x = 0; x < w; x++) {
                    img[y][x] = (x+1) % (y+1);
                }
            }
            break;
        case 3:
            for (y = 0; y < h; y++) {
                for (x = 0; x < w; x++) {
                    img[y][x] = 0;
                }
            }
            // in average n events per bin
            for (int i = 0; i < w*h/10; i++) {
                x = rand() % w;
                y = rand() % h;
                img[y][x] += 1;
            }
            for (int i = 0; i < 10; i++) {
                x = rand() % w;
                y = rand() % h;
                for (int j = 0; j < 250; j++) {
                    int xs = x + rand() % 30 - 15;
                    int ys = y + rand() % 30 - 15;
                    if (xs < 0 || xs >= w || ys < 0 || ys >= h) continue;
                    img[ys][xs] += 1;
                }
            }
    }
}

int ExportToPGM(int **img, int w, int h, char *filename)
{
    FILE *f;
    char line[50000];

    f = fopen (filename, "wb");
    if (f ==  NULL) {
        fprintf(stderr, "fopen(%s)\n", filename);
        return -1;
    }


    int min = 1000000000;
    int max = -1000000000;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (max < img[y][x]) max = img[y][x];
            if (min > img[y][x]) min = img[y][x];
            if (img[y][x] < -1000) printf("%d    %d %d\n", min, x, y); 
        }
    }

    fprintf (f, "P5\n");
    fprintf (f, "# width height\n");
    fprintf (f, "%d %d\n", w, h);
    fprintf (f, "255\n");

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            line[x] = (char) ( (double) (img[y][x] - min) / 
                               (double) (max - min) * 255.);
        }
        if (fwrite (line, sizeof(char)*w, 1, f) != 1) {
            fprintf(stderr, "fwrite(%s)\n", filename);
            fclose (f);
            return -1;
        }
    }
    fclose (f);
    return 0;
}

int main(int n_args, char** args)
{
    clock_t t0, t1;
    srand((unsigned int) time(NULL));

    int w = 10000;
    int h = 400;
    double r0 = 9.2;
    double r1 = 25.8;
    double r2 = 37.4;

    int **img1 = (int **) malloc(h * sizeof(int *));
    int **img2 = (int **) malloc(h * sizeof(int *));
    for (int y = 0; y < h; y++) {
        img1[y] = (int *) malloc(w * sizeof(int));
        img2[y] = (int *) malloc(w * sizeof(int));
    }

    CreateTestImage(img1, w, h, 3);
    ExportToPGM(img1, w, h, "input.pgm");

    Ring *ring = CreateRing(r0, r1, r2);

    // start benchmark
    t0 = clock();

    Convolve(img1, img2, w, h, ring);
    
    t1 = clock();
    double dt = ((double) (t1-t0))/CLOCKS_PER_SEC; 
    fprintf(stderr, "runtime for convolving: dt = %.2f s\n", dt);
    // end benchmark

    ExportToPGM(img2, w, h, "output.pgm");
    //for (int i = 0; i < 10
    return 0;
}