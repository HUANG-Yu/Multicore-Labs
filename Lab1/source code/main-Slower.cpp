//
//  main.cpp
//  stsm
//
// sequential version of travelling sales man using modified Held–Karp algorithm (without + dij)
//  Created by Yu Huang on 16/3/16.
//  Copyright (c) 2016 Yu Huang. All rights reserved.
//

// pass argument when run: Product- Edit Scheme- ... -Run-Argument
// shortcut to open run argument: Command+< (Command + Shift + ,)

// using C++ library next_permutation method - optimal solution

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <pthread.h>
#include <math.h>

using namespace std;

// store all the permutes
vector<vector<int> > permutes;
// memoize each partition's min_cost
vector<int> min_costs;
// memoize each partition's min_cost corresponding permutes index
vector<int> min_costs_indices;
// the input distance from the txt file
vector<vector<int> > dists;
// the number of threads passed in
int t;
// workload of each thread
int each;

inline int fac (int n) {
    return (n == 1)? 1: fac(n-1) * n;
}

void* cal_local_min (void* args) {
    long index = (long) args;
    long upper_bound = (index == t-1)? permutes.size(): index + each;
    for (long i = index; i < upper_bound; i++) {
        int cur_min_cost = 0;
        for (int j = 0; j < permutes[i].size() - 1; j++) {
            cur_min_cost += dists[permutes[i][j]][permutes[i][j+1]];
            if (cur_min_cost > min_costs[index]) {
                break;
            }
        }
        if (cur_min_cost < min_costs[index]) {
            min_costs[index] = cur_min_cost;
            min_costs_indices[index] = (int)i;
        }
    }
    return args;
}

int main(int argc, char * argv[]) {
    // IO read at once
    FILE* file = fopen (argv[3], "r");
    // the number of cities
    int x = (int)strtol(argv[1], NULL, 10);
    // the number of threads
    t = (int)strtol(argv[2], NULL, 10);
    dists.resize(x, vector<int>(x, 0));
    int num = 0;
    fscanf(file, "%d", &num);
    int row = 0;
    int col = 0;
    while (!feof (file))
    {
        dists[row][col++] = num;
        if (col == x) {
            row++;
            col = 0;
        }
        fscanf (file, "%d", &num);
    }
    fclose (file);
    // calculate the total number of permutations
    int n = fac(x-1);
    permutes.resize(n, vector<int>(x, 0));
    // store all permutations
    vector<int> cur_permutes(x, 0);
    // initialize the first choice
    for (int i = 1; i < x; i++) {
        cur_permutes[i] = i;
    }
    for (int i = 0; cur_permutes[0] == 0; i++) {
        permutes[i] = cur_permutes;
        next_permutation(cur_permutes.begin(), cur_permutes.end());
    }
    // work partitionning
    // tailor thread to the need
    if (t > n) {
        printf("%d threads were requested but the algorithm used only %d threads\n", t, n);
        t = n;
    }
    each = n/t;
    min_costs.resize(t, 2147483647);
    min_costs_indices.resize(t, 0);
    
    // thread functions invoke - type conversion needed for pthread
    pthread_t* thread_handles = (pthread_t *)malloc(t * sizeof(pthread_t));
    
    for (long thread = 0; thread < t; thread++) {
        pthread_create(&thread_handles[thread], NULL, cal_local_min, (void*) thread);
    }
    
    for (int thread = 0; thread < t; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }
    free(thread_handles);
    // JUST FOR TEST - cal_local_min((void*)4);
    
    int minCost = min_costs[0];
    int min_cost_index = min_costs_indices[0];
    // output result - calculate result sequentially
    for (int i = 1; i < min_costs.size(); i++) {
        if (min_costs[i] < minCost) {
            minCost = min_costs[i];
            min_cost_index = min_costs_indices[i];
        }
    }
    printf("Shortest path is:\n");
    for (int i = 0; i < x; i++) {
        printf("%d ", permutes[min_cost_index][i]);
    }
    printf("\ntotal weight = %d\n", minCost);
    return 0;
}
