#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "collections.h"
#include "minunit.h"

int tests_run = 0;

static void* populate(int init, int num)
{
    char* buf;
    void* array = resize_array(init);
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
static char* add_rs()
{
    int num_entries = 3;
    void* array = populate(0, num_entries);

    int s = resize_array_count(array);
    MU_ASSERT("Wrong number of items in array", s == num_entries);

    for (int i = 0; i < num_entries; i++)
    {
        char* res = resize_array_get(array, i);
        char* buf = (char*)malloc(8);
        sprintf(buf, "string%d", i);
        MU_ASSERT("Incorrect data", strcmp(res, buf) == 0);
    }

    resize_array_free(array, 1);
    return 0;
}

/*
 * Add enough items to the array to trigger a resize
 */
static char* add_rs_adjust()
{
    int num_entries = 30;
    void* array = populate(4, num_entries);

    int s = resize_array_count(array);
    MU_ASSERT("Wrong number of items in array", s == num_entries);

    for (int i = 0; i < num_entries; i++)
    {
        char* res = resize_array_get(array, i);
        char* buf = (char*)malloc(9);
        sprintf(buf, "string%d", i);
        MU_ASSERT("Incorrect data", strcmp(res, buf) == 0);
    }

    resize_array_free(array, 1);
    return 0;
}

static char* remove_rs()
{
    int num_entries = 5;
    void* array = populate(0, num_entries);

    int s = resize_array_count(array);
    MU_ASSERT("Wrong number of items in array", s == num_entries);

    resize_array_remove(array, 2);
    resize_array_remove(array, 3);
    resize_array_remove(array, 0);

    s = resize_array_count(array);
    MU_ASSERT("Wrong number of items in array after removals", s == 2);

    char* res = resize_array_get(array, 0);
    MU_ASSERT("Wrong item at 0", strcmp(res, "string1") == 0);
    res = resize_array_get(array, 1);
    MU_ASSERT("Wrong item at 1", strcmp(res, "string3") == 0);
}

static char* remove_rs_adjust()
{
    int num_entries = 30;
    void* array = populate(4, num_entries);

    for (int i = 0; i < num_entries - 1; i++)
    {
        resize_array_remove(array, 0);
    }

    int s = resize_array_count(array);
    MU_ASSERT("Wrong number of items in array after removals", s == 1);
    char* res = resize_array_get(array, 0);
    MU_ASSERT("Wrong item at 1", strcmp(res, "string29") == 0);
}

static char* all_tests()
{
    MU_RUN_TEST(add_rs);
    MU_RUN_TEST(add_rs_adjust);
    MU_RUN_TEST(remove_rs);
    MU_RUN_TEST(remove_rs_adjust);
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
        printf("All tests passed\n");
    }

    printf("%d tests run\n", tests_run);
    return result != 0;
}
