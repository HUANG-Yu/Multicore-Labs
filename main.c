//
//  main.c
//  MulticoreSamples
//
//  Created by Yu Huang on 16/2/14.
//  Copyright (c) 2016年 Yu Huang. All rights reserved.
//

// pass argument when run: Product- Edit Scheme- ... -Run-Argument
// shortcut to open run argument: Command+< (Command + Shift + ,)

// using C++ library next_permutation method - optimal solution

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define INT_MAX 2147483647

// store all the permutes
int* permutes_p;
// memoize each partition's min_cost
int* min_costs_p;
// memoize each partition's min_cost corresponding permutes index
int*  min_costs_indices_p;
// the input distance from the txt file
int* dists_p;
// the number of threads passed in
int t = 0;
// workload of each thread
int each;
int x = 0;
int n = 0;

extern inline int fac (int n) {
    return (n == 1)? 1: fac(n-1) * n;
}

extern inline void swap (int *p, int *q) {
    int t = *p;
    *p = *q;
    *q = t;
}

extern inline void reverse (int* nums, int begin, int end) {
    for (int i = begin; i < (begin + end + 1)/2; i +=1 ) {
        swap(nums + i, nums + end + begin - i);
    }
}

extern inline void next_permutation (int* nums, int size) {
    int i = size - 1;
    int j = size - 1;
    while (i > 0 && *(nums + i) <= *(nums + i - 1)) {
        i--;
    }
    if (i == 0) {
        reverse(nums, 0, size - 1);
        return;
    }
    while (*(nums + j) <= *(nums + i-1)) {
        j--;
    }
    swap(nums + j, nums + i - 1);
    reverse(nums, i, size - 1);
}

void* cal_local_min (void* args) {
    long index = (long) args;
    long upper_bound = (index == t-1)? n: index + each;
    for (long i = index; i < upper_bound; i += 1) {
        int cur_min_cost = 0;
        for (int j = 0; j < x-1; j += 1) {
            int row = *(permutes_p + i*x + j);
            int col = *(permutes_p + i*x + j + 1);
            cur_min_cost += *(dists_p + row*x + col);
            if (cur_min_cost > *(min_costs_p + index)) {
                break;
            }
        }
        if (cur_min_cost < *(min_costs_p + index)) {
            *(min_costs_p + index) = cur_min_cost;
            *(min_costs_indices_p + index) = (int)i;
        }
    }
    return args;
}

int main(int argc, char * argv[]) {
    // IO read at once
    FILE* file = fopen (argv[3], "r");
    // the number of cities
    x = (int)strtol(argv[1], NULL, 10);
    // the number of threads
    t = (int)strtol(argv[2], NULL, 10);
    dists_p = malloc(sizeof(int) * x * x);
    int num = 0;
    fscanf(file, "%d", &num);
    int row = 0;
    int col = 0;
    while (!feof (file))
    {
        *(dists_p + row * x + col++) = num;
        if (col == x) {
            row += 1;
            col = 0;
        }
        fscanf (file, "%d", &num);
    }
    fclose (file);
    // calculate the total number of permutations
    n = fac(x-1);
    permutes_p = malloc(sizeof(int) * n * x);
    // store all permutations
    int*  cur_permutes = malloc(sizeof(int) * x);
    // initialize the first choice
    for (int i = 0; i < x; i += 1) {
        *(cur_permutes + i) = i;
    }
    for (int i = 0; *cur_permutes == 0; i += 1) {
        for (int j = 0; j < x; j += 1) {
            *(permutes_p + i * x + j) = *(cur_permutes + j);
        }
        next_permutation(cur_permutes, x);
    }
    // work partitionning
    // tailor thread to the need
    if (t > n) {
        printf("%d threads were requested but the algorithm used only %d threads\n", t, n);
        t = n;
    }
    each = n/t;
    min_costs_p = malloc(sizeof(int) * t);
    for (int i = 0; i < t; i += 1) {
        *(min_costs_p + i) = INT_MAX;
    }
    min_costs_indices_p = malloc(sizeof(int) * t);
    // thread functions invoke - type conversion needed for pthread
    pthread_t* thread_handles = (pthread_t *)malloc(t * sizeof(pthread_t));
    
    for (long thread = 0; thread < t; thread += 1) {
        pthread_create(&thread_handles[thread], NULL, cal_local_min, (void*) thread);
    }
    
    for (int thread = 0; thread < t; thread += 1) {
        pthread_join(thread_handles[thread], NULL);
    }
    free(thread_handles);
    
    int minCost = *min_costs_p;
    int min_cost_index = *min_costs_indices_p;
    // output result - calculate result sequentially
    for (int i = 1; i < t; i += 1) {
        if (*(min_costs_p + i) < minCost) {
            minCost = *(min_costs_p + i);
            min_cost_index = *(min_costs_indices_p + i);
        }
    }
    printf("Shortest path is:\n");
    for (int i = 0; i < x; i += 1) {
        printf("%d ", *(permutes_p + min_cost_index * x + i));
    }
    printf("\ntotal weight = %d\n", minCost);
    return 0;
}

