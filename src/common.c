#include <stdlib.h>
#include "common.h"
#include "collections.h"

// A collection iterator
typedef struct {
    void* next_item;  // next item to return in call to _get_next()
    void* state;      // collection specific iterator state
    void* collection; // collection pointed to by this iterator
} iterator_t;

int clxns_count(void* collection)
{
    header* head = collection;
    return head->size;
}

void* clxns_iter_new(void* collection)
{
    iterator_t* iter = (iterator_t*)malloc(sizeof(iterator_t));
    iter->collection = collection;
    iter->state = ((header*)collection)->alloc_iter_state(collection);
    iter->next_item = 0;
    return iter;
}

int clxns_iter_move_next(void* iterator)
{
    iterator_t* iter = iterator;
    void* data;
    int rv = ((header*)iter->collection)->get_next_iter(iter->collection, iter->state, &data);
    iter->next_item = data;
    return rv;
}

void* clxns_iter_get_next(void* iterator)
{
    iterator_t* iter = iterator;
    return iter->next_item;
}

void clxns_iter_free(void* iterator)
{
    iterator_t* iter = iterator;
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
