# Collections Library in C

A slowly expanding collection of collection implementations in C for Linux and MacOS. So far we have,

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
* Shallow copy a collection to another of the same type
* Free a collection, and optionally the data it refers to

For example,

```c
/* Get the number of items in some collection */
int c = clxns_count(array);

/* Iterate... */
void *iter = clxns_iter_new(array);
while (clxns_iter_move_next(iter))
{
    char *res = clxns_iter_get_next(iter);
    printf("%s\n", res);
}

clxns_iter_free(iter);

/* Copy a collection */
void *duplicate = clxns_copy(array);

/* Free a collection and all memory it refers to */
clxns_free(array, 1);
```

## Building, Running Unit Tests and Installing
In the top level directory
```
$ make               # build the library and unit tests
$ make tests         # run the unit tests
$ sudo make install  # install to /usr/include
```
On Linux you will then need to run `ldconfig` to pick up the installed library
```
$ sudo ldconfig
```
You can confirm that the library was installed correctly
```
$ ldconfig -p | grep libclxns
        libclxns.so (libc6,x86-64) => /usr/local/lib/libclxns.so
```
