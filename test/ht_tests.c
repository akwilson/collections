/*
 * Unit test for the hash table
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "../src/collections.h"

/*
 * Populate a hash table with some test data
 */
static void *populate(int init, int num)
{
    char *k, *v;
    void *ht = hash_table(init);
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

/*
 * Test that items can be added to the hash table
 */
char *ht_add_items()
{
    void *ht = hash_table(0);
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

/*
 * Replace the value of an existing key
 */
char *ht_replace()
{
    void *ht = hash_table(0);
    hash_table_add(ht, "AAA", "aaa");
    hash_table_add(ht, "BBB", "bbb");
    hash_table_add(ht, "AAA", "xxx");

    int cnt = clxns_count(ht);
    MU_ASSERT("Wrong number of items after add", cnt == 2);

    char *value;
    C_STATUS st = hash_table_get(ht, "AAA", (void*)&value);
    MU_ASSERT("Wrong status after first get", st == C_OK);
    MU_ASSERT("Wrong value after first get", !strcmp("xxx", value));

    st = hash_table_get(ht, "BBB", (void*)&value);
    MU_ASSERT("Wrong status after first get", st == C_OK);
    MU_ASSERT("Wrong value after first get", !strcmp("bbb", value));

    hash_table_free(ht, 0);
    return 0;
}

/*
 * Lookup items in the hash table
 */
char *ht_get_items()
{
    int num = 30;
    void *ht = populate(0, num);

    int cnt = clxns_count(ht);
    MU_ASSERT("Wrong number of items after populate get", cnt == num);

    char *value;
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

    hash_table_free(ht, 0);
    return 0;
}

/*
 * Iterate over a sparsely populated hash table
 */
char *ht_iterate_sparse()
{
    void *ht = hash_table(0);
    hash_table_add(ht, "AAA", "aaa");
    hash_table_add(ht, "BBB", "bbb");

    int i = 0;
    void *iter = clxns_iter_new(ht);
    while (clxns_iter_move_next(iter))
    {
        kvp *val = clxns_iter_get_next(iter);
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

/*
 * Test the hash table iterator.
 *
 * Key/value pairs take the form "STRING${INDEX_VALUE}". The order of items returned is uncertain, so
 * this test checks that each key and value is the same index  *value and that the sum of the index
 * values is as expected.
 */
char *ht_iterate()
{
    int num_entries = 30;
    void *ht = populate(0, num_entries);

    int i = 0, idx_tot = 0;
    void *iter = clxns_iter_new(ht);
    while (clxns_iter_move_next(iter))
    {
        kvp *val = clxns_iter_get_next(iter);
        char *idx = val->key + 6;
        int key_idx = atoi(idx);

        idx = val->value + 6;
        int val_idx = atoi(idx);
        MU_ASSERT("Key and value indexes do not match", key_idx == val_idx);

        idx_tot += val_idx;
        i++;
    }

    int expected_idx_tot = num_entries  *(num_entries - 1) / 2;
    MU_ASSERT("Index totals incorrect. Missing or dupe values in table?", idx_tot == expected_idx_tot);
    MU_ASSERT("Incorrect iter count", i == num_entries);
    clxns_iter_free(iter);
    hash_table_free(ht, 1);
    return 0;
}

/*
 * Iterate over a hash table with sequentially empty slots
 */
char *ht_iterate_sparse_ish()
{
    // Create a new hash table and add a small number of items
    void *ht = hash_table(0);
    hash_table_add(ht, "AAA", "aaa");
    hash_table_add(ht, "BBB", "bbb");
    hash_table_add(ht, "XXX", "xxx");
    hash_table_add(ht, "GGG", "ggg");
    hash_table_add(ht, "QQQ", "qqq");

    int i = 0;
    void *iter = clxns_iter_new(ht);
    while (clxns_iter_move_next(iter))
    {
        clxns_iter_get_next(iter);
        i++;
    }

    MU_ASSERT("Incorrect iter count for sparse-ish table", i == 5);
    clxns_iter_free(iter);
    hash_table_free(ht, 0);

    return 0;
}

/*
 * Iterate over an empty hash table. i.e. don't return anything and don't crash...
 */
char *ht_iterate_empty()
{
    void *ht = hash_table(0);
    void *iter = clxns_iter_new(ht);
    int i = 0;
    while (clxns_iter_move_next(iter))
    {
        i++;
    }

    MU_ASSERT("Incorrect empty iter count", i == 0);
    clxns_iter_free(iter);
    return 0;
}

/*
 * Remove items from a hash table
 */
char *ht_remove_items()
{
    int num = 30;
    void *ht = populate(0, num);

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

/*
 * Copy a hash table. Should copy the table and all of its structures, does not copy the items in the table.
 */
char *ht_copy()
{
    // Create a new hash tble and add a few items
    void *ht = hash_table(0);
    hash_table_add(ht, "AAA", "aaa");
    hash_table_add(ht, "BBB", "bbb");
    hash_table_add(ht, "XXX", "xxx");
    hash_table_add(ht, "GGG", "ggg");
    hash_table_add(ht, "QQQ", "qqq");

    // Copy it
    void *ht2 = hash_table_copy(ht);

    int cnt = clxns_count(ht2);
    MU_ASSERT("Wrong number of items after copy", cnt == 5);

    /*
     * Add an item to the new hash table. Check the the original is untouched and the item is
     * added only to the copy. First check table length.
     */
    hash_table_add(ht2, "ZZZ", "zzz");
    cnt = clxns_count(ht2);
    MU_ASSERT("Wrong number of items after copy insert", cnt == 6);
    cnt = clxns_count(ht);
    MU_ASSERT("Wrong number of items after copy insert orig", cnt == 5);

    // Then check the new item can be found
    char *value;
    C_STATUS st = hash_table_get(ht, "ZZZ", (void*)&value);
    MU_ASSERT("Wrong status after copy orig get", st == CE_MISSING);
    st = hash_table_get(ht2, "ZZZ", (void*)&value);
    MU_ASSERT("Wrong status after copy new get", st == C_OK);
    MU_ASSERT("Wrong value after copy new get", !strcmp("zzz", value));

    hash_table_free(ht, 0);
    hash_table_free(ht2, 0);
    return 0;
}
