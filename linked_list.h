/**
 * @defgroup Lists Single-Linked List
 * @brief An abstract datatype used to represent a
 *        single-linked list.
 * @author J. Pettersson
 * @date 2025-04-23
 * 
 * This module provides an implementation of a single-linked
 * list. The underlying structure of the list is: a head
 * structure with nodes dynamically allocated. The head
 * structure points to the first node of the list and to two
 * optional user-implemented functions:
 * 
 *  - A value-handler function that should, in the event
 *    of a node removal or list deletion, handle the value(s)
 *    stored in the node(s) of the list. This function would
 *    ensure no memory leaks.
 *  - A sort-sequence function that would, in the event of
 *    a node insertion, compare the new value with another
 *    value to determine the position in the list where the
 *    new node (containing a reference to the new value) shall
 *    be inserted. This function would determine the list's
 *    order: ascending or descending.
 * 
 * Each node stores the following elements: a reference to a
 * value and a reference to the next node.
 * 
 * When a sort-sequence function is not specified, the list
 * structure resorts to the default insertion method:
 * head-insertion. In other words, each new node is added to
 * the front of the list. Thus, the list's head structure
 * would point to the youngest node. The last node in the list
 * would be the oldest node. In place of the reference to the
 * next node, the oldest node stores (or points to) NULL.
 *
 * The list can in principle store an unlimited amount of
 * nodes as long as there is enough space in the memory
 * in the host device to store the information.
 * 
 * Example usage:
 *
 *      Linked_List *list = list_empty();
 *      list_value_handler(list, handler_function);
 *      list_sort_sequence(list, sorting_function);
 *      list_insert(list, value_1);
 *      list_insert(list, value_2);
 *      list_insert(list, value_3);
 *      list_is_empty(list);            // returns false
 *      list_length(list);              // returns 3
 *      list_inspect(list, 2);          // returns value_2
 *      list_remove(list, 1);           // removes value_3 node
 *      list_destroy(list);             // removes remaining nodes + list
 *
 * @{
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

typedef struct Linked_List Linked_List;
typedef void (*value_handler_func)(void *);
typedef int (*sort_sequence_func)(void *, void *);

/**
 * @brief Creates an empty list
 *
 * This function allocates memory to create a head
 * structure that shall represent and handle a linked list.
 *
 * @return On success, a new linked list; on failure, NULL
 * @see list_destroy
 */
Linked_List *list_empty(void);

/**
 * @brief To provide reference to a value-handler function
 * 
 * This function allows the user to provide the list structure
 * with a reference to a value-handler function that will
 * be executed in the event of a node removal. The 
 * value-handler function contains instructions that will
 * deallocate all memory of a node's value.
 * 
 * A value-handler function is necessary to prevent memory
 * leaks only if the memory of values (the reference of which
 * are stored within the nodes) are dynamically allocated and
 * cannot be deallocated in any other way.
 * 
 * An example of a value-handler function:
 * 
 *      void handler_function(void *v) {
 *          Value *value = (Value *)v;
 *          free(value);
 *      }
 * 
 * @warning this function must be called with a defined
 *          value-handler function as argument before calling
 *          to list_remove or list_destroy for the first time if
 *          memory has been allocated for value and the memory
 *          cannot be deallocated through other means, or else
 *          memory leaks will occur.
 *
 * @param l The linked list
 * @param f The value-handler function
 * @return On success, 1; on failure, -1
 * @see list_remove_node
 */
int list_value_handler(Linked_List *l, value_handler_func f);

/**
 * @brief To provide reference to a sort-sequence function
 * 
 * This function allows the user to provide the list structure
 * with a reference to a sort-sequence function that will
 * be executed in the event of a node insertion. The 
 * sort-sequence function compares two values (the new value
 * and an existing value stored in the list) with each other in
 * order to determine the position in the list where the new node
 * (containing a reference to the new value) shall be inserted.
 * 
 * The first argument of the sort-sequence function should be
 * the new value, the second argument should be the existing
 * value stored within the list.
 * 
 * To produce an ascending-ordered list: when the new value is
 * larger than the value it is compared to, let the sort-sequence
 * function return a positive integer. Otherwise, let it return a
 * negative integer.
 * 
 * To produce an descending-ordered list: when the new value is
 * smaller than the value it is compared to, let the sort-sequence
 * function return a positive integer. Otherwise, let it return a
 * negative integer.
 * 
 * Also, optionally, if the new value is equal to the value it
 * is compared to, the function can return 0.
 * 
 * When a sort-sequence function is not specified, the list
 * structure resorts to the default insertion method:
 * head-insertion. In other words, each new node is added to
 * the front of the list. Thus, the list's head structure
 * would point to the youngest node. The last node in the list
 * would be the oldest node. In place of the reference to the
 * next node, the oldest node stores (or points to) NULL.
 * 
 * An example of a sort-sequence function for an
 * ascending-ordered list:
 * 
 *      void sorting_function(void *v1, void *v2) {
 *          Value new_value = *(Value *)v1;
 *          Value existing_value = *(Value *)v2;
 *          
 *          // Optional if-statement
 *          if (new_value == existing_value) {
 *              return 0;
 *          }
 * 
 *          if (new_value > existing_value) {
 *              return 1;
 *          }
 *          else {
 *              return -1;
 *          }
 *      }
 * 
 * @warning this function must be called with a defined
 *          sort-sequence function as argument before calling
 *          to list_insert for the first time if it is intended
 *          that the list be sorted, or else the order of the
 *          list would be undefined.
 *
 * @param l The linked list
 * @param f The sort-sequence function
 * @return On success, 1; on failure, -1
 * @see list_insert
 */
int list_sort_sequence(Linked_List *l, sort_sequence_func f);

/**
 * @brief Inserts a value in a list
 *
 * This function allocates memory to create a node, stores
 * the specified reference to a value within that node, and
 * inserts that node into the list.
 *
 * @param l The linked list
 * @param value A reference to a value
 * @return On success, the updated linked list;
 *         on failure, NULL
 * @see list_remove_node
 * @see list_sort_sequence
 */
Linked_List *list_insert(Linked_List *l, void *value);

/**
 * @brief Checks if a list is storing values or not
 * @param l The linked list
 * @return If list is empty or NULL, true; otherwise, false
 */
bool list_is_empty(Linked_List *l);

/**
 * @brief Returns the amount of values stored in a list
 * @param l The linked list
 * @return The size of the list; -1 if list is NULL
 */
int list_length(Linked_List *l);

/**
 * @brief Returns a reference to a specific value from a list
 * 
 * This function traverses through the list to locate the
 * node at the specified position and returns the reference
 * to the value that the node is storing.
 * 
 * @param l The linked list
 * @param pos The position of the value
 * @return A reference to the value; NULL if pos is out of
 *         bounds or list is NULL
 */
void *list_inspect(Linked_List *l, int pos);

/**
 * @brief Removes a specific value from a list
 *
 * This function deallocates memory of a node at the
 * specified position along with all the allocated memory
 * associated with that node: the value within the node.
 * 
 * @note the list's position index starts at 1.
 * @note Cannot directly deallocate memory of a node's value
 * without a defined value-handler function.
 *
 * @param l The linked list
 * @param pos The position of the node to be removed
 * @return The updated linked list; NULL if pos is out of
 *         bounds or list is NULL
 * @see list_insert
 * @see list_value_handler
 */
Linked_List *list_remove(Linked_List *l, int pos);

/**
 * @brief Deallocates all allocated memory for a list
 *
 * This function deallocates all memory allocated for a
 * list's structure along with all the allocated memory
 * associated with the list: the remaining unallocated nodes
 * within the list and the values within the nodes.
 * 
 * @note Cannot directly deallocate memory of a node's value
 * without a defined value-handler function.
 *
 * @param l The linked list
 * @see list_empty
 * @see list_remove_node
 * @see list_value_handler
 */
void list_destroy(Linked_List *l);

/** @} */

#endif // LINKED_LIST_H