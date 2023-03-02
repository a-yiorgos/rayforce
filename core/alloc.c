#include <stdio.h>
#include <string.h>
#include "alloc.h"
#include "storm.h"
#include <sys/mman.h>
#include <assert.h>

#define MAP_ANONYMOUS 0x20

// Global allocator reference
a0 GLOBAL_A0 = NULL;

extern void *a0_alloc(int size)
{
    int i, order;
    i8 *block;
    void *buddy;

    // calculate minimal order for this size
    i = 0;
    while (BLOCK_SIZE(i) < size + 1) // one more byte for storing order
        i++;

    order = i = (i < MIN_ORDER) ? MIN_ORDER : i;

    // level up until non-null list found
    for (;; i++)
    {
        if (i > MAX_ORDER)
            return NULL;
        if (GLOBAL_A0->freelist[i])
            break;
    }

    // remove the block out of list
    block = GLOBAL_A0->freelist[i];
    GLOBAL_A0->freelist[i] = *(void **)GLOBAL_A0->freelist[i];

    // split until i == order
    while (i-- > order)
    {
        buddy = BUDDY_OF(block, i);
        GLOBAL_A0->freelist[i] = buddy;
    }

    // store order in previous byte
    *((i8 *)(block - 1)) = order;
    return block;
}

extern void a0_free(void *block)
{
    int i;
    void *buddy;
    void **p;

    // fetch order in previous byte
    i = *((i8 *)(((i8 *)block) - 1));

    for (;; i++)
    {
        // calculate buddy
        buddy = BUDDY_OF(block, i);
        p = &(GLOBAL_A0->freelist[i]);

        // find buddy in list
        while ((*p != NULL) && (*p != buddy))
            p = (void **)*p;

        // not found, insert into list
        if (*p != buddy)
        {
            *(void **)block = GLOBAL_A0->freelist[i];
            GLOBAL_A0->freelist[i] = block;
            return;
        }
        // found, merged block starts from the lower one
        block = (block < buddy) ? block : buddy;
        // remove buddy out of list
        *p = *(void **)*p;
    }
}

extern void a0_init()
{
    a0 alloc;

    alloc = (a0)mmap(NULL, sizeof(struct A0),
                     PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    assert((alloc != MAP_FAILED));

    memset(alloc, 0, sizeof(struct A0));
    GLOBAL_A0 = alloc;
    GLOBAL_A0->freelist[MAX_ORDER] = GLOBAL_A0->pool;
}

extern void a0_deinit()
{
    munmap(GLOBAL_A0, sizeof(struct A0));
}