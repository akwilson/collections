#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "../src/collections.h"

static void* populate(int init, int num)
{
    char *k, *v;
    void* ht = hash_table(init);
    for (int i = 0; i < num; i++)
    {
        k = (char*)malloc(9);
        sprintf(k, "string%d", i);
        v = (char*)malloc(9);
        sprintf(v, "STRING%d", i);
        hash_table_add(ht, k, v);
    }

    return ht;
}

char* ht_add_items()
{
    void* ht = hash_table(0);
    hash_table_add(ht, "AAA", "aaa");
    hash_table_add(ht, "BBB", "bbb");
    hash_table_add(ht, "XXX", "xxx");
    hash_table_add(ht, "GGG", "ggg");
    hash_table_add(ht, "QQQ", "qqq");

    int cnt = clxns_count(ht);
    MU_ASSERT("Wrong number of items after add", cnt == 5);

    hash_table_free(ht, 0);
    return 0;
}

char* ht_replace()
{
    void* ht = hash_table(0);
    hash_table_add(ht, "AAA", "aaa");
    hash_table_add(ht, "BBB", "bbb");
    hash_table_add(ht, "AAA", "xxx");

    int cnt = clxns_count(ht);
    MU_ASSERT("Wrong number of items after add", cnt == 2);

    char* value;
    C_STATUS st = hash_table_get(ht, "AAA", (void*)&value);
    MU_ASSERT("Wrong status after first get", st == C_OK);
    MU_ASSERT("Wrong value after first get", !strcmp("xxx", value));

    st = hash_table_get(ht, "BBB", (void*)&value);
    MU_ASSERT("Wrong status after first get", st == C_OK);
    MU_ASSERT("Wrong value after first get", !strcmp("bbb", value));

    hash_table_free(ht, 0);
    return 0;
}

char* ht_get_items()
{
    int num = 30;
    void* ht = populate(0, num);

    int cnt = clxns_count(ht);
    MU_ASSERT("Wrong number of items after populate get", cnt == num);

    char* value;
    C_STATUS st = hash_table_get(ht, "string0", (void*)&value);
    MU_ASSERT("Wrong status after first get", st == C_OK);
    MU_ASSERT("Wrong value after first get", !strcmp("STRING0", value));

    // check no item
    st = hash_table_get(ht, "string31", (void*)&value);
    MU_ASSERT("Wrong status after second get", st == CE_MISSING);
    MU_ASSERT("Wrong value after second get", value == 0);

    char *k, *v;
    for (int i = 0; i < num; i++)
    {
        k = (char*)malloc(9);
        sprintf(k, "string%d", i);
        st = hash_table_get(ht, k, (void*)&value);
        MU_ASSERT("Wrong status after loop get", st == C_OK);

        v = (char*)malloc(9);
        sprintf(v, "STRING%d", i);
        MU_ASSERT("Wrong value after loop get", !strcmp(v, value));
    }

    hash_table_free(ht, 1);
    return 0;
}
