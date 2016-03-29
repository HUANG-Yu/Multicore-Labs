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

// using C++ library next_permutation method

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char * argv[]) {
    // IO read at once
    FILE* file = fopen (argv[3], "r");
    int x = (int)strtol(argv[1], NULL, 10);
    int dists[x][x];
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
    // C++ next_permutation
    int minCostPath[x];
    int vertices[x];
    int minCost = 2147483647;
    for (int i = 0; i < x; i++) {
        vertices[i] = i;
    }
    do {
        // calculates path
        int curPathCost = 0;
        for (int i = 0; i < x-1; i++) {
            curPathCost += dists[vertices[i]][vertices[i+1]];
            if (curPathCost > minCost) {
                break;
            }
        }
        if (curPathCost < minCost) {
            minCost = curPathCost;
            for (int j = 0; j < x; j++) {
                minCostPath[j] = vertices[j];
            }
        }
    } while (next_permutation(vertices, vertices+x) && vertices[0] == 0);
    // output result
    printf("Shortest path is:\n");
    for (int i = 0; i < x; i++) {
        printf("%d ", minCostPath[i]);
    }
    printf("\ntotal weight = %d\n", minCost);
    return 0;
}
