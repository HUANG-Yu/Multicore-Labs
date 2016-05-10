//  Created by Yu Huang on 16/2/14.
//  Copyright (c) 2016年 Yu Huang. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include  <unistd.h>

bool is_valid(char board[9][9], int row, int col) {
    char cur = board[row][col];
    for (int i = 0; i < 9; i++) {
        // compare selected row
        char comp_row = board[row][i];
        if (i != col && comp_row == cur) {
            return false;
        }
        // compare selected column
        char comp_col = board[i][col];
        if (i != row && comp_col == cur) {
            return false;
        }
    }
    // compare selected grid
    int grid_row = (row / 3) * 3;
    int grid_col = (col / 3) * 3;
    for (int i = grid_row; i < grid_row + 3; i++) {
        for (int j = grid_col; j < grid_col + 3; j++) {
            char comp = board[i][j];
            if (i != row && j != col && comp == cur) {
                return false;
            }
        }
    }
    return true;
}

bool solve(char board[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == '0') {
                for (int k = 1; k <= 9; k++) {
                    board[i][j] = k + '0';
                    if (is_valid(board, i, j) && solve(board)) {
                        return true;
                    }
                    board[i][j] = '0';
                }
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char * argv[]) {
    // IO read at once
    FILE* file = fopen(argv[1], "r");
    char board[9][9];
    char num = 0;
    num = fgetc(file);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = num;
            printf("%c ", board[i][j]);
            num = fgetc(file);
        }
        num = fgetc(file);
        printf("\n");
    }
    printf("\n");
    fclose(file);
    // running solver
    solve(board);
    // print output to screen
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    return 0;
}