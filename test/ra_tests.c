/*
 * Unit tests for the resize array
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/collections.h"
#include "minunit.h"

/*
 * Utility function to populate an array with test data
 */
static void *populate(int init, int num)
{
    char *buf;
    void *array = resize_array(init);
    for (int i = 0; i < num; i++)
    {
        buf = (char*)malloc(9);
        sprintf(buf, "string%d", i);
        resize_array_add(array, buf);
    }

    return array;
}

/*
 * Add three entries to the array. No resize required
 */
char *ra_add()
{
    int num_entries = 3;
    void *array = populate(0, num_entries);

    int s = clxns_count(array);
    MU_ASSERT("Wrong number of items in array", s == num_entries);

    char *res;
    char buf[8];
    for (int i = 0; i < num_entries; i++)
    {
        C_STATUS err = resize_array_get(array, i, (void**)&res);
        MU_ASSERT("Non-zero error code on get", err == C_OK);
        sprintf(buf, "string%d", i);
        MU_ASSERT("Incorrect data", strcmp(res, buf) == 0);
    }

    clxns_free(array, 1);
    return 0;
}

/*
 * Add enough items to the array to trigger a resize
 */
char *ra_add_adjust()
{
    int num_entries = 30;
    void *array = populate(4, num_entries);

    int s = clxns_count(array);
    MU_ASSERT("Wrong number of items in array", s == num_entries);

    char buf[9];
    int i = 0;
    void *iter = clxns_iter_new(array);
    while (clxns_iter_move_next(iter))
    {
        char *res = clxns_iter_get_next(iter);
        sprintf(buf, "string%d", i++);
        MU_ASSERT("Incorrect data in iter", strcmp(res, buf) == 0);
    }

    MU_ASSERT("Incorrect iter count", i == 30);
    clxns_iter_free(iter);
    clxns_free(array, 1);
    return 0;
}

/*
 * Remove items from a small array. No resize operations.
 */
char *ra_remove()
{
    int num_entries = 5;
    void *array = populate(0, num_entries);

    int s = clxns_count(array);
    MU_ASSERT("Wrong number of items in array", s == num_entries);

    char *res;
    C_STATUS err = resize_array_remove(array, 2, (void**)&res);
    MU_ASSERT("Non-zero error code on remove", err == C_OK);
    free(res);
    err = resize_array_remove(array, 3, (void**)&res);
    MU_ASSERT("Non-zero error code on remove", err == C_OK);
    free(res);
    err = resize_array_remove(array, 0, (void**)&res);
    MU_ASSERT("Non-zero error code on remove", err == C_OK);
    MU_ASSERT("Remove should return value if asked", strcmp(res, "string0") == 0);
    free(res);

    s = clxns_count(array);
    MU_ASSERT("Wrong number of items in array after removals", s == 2);

    err = resize_array_get(array, 0, (void**)&res);
    MU_ASSERT("Non-zero error code on get", err == C_OK);
    MU_ASSERT("Wrong item at 0", strcmp(res, "string1") == 0);
    err = resize_array_get(array, 1, (void**)&res);
    MU_ASSERT("Non-zero error code on get", err == C_OK);
    MU_ASSERT("Wrong item at 1", strcmp(res, "string3") == 0);

    clxns_free(array, 1);
    return 0;
}

/*
 * Remove items from a larger array. Resize up and down.
 */
char *ra_remove_adjust()
{
    int num_entries = 30;
    void *array = populate(4, num_entries);

    char *res;
    C_STATUS err;
    for (int i = 0; i < num_entries - 1; i++)
    {
        err = resize_array_remove(array, 0, (void**)&res);
        MU_ASSERT("Non-zero error code on remove", err == C_OK);
        free(res);
    }

    int s = clxns_count(array);
    MU_ASSERT("Wrong number of items in array after removals", s == 1);
    err = resize_array_get(array, 0, (void**)&res);
    MU_ASSERT("Non-zero error code on get", err == C_OK);
    MU_ASSERT("Wrong item at 1", strcmp(res, "string29") == 0);

    clxns_free(array, 1);
    return 0;
}

/*
 * Check error conditions are triggered at the appropriate time
 */
char *ra_check_errors()
{
    void *array = resize_array(0);
    // get empty array
    char *res;
    C_STATUS err = resize_array_get(array, 0, (void**)&res);
    MU_ASSERT("Empty array should return error on get", err == CE_BOUNDS);

    err = resize_array_remove(array, 0, 0);
    MU_ASSERT("Empty array should return error on remove", err == CE_BOUNDS);

    resize_array_add(array, "string0");
    err = resize_array_get(array, 99, (void**)&res);
    MU_ASSERT("Array should return OOB on get", err == CE_BOUNDS);

    err = resize_array_remove(array, 99, 0);
    MU_ASSERT("Array should return OOB on remove", err == CE_BOUNDS);

    clxns_free(array, 0);
    return 0;
}

/*
 * Exchange two items in the array
 */
char *ra_exchange()
{
    int num_entries = 5;
    void *array = populate(0, num_entries);

    C_STATUS status = resize_array_exchange(array, -1, 3);
    MU_ASSERT("Wrong status after exchange", status == CE_BOUNDS);

    status = resize_array_exchange(array, 3, -1);
    MU_ASSERT("Wrong status after exchange", status == CE_BOUNDS);

    status = resize_array_exchange(array, 5, 3);
    MU_ASSERT("Wrong status after exchange", status == CE_BOUNDS);

    status = resize_array_exchange(array, 3, 5);
    MU_ASSERT("Wrong status after exchange", status == CE_BOUNDS);

    status = resize_array_exchange(array, 0, 4);
    MU_ASSERT("Wrong status after exchange", status == C_OK);

    char *res;
    resize_array_get(array, 0, (void**)&res);
    MU_ASSERT("Wrong value at pos 0 after exchange", strcmp("string4", res) == 0);

    resize_array_get(array, 4, (void**)&res);
    MU_ASSERT("Wrong value at pos 0 after exchange", strcmp("string0", res) == 0);

    clxns_free(array, 1);
    return 0;
}

/*
 * Check the shallow copy routine
 */
char *ra_copy_array()
{
    int num_entries = 5;
    void *array = populate(0, num_entries);
    void *array2 = clxns_copy(array);

    int s = clxns_count(array2);
    MU_ASSERT("Wrong number of items in array", s == num_entries);

    resize_array_add(array2, "akw");

    s = clxns_count(array);
    MU_ASSERT("Wrong number of items in orig array after add", s == num_entries);
    s = clxns_count(array2);
    MU_ASSERT("Wrong number of items in new array after add", s == num_entries + 1);

    char *res;
    C_STATUS status = resize_array_get(array, 5, (void**)&res);
    MU_ASSERT("Item should not be added to original array", status == CE_BOUNDS);

    status = resize_array_get(array2, 5, (void**)&res);
    MU_ASSERT("Item should be added to new array status", status == C_OK);
    MU_ASSERT("Item should be added to new array", strcmp("akw", res) == 0);

    clxns_free(array2, 0);
    clxns_free(array, 1);
    return 0;
}

/*
 * Insert items in to the beginning, middle and end of an array
 */
char *ra_insert()
{
    void *array = resize_array(0);
    resize_array_add(array, "string1");
    resize_array_add(array, "string2");
    resize_array_add(array, "string3");
    resize_array_add(array, "string5");
    resize_array_add(array, "string6");
    resize_array_add(array, "string7");
    resize_array_add(array, "string8");

    // insert beginning, middle, end
    resize_array_insert(array, 0, "string0");
    resize_array_insert(array, 4, "string4");
    resize_array_insert(array, 9, "string9");

    // tests
    int s = clxns_count(array);
    MU_ASSERT("Wrong number of items in array after insert", s == 10);

    char buf[16];
    int i = 0;
    void *iter = clxns_iter_new(array);
    while (clxns_iter_move_next(iter))
    {
        char *res = clxns_iter_get_next(iter);
        sprintf(buf, "string%d", i++);
        MU_ASSERT("Incorrect data in iter after insert", !strcmp(res, buf));
    }

    MU_ASSERT("Incorrect iter count after insert", i == 10);

    clxns_iter_free(iter);
    clxns_free(array, 0);
    return 0;
}

/*
 * Replace items in an array
 */
char *ra_replace()
{
    void *array = resize_array(0);
    resize_array_add(array, "stringx");
    resize_array_add(array, "string1");
    resize_array_add(array, "string2");
    resize_array_add(array, "stringx");
    resize_array_add(array, "string4");
    resize_array_add(array, "stringx");

    // insert beginning, middle, end
    resize_array_replace(array, 0, "string0");
    resize_array_replace(array, 3, "string3");
    resize_array_replace(array, 5, "string5");

    // tests
    int s = clxns_count(array);
    MU_ASSERT("Wrong number of items in array after replace", s == 6);

    char buf[8];
    int i = 0;
    void *iter = clxns_iter_new(array);
    while (clxns_iter_move_next(iter))
    {
        char *res = clxns_iter_get_next(iter);
        sprintf(buf, "string%d", i++);
        MU_ASSERT("Incorrect data in iter after replace", !strcmp(res, buf));
    }

    MU_ASSERT("Incorrect iter count after replace", i == 6);

    clxns_iter_free(iter);
    clxns_free(array, 0);
    return 0;
}
