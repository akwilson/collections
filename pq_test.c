#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "collections.h"
#include "minunit.h"

int tests_run = 0;

static int compare(void* first, void* second)
{
    return strcmp((const char*)first, (const char*)second);
}

static char* add_items()
{
    void* pq = priority_queue(0, compare);
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

    return 0;
}

static char* peek_items()
{
    void* pq = priority_queue(0, compare);
    priority_queue_add(pq, "CCC");
    priority_queue_add(pq, "BBB");
    priority_queue_add(pq, "DDD");
    priority_queue_add(pq, "GGG");
    priority_queue_add(pq, "AAA");

    char* res;
    C_STATUS status = priority_queue_peek(pq, (void*)&res);
    MU_ASSERT("Wrong status at peek head", status == C_OK);
    MU_ASSERT("Wrong item at peek head", strcmp(res, "AAA") == 0);

    int cnt = clxns_count(pq);
    MU_ASSERT("Wrong item return count", cnt == 5);

    pq = priority_queue(0, compare);
    status = priority_queue_peek(pq, (void*)&res);
    MU_ASSERT("Wrong status at peek on empty queue", status == CE_BOUNDS);

    return 0;
}

static char* pop_items()
{
    void* pq = priority_queue(0, compare);
    priority_queue_add(pq, "CCC");
    priority_queue_add(pq, "BBB");
    priority_queue_add(pq, "DDD");
    priority_queue_add(pq, "GGG");
    priority_queue_add(pq, "AAA");

    char* res;
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

    return 0;
}

static char* iterate_items()
{
    void* pq = priority_queue(0, compare);
    priority_queue_add(pq, "CCC");
    priority_queue_add(pq, "BBB");
    priority_queue_add(pq, "DDD");
    priority_queue_add(pq, "GGG");
    priority_queue_add(pq, "AAA");

    void* iter = clxns_iter_new(pq);
    int avail = clxns_iter_move_next(iter);
    MU_ASSERT("Wrong iter move next rv", avail == 1);
    char* item = clxns_iter_get_next(iter);
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

    return 0;
}

static char* all_tests()
{
    MU_RUN_TEST(add_items);
    MU_RUN_TEST(peek_items);
    MU_RUN_TEST(pop_items);
    // MU_RUN_TEST(iterate_items);
    return 0;
}

int main(int argc, char* argv[])
{
    char* result = all_tests();
    if (result != 0)
    {
        printf("%s\n", result);
    }
    else
    {
        printf("All priority queue tests passed\n");
    }

    printf("%d priority queue tests run\n", tests_run);
    return result != 0;
}
