/**
 * @brief An abstract datatype used to represent a Sudoku cell.
 * @author J. Pettersson
 * @date 2025-04-30
 * 
 * This module provides an implementation of a Sudoku cell.
 * The underlying structure of the cell is a node that stores
 * the following information:
 * 
 *  - the cell's position-coordinates on the Sudoku board.
 *    This is denoted by grid, x, and y, where x and y are
 *    the coordinates of the cell within the grid it is
 *    located in. The index of these position-coordinates
 *    start at 0.
 *  - a reference to a linked list that stores all the values
 *    that have not (yet) been ruled out as ineligible. One of
 *    these values should be the solution to the cell.
 * 
 * This implementation can currently only store and handle
 * values of type integer between 1 to 9.
 * 
 * Example usage:
 * 
 *      Cell *cell = cell_create(0, 1, 2, BOARD_SIZE);  // BOARD_SIZE = 9
 *      int grid = cell_grid_pos(new_cell);             // returns 0
 *      int x = cell_x_pos(new_cell);                   // returns 1
 *      int y = cell_y_pos(new_cell);                   // returns 2
 *      int val = cell_inspect_eligible_values(cell, 1);// returns some value
 *                                                         between 1 to 9
 *      Cell *new_cell = cell_duplicate(cell, BOARD_SIZE);
 *      int new_grid = cell_grid_pos(new_cell);         // returns 0
 *      int new_x = cell_x_pos(new_cell);               // returns 1
 *      int new_y = cell_y_pos(new_cell);               // returns 2
 *      cell_update_eligible_values(cell, 1);           // removes 1 from list
 *      int size_1 = cell_eligible_values_length(cell); // returns 8
 *      int size_2 = cell_eligible_values_length(new_cell); // returns 9
 *      cell_destroy(cell);                             // removes cell + list
 *      cell_destroy(new_cell);                         // removes cell + list
 * 
 * @{
 */

#ifndef SUDOKU_CELL_H
#define SUDOKU_CELL_H

typedef struct Cell Cell;

/**
 * @brief Creates an empty cell
 *
 * This function allocates memory to create a node structure
 * that shall represent a cell and where relevant data will
 * be stored. A new cell initializes its eligible-values list
 * with values from 1 to 9.
 * 
 * @note the index of the position-coordinates starts at 0.
 *
 * @param grid The grid-coordinate
 * @param x The x-coordinate within the grid
 * @param y The y-coordinate within the grid
 * @param grid_size The size of a grid (= x * y)
 * @return On success, a new, initialized cell;
 *         on failure, NULL
 * @see cell_destroy
 */
Cell *cell_create(int grid, int x, int y, int grid_size);

/**
 * @brief Creates an identical copy of a cell
 *
 * This function allocates memory to create a duplicate of
 * an existing cell.
 * 
 * @warning the duplicate cell is its own instance,
 * independent and isolated from the original cell. And the
 * value(s) of the duplicate cell, though they are the same
 * as the value(s) of the original cell as it was being
 * duplicated, are also discrete and inaccessible from the
 * original cell. Therefore, a cell created and initialized
 * through cell_duplicate() needs to be deallocated the
 * same way a cell that was created and initialized through
 * cell_create().
 *
 * @param original_cell The cell to be copied
 * @param grid_size The size of a grid (= x * y)
 * @return On success, a new, initialized cell copy of the
 *         original cell; on failure, NULL
 * @see cell_create
 * @see cell_destroy
 */
Cell *cell_duplicate(Cell *original_cell, int grid_size);

/**
 * @brief Returns a specific eligible value of a cell
 * 
 * This function traverses through the cell's list of
 * eligible values to locate the value at the specified
 * position and returns that value.
 * 
 * @note the eligible-values list's position starts at 1.
 * 
 * @param c The cell
 * @param pos The position of the value
 * @return The value at the specified position; -1 if pos
 *         is out of bounds, or cell or values list is NULL
 * @see cell_eligible_values_length
 */
int cell_inspect_eligible_values(Cell *c, int pos);

/**
 * @brief Removes a specific eligible value of a cell
 *
 * This function deallocates memory associated with the
 * specified value at the cell's eligible-values list.
 * The intent of this function is to update the
 * eligible-values list by removing a value determined
 * to no longer be eligible.
 * 
 * If the cell contains the specified value, the function
 * updates the cell and returns the updated cell. If the
 * cell doesn't contain the specified value, the function
 * makes no changes to the cell and returns the cell in the
 * same state as it was when it was passed to the function.
 * 
 * @param c The cell
 * @param remove_val The value to be removed
 * @return On success, the cell; on failure, NULL
 */
Cell *cell_update_eligible_values(Cell *c, int remove_val);

/**
 * @brief Returns a cell's grid-coordinates
 * @note the index of the grid-coordinates starts at 0.
 * @param c The cell
 * @return The grid-coordinates; -1 if cell is NULL
 */
int cell_grid_pos(Cell *c);

/**
 * @brief Returns a cell's x-coordinates within a grid
 * @note the index of the x-coordinates starts at 0.
 * @param c The cell
 * @return The x-coordinates; -1 if cell is NULL
 */
int cell_x_pos(Cell *c);

/**
 * @brief Returns a cell's y-coordinates within a grid
 * @note the index of the y-coordinates starts at 0.
 * @param c The cell
 * @return The y-coordinates; -1 if cell is NULL
 */
int cell_y_pos(Cell *c);

/**
 * @brief Returns a cell's current amount of eligible values
 * @param c The cell
 * @return The size of that list of values; -1 if the cell
 *         or its eligible-values list is NULL
 */
int cell_eligible_values_length(Cell *c);

/**
 * @brief Deallocates all allocated memory for a cell
 *
 * This function deallocates all memory allocated for a
 * cell's structure along with all the allocated memory
 * associated with the cell: the list of the cell's eligible
 * values and the remaining values within that list (if
 * there are any).
 *
 * @param v The cell
 * @see cell_create
 * @see cell_duplicate
 * @see cell_update_eligible_values
 */
void cell_destroy(void *v);

/** @} */

#endif // SUDOKU_CELL_H