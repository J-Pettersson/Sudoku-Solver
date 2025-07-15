/* A datatype that represents a single-linked list.
 *
 * Author:     J. Pettersson
 * Date:       2025-04-23
 */

#include "linked_list.h"
#include <stdlib.h>
#include <assert.h>

typedef struct Node Node;

/* ------------------------ Structs ------------------------ */

struct Node {
    Node *next;
    void *value;
};

struct Linked_List {
    Node *front;
    value_handler_func value_handler;
    sort_sequence_func sort_sequence;
};

/* ------------- External Function Definitions ------------- */

Linked_List *list_empty(void) {
    return calloc(1, sizeof(Linked_List));
}

int list_value_handler(Linked_List *l, value_handler_func f) {
    if (l == NULL || f == NULL) {
        return -1;
    }
    l->value_handler = f;

    return 1;
}

int list_sort_sequence(Linked_List *l, sort_sequence_func f) {
    if (l == NULL || f == NULL) {
        return -1;
    }
    l->sort_sequence = f;

    return 1;
}

Linked_List *list_insert(Linked_List *l, void *value) {
    Node *new_n;
    Node *prev_n;
    Node *current_n;
    int size;
    int sort = 0;
    
    if (l == NULL) {
        return NULL;
    }

    if ((new_n = calloc(1, sizeof(Node))) == NULL) {
        return NULL;
    }

    if (list_is_empty(l)) {
        l->front = new_n;
    }
    else if (l->sort_sequence == NULL) {
        new_n->next = l->front;
        l->front = new_n;
    }
    else {
        size = list_length(l);
        prev_n = l->front;
        current_n = l->front;

        for (int i = 0 ; i < size ; i++) {
            sort = l->sort_sequence(value, current_n->value);

            if (sort <= 0) {

                if (i == 0) {
                    new_n->next = l->front;
                    l->front = new_n;
                }
                else {
                    new_n->next = current_n;
                    prev_n->next = new_n;
                }
                break;
            }
            prev_n = current_n;
            current_n = current_n->next;
        }

        if (sort > 0) {
            new_n->next = prev_n->next;
            prev_n->next = new_n;
        }
    }
    new_n->value = value;

    return l;
}

bool list_is_empty(Linked_List *l) {
    if (l == NULL) {
        return true;
    }

    return l->front == NULL;
}

int list_length(Linked_List *l) {
    int count;
    Node *n;

    if (l == NULL) {
        return -1;
    }

    if (list_is_empty(l)) {
        return 0;
    }

    count = 0;
    n = l->front;

    while(n != NULL) {
        n = n->next;
        count++;
    }

    return count;
}

void *list_inspect(Linked_List *l, int pos) {
    Node *n;

    if (l == NULL) {
        return NULL;
    }

    if (pos <= 0 || pos > list_length(l)) {
        return NULL;
    }

    n = l->front;

    for (int i = 1 ; i < pos ; i++) {
        n = n->next;
    }

    return n->value;
}

Linked_List *list_remove(Linked_List *l, int pos) {
    Node *prev;
    Node *node;

    if (l == NULL) {
        return NULL;
    }

    if (pos <= 0 || pos > list_length(l)) {
        return NULL;
    }

    node = l->front;

    for (int i = 1 ; i < pos ; i++) {
        prev = node;
        node = node->next;
    }

    if (pos == 1) {
        l->front = node->next;
    } else {
        prev->next = node->next;
    }

    if (l->value_handler != NULL) {
        l->value_handler(node->value);
    }

    free(node);

    return l;
}

void list_destroy(Linked_List *l) {
    int size;

    if (l == NULL) {
        return;
    }

    size = list_length(l);
    for (int i = 0 ; i < size ; i++) {
        list_remove(l, 1);
    }

    free(l);
}