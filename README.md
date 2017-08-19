# Collections Library in C

A slowly expanding collection of collection implementations in C. So far we have,

## Resizing Array
* Add items to the array and it will expend / contract as required
* Doubles in size whenever it is full
* Halves in size whenever a quarter full

## Common Functions
* Return the number of items in the collection
* Iterate over the collection

For example,

```c
// get the number of items in some collection
int c = clxns_count(array);

// Iterate...
void* iter = clxns_iter_new(array);
while (clxns_iter_move_next(iter))
{
    char* res = clxns_iter_get_next(iter);
    printf("%s\n", res);
}

clxns_iter_free(iter);
```
