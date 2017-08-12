#ifndef COLLECTIONS_H
#define COLLECTIONS_H

// create and return a new array. Specify the initial size.
void* resize_array(int init_size);

// add an item to the array
int resize_array_add(void* array, void* item);

// access an item at the given position in the array
void* resize_array_get(void* array, int index);

// remove an item from the array. Return the value removed.
void* resize_array_remove(void* array, int index);

// return the number of items in the array
int resize_array_count(void* array);

// free the array and its contents
void resize_array_free(void* array, int items);

#endif
