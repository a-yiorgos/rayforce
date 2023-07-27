/*
 *   Copyright (c) 2023 Anton Kundenko <singaraiona@gmail.com>
 *   All rights reserved.

 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:

 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.

 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include "hash.h"
#include "rayforce.h"
#include "heap.h"
#include "util.h"

CASSERT(sizeof(struct bucket_t) == 16, hash_h)
CASSERT(sizeof(struct ht_t) == 24, hash_h)

ht_t ht_new(u64_t size, hash_f hash, cmp_f cmp)
{
    size = next_power_of_two_u64(size);

    i64_t i;
    bucket_t *buckets;
    ht_t ht = (ht_t)heap_malloc(sizeof(struct ht_t) + size * sizeof(struct bucket_t));

    ht->size = size;
    ht->hash = hash;
    ht->cmp = cmp;

    buckets = ht->buckets;

    for (i = 0; i < size; i++)
        buckets[i].key = NULL_I64;

    return ht;
}

nil_t ht_free(ht_t ht)
{
    heap_free(ht);
}

nil_t ht_rehash(ht_t *table)
{
    // i64_t i, old_size = table->size, key, val, factor, index;
    // bucket_t *old_buckets = table->buckets, *new_buckets;

    // // Double the table size.
    // table->size *= 2;
    // table->buckets = (bucket_t *)heap_malloc(table->size * sizeof(bucket_t));
    // factor = table->size - 1;

    // new_buckets = table->buckets;

    // for (i = 0; i < table->size; i++)
    //     new_buckets[i].key = NULL_I64;

    // for (i = 0; i < old_size; i++)
    // {
    //     if (old_buckets[i].key != NULL_I64)
    //     {
    //         key = old_buckets[i].key;
    //         val = old_buckets[i].val;
    //         index = table->hasher(key) & factor;

    //         // Linear probing.
    //         while (new_buckets[index].key != NULL_I64)
    //         {
    //             if (index == table->size)
    //                 panic("ht is full!!");

    //             index = index + 1;
    //         }

    //         new_buckets[index].key = key;
    //         new_buckets[index].val = val;
    //     }
    // }

    // heap_free(old_buckets);
}

bucket_t *ht_get(ht_t *ht, i64_t key)
{
    while (true)
    {
        bucket_t *buckets = (*ht)->buckets;
        u64_t i = 0, size = (*ht)->size, factor = size - 1,
              index = (*ht)->hash(key) & factor;
        cmp_f cmp = (*ht)->cmp;

        for (i = index; i < size; i++)
        {
            if (buckets[i].key == NULL_I64)
                return buckets + i;
            if (cmp(buckets[i].key, key) == 0)
                return buckets + i;
        }

        ht_rehash(ht);
    }
}
