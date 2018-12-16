/*
 * Functions that work across all collection types
 */

#include <stdlib.h>
#include "common.h"
#include "collections.h"

// A collection iterator
typedef struct {
    void *next_item;  // next item to return in call to _get_next()
    void *state;      // collection specific iterator state
    void *collection; // collection pointed to by this iterator
} iterator_t;

/*
 * Returns the number of items in the collection
 */
int clxns_count(void *collection)
{
    header *head = collection;
    return head->size;
}

/*
 * Creates a new iterator pointing to the first item in the collection
 */
void *clxns_iter_new(void *collection)
{
    iterator_t *iter = (iterator_t*)malloc(sizeof(iterator_t));
    iter->collection = collection;
    iter->state = ((header*)collection)->alloc_iter_state(collection);
    iter->next_item = 0;
    return iter;
}

/*
 * Moves the iterator on to the next item in the collection. Returns 0 if there are no more items.
 */
int clxns_iter_move_next(void *iterator)
{
    iterator_t *iter = iterator;
    void *data;
    int rv = ((header*)iter->collection)->get_next_iter(iter->collection, iter->state, &data);
    iter->next_item = data;
    return rv;
}

/*
 * Retrieves the next item from the iterator
 */
void *clxns_iter_get_next(void *iterator)
{
    iterator_t *iter = iterator;
    return iter->next_item;
}

/*
 * Frees memory used by the iterator. Has no effect on the collection.
 */
void clxns_iter_free(void *iterator)
{
    iterator_t *iter = iterator;
    if (((header*)iter->collection)->free_iter != 0)
    {
        ((header*)iter->collection)->free_iter(iter->state);
    }
    else
    {
        free(iter->state);
    }

    free(iter);
}
