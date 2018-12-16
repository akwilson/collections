#include <stdlib.h>
#include <string.h>
#include "collections.h"
#include "common.h"

// Default size of a hash table if none is supplied by the user
#define DEF_SIZE 7

// An item in the hash table
typedef struct _node
{
    kvp key_value;      // key and value pair
    unsigned long hash; // the hash of the key
    struct _node *next; // next item in the linked list of nodes
} node;

// Set of pointers to iterate over the hash table
typedef struct _iter_ptrs
{
    node **head; // first node in the hash table array
    node **tail; // last node in the hash table array
    node *next;  // next item pointer
} iter_ptr;

// The hash table
typedef struct _hash_tab
{
    header head;
    node **array;  // hash table slots
    int capacity;  // number of items in the hash_table
    int base_cap;  // the initial / minimum size
    int num_array; // number of slots filled in the array
} hash_tab;

/*
 * Creates a new hash table iterator and points it to the first item in the table.
 */
static void *alloc_iter_state(void *table)
{
    hash_tab *ht = table;
    iter_ptr *rv = (iter_ptr*)malloc(sizeof(iter_ptr));
    for (int i = 0; i < ht->capacity; i++)
    {
        rv->head = ht->array + i;
        if (*(rv->head))
        {
            break;
        }
    }

    rv->tail = ht->array + ht->capacity - 1;
    rv->next = *(rv->head);
    return rv;
}

static int get_next_node(void* iter_state, node** next)
{
    iter_ptr* iptr = iter_state;

    if (iptr->next)
    {
        *next = iptr->next;
        iptr->next = iptr->next->next;

        if (!iptr->next && iptr->head != iptr->tail)
        {
            // Find the next occupied slot in the array
            do
            {
                iptr->head++;
            } while (!(*iptr->head) && iptr->head != iptr->tail);

            iptr->next = iptr->head ? *(iptr->head) : 0;
        }

        return 1;
    }

    *next = 0;
    return 0;
}

static int get_next_iter(void* table, void* iter_state, void** next)
{
    node* nn;
    if (get_next_node(iter_state, &nn))
    {
        *next = &nn->key_value;
        return 1;
    }

    *next = 0;
    return 0;
}

/*
 * Resize the hash table to the new size. Allows the hash table to expand and shrink as items
 * are added and removed. Stops too many items colliding and being added to sequential searches
 * when the table is growing.
 *
 * A resize operation creates a new array in the hash table and iterates over all nodes to
 * find them a new slot.
 */
static void resize(hash_tab *ht, int new_size)
{
    node **array = calloc(new_size, sizeof(node*));

    void *iter = alloc_iter_state(ht);
    node *nn;
    while (get_next_node(iter, &nn))
    {
        int slot = nn->hash % new_size;
        node **head = &array[slot];
        nn->next = *head;
        *head = nn;
    }

    free(iter);
    free(ht->array);

    ht->array = array;
    ht->capacity = new_size;
}

/*
 * Hashes a string. Courtesy of http://www.cse.yorku.ca/~oz/hash.html
 */
static unsigned long hash(unsigned char *str)
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

static node** get_slot_head(hash_tab* ht, unsigned char* key, unsigned long *hash_val)
{
    *hash_val = hash(key);
    int slot = *hash_val % ht->capacity;

    return &ht->array[slot];
}

static node* new_node(char* key, void* value, unsigned long hash_val)
{
    node* rv = (node*)malloc(sizeof(node));
    rv->hash = hash_val;
    rv->key_value.key = key;
    rv->key_value.value = value;
    rv->next = 0;
    return rv;
}

static node** find(node** head, unsigned long hash_val)
{
    while (*head)
    {
        if ((*head)->hash == hash_val)
        {
            return head;
        }

        head = &(*head)->next;
    }

    return 0;
}

void *hash_table(int init_size)
{
    int sz = init_size <= DEF_SIZE ? DEF_SIZE : init_size;

    hash_tab *ht = (hash_tab*)malloc(sizeof(hash_tab));
    node **array = calloc(sz, sizeof(node*));
    ht->array = array;
    ht->capacity = sz;
    ht->base_cap = sz;
    ht->num_array = 0;

    ht->head.size = 0;
    ht->head.alloc_iter_state = alloc_iter_state;
    ht->head.get_next_iter = get_next_iter;
    ht->head.free_iter = 0;

    return ht;
}

void hash_table_add(void *table, char *key, void *value)
{
    hash_tab *ht = table;

    if (ht->num_array > ht->capacity / 2)
    {
        resize(ht, ht->capacity * 2);
    }

    unsigned long hash_val;
    node **head = get_slot_head(ht, key, &hash_val);
    if (*head)
    {
        // collision
        node **ptr = find(head, hash_val);
        if (ptr)
        {
            (*ptr)->key_value.key = key;
            (*ptr)->key_value.value = value;
            (*ptr)->hash = hash_val;
            return;
        }
    }
    else
    {
        ht->num_array++;
    }

    node *nn = new_node(key, value, hash_val);
    nn->next = *head;
    *head = nn;
    ht->head.size++;
}

C_STATUS hash_table_get(void* table, char* key, void** value)
{
    hash_tab* ht = table;

    unsigned long hash_val;
    node** head = get_slot_head(ht, key, &hash_val);
    if (*head)
    {
        node** ptr = find(head, hash_val);
        if (ptr)
        {
            *value = (*ptr)->key_value.value;
            return C_OK;
        }
    }

    *value = 0;
    return CE_MISSING;
}

C_STATUS hash_table_remove(void* table, char* key)
{
    hash_tab* ht = table;

    if (ht->num_array > ht->base_cap && ht->num_array <= ht->capacity / 4)
    {
        resize(ht, ht->capacity / 4);
    }

    unsigned long hash_val;
    node** head = get_slot_head(ht, key, &hash_val);
    if (*head)
    {
        node** ptr = find(head, hash_val);
        if (ptr)
        {
            node* rm = (*ptr);
            if (head == ptr && rm->next == 0)
            {
                ht->num_array--;
            }

            (*ptr) = rm->next;
            free(rm);
            ht->head.size--;
            return C_OK;
        }
    }

    return CE_MISSING;
}

/*
 * Copies a hash table. Performs a shallow copy by creating a new table and adding
 * all of the values from the original in to it.
 */
void *hash_table_copy(void *table)
{
    hash_tab *orig = table;
    hash_tab *rv = hash_table(orig->capacity);

    void *iter = alloc_iter_state(orig);
    node *nn;
    while (get_next_node(iter, &nn))
    {
        hash_table_add(rv, nn->key_value.key, nn->key_value.value);
    }

    free(iter);
    return rv;
}

/*
 * Frees a hash table. If items is non-zero this method will also attempt to free any memory
 * pointed to by the keys and values.
 */
void hash_table_free(void* table, int items)
{
    hash_tab* ht = table;
    iter_ptr* iter = alloc_iter_state(table);
    node* next;
    while (get_next_node(iter, &next))
    {
        if (items)
        {
            free(next->key_value.key);
            free(next->key_value.value);
        }

        free(next);
    }

    free(ht->array);
    free(ht);
    free(iter);
}
