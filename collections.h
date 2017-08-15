#ifndef COLLECTIONS_H
#define COLLECTIONS_H

// return codes
enum C_STATUS
{
    C_OK      = 0,
    CE_BOUNDS = 1   // requested item was out of bounds of the array
};

// create and return a new array. Specify the initial size.
void* resize_array(int init_size);

// add an item to the array
void resize_array_add(void* array, void* item);

// access an item at the given position in the array
enum C_STATUS resize_array_get(void* array, int index, void** item);

// remove an item from the array, set item to the value if non-zero is passed in
enum C_STATUS resize_array_remove(void* array, int index, void** item);

// return the number of items in the array
int resize_array_count(void* array);

// free the array and its contents if items is non-zero
void resize_array_free(void* array, int items);

#endif
