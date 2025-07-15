/* A compilation of helper functions for sudoku_solver.c
 *
 * Author:     J. Pettersson
 * Date:       2025-04-30
 */

#include "sudoku_solver_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE (GRID_SIZE + 10)

#define GRIDS_IN_ROW_NUM 3
#define GRIDS_IN_COL_NUM 3

#define SUCCESS_RET 1
#define STALEMATE_RET 0
#define ERROR_RET -1

/* ----------------------- Start Functions ----------------------- 
   Preparatory functions performed at the start of the program.
   --------------------------------------------------------------- */

/* Initializes the internal representation of the Sudoku board
 * with values of 0.
 */
void init_board(int board[GRID_SIZE][Y_SIZE][X_SIZE]) {
    for (int grid = 0 ; grid < GRID_SIZE ; grid++) {
        for (int y = 0 ; y < Y_SIZE ; y++) {
            for (int x = 0 ; x < X_SIZE ; x++) {
                board[grid][y][x] = 0; 
            }
        }
    }
}

/* ----------------------- Print Functions ----------------------- 
   Functions whose main purpose is to print information to
   the terminal.
   --------------------------------------------------------------- */

/* Prints information regrading the program's purpose and capacity.
 */
void print_welcome_msg(void) {
    printf("\nThis program solves numerical (between 1 to 9) 9x9 Sudoku\n"
            "problems using a combination of the elimination method and\n"
            "backtracking. It should be able to solve legal Sudoku\n"
            "problems of any difficulty. If there are more than one\n"
            "solution to the specified problem, the program will only\n"
            "present one of these solutions.\n");
}

/* Prints instructions on how to input a Sudoku board to the
 * program.
 */
void print_how_to_msg(void) {
    printf("\nHow to use:\n\n"
            "You will be asked to input a Sudoku problem you wish the\n"
            "program to solve.\n\n"
            "   To register your Sudoku board, input your values one\n"
            "   grid at a time. Enter all 9 values within a grid into a\n"
            "   single line, press ENTER and proceed to the next grid.\n\n"
            "   Within a grid and between the grids, traverse the values\n"
            "   row by row, from left to right.\n\n"
            "   Denote each known value by a digit from 1 to 9.\n"
            "   Denote each unknown value by '0'.\n\n"
            "During this process, the following commands apply. Write\n"
            "the number of the command you want to execute and\n"
            "press ENTER.\n\n"
            "   1. To erase all previous input and start over again.\n\n"
            "   2. To rewrite the input of the previous grid.\n\n"
            "   3. To close the program.\n");
}

/* Prints a Sudoku board.
 */
void print_board(int board[GRID_SIZE][Y_SIZE][X_SIZE]) {
    int start_grid;
    int grid;
    int y;

    for (int i = 0 ; i < Y_SIZE * GRIDS_IN_COL_NUM ; i++) {

        if (i % Y_SIZE == 0) {
            printf("* * * * * * * * * * * * *\n");
            start_grid = i;
            y = 0;
        }

        printf("* ");
        grid = start_grid;

        for (int j = 0 ; j < Y_SIZE ; j++) {
            for (int x = 0 ; x < X_SIZE ; x++) {
                printf("%d ", board[grid][y][x]);
            }
            printf("* ");
            grid++;
        }
        printf("\n");
        y++;
    }
    printf("* * * * * * * * * * * * *\n\n");
}

/* Prints to inform the user if the specified board has been
 * solved or not.
 */
void print_result_func(int board[GRID_SIZE][Y_SIZE][X_SIZE]) {
    bool found = validate_board(board, true);

    if (found) {
        printf("\nSolution found!\n\n");
        print_board(board);
    }
    else {
        printf("\nSolution not found.\n\n");
    }
}

/* Prints closing statement before successful program termination.
 */
void print_final_msg(void) {
    printf("\nExiting Program!\n");
}

/* --------------------- Read Input Functions ---------------------
   Funtions that reads user input from the terminal.
   ---------------------------------------------------------------- */

/* Requests and reads a Sudoku board to be solved. Function will
 * repeatedly request user input until user inputs a valid Sudoku
 * board and confirms its correctness.
 */
void read_board_input(int board[GRID_SIZE][Y_SIZE][X_SIZE]) {
    do {
        do {
            request_and_manage_user_input(board);
        } while (!validate_board(board, false) &&
                    user_input_error("Identical values within scope."));
    } while (!request_board_confirm(board));
}

/* Requests to input values grid by grid. Within a grid and
 * between the grids, the values are read row by row, from left
 * to right.
 * If user input is data not recognized/defined in this function,
 * the user is requested to try again. If user agrees, the user
 * is asked again to input the values for the same grid. But if
 * user refuses, program terminates.
 * The user can choose to (1) erase all previous input and start
 * over, (2) rewrite the values from the previous grid, or (3)
 * terminate the program.
 */
void request_and_manage_user_input(int board[GRID_SIZE][Y_SIZE][X_SIZE]) {
    int str_len;
    bool reset_same;
    bool reset_back;
    bool reset_all;
    char c;
    char in_str[BUFFER_SIZE];

    for (int i = 0 ; i < GRID_SIZE ; i++) {
        reset_same = false;
        reset_back = false;
        reset_all = false;

        printf("\nInput Grid %d: ", i + 1);

        if (fgets(in_str, sizeof(in_str), stdin) == NULL) {
            check_stdin();
        }

        str_len = strlen(in_str) - 1;

        if (str_len != 1 && str_len != GRID_SIZE) {
            if (str_len == ((BUFFER_SIZE - 1) - 1)) {
                clean_stdin();
            }

            reset_same = user_input_error("Invalid Input.");
        }

        if (str_len == 1) {
            c = in_str[0];

            if (c == '1') {
                if (request_yes_or_no("Reset all input?")) {
                    reset_all = true;
                }
                else {
                    reset_same = true;
                }
            }
            else if (c == '2') {
                if (request_yes_or_no("Reset previous input?")) {
                    reset_back = true;
                }
                else {
                    reset_same = true;
                }
            }
            else if (c == '3') {
                if (request_yes_or_no("Close program?")) {
                    exit(EXIT_SUCCESS);
                }
                else {
                    reset_same = true;
                }
            }
            else {
                reset_same = user_input_error("Invalid Input");
            }
        }

        if (str_len == GRID_SIZE) {
            in_str[strcspn(in_str, "\n")] = '\0';

            if (!validate_digit_input(in_str)) {
                reset_same = user_input_error("Invalid Input.");
            }

            store_grid_values_input(board, i, in_str);
        }

        if (reset_all) {
            i = -1;
        }
        else if (reset_back) {
            if (i == 0) {
                i = -1;
            } else {
                i -= 2;
            }
        }
        else if (reset_same) {
            i--;
        }
    }
}

/* Requests to ascertain the correctness of the inputted Sudoku
 * board.
 * Returns: True if user confirms; false if user opposes
 */
bool request_board_confirm(int board[GRID_SIZE][Y_SIZE][X_SIZE]) {
    bool answer;

    printf("\nYou have registered the following sudoku board:\n\n");
    print_board(board);

    if (!(answer = request_yes_or_no("Is this correct?"))) {
        printf("Please try again.\n");
    }

    return answer;
}

/* Requests to play again with a new Sudoku board.
 * Returns: True if yes; false if no
 */
bool request_play_again() {
    return request_yes_or_no("Play again?");
}

/* Requests a yes or no answer.
 * Returns: True if yes; false if no
 */
bool request_yes_or_no(char *msg) {
    int c;

    do {
        printf("%s (y/n): ", msg);

        c = getchar();
        check_stdin();

        if (c != '\n') {
            clean_stdin();
        }

    } while (c != EOF && c != 'y' && c != 'n');

    return c == 'y';
}

/* ---------------- Manage Input Stream Functions ----------------
   Helper functions for safe and accurate user input readings.
   --------------------------------------------------------------- */

/* Cleans the input stream (until the next new line or EOF) to
 * properly process the next line of user input.
 */
void clean_stdin(void) {
    int c;

    do {
        c = getchar();
        check_stdin();
    } while (c != '\n' && c != EOF);
}

/* Tests and checks the input stream for errors or EOF and handles
 * these scenarios appropriately.
 */
void check_stdin(void) {
    if (ferror(stdin)) {
        run_time_error("Failed to read input.");
    }

    if (feof(stdin)) {
        exit(EXIT_SUCCESS);
    }
}

/* -------------------- Store Input Functions --------------------
   Functions that processes user input by verifying and storing
   the data.
   --------------------------------------------------------------- */

/* Checks if the values are digits within the range of 1 to 9.
 * Returns: True if within range; false if not.
 */
bool validate_digit_input(char *in_str) {
    for (int i = 0 ; i < GRID_SIZE ; i++) {

        if (in_str[i] < '0' || in_str[i] > '9') {
            return false;
        }
    }

    return true;
}

/* Stores a grid's values, passed as a string, into the
 * specified grid of the program's Sudoku board structure.
*/
void store_grid_values_input(int board[GRID_SIZE][Y_SIZE][X_SIZE], 
                                int grid_i, char *in_str) {
    int i = 0;
    for (int y = 0 ; y < Y_SIZE ; y++) {
        for (int x = 0 ; x < X_SIZE ; x++) {
            board[grid_i][y][x] = in_str[i] - '0';
            i++;
        }
    }
}

/* ------------------- Validate Board Functions -------------------
   Functions that checks and validates the Sudoku board's state.
   ---------------------------------------------------------------- */

/* Checks each grid, row, and column for any repeated known values
 * within their respective scopes. And, if the board is presumed
 * solved (i.e. 'solved' is passed the argument true), checks for
 * any remaining unknown values.
 * The board is considered valid when no repeated values and, if 
 * presumed solved, no unknown values, are found.
 * Returns: True if board is valid; false if not.
*/
bool validate_board(int board[GRID_SIZE][Y_SIZE][X_SIZE], bool solved) {
    bool valid = false;

    if (check_each_grid(board)) {
        if (check_each_row(board)) {
            if (check_each_column(board)) {
                valid = true;
            }
        }
    }

    if (!valid) {
        return false;
    }

    if (solved) {

        for (int grid = 0 ; grid < GRID_SIZE ; grid++) {
            for (int y = 0 ; y < Y_SIZE ; y++) {
                for (int x = 0 ; x < X_SIZE ; x++) {
    
                    if (board[grid][y][x] == 0) {
                        return false;
                    }
                }
            }
        }
    }
    
    return true;    
}

/* Checks each grid for any repeated known values within their
 * respective scopes.
 * Returns: True if there are no repetitions; false if there are.
*/
bool check_each_grid(int board[GRID_SIZE][Y_SIZE][X_SIZE]) {
    int digits[GRID_SIZE];
    int i;

    for (int grid = 0 ; grid < GRID_SIZE ; grid++) {
        i = 0;

        for (int y = 0 ; y < Y_SIZE ; y++) {
            for (int x = 0 ; x < X_SIZE ; x++) {
                digits[i] = board[grid][y][x];
                i++;
            }
        }

        if (checks_repeating_digits(digits)) {
            return false;
        }
    }

    return true;
}

/* Checks each row for any repeated known values within their
 * respective scopes.
 * Returns: True if there are no repetitions; false if there are.
*/
bool check_each_row(int board[GRID_SIZE][Y_SIZE][X_SIZE]) {
    int digits[GRID_SIZE];
    int start_grid;
    int grid = 0;
    int y = 0;
    int i;
    
    for (int row = 0 ; row < GRID_SIZE ; row++) {

        if (row % X_SIZE == 0) {
            if (row == 0) {
                start_grid = 0;
            }
            else {
                start_grid += GRIDS_IN_ROW_NUM;
            }
            y = 0;
        }

        i = 0;
        grid = start_grid;
        
        for (int j = 0 ; j < Y_SIZE ; j++) {
            for (int x = 0 ; x < X_SIZE ; x++) {
                digits[i] = board[grid][y][x];
                i++;
            }
            grid++;
        }
        y++;

        if (checks_repeating_digits(digits)) {
            return false;
        }
    }

    return true;
}

/* Checks each column for any repeated known values within their
 * respective scopes.
 * Returns: True if there are no repetitions; false if there are.
*/
bool check_each_column(int board[GRID_SIZE][Y_SIZE][X_SIZE]) {
    int digits[GRID_SIZE];
    int start_grid = -1;
    int grid = 0;
    int x = 0;
    int i;
    
    for (int col = 0 ; col < GRID_SIZE ; col++) {

        if (col % Y_SIZE == 0) {
            if (col == 0) {
                start_grid = 0;
            }
            else {
                start_grid++;
            }
            x = 0;
        }
        
        i = 0;
        grid = start_grid;

        for (int j = 0 ; j < Y_SIZE ; j++) {
            for (int y = 0 ; y < X_SIZE ; y++) {
                digits[i] = board[grid][y][x];
                i++;
            }
            grid += GRIDS_IN_ROW_NUM;
        }
        x++;

        if (checks_repeating_digits(digits)) {
            return false;
        }
    }

    return true;
}

/* Checks if there are repeating digits within the array
 * passed to the function.
 * Returns: True if there are repeating digits; false if there
 *          is only one instance of every digit in the array
*/
bool checks_repeating_digits(int *digits) {
    int counter[GRID_SIZE];
    int val;

    for (int i = 0 ; i < GRID_SIZE ; i++) {
        counter[i] = -1;
    }

    for (int j = 0 ; j < GRID_SIZE ; j++) {
        val = digits[j];

        if (val != 0) {
            counter[val - 1]++;
        }
    }

    for (int k = 0 ; k < GRID_SIZE ; k++) {

        if (counter[k] > 1) {
            return true;
        }
    }

    return false;
}

/* ----------------------- Error Functions ----------------------- 
   Functions that handle errors.
   --------------------------------------------------------------- */

/* Handles user input error by printing the error message passed
 * to the function and requesting the user to rewrite the input. If
 * user opposes to try again, the function terminates the program.
 * Returns: True if user agrees to try again.
 */
bool user_input_error(char *error_msg) {
    printf("\nInput error: %s\n", error_msg);

    if (!request_yes_or_no("Try again?")) {
        exit(EXIT_SUCCESS);
    }

    return true;
}

/* Handles run-time error by printing the error message passed to
 * the function and terminating the program.
 */
void run_time_error(char *error_msg) {
    printf("\nRun-time error: %s\n", error_msg);
    exit(EXIT_FAILURE);
}

/* ----------------------- Solver Functions -----------------------
   Functions used to solve the Sudoku board.
   ---------------------------------------------------------------- */

/* Finds a solution to the specified Sudoku problem.
*/
void solve_sudoku(int board[GRID_SIZE][Y_SIZE][X_SIZE]) {
    Linked_List *unknowns;
    
    if ((unknowns = empty_unknowns_list(board)) == NULL) {
        run_time_error("Failed to set up solver");
    }

    if (list_is_empty(unknowns)) {
        list_destroy(unknowns);
        return;
    }
    
    if (solve_elimination_method(board, unknowns) == ERROR_RET) {
        list_destroy(unknowns);
        run_time_error("Failed to execute elimination method.");
    }

    if (solve_backtrack_method(board, unknowns) == ERROR_RET) {
        list_destroy(unknowns);
        run_time_error("Failed to execute backtracking method.");
    }

    list_destroy(unknowns);
}

/* Creates a linked list to store information regarding all the
 * cells of the Sudoku board where values are still unknown. The
 * information about a cell is comprised of its position (grid, x, y)
 * on the board and the list of values eligible to be stored in
 * that position.
 * Returns: On success, the linked list; on failure, NULL
*/
Linked_List *empty_unknowns_list(int board[GRID_SIZE][Y_SIZE][X_SIZE]) {
    Linked_List *unknowns;
    Cell *c;

    if ((unknowns = list_empty()) == NULL) {
        return NULL;
    }

    if ((list_value_handler(unknowns, cell_destroy)) == ERROR_RET) {
        list_destroy(unknowns);
        return NULL;
    }

    for (int grid = 0 ; grid < GRID_SIZE ; grid++) {
        for (int y = 0 ; y < Y_SIZE ; y++) {
            for (int x = 0 ; x < X_SIZE ; x++) {

                if (board[grid][y][x] == 0) {

                    if ((c = cell_create(grid, x, y, GRID_SIZE)) == NULL) {
                        list_destroy(unknowns);
                        return NULL;
                    }
                    
                    if (list_insert(unknowns, c) == NULL) {
                        cell_destroy(c);
                        list_destroy(unknowns);
                        return NULL;
                    }
                }
            }
        }
    }

    return unknowns;
}

/* ---------------------- Elimination Method ---------------------- */

/* Looks for a solution to the specified Sudoku problem by searching
 * for non-eligible values and removing those values from the
 * evaluated cell's eligible-values list. This process of
 * search-and-remove ends when the current search method is unable
 * to make new finds, either because the problem has been solved or
 * the elimination method has reached a stalemate (a solution may
 * exist, but the method is unable to progress further due to
 * insufficient information) or the problem is unsolvable.
 * Returns: 1 if the problem is solved or the method has reached
 *          a stalemate; 0 if the problem is unsolvable; -1 if an
 *          error occurred.
 */
int solve_elimination_method(int board[GRID_SIZE][Y_SIZE][X_SIZE],
                                Linked_List *unknowns) {
    int unknowns_size;
    int eligible_size;
    Cell *c;
    int grid_pos;
    int x_pos;
    int y_pos;
    int value;
    bool is_modified = true;

    if (unknowns == NULL) {
        return ERROR_RET;
    }

    while (is_modified) {
        is_modified = false;
        unknowns_size = list_length(unknowns);

        for (int i = 0 ; i < unknowns_size ; i++) {
            c = (Cell *)list_inspect(unknowns, i + 1);

            eligible_size = cell_eligible_values_length(c);

            if (eligible_size == ERROR_RET || eligible_size == 0) {
                return eligible_size;
            }

            grid_pos = cell_grid_pos(c);
            x_pos = cell_x_pos(c);
            y_pos = cell_y_pos(c);

            if (!update_constraints(board, c, grid_pos, x_pos, y_pos)) {
                return ERROR_RET;
            }

            eligible_size = cell_eligible_values_length(c);

            if (eligible_size == ERROR_RET || eligible_size == 0) {
                return eligible_size;
            }

            if (eligible_size == 1) {
                value = cell_inspect_eligible_values(c, 1);
                board[grid_pos][y_pos][x_pos] = value;

                list_remove(unknowns, i + 1);

                unknowns_size--;
                i--;
                is_modified = true;
            }
        }
    }

    return SUCCESS_RET;
}

/* Updates a cell's eligible-values list by traversing the specified
 * cell's row, column, and grid for known values and removing those
 * values from the cell's eligible-values list.
 * Returns: True on success; false if an error occurred.
*/
bool update_constraints(int board[GRID_SIZE][Y_SIZE][X_SIZE], Cell *c,
                            int grid_pos, int x_pos, int y_pos) {

    if (!update_constraints_grid(board, c, grid_pos)) {
        return false;
    }

    if (!update_constraints_row(board, c, grid_pos, y_pos)) {
        return false;
    }

    if (!update_constraints_col(board, c, grid_pos, x_pos)) {
        return false;
    }

    return true;
}

/* Updates a cell's eligible-values list by traversing the specified
 * cell's grid for known values and removing those values from the
 * cell's eligible-values list.
 * Returns: True on success; false if an error occurred.
*/
bool update_constraints_grid(int board[GRID_SIZE][Y_SIZE][X_SIZE], Cell *c,
                                int grid_pos) {
    int remove_num;

    for (int y = 0 ; y < Y_SIZE ; y++) {
        for (int x = 0 ; x < X_SIZE ; x++) {
            
            if ((remove_num = board[grid_pos][y][x]) != 0) {
                if (cell_update_eligible_values(c, remove_num) == NULL) {
                    return false;
                }
            }
        }
    }

    return true;
}

/* Updates a cell's eligible-values list by traversing the specified
 * cell's row for known values and removing those values from the
 * cell's eligible-values list.
 * Returns: True on success; false if an error occurred.
*/
bool update_constraints_row(int board[GRID_SIZE][Y_SIZE][X_SIZE], Cell *c,
                                int grid_pos, int y_pos) {
    int remove_num;
    int grid = grid_pos - (grid_pos % GRIDS_IN_ROW_NUM);
    int x = 0;

    for (int row = 0 ; row < GRID_SIZE ; row++) {
        
        if (row % X_SIZE == 0 && row != 0) {
            grid++;
            x = 0;
        }

        if ((remove_num = board[grid][y_pos][x]) != 0) {
            if (cell_update_eligible_values(c, remove_num) == NULL) {
                return false;
            }
        }
        x++;
    }

    return true;
}

/* Updates a cell's eligible-values list by traversing the specified
 * cell's column for known values and removing those values from the
 * cell's eligible-values list.
 * Returns: True on success; false if an error occurred.
*/
bool update_constraints_col(int board[GRID_SIZE][Y_SIZE][X_SIZE], Cell *c,
                                int grid_pos, int x_pos) {
    int remove_num;
    int grid = grid_pos % Y_SIZE;
    int y = 0;

    for (int col = 0 ; col < GRID_SIZE ; col++) {

        if (col % Y_SIZE == 0 && col != 0) {
            grid += 3;
            y = 0;
        }

        if ((remove_num = board[grid][y][x_pos]) != 0) {
            if (cell_update_eligible_values(c, remove_num) == NULL) {
                return false;
            }
        }
        y++;
    }

    return true;
}

/* --------------------- Backtracking Method --------------------- */

/* Looks for a solution to the specified Sudoku problem through
 * brute-force. Whenever an attempt to reach the solution fails, the
 * function backtracks to the most recent state that has at least one
 * untried attempt. This is implemented recursively.
 * 
 * The process is as follows:
 *  - A cell that has no known value is selected.
 *  - An eligible value from the cell's eligible-values list is put
 *    on trial.
 *  - The elimination method attempts to solve the problem.
 * 
 * The elimination method provides three different outcomes:
 *  - A solution is reached, the function exits.
 *  - A stalemate is reached (a solution may exist, but the method
 *    is unable to progress further due to insufficient information).
 *    The function recurses (again), an eligible value of another
 *    cell (that has no known value) is put on trial.
 *  - The elimination method is unable to find a solution.
 *    Backtracking, then, occurs in one of the two ways stated:
 *      - To the recently tried eligible value. The value is deemed
 *        ineligible, and the next eligible value of the current cell
 *        is tried.
 *      - Or, if the current cell has no eligible values left, to the
 *        previous cell. The function continues to backtrack to
 *        previous evaluated cells until it encounters a cell that
 *        has at least one untried value. 
 * 
 * This function should, in principle, be able to solve any legal
 * 9x9 Sudoku problem.
 * 
 * Returns: 1 if the problem is solved; 0 if the elimination method
 *          has reached a stalemate or is unable to find a solution;
 *          -1 if an error occurred.
 */
int solve_backtrack_method(int board[GRID_SIZE][Y_SIZE][X_SIZE],
                            Linked_List *unknowns) {
    int board_duplicate[GRID_SIZE][Y_SIZE][X_SIZE];
    Linked_List *unknowns_duplicate;
    Cell *temp_cell;
    int temp_eligible_size;
    Cell *cell;
    int val;
    int grid;
    int x;
    int y;
    int elret;
    int btret;

    if (unknowns == NULL) {
        return ERROR_RET;
    }

    if (list_is_empty(unknowns)) {
        return SUCCESS_RET;
    }

    temp_cell = (Cell *)list_inspect(unknowns, 1);
    temp_eligible_size = cell_eligible_values_length(temp_cell);

    if (temp_eligible_size == ERROR_RET || temp_eligible_size == 0) {
        return temp_eligible_size;
    }

    if ((cell = cell_duplicate(temp_cell, GRID_SIZE)) == NULL) {
        return ERROR_RET;
    }

    list_remove(unknowns, 1);

    grid = cell_grid_pos(cell);
    x = cell_x_pos(cell);
    y = cell_y_pos(cell);

    while (cell_eligible_values_length(cell) > 0) {
        
        if ((unknowns_duplicate = duplicate_unknown_list(unknowns)) == NULL) {
            cell_destroy(cell);
            return ERROR_RET;
        }

        duplicate_board(board_duplicate, board);

        val = cell_inspect_eligible_values(cell, 1);
        cell_update_eligible_values(cell, val);

        board_duplicate[grid][y][x] = val;

        elret = solve_elimination_method(board_duplicate, unknowns_duplicate);

        if (elret == ERROR_RET) {
            return ERROR_RET;
        }

        if (elret == SUCCESS_RET) {
            btret = solve_backtrack_method(board_duplicate,
                                                unknowns_duplicate);
            
            if (btret == ERROR_RET) {
                return ERROR_RET;
            }
        
            if (btret == SUCCESS_RET) {
                duplicate_board(board, board_duplicate);

                list_destroy(unknowns_duplicate);
                cell_destroy(cell);

                return SUCCESS_RET;
            }
        }
        list_destroy(unknowns_duplicate);
    }
    cell_destroy(cell);

    return STALEMATE_RET;
}

/* Duplicates a Sudoku board (represented as the same structure
 * as the program's internal structure for the main Sudoku board,
 * a 3D-array structure).
 * The board passed as the first argument to the function copies
 * all the values of the board passed as the second argument.
 * At the end of this function, both boards will be discrete but 
 * identical.
 */
void duplicate_board(int new_board[GRID_SIZE][Y_SIZE][X_SIZE],
                        int original_board[GRID_SIZE][Y_SIZE][X_SIZE]) {
    for (int grid = 0 ; grid < GRID_SIZE ; grid++) {
        for (int y = 0 ; y < Y_SIZE ; y++) {
            for (int x = 0 ; x < X_SIZE ; x++) {
                new_board[grid][y][x] = original_board[grid][y][x];
            }
        }
    }
}

/* Duplicates a linked list storing information regarding all the
 * cells of the Sudoku board where values are still unknown. The
 * information about a cell is comprised of its position (grid, x, y)
 * on the board and the list of values eligible to be stored in
 * that position. All these data is copied to the new list, producing
 * a list that is discrete and independent from the list it copied.
 * Returns: On success, the linked list, on failure, NULL
*/
Linked_List *duplicate_unknown_list(Linked_List *unknowns) {
    Linked_List *list_new;
    int unknowns_size;
    Cell *new_cell;
    Cell *original_cell;

    if (unknowns == NULL) {
        return NULL;
    }

    if ((list_new = list_empty()) == NULL) {
        return NULL;
    }

    if (list_value_handler(list_new, cell_destroy) == ERROR_RET) {
        list_destroy(list_new);
        return NULL;
    }

    unknowns_size = list_length(unknowns);

    for (int i = 0 ; i < unknowns_size ; i++) {

        original_cell = (Cell *)list_inspect(unknowns, i + 1);

        if ((new_cell = cell_duplicate(original_cell, GRID_SIZE)) == NULL) {
            list_destroy(list_new);
            return NULL;
        }

        if (list_insert(list_new, new_cell) == NULL) {
            list_destroy(list_new);
            return NULL;
        }
    }

    return list_new;
}