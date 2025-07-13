/**
 * @brief A compilation of helper functions for sudoku_solver.c
 * @author J. Pettersson
 * @date 2025-04-30
 * 
 * @{
 */

#ifndef SUDOKU_SOLVER_UTILS_H
#define SUDOKU_SOLVER_UTILS_H

#include "../DataStructures/LinkedList/linked_list.h"
#include "sudoku_cell.h"
#include <stdbool.h>

#define GRID_SIZE 9
#define X_SIZE 3
#define Y_SIZE 3

void init_board(int board[GRID_SIZE][Y_SIZE][X_SIZE]);
void print_welcome_msg(void);
void print_how_to_msg(void);
void print_board(int board[GRID_SIZE][Y_SIZE][X_SIZE]);
void print_result_func(int board[GRID_SIZE][Y_SIZE][X_SIZE]);
void print_final_msg(void);
void read_board_input(int board[GRID_SIZE][Y_SIZE][X_SIZE]);
void request_and_manage_user_input(int board[GRID_SIZE][Y_SIZE][X_SIZE]);
bool request_board_confirm(int board[GRID_SIZE][Y_SIZE][X_SIZE]);
bool request_play_again();
bool request_yes_or_no(char *msg);
void clean_stdin(void);
void check_stdin(void);
bool validate_digit_input(char *in_str);
void store_grid_values_input(int board[GRID_SIZE][Y_SIZE][X_SIZE],
                                int grid_i, char *in_str);
bool validate_board(int board[GRID_SIZE][Y_SIZE][X_SIZE], bool solved);
bool check_each_grid(int board[GRID_SIZE][Y_SIZE][X_SIZE]);
bool check_each_row(int board[GRID_SIZE][Y_SIZE][X_SIZE]);
bool check_each_column(int board[GRID_SIZE][Y_SIZE][X_SIZE]);
bool checks_repeating_digits(int *digits);
bool user_input_error(char *error_msg);
void run_time_error(char *error_msg);
void solve_sudoku(int board[GRID_SIZE][Y_SIZE][X_SIZE]);
Linked_List *empty_unknowns_list(int board[GRID_SIZE][Y_SIZE][X_SIZE]);
int solve_elimination_method(int board[GRID_SIZE][Y_SIZE][X_SIZE],
                                Linked_List *unknowns);
bool update_constraints(int board[GRID_SIZE][Y_SIZE][X_SIZE], Cell *c,
                            int grid_pos, int x_pos, int y_pos);
bool update_constraints_grid(int board[GRID_SIZE][Y_SIZE][X_SIZE], Cell *c,
                                int grid_pos);
bool update_constraints_row(int board[GRID_SIZE][Y_SIZE][X_SIZE], Cell *c,
                                int grid_pos, int y_pos);
bool update_constraints_col(int board[GRID_SIZE][Y_SIZE][X_SIZE], Cell *c,
                                int grid_pos, int x_pos);
int solve_backtrack_method(int board[GRID_SIZE][Y_SIZE][X_SIZE],
                            Linked_List *unknowns);
void duplicate_board(int new_board[GRID_SIZE][Y_SIZE][X_SIZE],
                        int original_board[GRID_SIZE][Y_SIZE][X_SIZE]);
Linked_List *duplicate_unknown_list(Linked_List *unknowns);

/** @} */

#endif // SUDOKU_SOLVER_UTILS_H