#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "collections.h"

#define DEF_SIZE 8

typedef struct rs_array
{
    header head;
    void** buff;  // the data in the array
    int capacity; // the number of items allocated to the array
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

/*
 * Allocates an integer to keep track of the position in the array
 */
static void* alloc_iter_state()
{
    int* st = (int*)malloc(sizeof(int));
    *st = 0;
    return st;
}

static int get_next_iter(void* array, void* iter_state, void** next)
{
    rs_array* ra = array;
    int* cur = (int*)iter_state;
    C_STATUS st = resize_array_get(array, *cur, next);
    (*cur)++;
    return st == C_OK;
}

void* resize_array(int init_size)
{
    int sz = init_size == 0 ? DEF_SIZE : init_size;
    rs_array* rv = (rs_array*)malloc(sizeof(rs_array));
    void** buffer = malloc(sz * sizeof(void*));
    rv->buff = buffer;
    rv->capacity = sz;
    rv->head.size = 0;
    rv->head.alloc_iter_state = alloc_iter_state;
    rv->head.get_next_iter = get_next_iter;
    return rv;
}

/*
 * Adds an item to the array. Doubles the size of the array when full.
 */
void resize_array_add(void* array, void* item)
{
    rs_array* ra = array;

    if (ra->head.size == ra->capacity)
    {
        resize(ra, ra->capacity * 2);
    }

    ra->buff[ra->head.size] = item;
    ra->head.size++;
}

C_STATUS resize_array_get(void* array, int index, void** item)
{
    rs_array* ra = array;
    if (index < 0 || index >= ra->head.size)
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
C_STATUS resize_array_remove(void* array, int index, void** item)
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

    for (int i = index; i < ra->head.size - 1; i++)
    {
        ra->buff[i] = ra->buff[i + 1];
    }

    ra->head.size--;

    if (ra->head.size <= ra->capacity / 4)
    {
        resize(ra, ra->capacity / 4);
    }

    return C_OK;
}

/*
 * Free the array and its buffer. Optionally free all items within.
 */
void resize_array_free(void* array, int items)
{
    rs_array* ra = array;
    if (items)
    {
        for (int i = 0; i < ra->head.size; i++)
        {
            free(ra->buff[i]);
        }
    }

    free(ra->buff);
    free(ra);
}
