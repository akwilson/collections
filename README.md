# Collections Library in C

A slowly expanding collection of collection implementations in C. So far we have,

## Resizing Array
* Add items to the array and it will expend / contract as required
* Doubles in size whenever it is full
* Halves in size whenever a quarter full

## Priority Queue
* Add items to the queue and initialise with a compare function
* Compare function defines priority
* Items will be returned in priority order

## Hash Table
* Associates values to keys using a hash function
* Collisions handled by sequential chaining
* Table resizes dynamically to minimise collisions and wasted space

## Common Functions
* Return the number of items in the collection
* Iterate over the collection

For example,

```c
// Get the number of items in some collection
int c = clxns_count(array);

// Iterate...
void *iter = clxns_iter_new(array);
while (clxns_iter_move_next(iter))
{
    char *res = clxns_iter_get_next(iter);
    printf("%s\n", res);
}

clxns_iter_free(iter);
```

## Building and Running Unit Tests
in the top level directory
```
$ make
$ ./scripts/runTests.sh
```
