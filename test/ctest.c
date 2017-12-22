#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "testdef.h"

int tests_run = 0;

static char* all_tests()
{
    MU_RUN_TEST(ra_add);
    MU_RUN_TEST(ra_add_adjust);
    MU_RUN_TEST(ra_remove);
    MU_RUN_TEST(ra_remove_adjust);
    MU_RUN_TEST(ra_check_errors);
    MU_RUN_TEST(ra_exchange);
    MU_RUN_TEST(ra_copy_array);

    MU_RUN_TEST(pq_add_items);
    MU_RUN_TEST(pq_peek_items);
    MU_RUN_TEST(pq_pop_items);
    MU_RUN_TEST(pq_pop_items_max);
    MU_RUN_TEST(pq_iterate_items);
    MU_RUN_TEST(pq_copy_queue);

    MU_RUN_TEST(ht_add_items);
    MU_RUN_TEST(ht_get_items);
    MU_RUN_TEST(ht_replace);
    MU_RUN_TEST(ht_iterate_sparse);
    MU_RUN_TEST(ht_iterate);
    MU_RUN_TEST(ht_iterate_empty);
    MU_RUN_TEST(ht_remove_items);
    MU_RUN_TEST(ht_copy);

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
        printf("All collections unit tests passed\n");
    }

    printf("%d tests run\n", tests_run);
    return result != 0;
}
