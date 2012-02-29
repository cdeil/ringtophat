#ifndef RINGTOPHAT_H
#define RINGTOPHAT_H

typedef struct {
        int r0;
        int r1;
        int r2;
        double alpha;
        int **mask;
        int n_off_border;
        int n_on_border;
        int **off_left_border;
        int **off_right_border;
        int **on_left_border;
        int **on_right_border;
} Ring;

void Convolve(int **, int **, int, int, Ring *);
Ring *CreateRing();

#endif
