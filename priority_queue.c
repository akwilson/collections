#include <stdlib.h>
#include "common.h"
#include "collections.h"

typedef struct p_queue
{
    header head;
    void* array;
    int (*compare)(void* first, void* second);
} p_queue;

static void* alloc_iter_state()
{
}

static int get_next_iter(void* array, void* iter_state, void** next)
{
}

static int greaterThan(p_queue* pq, int first, int second)
{
    void* fVal;
    void* sVal;
    resize_array_get(pq->array, first, &fVal);
    resize_array_get(pq->array, second, &sVal);
    if (pq->compare(fVal, sVal) > 0)
    {
        return 1;
    }

    return 0;
}

static void sink(void* pqueue, int key)
{
}

static void swim(p_queue* pq, int key)
{
    while (key > 1 && greaterThan(pq, key / 2, key))
    {
        resize_array_exchange(pq->array, key, key / 2);
        key /= 2;
    }
}

void* priority_queue(int init_size, int (*compare)(void* first, void* second))
{
    p_queue* rv = (p_queue*)malloc(sizeof(p_queue));
    rv->array = resize_array(init_size);
    rv->compare = compare;
    rv->head.size = 0;
    rv->head.alloc_iter_state = alloc_iter_state;
    rv->head.get_next_iter = get_next_iter;

    // 1 based array for the heap
    resize_array_add(rv->array, NULL);
    return rv;
}

/*
 * Add value to the end of the array, i.e. the end of the heap and
 * swim it up the heap until it finds the root or a parent with a
 * higher value
 */
C_STATUS priority_queue_add(void* pqueue, void* item)
{
    if (item == NULL)
    {
        return CE_NULL_ITEM;
    }

    p_queue* pq = pqueue;
    resize_array_add(pq->array, item);
    swim(pq, ++(pq->head.size));
    return C_OK;
}

C_STATUS priority_queue_pop(void* pqueue, void** item)
{
}

C_STATUS priority_queue_peek(void* pqueue, void** item)
{
    p_queue* pq = pqueue;
    return resize_array_get(pq->array, 1, item);
}

void priority_queue_free(void* array, int items)
{
}
