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

char* ht_iterate_sparse()
{
    void* ht = hash_table(0);
    hash_table_add(ht, "AAA", "aaa");
    hash_table_add(ht, "BBB", "bbb");

    int i = 0;
    void* iter = clxns_iter_new(ht);
    while (clxns_iter_move_next(iter))
    {
        kvp* val = clxns_iter_get_next(iter);
        if (!i)
        {
            MU_ASSERT("Incorrect key after first iterate", !strcmp(val->key, "AAA"));
            MU_ASSERT("Incorrect value after first iterate", !strcmp(val->value, "aaa"));
            i++;
        }
        else
        {
            MU_ASSERT("Incorrect key after second iterate", !strcmp(val->key, "BBB"));
            MU_ASSERT("Incorrect value after second iterate", !strcmp(val->value, "bbb"));
        }
    }

    MU_ASSERT("Incorrect sparse iter count", i == 1);
    clxns_iter_free(iter);
    hash_table_free(ht, 0);
    return 0;
}

char* ht_iterate()
{
    int num_entries = 30;
    void* ht = populate(0, num_entries);
    char buf[9];

    int i = 0;
    void* iter = clxns_iter_new(ht);
    while (clxns_iter_move_next(iter))
    {
        kvp* val = clxns_iter_get_next(iter);
        printf("%s %s\n", val->key, val->value);
        /*
        sprintf(buf, "string%d", i);
        MU_ASSERT("Incorrect key after iterate", !strcmp(val->key, buf));

        sprintf(buf, "STRING%d", i++);
        MU_ASSERT("Incorrect value after iterate", !strcmp(val->value, buf));
        */
        i++;
    }

    MU_ASSERT("Incorrect iter count", i == num_entries);
    clxns_iter_free(iter);
    hash_table_free(ht, 1);
    return 0;
}

char* ht_iterate_empty()
{
    void* ht = hash_table(0);
    void* iter = clxns_iter_new(ht);
    int i = 0;
    while (clxns_iter_move_next(iter))
    {
        i++;
    }

    MU_ASSERT("Incorrect empty iter count", i == 0);
    clxns_iter_free(iter);
    return 0;
}

char* ht_remove_items()
{
    int num = 30;
    void* ht = populate(0, num);

    C_STATUS st = hash_table_remove(ht, "string31");
    MU_ASSERT("Wrong status after first remove", st == CE_MISSING);

    int cnt;
    char *k, *value;
    for (int i = 0; i < num; i++)
    {
        k = (char*)malloc(9);
        sprintf(k, "string%d", i);
        st = hash_table_remove(ht, k);
        MU_ASSERT("Wrong status after loop remove", st == C_OK);

        st = hash_table_get(ht, k, (void*)&value);
        MU_ASSERT("Wrong status after remove get", st == CE_MISSING);

        cnt = clxns_count(ht);
        MU_ASSERT("Wrong number of items after remove", cnt == num - i - 1);
    }

    hash_table_free(ht, 1);
    return 0;
}
