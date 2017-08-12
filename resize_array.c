#include <stdlib.h>
#include <stdio.h>

#define DEF_SIZE 8

typedef struct rs_array {
    void** buff;
    int capacity;
    int size;
} rs_array;

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

void resize_array_add(void* array, void* item)
{
    rs_array* ra = array;

    if (ra->size == ra->capacity)
    {
        int sz = ra->capacity * 2;
        void** buffer = realloc(ra->buff, sz * sizeof(void*));
        ra->buff = buffer;
        ra->capacity = sz;
    }

    ra->buff[ra->size] = item;
    ra->size++;
}

void* resize_array_get(void* array, int index)
{
    rs_array* ra = array;
    return ra->buff[index];
}

void resize_array_remove(void* array, int index)
{
    rs_array* ra = array;
    for (int i = index; i < ra->size - 1; i++)
    {
        ra->buff[i] = ra->buff[i + 1];
    }
}

int resize_array_count(void* array)
{
    rs_array* ra = array;
    return ra->size;
}

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
