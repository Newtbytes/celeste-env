#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "environment.h"

#define FRAMES 1000000

int main() {
    init();
    reset();

    clock_t t0 = clock();

    for (int i = 0; i < FRAMES; i++) {
        step((unsigned short)rand());
    }

    clock_t t1 = clock();

    double elapsed_time = (double)(t1 - t0) / CLOCKS_PER_SEC;
    double fps = FRAMES / elapsed_time;

    printf("Elapsed Time: %.2f seconds\n", elapsed_time);
    printf("Frames Per Second (FPS): %.2f\n", fps);

    close();

    return 0;
}