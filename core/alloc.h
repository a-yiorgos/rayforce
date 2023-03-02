#ifndef ALLOC_H_
#define ALLOC_H_

#include "storm.h"

#define MIN_ORDER (1ll << 2)
#define MAX_ORDER (1ll << 4)

#define POOL_SIZE (1ll << MAX_ORDER)
#define BLOCK_SIZE(i) (1ll << (i))

#define POOL_BASE ((i64)GLOBAL_A0->pool)
#define MEM_OFFSET(b) ((i64)b - POOL_BASE)
#define BUDDY_OF(b, i) ((void *)((MEM_OFFSET(b) ^ (1 << (i))) + POOL_BASE))

#define PAGE_SIZE 4096

typedef struct A0
{
    void *freelist[MAX_ORDER + 2]; // one more slot for first block in pool
    i8 pool[POOL_SIZE];
} __attribute__((aligned(PAGE_SIZE))) * a0;

extern void *a0_alloc(int size);
extern void a0_free(void *block);

extern void a0_init();
extern void a0_deinit();

#endif
