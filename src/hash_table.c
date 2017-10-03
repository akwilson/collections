#include <stdlib.h>
#include "collections.h"
#include "common.h"

#define DEF_SIZE 7

// An item in the hash table
typedef struct _node
{
    unsigned int hash;  // the hash of the key
    char* key;          // the data key
    void* value;        // the data value
    struct _node* next; // next item in the linked list of nodes
} node;

// The hash table
typedef struct _hash_tab
{
    header head;
    node* array;   // hash table slots
    int capacity;  // number of items in the hash_table
    int base_cap;  // the initial / minimum size
    int num_array; // number of slots filled in the array
} hash_tab;

/*
 * Hashes a string. Courtesy of http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
    {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

void* hash_table(int init_size)
{
    int sz = init_size == 0 ? DEF_SIZE : init_size;

    hash_tab* ht = (hash_tab*)malloc(sizeof(hash_tab));
    node* array = malloc(sz * sizeof(node));
    ht->array = array;
    ht->capacity = sz;
    ht->base_cap = sz;
    ht->capacity = 0;

    ht->head.size = 0;
    ht->head.alloc_iter_state = 0;
    ht->head.get_next_iter = 0;
    ht->head.free_iter = 0;

    return ht;
}

void hash_table_add(void* table, char* key, void* value)
{
}

C_STATUS hash_table_get(void* table, char* key, void** value)
{
    *value = "";
    return C_OK;
}

C_STATUS hash_table_remove(void* table, char* key)
{
    return C_OK;
}

void* hash_table_copy(void* table)
{
    return 0;
}

void hash_table_free(void* table, int items)
{
}
