//  Created by Yu Huang on 16/4/24.
//  Copyright (c) 2016年 Yu Huang. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include <omp.h>
#include <unistd.h>
#include <string.h>

char fill[9][9];

bool unfill_flag = true;

void fill_numbers(int index, char board[9][9], bool prefill) {
    if (!unfill_flag) {
        return;
    }
    // board is full
    if (index == 81) {
        if (unfill_flag == false) {
            return;
        }
        #pragma omp critical
        {
            if (unfill_flag) {
                unfill_flag = false;
                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 9; j++) {
                        fill[i][j] = board[i][j];
                    }
                }
            }
        }
        return;
    }
    int row = index / 9;
    int col = index % 9;
    if (prefill) {
        // check validity
        for (int i = 0; unfill_flag && i < 9; i++) {
            // compare selected row
            char comp_row = board[row][i];
            if (i != col && comp_row == board[row][col]) {
                board[row][col] = '0';
                return;
            }
            // compare selected column
            char comp_col = board[i][col];
            if (i != row && comp_col == board[row][col]) {
                board[row][col] = '0';
                return;
            }
        }
        // compare selected grid
        int grid_row = (row / 3) * 3;
        int grid_col = (col / 3) * 3;
        for (int i = grid_row; unfill_flag && i < grid_row + 3; i++) {
            for (int j = grid_col; unfill_flag && j < grid_col + 3; j++) {
                char comp = board[i][j];
                if (i != row && j != col && comp == board[row][col]) {
                    board[row][col] = '0';
                    return;
                }
            }
        }
        fill_numbers(index + 1, board, false);
    }
    if (board[row][col] != '0') {
        fill_numbers(index + 1, board, false);
        return;
    }
    // try out from number 1 - 9
    for (int num = 1; num <= 9; num++) {
        char cur = num + '0';
        bool flag = true;
        // check validity of selected number
        for (int i = 0; unfill_flag &&flag && i < 9; i++) {
            // compare selected row
            char comp_row = board[row][i];
            if (i != col && comp_row == cur) {
                flag = false;
            }
            // compare selected column
            char comp_col = board[i][col];
            if (i != row && comp_col == cur) {
                flag = false;
            }
        }
        // compare selected grid
        int grid_row = (row / 3) * 3;
        int grid_col = (col / 3) * 3;
        for (int i = grid_row; unfill_flag &&flag && i < grid_row + 3; i++) {
            for (int j = grid_col; unfill_flag && flag && j < grid_col + 3; j++) {
                char comp = board[i][j];
                if (i != row && j != col && comp == cur) {
                    flag = false;
                }
            }
        }
        if (flag && unfill_flag) {
            board[row][col] = num + '0';
            /* for debugging only
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    printf("%c ", board[i][j]);
                }
                printf("\n");
            }
            printf("\n"); */
            fill_numbers(index + 1, board, false);
        }
    }
    // reset the board if number invalid
    board[row][col] = '0';
    return;
}


int main(int argc, char * argv[]) {
    // IO read at once
    FILE* file = fopen(argv[1], "r");
    // record the first position need to be filled
    int first_zero = -1;
    char board[9][9];
    char num = 0;
    num = fgetc(file);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = num;
            if (first_zero < 0 && num == '0') {
                first_zero = i * 9 + j;
            }
            num = fgetc(file);
        }
        num = fgetc(file);
    }
    fclose(file);

    // set number of thread
    int t = (argv[2] == NULL)? 9: (int)strtol(argv[2], NULL, 10);
    if (t > 9) {
        printf("%d threads are given but only 9 threads are used\n", t);
        t = 9;
    }
    omp_set_num_threads(t);
    // running solver
    #pragma omp parallel for schedule(dynamic) firstprivate(board)
    for (int i = 1; i <= 9; i++) {
        // uncomment sleep to see the speedup of parallel sudoku
        // sleep(1);
        board[first_zero / 9][first_zero % 9] = i + '0';
        fill_numbers(first_zero, board, true);
    }
    
    // write output to file
    char* argv1 = argv[1];
    argv1[strlen(argv1) - 2] = '\0';
    char* sol = "sol";
    char* filename = strcat(argv1, sol);
    FILE *fp = fopen(filename, "w+");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            fputc(fill[i][j], fp);
        }
        fputs("\n", fp);
    }
    
    fclose(fp);
    return 0;
}