#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <stdbool.h>

int main(int argc, char * argv[]) {
    double tstart = 0.0, ttaken = 0.0;
    // read IO
    int n = (int)strtol(argv[1], NULL, 10);
    int t = (int)strtol(argv[2], NULL, 10);
    
    int flags_size = (n-1)/2 + 1;
    bool flags[flags_size];
    int sqrt_n = (int)sqrt((double)n);
    bool* p = &flags[0];
    for (int i = 0; i < flags_size; i++) {
        *(p+i) = true;
    }
    // run Sieve Algo
    if (t > sqrt_n) {
        printf("%d threads were requested but the algorithm used only %d threads\n", t, sqrt_n);
        t = sqrt_n;
    }
    omp_set_num_threads(t);
    tstart = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic)
    for (int i = 3; i <= sqrt_n ; i += 2) {
        int incr = i * 2;
        if (i >= 9 && i % 3 == 0) {
            continue;
        }
        if (i >= 25 && i % 5 == 0) {
            continue;
        }
        if (i >= 121 && i % 11 == 0) {
            continue;
        }
        if (i >= 169 && i % 13 == 0) {
            continue;
        }
        if (*(p + i/2)) {
            for (int j = i * i; j > 0 && j <= n; j += incr) {
                *(p + j/2) = false;
            }
        }
        
    }
    ttaken = omp_get_wtime() - tstart;
    printf("Time take for the main part: %f\n", ttaken);
    // output to file
    FILE *fp = fopen("./N.txt ", "w+");
    int line_num = 1;
    fprintf(fp, "%d, %d, %d\n", line_num++, 2, 0);
    fprintf(fp, "%d, %d, %d\n", line_num++, 3, 1);
    int last_pos = 1;
    for (int i = 2; i < flags_size; i++) {
        if (*(p + i)) {
            fprintf(fp, "%d, %d, %d\n", line_num++, i*2 + 1, (i - last_pos)*2);
            last_pos = i;
        }
    }
    fclose(fp);
    return 0;
     
}