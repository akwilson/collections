#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "collections.h"
#include "minunit.h"

int tests_run = 0;

/*
 * Add three entries to the array. No resize required
 */
static char* add_rs()
{
    int num_entries = 3;
    char* buf;
    void* array = resize_array(0);
    for (int i = 0; i < num_entries; i++)
    {
        buf = (char*)malloc(8);
        sprintf(buf, "string%d", i);
        resize_array_add(array, buf);
    }

    int s = resize_array_count(array);
    MU_ASSERT("Wrong number of items in array", s == num_entries);

    for (int i = 0; i < num_entries; i++)
    {
        char* res = resize_array_get(array, i);

        buf = (char*)malloc(8);
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
    char* buf;
    void* array = resize_array(4);
    for (int i = 0; i < num_entries; i++)
    {
        buf = (char*)malloc(9);
        sprintf(buf, "string%d", i);
        resize_array_add(array, buf);
    }

    int s = resize_array_count(array);
    MU_ASSERT("Wrong number of items in array", s == num_entries);

    for (int i = 0; i < num_entries; i++)
    {
        char* res = resize_array_get(array, i);
    
        buf = (char*)malloc(9);
        sprintf(buf, "string%d", i);
        MU_ASSERT("Incorrect data", strcmp(res, buf) == 0);
    }

    resize_array_free(array, 1);
    return 0;
}

static char* all_tests()
{
    MU_RUN_TEST(add_rs);
    MU_RUN_TEST(add_rs_adjust);
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
