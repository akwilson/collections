#ifndef COMMON_H
#define COMMON_H

// Useful macro to repress unused warnings from the compiler
#define UNUSED(...) (void)(__VA_ARGS__)

// Header object common to all collections
typedef struct header
{
    // Number of items in the collection
    size_t size;
    // Allocates state for the iterator
    void *(*alloc_iter_state)(const void *collection);
    // Gets the next iterator item in the collection
    int (*get_next_iter)(const void *collection, void *state, void **data);
    // Frees the allocated iterator state
    void (*free_iter)(void *state);
    // Copies the collection
    void *(*copy_collection)(const void *collection);
    // Frees the collection
    void (*free_collection)(void *collection, int items);
} header;

#endif
