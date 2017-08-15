#include <stdlib.h>
#include <stdio.h>
#include "collections.h"

#define DEF_SIZE 8

typedef struct rs_array {
    void** buff;
    int capacity;
    int size;
} rs_array;

/*
 * Does the actual resizing
 */
static void resize(rs_array* ra, int new_size)
{
    void** buffer = realloc(ra->buff, new_size * sizeof(void*));
    ra->buff = buffer;
    ra->capacity = new_size;
}

void* resize_array(int init_size)
{
    int sz = init_size == 0 ? DEF_SIZE : init_size;
    rs_array* rv = (rs_array*)malloc(sizeof(rs_array));
    void** buffer = malloc(sz * sizeof(void*));
    rv->buff = buffer;
    rv->capacity = sz;
    rv->size = 0;
    return rv;
}

/*
 * Adds an item to the array. Doubles the size of the array when full.
 */
void resize_array_add(void* array, void* item)
{
    rs_array* ra = array;

    if (ra->size == ra->capacity)
    {
        resize(ra, ra->capacity * 2);
    }

    ra->buff[ra->size] = item;
    ra->size++;
}

enum C_STATUS resize_array_get(void* array, int index, void** item)
{
    rs_array* ra = array;
    if (index < 0 || index >= ra->size)
    {
        return CE_BOUNDS;
    }

    *item = ra->buff[index];
    return C_OK;
}

/*
 * Removes an item from the array and shuffles everything after that
 * point back one space. Reallcoates the array if it's one quarter full.
 */
enum C_STATUS resize_array_remove(void* array, int index, void** item)
{
    rs_array* ra = array;
    void* rv;
    int err = resize_array_get(array, index, &rv);
    if (err)
    {
        return err;
    }
    else if (item)
    {
        *item = rv;
    }

    for (int i = index; i < ra->size - 1; i++)
    {
        ra->buff[i] = ra->buff[i + 1];
    }

    ra->size--;

    if (ra->size <= ra->capacity / 4)
    {
        resize(ra, ra->capacity / 4);
    }

    return C_OK;
}

int resize_array_count(void* array)
{
    rs_array* ra = array;
    return ra->size;
}

/*
 * Free the array and its buffer. Optionally free all items within.
 */
void resize_array_free(void* array, int items)
{
    rs_array* ra = array;
    if (items)
    {
        for (int i = 0; i < ra->size; i++)
        {
            free(ra->buff[i]);
        }
    }

    free(ra->buff);
    free(ra);
}
