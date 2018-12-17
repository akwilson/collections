#ifndef COLLECTIONS_H
#define COLLECTIONS_H

// Key/value pair, returned by the hash table iterator
typedef struct _kvp
{
    char *key;
    void *value;
} kvp;

// Function return codes
typedef enum
{
    C_OK         = 0,
    CE_BOUNDS    = 1,  // requested item was out of bounds of the array
    CE_NULL_ITEM = 2,  // add null item to priority queue
    CE_MISSING   = 3   // item not found in hash table 
} C_STATUS;

// == COMMON ==================================================================

// Return the number of items in the array
size_t clxns_count(const void *collection);

// Create a new iterator
void *clxns_iter_new(const void *collection);

// Check if the iterator has more items to consume
int clxns_iter_move_next(void *iterator);

// Return the next item in the collection
void *clxns_iter_get_next(void *iterator);

// Free the iterator
void clxns_iter_free(void *iterator);

// == RESIZE ARRAY =============================================================

// Create and return a new array. Specify the initial size.
void *resize_array(size_t init_size);

// Add an item to the array
void resize_array_add(void *array, void *item);

// Insert an item in to the middle of an array
C_STATUS resize_array_insert(void *array, size_t index, void *item);

// Replace an item in the array with a different item
void resize_array_replace(void *array, size_t index, void *item);

// Access an item at the given position in the array
C_STATUS resize_array_get(const void *array, size_t index, void **item);

// Swap two items in the array
C_STATUS resize_array_exchange(void *array, size_t first, size_t second);

// Remove an item from the array, set item to the value if non-zero is passed in
C_STATUS resize_array_remove(void *array, size_t index, void **item);

// Shallow copy a resize array
void *resize_array_copy(const void *array);

// Free the array and its contents if items is non-zero
void resize_array_free(void *array, int items);

// == PRIORITY QUEUE ===========================================================

/*
 * Create and return a new priority queue, specify initial
 * size and a callback to compare items
 * _min orders ascending, _max orders descending
 */
void *priority_queue_min(size_t init_size, int (*compare)(const void *first, const void *second));
void *priority_queue_max(size_t init_size, int (*compare)(const void *first, const void *second));

// Add an item to the priority queue. May return CE_NULL_ITEM.
C_STATUS priority_queue_add(void *pqueue, void *item);

// Look at but do not remove the head of the queue
C_STATUS priority_queue_peek(const void *pqueue, void **item);

// Remove the head of the priority queue
C_STATUS priority_queue_pop(void *pqueue, void **item);

// Shallow copy a priority queue
void *priority_queue_copy(const void *pqueue);

// Free the priority queue and its contents if items is non-zero
void priority_queue_free(void *array, int items);

// == HASH TABLE ==============================================================

// Create and return a new hash table. Specify the initial size.
void *hash_table(size_t init_size);

// Associate a key with a value
void hash_table_add(void *table, char *key, void *value);

// Return the value associated with the key
C_STATUS hash_table_get(const void *table, const char *key, void **value);

// Remove the key/value pair
C_STATUS hash_table_remove(void *table, const char *key);

// Shallow copy the hash table
void *hash_table_copy(const void *table);

// Free the hash table and its contents if items is non-zero
void hash_table_free(void *table, int items);

#endif
