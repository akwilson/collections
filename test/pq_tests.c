/*
 * Unit tests for the priority queue
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/collections.h"
#include "minunit.h"

/*
 * Compares two strings. Used by the priority queue to add new items to the correct position.
 */
static int compare(const void *first, const void *second)
{
    return strcmp((const char*)first, (const char*)second);
}

/*
 * Test the addition of new items to the queue
 */
char *pq_add_items()
{
    void *pq = priority_queue_min(0, compare);
    C_STATUS status = priority_queue_add(pq, "CCC");
    MU_ASSERT("Bad return status after item add", status == C_OK);

    status = priority_queue_add(pq, "BBB");
    MU_ASSERT("Bad return status after item add", status == C_OK);

    status = priority_queue_add(pq, "DDD");
    MU_ASSERT("Bad return status after item add", status == C_OK);

    status = priority_queue_add(pq, "GGG");
    MU_ASSERT("Bad return status after item add", status == C_OK);

    status = priority_queue_add(pq, "AAA");
    MU_ASSERT("Bad return status after item add", status == C_OK);

    status = priority_queue_add(pq, "FFF");
    MU_ASSERT("Bad return status after item add", status == C_OK);

    status = priority_queue_add(pq, "EEE");
    MU_ASSERT("Bad return status after item add", status == C_OK);

    int cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count", cnt == 7);

    clxns_free(pq, 0);
    return 0;
}

/*
 * Peek items at the top of the queue
 */
char *pq_peek_items()
{
    void *pq = priority_queue_min(0, compare);
    priority_queue_add(pq, "CCC");
    priority_queue_add(pq, "BBB");
    priority_queue_add(pq, "DDD");
    priority_queue_add(pq, "GGG");
    priority_queue_add(pq, "AAA");

    char *res;
    C_STATUS status = priority_queue_peek(pq, (void*)&res);
    MU_ASSERT("Wrong status at peek head", status == C_OK);
    MU_ASSERT("Wrong item at peek head", strcmp(res, "AAA") == 0);

    int cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count", cnt == 5);

    clxns_free(pq, 0);
    pq = priority_queue_min(0, compare);
    status = priority_queue_peek(pq, (void*)&res);
    MU_ASSERT("Wrong status at peek on empty queue", status == CE_BOUNDS);

    clxns_free(pq, 0);
    return 0;
}

/*
 * Pop the min item from the queue
 */
char *pq_pop_items()
{
    void *pq = priority_queue_min(0, compare);
    priority_queue_add(pq, "CCC");
    priority_queue_add(pq, "BBB");
    priority_queue_add(pq, "DDD");
    priority_queue_add(pq, "GGG");
    priority_queue_add(pq, "AAA");

    char *res;
    C_STATUS status = priority_queue_pop(pq, (void*)&res);
    MU_ASSERT("Wrong status at pop head", status == C_OK);
    MU_ASSERT("Wrong item at pop head", strcmp(res, "AAA") == 0);
    int cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count", cnt == 4);

    status = priority_queue_pop(pq, (void*)&res);
    MU_ASSERT("Wrong status at pop head", status == C_OK);
    MU_ASSERT("Wrong item at pop head", strcmp(res, "BBB") == 0);
    cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count", cnt == 3);

    status = priority_queue_pop(pq, (void*)&res);
    MU_ASSERT("Wrong status at pop head", status == C_OK);
    MU_ASSERT("Wrong item at pop head", strcmp(res, "CCC") == 0);
    cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count", cnt == 2);

    status = priority_queue_pop(pq, (void*)&res);
    MU_ASSERT("Wrong status at pop head", status == C_OK);
    MU_ASSERT("Wrong item at pop head", strcmp(res, "DDD") == 0);
    cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count", cnt == 1);

    status = priority_queue_pop(pq, (void*)&res);
    MU_ASSERT("Wrong status at pop head", status == C_OK);
    MU_ASSERT("Wrong item at pop head", strcmp(res, "GGG") == 0);
    cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count", cnt == 0);

    status = priority_queue_pop(pq, (void*)&res);
    MU_ASSERT("Wrong status at pop on empty queue", status == CE_BOUNDS);

    clxns_free(pq, 0);
    return 0;
}

/*
 * Pop the max item from the queue
 */
char *pq_pop_items_max()
{
    void *pq = priority_queue_max(0, compare);
    priority_queue_add(pq, "CCC");
    priority_queue_add(pq, "BBB");
    priority_queue_add(pq, "DDD");
    priority_queue_add(pq, "GGG");
    priority_queue_add(pq, "AAA");

    char *res;
    C_STATUS status = priority_queue_pop(pq, (void*)&res);
    MU_ASSERT("Wrong status at pop head max", status == C_OK);
    MU_ASSERT("Wrong item at pop head max", strcmp(res, "GGG") == 0);
    int cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count max", cnt == 4);

    status = priority_queue_pop(pq, (void*)&res);
    MU_ASSERT("Wrong status at pop head max", status == C_OK);
    MU_ASSERT("Wrong item at pop head max", strcmp(res, "DDD") == 0);
    cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count", cnt == 3);

    status = priority_queue_pop(pq, (void*)&res);
    MU_ASSERT("Wrong status at pop head max", status == C_OK);
    MU_ASSERT("Wrong item at pop head max", strcmp(res, "CCC") == 0);
    cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count max", cnt == 2);

    status = priority_queue_pop(pq, (void*)&res);
    MU_ASSERT("Wrong status at pop head max", status == C_OK);
    MU_ASSERT("Wrong item at pop head max", strcmp(res, "BBB") == 0);
    cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count max", cnt == 1);

    status = priority_queue_pop(pq, (void*)&res);
    MU_ASSERT("Wrong status at pop head max", status == C_OK);
    MU_ASSERT("Wrong item at pop head max", strcmp(res, "AAA") == 0);
    cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count max", cnt == 0);

    status = priority_queue_pop(pq, (void*)&res);
    MU_ASSERT("Wrong status at pop on empty queue max", status == CE_BOUNDS);

    clxns_free(pq, 0);
    return 0;
}

/*
 * Iterate over the queue. Items should be returned in order.
 */
char *pq_iterate_items()
{
    void *pq = priority_queue_min(0, compare);
    priority_queue_add(pq, "CCC");
    priority_queue_add(pq, "BBB");
    priority_queue_add(pq, "DDD");
    priority_queue_add(pq, "GGG");
    priority_queue_add(pq, "AAA");

    void *iter = clxns_iter_new(pq);
    int avail = clxns_iter_move_next(iter);
    MU_ASSERT("Wrong iter move next rv", avail == 1);
    char *item = clxns_iter_get_next(iter);
    MU_ASSERT("Wrong iter value", strcmp(item, "AAA") == 0);

    avail = clxns_iter_move_next(iter);
    MU_ASSERT("Wrong iter move next rv", avail == 1);
    item = clxns_iter_get_next(iter);
    MU_ASSERT("Wrong iter value", strcmp(item, "BBB") == 0);

    avail = clxns_iter_move_next(iter);
    MU_ASSERT("Wrong iter move next rv", avail == 1);
    item = clxns_iter_get_next(iter);
    MU_ASSERT("Wrong iter value", strcmp(item, "CCC") == 0);

    avail = clxns_iter_move_next(iter);
    MU_ASSERT("Wrong iter move next rv", avail == 1);
    item = clxns_iter_get_next(iter);
    MU_ASSERT("Wrong iter value", strcmp(item, "DDD") == 0);

    avail = clxns_iter_move_next(iter);
    MU_ASSERT("Wrong iter move next rv", avail == 1);
    item = clxns_iter_get_next(iter);
    MU_ASSERT("Wrong iter value", strcmp(item, "GGG") == 0);

    avail = clxns_iter_move_next(iter);
    MU_ASSERT("Wrong iter move next rv", avail == 0);

    clxns_iter_free(iter);
    clxns_free(pq, 0);
    return 0;
}

/*
 * Copy a priority queue
 */
char *pq_copy_queue()
{
    void *pq = priority_queue_min(0, compare);
    priority_queue_add(pq, "CCC");
    priority_queue_add(pq, "BBB");
    priority_queue_add(pq, "DDD");
    priority_queue_add(pq, "GGG");
    priority_queue_add(pq, "AAA");

    void *pq2 = clxns_copy(pq);

    int cnt = clxns_count(pq2);
    MU_ASSERT("Wrong item return count on new q", cnt == 5);

    priority_queue_add(pq2, "AA");
    cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count on orig q after add", cnt == 5);
    cnt = clxns_count(pq2);
    MU_ASSERT("Wrong item return count on new q after add", cnt == 6);

    char *res;
    C_STATUS status = priority_queue_peek(pq, (void*)&res);
    MU_ASSERT("Wrong status at peek head on orig", status == C_OK);
    MU_ASSERT("Wrong item at peek head on orig", strcmp(res, "AAA") == 0);

    status = priority_queue_peek(pq2, (void*)&res);
    MU_ASSERT("Wrong status at peek head on new", status == C_OK);
    MU_ASSERT("Wrong item at peek head on new", strcmp(res, "AA") == 0);

    clxns_free(pq2, 0);
    clxns_free(pq, 0);
    return 0;
}
