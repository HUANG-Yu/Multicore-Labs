#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>

int main(int argc, char * argv[]) {
    double tstart = 0.0, ttaken = 0.0;
    // read IO
    int n = (int)strtol(argv[1], NULL, 10);
    int t = (int)strtol(argv[2], NULL, 10);
    // shift the upper bound to match array index
    int upper = (n + 1) / 2 - 2;
    n -= 1;
    bool flags[n];
    bool* p = &flags[0];
    for (int i = 0; i < n; i++) {
        *(p+i) = true;
    }
    if (t > upper) {
        printf("%d threads were requested but the algorithm used only %d threads\n", t, upper);
        t = upper;
    }
    omp_set_num_threads(t);
    // run Sieve Algo
    tstart = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i <= upper; i++) {
        int incr = i + 2;
        if (*(p + i) != false) {
            // j > 0 to avoid overflow situation when n is big
            for (int j = (i+2) * (i+2); j > 0 && j <= n + 1; j += incr) {
                flags[j-2] = false;
            }
        }
    }
    ttaken = omp_get_wtime() - tstart;
    printf("Time take for the main part: %f\n", ttaken);
    // output to file
    FILE *fp = fopen("./N.txt", "w+");
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