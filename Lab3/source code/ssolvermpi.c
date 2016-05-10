//  Created by Yu Huang on 16/4/24.
//  Copyright (c) 2016年 Yu Huang. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>

bool fill_flag = false;

char* argv1;

void fill_numbers(int index, char board[9][9], bool prefill) {
    // board is full
    if (index == 81) {
        if (fill_flag) {
            return;
        }
        fill_flag = true;
        char* sol = "sol";
        char* filename = strcat(argv1, sol);
        FILE *fp = fopen(filename, "w+");
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                fputc(board[i][j], fp);
            }
            fputs("\n", fp);
        }
        fclose(fp);
        return;
    }
    int row = index / 9;
    int col = index % 9;
    if (prefill) {
        // check validity
        for (int i = 0; i < 9; i++) {
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
        for (int i = grid_row; i < grid_row + 3; i++) {
            for (int j = grid_col; j < grid_col + 3; j++) {
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
        for (int i = 0; flag && i < 9; i++) {
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
        for (int i = grid_row; flag && i < grid_row + 3; i++) {
            for (int j = grid_col; flag && j < grid_col + 3; j++) {
                char comp = board[i][j];
                if (i != row && j != col && comp == cur) {
                    flag = false;
                }
            }
        }
        if (flag) {
            board[row][col] = num + '0';
            fill_numbers(index + 1, board, false);
        }
    }
    // reset the board if number invalid
    board[row][col] = '0';
    return;
}


int main(int argc, char * argv[]) {
    char board[9][9];
    // number of processes
    int comm_sz;
    // my process rank
    int my_rank;
    int first_zero = -1;
    int row = 0;
    int col = 0;
    
    // IO read at once
    FILE* file = fopen(argv[1], "r");
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
    
    // revise global variable
    argv1 = argv[1];
    argv1[strlen(argv1) - 2] = '\0';
    
    row = first_zero / 9;
    col = first_zero % 9;
    
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    if (my_rank == 0) {
        // running sudoku solver
        for (int num = 1; num <= 3; num++) {
            if (fill_flag == false) {
                board[row][col] = num + '0';
                fill_numbers(first_zero, board, true);
            }
        }
    } else if (my_rank == 1) {
        // running sudoku solver
        for (int num = 4; num <= 6 ; num++) {
            if (fill_flag == false) {
                board[row][col] = num + '0';
                fill_numbers(first_zero, board, true);
            }
        }
    } else {
        // running sudoku solver
        for (int num = 7; num <= 9; num++) {
            if (fill_flag == false) {
                board[row][col] = num + '0';
                fill_numbers(first_zero, board, true);
            }
        }
    }
    
    MPI_Finalize();
    return 0;
}