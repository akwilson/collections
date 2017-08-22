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

static void sink(void* array, int key)
{
}

static void swim(void* array, int key)
{
}

static void exchange(void* array, int key_a, int key_b)
{
}

void* priority_queue(int init_size, int (*compare)(void* first, void* second))
{
    p_queue* rv = (p_queue*)malloc(sizeof(p_queue));
    rv->array = resize_array(init_size);
    rv->compare = compare;
    rv->head.size = 0;
    rv->head.alloc_iter_state = alloc_iter_state;
    rv->head.get_next_iter = get_next_iter;
    return rv;
}

C_STATUS priority_queue_add(void* pqueue, void* item)
{
    if (item == NULL)
    {
        return CE_NULL_ITEM;
    }

    p_queue* pq = pqueue;
    resize_array_add(pq->array, item);
    swim(pq->array, ++pq->head.size);
}

void* priority_queue_pop(void* pqueue)
{
}

void* priority_queue_peek(void* pqueue)
{
}

void priority_queue_free(void* array, int items)
{
}
