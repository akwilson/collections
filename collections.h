#ifndef COLLECTIONS_H
#define COLLECTIONS_H

// return codes
typedef enum
{
    C_OK         = 0,
    CE_BOUNDS    = 1,   // requested item was out of bounds of the array
    CE_NULL_ITEM = 2    // add null item to collection
} C_STATUS;

// == COMMON ==================================================================
// return the number of items in the array
int clxns_count(void* collection);

// create a new iterator
void* clxns_iter_new(void* collection);

// check if the iterator has more items to consume
int clxns_iter_move_next(void* iterator);

// return the next item in the collection
void* clxns_iter_get_next(void* iterator);

// free the iterator
void clxns_iter_free(void* iterator);

// == RESIZE ARRAY =============================================================
// create and return a new array. Specify the initial size.
void* resize_array(int init_size);

// add an item to the array
void resize_array_add(void* array, void* item);

// access an item at the given position in the array
C_STATUS resize_array_get(void* array, int index, void** item);

// swap two items in the array
C_STATUS resize_array_exchange(void* array, int first, int second);

// remove an item from the array, set item to the value if non-zero is passed in
C_STATUS resize_array_remove(void* array, int index, void** item);

// free the array and its contents if items is non-zero
void resize_array_free(void* array, int items);

// == PRIORITY QUEUE ===========================================================
// create and return a new priority queue, specify initial
// size and a callback to compare items
void* priority_queue(int init_size, int (*compare)(void* first, void* second));

// add an item to the priority queue
// may return CE_NULL_ITEM
C_STATUS priority_queue_add(void* pqueue, void* item);

// look at but do not remove the head of the queue
C_STATUS priority_queue_peek(void* pqueue, void** item);

// remove the head of the priority queue
C_STATUS priority_queue_pop(void* pqueue, void** item);

// free the priority queue and its contents if items is non-zero
void priority_queue_free(void* array, int items);

#endif
