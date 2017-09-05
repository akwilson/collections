// header object common to all collections
typedef struct header
{
    // number of items in the collection
    int size;
    // allocates state for the iterator
    void* (*alloc_iter_state)(void* collection);
    // gets the next iterator item in the collection
    int (*get_next_iter)(void* collection, void* state, void** data);
} header;
