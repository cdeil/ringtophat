/**
 * \file ringtophat.c
 * \brief fast? convolution methods
 * @param im
 * @param marker image containing the markers
 * @param threshold threshold to which to flood
 * @return segmented-image the segmented image will be written to a file
 */

#include "ringtophat.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * Convolve
 * convolves an image with a ring mask
 * @param img1 input image
 * @param img2 output image
 * @param w width of images
 * @param h height of images
 * @param ring structure with ring mask
 */
void Convolve(int **img1, int **img2, int w, int h, Ring *ring)
{
    double S, c1, c2;
    int i;

    for (int y = 0; y < h; y++) {
        c1 = 0;
        c2 = 0;
        for (int x = 0; x < w; x++) {
            // if we are too close to a border, we cannot rely on a previous
            // ring, so we have to calculate it the usual way
            if (x < ring->r2 || x >= w - ring->r2 
             || y < ring->r2 || y >= h - ring->r2) {
                c1 = 0;
                c2 = 0;
                int in1 = 0, out1 = 0, in2 = 0, out2 = 0;
                for (int ys = -ring->r2; ys <= ring->r2; ys++) {
                    int yb = y + ys;
                    for (int xs = -ring->r2; xs <= ring->r2; xs++) {
                        int xb = x + xs;
                        switch (ring->mask[ys + ring->r2][xs + ring->r2]) {
                            case 1: // on the ring
                                if (xb < 0 || xb >= w || yb < 0 || yb >= h) {
                                    out1 += 1;
                                } else {
                                    c1 += img1[yb][xb];
                                    in1 += 1;
                                }                       
                                break;
                            case 2: // inside of on-region
                                if (xb < 0 || xb >= w || yb < 0 || yb >= h) {
                                    out2 += 1;
                                } else {
                                    c2 += img1[yb][xb];
                                    in2 += 1;
                                }
                                break;
                        }
                    }
                }
                c1 = c1 * (in1+out1) / (double) in1;
                c2 = c2 * (in2+out2) / (double) in2;
            } else {
                int xb, yb;
                for (i = 0; i < ring->n_off_border; i++) {
                    if (y + ring->off_left_border[i][1] < 0 || 
                        y + ring->off_left_border[i][1] >= h) {
                        continue;
                    }

                    // confusing, but it is correct like this with the -1 here, 
                    // because the center of the ring has already moved, so the
                    // ring->left border is -1 away
                    xb = x + ring->off_left_border[i][0]-1;
                    yb = y + ring->off_left_border[i][1];
                    c1 -= img1[yb][xb];

                    xb = x + ring->off_right_border[i][0];
                    yb = y + ring->off_right_border[i][1];
                    c1 += img1[yb][xb];
                }
                for (i = 0; i < ring->n_on_border; i++) {
                    if (y + ring->on_left_border[i][1] < 0 || 
                        y + ring->on_left_border[i][1] >= h) {
                        continue;
                    }

                    xb = x + ring->on_left_border[i][0]-1;
                    yb = y + ring->on_left_border[i][1];
                    c2 -= img1[yb][xb];

                    xb = x + ring->on_right_border[i][0];
                    yb = y + ring->on_right_border[i][1];
                    c2 += img1[yb][xb];
                }
            }
            S = sqrt(2.0*fabs(c2*log((1.+ring->alpha)/ring->alpha*(c2/(c2+c1)))
                            + c1*log((1.+ring->alpha)*(c1/(c2+c1)))));
            if (c2 < ring->alpha*c1) {
                S = -S;
            }
            img2[y][x] = (int) S;
        }
    }
}

/**
 * CreateRing
 * creates the ring structure
 * @param r0 radius of on region
 * @param r1 inner radius of off region
 * @param r2 outer radius of off region
 */
Ring *CreateRing(double r0, double r1, double r2)
{
    int x, y, X, Y;

    int n_r0 = (int) round(r0);
    int n_r1 = (int) round(r1);
    int n_r2 = (int) round(r2);

    double alpha;
    int n_on_border;
    int n_off_border;
    int ring_width = 2*n_r2 + 1;
    int **on_left_border;
    int **on_right_border;
    int **off_left_border;
    int **off_right_border;

    int **mask = (int **) malloc(ring_width * sizeof(int *));
    for (int j = 0; j < ring_width; j++) {
        mask[j] = (int *) calloc(ring_width, sizeof(int));
    }
    int on = 0;
    int off = 0;
    // calculate the mask and count how many bins in on- and off-region
    for (y = -n_r2; y <= n_r2; y++) {
        Y = y + n_r2;
        for (x = -n_r2; x <= n_r2; x++) {
            X = x + n_r2;
            double d = sqrt(x*x + y*y);
            if (d < r0) {
                mask[Y][X] = 2;
                on += 1;
            } else if (d < r1) {
                mask[Y][X] = 0;
            } else if (d < r2) {
                mask[Y][X] = 1;
                off += 1;
            } else {
                mask[Y][X] = 0;
            }
        }
    }
    // alpha must be recalculated because of the discretization
    alpha = (double) on / (double) off;

    // count how many bins lie on one side (symmetry) of the border
    n_off_border = 0;     // off-region
    n_on_border = 0;  // on-region
    for (y = -n_r2; y <= n_r2; y++) {
        Y = y + n_r2;
        for (x = -n_r2; x <= n_r2; x++) {
            X = x + n_r2;
            if (mask[Y][X] == 1) {
                if (x == n_r2) {
                    n_off_border += 1;
                } else if (mask[Y][X+1] == 0) {
                    n_off_border += 1;
                }
            }
            if (mask[Y][X] == 2) {
                if (x == n_r2) {
                    n_on_border += 1;
                } else if (mask[Y][X+1] == 0) {
                    n_on_border += 1;
                }
            }
            //fprintf(stderr, "%d", mask[Y][X]);
        }
        //fprintf(stderr, "\n");
    }

    // reserve memory for the border positions
    off_left_border = (int **) malloc(n_off_border * sizeof(int *));
    off_right_border = (int **) malloc(n_off_border * sizeof(int *));
    for (int j = 0; j < n_off_border; j++) {
        off_left_border[j] = (int *) malloc(2*sizeof(int));
        off_right_border[j] = (int *) malloc(2*sizeof(int));
    }
    on_left_border = (int **) malloc(n_on_border * sizeof(int *));
    on_right_border = (int **) malloc(n_on_border * sizeof(int *));
    for (int j = 0; j < n_on_border; j++) {
        on_left_border[j] = (int *) malloc(2*sizeof(int));
        on_right_border[j] = (int *) malloc(2*sizeof(int));
    }

    // and fill it with the neighbours of the border pixels of the mask
    n_off_border = 0;
    n_on_border = 0;
    for (y = -n_r2; y <= n_r2; y++) {
        Y = y + n_r2;
        for (x = -n_r2; x <= n_r2; x++) {
            X = x + n_r2;
            if (mask[Y][X] == 1) {
                if (x == -n_r2) {  // if at border
                    off_left_border[n_off_border][0] = x;
                    off_left_border[n_off_border][1] = y;
                    n_off_border++;
                } else if (mask[Y][X-1] == 0) {
                    off_left_border[n_off_border][0] = x;
                    off_left_border[n_off_border][1] = y;
                    n_off_border++;
                }
            } else

            if (mask[Y][X] == 2) {
                if (x == -n_r2) {  // if at border
                    on_left_border[n_on_border][0] = x;
                    on_left_border[n_on_border][1] = y;
                    n_on_border++;
                } else if (mask[Y][X-1] == 0) {
                    on_left_border[n_on_border][0] = x;
                    on_left_border[n_on_border][1] = y;
                    n_on_border++;
                } 
            }
        }
    }
    n_off_border = 0;
    n_on_border = 0;
    for (y = -n_r2; y <= n_r2; y++) {
        Y = y + n_r2;
        for (x = -n_r2; x <= n_r2; x++) {
            X = x + n_r2;
            if (mask[Y][X] == 1) {
                if (x == n_r2) {
                    off_right_border[n_off_border][0] = x;
                    off_right_border[n_off_border][1] = y;
                    n_off_border++;
                } else if (mask[Y][X+1] == 0) {
                    off_right_border[n_off_border][0] = x;
                    off_right_border[n_off_border][1] = y;
                    n_off_border++;
                }
            }
            if (mask[Y][X] == 2) {
                if (x == n_r2) {
                    on_right_border[n_on_border][0] = x;
                    on_right_border[n_on_border][1] = y;
                    n_on_border++;
                } else if (mask[Y][X+1] == 0) {
                    on_right_border[n_on_border][0] = x;
                    on_right_border[n_on_border][1] = y;
                    n_on_border++;
                }
            }
        }
    }

    // plot the mask and mark the outside borders
    for (y = -n_r2; y <= n_r2; y++) {
        Y = y + n_r2;
        for (x = -n_r2; x <= n_r2; x++) {
            X = x + n_r2;
            if (mask[Y][X] == 1) {
                fprintf(stderr, "1");
            } else if (mask[Y][X] == 2) {
                fprintf(stderr, "2");
            } else {
                if (mask[Y][X-1] == 1) {
                    fprintf(stderr, "/");
                } else if (mask[Y][X+1] == 1) {
                    fprintf(stderr, "*");
                } else if (mask[Y][X-1] == 2) {
                    fprintf(stderr, "-");
                } else if (mask[Y][X+1] == 2) {
                    fprintf(stderr, "+");
                } else {
                    fprintf(stderr, "0");
                }
            }
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "ring:\n"
                    "    %d bins in on region, %d bins in off region\n"
                    "    %d bins in whole window\n"
                    "    theoretical speedup: %f\n",
                    on, off, ring_width*ring_width,
                    (double) (ring_width*ring_width) / 
                    (double) (n_off_border+n_on_border));

    Ring *ring = (Ring *) malloc(sizeof(Ring));
    ring->r0 = n_r0;
    ring->r1 = n_r1;
    ring->r2 = n_r2;
    ring->alpha = alpha;
    ring->n_off_border = n_off_border;
    ring->n_on_border = n_on_border;
    ring->on_left_border = on_left_border;
    ring->on_right_border = on_right_border;
    ring->off_left_border = off_left_border;
    ring->off_right_border = off_right_border;
    ring->mask = mask;

    return ring;
}

