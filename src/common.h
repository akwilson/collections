#ifndef COMMON_H
#define COMMON_H

// Useful macro to repress unused warnings from the compiler
#define UNUSED(...) (void)(__VA_ARGS__)

// Header object common to all collections
typedef struct header
{
    // number of items in the collection
    size_t size;
    // allocates state for the iterator
    void* (*alloc_iter_state)(const void* collection);
    // gets the next iterator item in the collection
    int (*get_next_iter)(const void* collection, void* state, void** data);
    // function to free the allocated iterator state
    void (*free_iter)(void* state);
} header;

#endif
