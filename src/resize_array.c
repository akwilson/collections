/*
 * Implementation of the resizing array. The array expands and contracts
 * as items are added to and removed from it.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "collections.h"

// Default size if none is provided by the user
#define DEF_SIZE 8

// The resize array structure
typedef struct rs_array
{
    header head;
    void **buff;     // the data in the array
    size_t capacity; // the number of items allocated to the array
    size_t base_cap; // the intial / minimum size
} rs_array;

/*
 * Does the actual array resizing
 */
static void resize(rs_array *ra, size_t new_size)
{
    void **buffer = realloc(ra->buff, new_size * sizeof(void*));
    ra->buff = buffer;
    ra->capacity = new_size;
}

/*
 * Creates a new iterator and points it to the first item in the array.
 * Allocates an integer to keep track of the position in the array
 */
static void *alloc_iter_state(const void *array)
{
    UNUSED(array);

    int *st = (int*)malloc(sizeof(int));
    *st = 0;
    return st;
}

/*
 * Gets the next item from the iterator
 */
static int get_next_iter(const void *array, void *iter_state, void **next)
{
    int *cur = (int*)iter_state;
    C_STATUS st = resize_array_get(array, *cur, next);
    (*cur)++;
    return st == C_OK;
}

/*
 * Creates a new resizable array. Uses the default size if none is
 * specified by the user.
 */
void *resize_array(size_t init_size)
{
    size_t sz = init_size <= DEF_SIZE ? DEF_SIZE : init_size;
    rs_array *rv = (rs_array*)malloc(sizeof(rs_array));
    void **buffer = malloc(sz * sizeof(void*));
    rv->buff = buffer;
    rv->capacity = sz;
    rv->base_cap = sz;
    rv->head.size = 0;
    rv->head.alloc_iter_state = alloc_iter_state;
    rv->head.get_next_iter = get_next_iter;
    rv->head.free_iter = 0;
    return rv;
}

/*
 * Adds an item to the array. Doubles the size of the array when full.
 */
void resize_array_add(void *array, void *item)
{
    rs_array *ra = array;

    if (ra->head.size == ra->capacity)
    {
        resize(ra, ra->capacity * 2);
    }

    ra->buff[ra->head.size] = item;
    ra->head.size++;
}

/*
 * Gets at item from the resizable array at the index specified. The item
 * parameter will point to the data item.
 */
C_STATUS resize_array_get(const void *array, size_t index, void **item)
{
    const rs_array *ra = array;
    if (index >= ra->head.size)
    {
        return CE_BOUNDS;
    }

    *item = ra->buff[index];
    return C_OK;
}

/*
 * Swaps two items in the array at the index values specified.
 */
C_STATUS resize_array_exchange(void *array, size_t first, size_t second)
{
    rs_array *ra = array;
    if (first >= ra->head.size || second >= ra->head.size)
    {
        return CE_BOUNDS;
    }

    void *tmp = ra->buff[first];
    ra->buff[first] = ra->buff[second];
    ra->buff[second] = tmp;
    return C_OK;
}

/*
 * Removes an item from the array and shuffles everything after that
 * point back one space. Reallcoates the array if it's one quarter full.
 */
C_STATUS resize_array_remove(void *array, size_t index, void **item)
{
    rs_array *ra = array;
    void *rv;
    int err = resize_array_get(array, index, &rv);
    if (err)
    {
        return err;
    }
    else if (item)
    {
        *item = rv;
    }

    for (size_t i = index; i < ra->head.size - 1; i++)
    {
        ra->buff[i] = ra->buff[i + 1];
    }

    ra->head.size--;

    if (ra->head.size > ra->base_cap && ra->head.size <= ra->capacity / 4)
    {
        resize(ra, ra->capacity / 4);
    }

    return C_OK;
}

/*
 * Shallow copies a resizable array
 */
void *resize_array_copy(const void *array)
{
    const rs_array *ra = array;
    rs_array *rv = (rs_array*)malloc(sizeof(rs_array));
    memcpy(rv, ra, sizeof(rs_array));

    void **buffer = malloc(rv->capacity * sizeof(void*));
    rv->buff = buffer;
    memcpy(rv->buff, ra->buff, rv->head.size * sizeof(void*));
    return rv;
}

/*
 * Free the array and its buffer. Optionally free all items within.
 */
void resize_array_free(void *array, int items)
{
    rs_array *ra = array;
    if (items)
    {
        for (size_t i = 0; i < ra->head.size; i++)
        {
            free(ra->buff[i]);
        }
    }

    free(ra->buff);
    free(ra);
}
