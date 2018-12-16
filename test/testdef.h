#ifndef TESTDEF_H
#define TESTDEF_H

// == RESIZE ARRAY ============================================================

char *ra_add(void);
char *ra_add_adjust(void);
char *ra_remove(void);
char *ra_remove_adjust(void);
char *ra_check_errors(void);
char *ra_exchange(void);
char *ra_copy_array(void);

// == PRIORITY QUEUE ==========================================================

char *pq_add_items(void);
char *pq_peek_items(void);
char *pq_pop_items(void);
char *pq_pop_items_max(void);
char *pq_iterate_items(void);
char *pq_copy_queue(void);

// == HASH TABLE ==============================================================

char *ht_add_items(void);
char *ht_replace(void);
char *ht_get_items(void);
char *ht_iterate_sparse(void);
char *ht_iterate(void);
char *ht_iterate_sparse_ish(void);
char *ht_iterate_empty(void);
char *ht_remove_items(void);
char *ht_copy(void);

#endif
