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
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include "string.h"
#include "symbols.h"
#include "rayforce.h"
#include "heap.h"
#include "util.h"
#include "runtime.h"
#include "ops.h"
#include "atomic.h"

/*
 * Simplefied version of murmurhash
 */
static inline u64_t str_hash(lit_p key, u64_t len)
{
    u64_t i, k, k1;
    u64_t hash = 0x1234ABCD1234ABCD;
    u64_t c1 = 0x87c37b91114253d5ULL;
    u64_t c2 = 0x4cf5ad432745937fULL;
    const int r1 = 31;
    const int r2 = 27;
    const u64_t m = 5ULL;
    const u64_t n = 0x52dce729ULL;

    // Process each 8-byte block of the key
    for (i = 0; i + 7 < len; i += 8)
    {
        k = (u64_t)key[i] |
            ((u64_t)key[i + 1] << 8) |
            ((u64_t)key[i + 2] << 16) |
            ((u64_t)key[i + 3] << 24) |
            ((u64_t)key[i + 4] << 32) |
            ((u64_t)key[i + 5] << 40) |
            ((u64_t)key[i + 6] << 48) |
            ((u64_t)key[i + 7] << 56);

        k *= c1;
        k = (k << r1) | (k >> (64 - r1));
        k *= c2;

        hash ^= k;
        hash = ((hash << r2) | (hash >> (64 - r2))) * m + n;
    }

    // Process the tail of the data
    k1 = 0;
    switch (len & 7)
    {
    case 7:
        k1 ^= ((u64_t)key[i + 6]) << 48; // fall through
    case 6:
        k1 ^= ((u64_t)key[i + 5]) << 40; // fall through
    case 5:
        k1 ^= ((u64_t)key[i + 4]) << 32; // fall through
    case 4:
        k1 ^= ((u64_t)key[i + 3]) << 24; // fall through
    case 3:
        k1 ^= ((u64_t)key[i + 2]) << 16; // fall through
    case 2:
        k1 ^= ((u64_t)key[i + 1]) << 8; // fall through
    case 1:
        k1 ^= ((u64_t)key[i]);
        k1 *= c1;
        k1 = (k1 << r1) | (k1 >> (64 - r1));
        k1 *= c2;
        hash ^= k1;
    }

    // Finalize the hash
    hash ^= len;
    hash ^= (hash >> 33);
    hash *= 0xff51afd7ed558ccdULL;
    hash ^= (hash >> 33);
    hash *= 0xc4ceb9fe1a85ec53ULL;
    hash ^= (hash >> 33);

    return hash;
}

symbols_p symbols_create(nil_t)
{
    symbols_p symbols = (symbols_p)heap_mmap(sizeof(struct symbols_t));

    symbols->size = SYMBOLS_SIZE;
    symbols->count = 0;
    symbols->syms = (symbol_p *)heap_mmap(SYMBOLS_SIZE * sizeof(symbol_p));

    return symbols;
}

nil_t symbols_destroy(symbols_p symbols)
{
    mmap_free(symbols->syms, symbols->size * sizeof(symbol_p));
    heap_unmap(symbols, sizeof(struct symbols_t));
}

i64_t symbols_intern(lit_p str, u64_t len)
{
    i64_t index;
    str_p intr;
    symbols_p symbols = runtime_get()->symbols;
    symbol_p new_bucket, current_bucket, b, *syms;

    syms = symbols->syms;
    index = str_hash(str, len) % symbols->size;
    intr = heap_intern(len + 1);

    new_bucket = (symbol_p)heap_alloc(sizeof(struct symbol_t));
    if (new_bucket == NULL)
        return NULL_I64;

    memcpy(intr, str, len);
    intr[len] = '\0';

    new_bucket->str = intr;

    for (;;)
    {
        current_bucket = __atomic_load_n(&syms[index], __ATOMIC_ACQUIRE);
        b = current_bucket;

        while (b != NULL)
        {
            if (strncmp(b->str, str, len) == 0)
            {
                heap_untern(len + 1);
                heap_free(new_bucket);
                return (i64_t)b->str;
            }

            b = __atomic_load_n(&b->next, __ATOMIC_ACQUIRE);
        }

        new_bucket->next = current_bucket;
        if (__atomic_compare_exchange(&syms[index], &current_bucket, &new_bucket, 1, __ATOMIC_RELEASE, __ATOMIC_RELAXED))
        {
            __atomic_fetch_add(&symbols->count, 1, __ATOMIC_RELAXED);
            return (i64_t)intr;
        }
    }
}

str_p str_from_symbol(i64_t key)
{
    return (str_p)key;
}

u64_t symbols_count(symbols_p symbols)
{
    return symbols->count;
}
