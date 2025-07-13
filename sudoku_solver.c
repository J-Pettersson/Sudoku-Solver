/**
 * @brief The main file of a program that solves 9x9 Sudoku problems.
 * @author J. Pettersson
 * @date 2025-04-30
 * 
 * This program reads a numerical (between 1 to 9) 9x9 Sudoku board,
 * validates it, attempts to solve it, prints either a solution or an
 * unsolved notice, and repeats this process until the user decides
 * to terminate the program. User interface takes place through the
 * terminal. The program attempts to solve the specified Sudoku
 * problems using a combination of the elimination method and
 * backtracking. Hence, it should be able to solve legal Sudoku
 * problems of any difficulty. If there are more than one solution to
 * the specified problem, the program will only present one of these
 * solutions.
 */

#include "sudoku_solver_utils.h"

int main(void) {

    int board[GRID_SIZE][Y_SIZE][X_SIZE];

    init_board(board);
    print_welcome_msg();

    do {
        print_how_to_msg();
        read_board_input(board);
        solve_sudoku(board);
        print_result_func(board);
    } while (request_play_again());

    print_final_msg();

    return 0;
}