/* A datatype that represents a Sudoku cell.
 *
 * Author:     J. Pettersson
 * Date:       2025-04-30
 */

#include "sudoku_cell.h"
#include "../DataStructures/LinkedList/linked_list.h"
#include <stdio.h>
#include <stdlib.h>

/* --------------- Internal Function Declarations ---------------- */

void eligible_value_destroy(void *v);
Linked_List *get_eligible_values_new(int grid_size);
int set_eligible_values_duplicate(Cell *new_cell, Cell *original_cell);

/* --------------------------- Structs --------------------------- */

typedef struct {
    int grid;
    int x;
    int y;
} Position;

struct Cell {
    Position position;
    Linked_List *eligible_values;
};

/* ---------------- External Function Definitions ---------------- */

Cell *cell_create(int grid, int x, int y, int grid_size) {
    Cell *c;
    Linked_List *l;

    if ((c = calloc(1, sizeof(Cell))) == NULL) {
        return NULL;
    }

    c->position.grid = grid;
    c->position.x = x;
    c->position.y = y;

    if ((l = get_eligible_values_new(grid_size)) == NULL) {
        free(c);
        return NULL;
    }

    c->eligible_values = l;

    return c;
}

Cell *cell_duplicate(Cell *original_cell, int grid_size) {
    Cell *new_cell;
    int grid;
    int x;
    int y;

    if (original_cell == NULL) {
        return NULL;
    }

    grid = cell_grid_pos(original_cell);
    x = cell_x_pos(original_cell);
    y = cell_y_pos(original_cell);

    if ((new_cell = cell_create(grid, x, y, grid_size)) == NULL) {
        return NULL;
    }

    if ((set_eligible_values_duplicate(new_cell, original_cell)) == -1) {
        cell_destroy(new_cell);
        return NULL;
    }

    return new_cell;
}

int cell_inspect_eligible_values(Cell *c, int pos) {
    int *val;

    if (c == NULL) {
        return -1;
    }

    if ((val = (int *)list_inspect(c->eligible_values, pos)) == NULL) {
        return -1;
    }

    return *val;
}

Cell *cell_update_eligible_values(Cell *c, int remove_val) {
    int values_size;
    int val;

    if (c == NULL) {
        return NULL;
    }

    values_size = cell_eligible_values_length(c);
    
    for (int i = 0 ; i < values_size ; i++) {

        if ((val = cell_inspect_eligible_values(c, i + 1)) == -1) {
            return NULL;
        }

        if (val == remove_val) {

            if ((list_remove(c->eligible_values, i + 1)) == NULL) {
                return NULL;
            }

            return c;
        }
    }

    return c;
}

int cell_grid_pos(Cell *c) {
    if (c == NULL) {
        return -1;
    }

    return c->position.grid;
}

int cell_x_pos(Cell *c) {
    if (c == NULL) {
        return -1;
    }
    
    return c->position.x;
}

int cell_y_pos(Cell *c) {
    if (c == NULL) {
        return -1;
    }
    
    return c->position.y;
}

int cell_eligible_values_length(Cell *c) {
    if (c == NULL) {
        return -1;
    }

    return list_length(c->eligible_values);
}

void cell_destroy(void *v) {
    Cell *c;
    
    if (v == NULL) {
        return;
    }
    
    c = (Cell *)v;
    list_destroy(c->eligible_values);
    free(c);
}

/* ---------------- Internal Function Definitions ---------------- */

/* Creates and initializes a linked list with values
 * from 1 to 9.
 * Returns: On success, the linked list; on failure, NULL
 */
Linked_List *get_eligible_values_new(int grid_size) {
    Linked_List *l;
    int *val;

    if ((l = list_empty()) == NULL) {
        return NULL;
    }

    if ((list_value_handler(l, eligible_value_destroy)) == -1) {
        list_destroy(l);
        return NULL;
    }

    for (int i = 0 ; i < grid_size ; i++) {

        if ((val = calloc(1, sizeof(int))) == NULL) {
            list_destroy(l);
            return NULL;
        }

        *val = i + 1;

        if (list_insert(l, val) == NULL) {
            list_destroy(l);
            return NULL;
        }
    }

    return l;
}

/* Used to deallocate memory allocated to store a value
 * in a cell's eligible-values list.
 */
void eligible_value_destroy(void *v) {
    if (v == NULL) {
        return;
    }
    
    free((int *)v);
}

/* Copies the values from one cell into another cell. The
 * first argument is a cell that will have its
 * eligible-values list receive values copied from the
 * second argument's list.
 * To avoid undefined behaviour, the first argument needs
 * to be a cell with its eligible-values list untampered
 * with since the cell's creation.
 * Returns: On success, 1; on failure, -1
 */
int set_eligible_values_duplicate(Cell *new_cell, Cell *original_cell) {
    Linked_List *temp_values;
    int grid_size;
    int values_size;
    int temp_size;
    int *original_v;
    int *temp_v;

    if (new_cell == NULL || original_cell == NULL) {
        return -1;
    }

    values_size = cell_eligible_values_length(original_cell);
    grid_size = cell_eligible_values_length(new_cell);
    
    if ((temp_values = get_eligible_values_new(grid_size)) == NULL) {
        return -1;
    }

    for (int i = 0 ; i < values_size ; i++) {
        original_v = (int *)list_inspect(original_cell->eligible_values, i + 1);

        for (int j = 0 ; j < grid_size ; j++) {
            temp_v = (int *)list_inspect(temp_values, j + 1);

            if (original_v == NULL || temp_v == NULL) {
                list_destroy(temp_values);
                return -1;
            }

            if (*original_v == *temp_v) {

                if (list_remove(temp_values, j + 1) == NULL) {
                    list_destroy(temp_values);
                    return -1;
                }

                grid_size--;
                break;
            }
        }
    }

    temp_size = list_length(temp_values);
    
    for (int i = 0 ; i < temp_size ; i++) {

        temp_v = (int *)list_inspect(temp_values, i + 1);

        if (cell_update_eligible_values(new_cell, *temp_v) == NULL) {
            list_destroy(temp_values);
            return -1;
        }
    }

    list_destroy(temp_values);

    return 1;
}