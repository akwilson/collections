/*
 * Implementation functions for the priority queue. Items are stored in order
 * defined by the compare function provided at initialisation/
 */

#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "collections.h"

// The priority queue structure
typedef struct p_queue
{
    header head;
    void *array;
    int (*compare)(void *first, void *second);
    int order;
} p_queue;

/*
 * Gets the next item from the iterator.
 */
static int get_next_iter(void *collection, void *iter_state, void **next)
{
    UNUSED(collection);

    p_queue *pq = iter_state;
    C_STATUS status = priority_queue_pop(pq, next);
    return status == C_OK;
}

/*
 * Frees a priority queue iterator
 */
static void free_iter(void *iter_state)
{
    priority_queue_free(iter_state, 0);
}

/*
 * Uses the comparison function to compare two items in the
 * queue at the index positions specified
 */
static int checkVals(p_queue *pq, int first, int second)
{
    void *fVal;
    void *sVal;
    resize_array_get(pq->array, first, &fVal);
    resize_array_get(pq->array, second, &sVal);

    int cv = pq->compare(fVal, sVal);
    if ((!pq->order && cv > 0) || (pq->order && cv < 0))
    {
        return 1;
    }

    return 0;
}

/*
 * Push a key further down the priority queue
 */
static void sink(p_queue *pq, int key)
{
    while (2 * key < pq->head.size)
    {
        int j = 2 * key;
        if (j < pq->head.size && checkVals(pq, j, j + 1))
        {
            j++;
        }

        if (!checkVals(pq, key, j))
        {
            break;
        }

        resize_array_exchange(pq->array, key, j);
        key = j;
    }
}

/*
 * Promote a key further up the priority queue
 */
static void swim(p_queue *pq, int key)
{
    while (key > 1 && checkVals(pq, key / 2, key))
    {
        resize_array_exchange(pq->array, key, key / 2);
        key /= 2;
    }
}

/*
 * Creates a new priority queue. Values are sorted based on the compare function. The order
 * parameter indicates direction.
 */
static void *new_pq(int init_size, int order, int (*compare)(void *first, void *second))
{
    p_queue *rv = (p_queue*)malloc(sizeof(p_queue));
    rv->array = resize_array(init_size);
    rv->compare = compare;
    rv->order = order;
    rv->head.size = 0;
    rv->head.alloc_iter_state = priority_queue_copy;
    rv->head.get_next_iter = get_next_iter;
    rv->head.free_iter = free_iter;

    // 1 based array for the heap
    resize_array_add(rv->array, NULL);
    return rv;
}

/*
 * Creates a new priority queue. Values are sorted based on the compare function. Smallest items first.
 */
void *priority_queue_min(int init_size, int (*compare)(void *first, void *second))
{
    return new_pq(init_size, 0, compare);
}

/*
 * Creates a new priority queue. Values are sorted based on the compare function. Largest items first.
 */
void *priority_queue_max(int init_size, int (*compare)(void *first, void *second))
{
    return new_pq(init_size, 1, compare);
}

/*
 * Add value to the end of the array, i.e. the end of the heap and
 * swim it up the heap until it finds the root or a parent with a
 * higher value
 */
C_STATUS priority_queue_add(void *pqueue, void *item)
{
    if (item == NULL)
    {
        return CE_NULL_ITEM;
    }

    p_queue *pq = pqueue;
    resize_array_add(pq->array, item);
    swim(pq, ++(pq->head.size));
    return C_OK;
}

/*
 * Pops the next item off the queue
 */
C_STATUS priority_queue_pop(void *pqueue, void **item)
{
    p_queue *pq = pqueue;
    int last = pq->head.size;
    C_STATUS status = resize_array_exchange(pq->array, 1, last);
    if (status == CE_BOUNDS)
    {
        return status;
    }

    resize_array_remove(pq->array, last, item);
    pq->head.size = last - 1;
    sink(pq, 1);
    return C_OK;
}

/*
 * Returns but does not remove the next item off the queue
 */
C_STATUS priority_queue_peek(void *pqueue, void **item)
{
    p_queue *pq = pqueue;
    return resize_array_get(pq->array, 1, item);
}

/*
 * Shallow copies a priority queue
 */
void *priority_queue_copy(void *pqueue)
{
    p_queue *pq = pqueue;
    p_queue *rv = (p_queue*)malloc(sizeof(p_queue));
    memcpy(rv, pq, sizeof(p_queue));
    rv->array = resize_array_copy(pq->array);
    return rv;
}

/*
 * Frees all of the memory pointed to by the priority queue. Optionally frees data too.
 */
void priority_queue_free(void *pqueue, int items)
{
    p_queue *pq = pqueue;
    resize_array_free(pq->array, items);
    free(pq);
}
