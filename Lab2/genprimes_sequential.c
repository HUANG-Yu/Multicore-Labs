#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>

int main(int argc, char * argv[]) {
    // read IO
    int n = (int)strtol(argv[1], NULL, 10);
    n -= 1;
    bool flags[n];
    bool* p = &flags[0];
    for (int i = 0; i < n; i++) {
        *(p+i) = true;
    }
    double tstart = 0.0, ttaken = 0.0;
    tstart = omp_get_wtime();
    // run Sieve Algo
    for (int i = 0; i <= (n + 1)/2 ; i++) {
        if (*(p + i) != false)
        for (int j = (i+2) * (i+2); j > 0 && j <= n + 1; j += i+2) {
            flags[j-2] = false;
        }
    }
    ttaken = omp_get_wtime() - tstart;
    printf("main part time: %f\n", ttaken);
    // output to file
    FILE *fp = fopen("./N.txt", "w+");
    if (fp == NULL) {
        printf("file open error");
    }
    int last_pos = 0;
    int line_num = 1;
    for (int i = 0; i < n; i++) {
        if (*(p + i)) {
            fprintf(fp, "%d, %d, %d\n", line_num++, i+2, i - last_pos);
            last_pos = i;
        }
    }
    fclose(fp);
    return 0;
     
}