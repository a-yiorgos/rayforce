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

#include "set.h"
#include "heap.h"

set_t set_new(u64_t size, hash_t hash)
{
    size = next_power_of_two_u64(size);
    i64_t i, *kv;
    set_t set = (set_t)heap_malloc(sizeof(struct set_t) + size * sizeof(i64_t));

    set->size = size;
    set->hash = hash;

    kv = set->keys;

    for (i = 0; i < size; i++)
        kv[i] = NULL_I64;

    return set;
}

nil_t set_free(set_t set)
{
    heap_free(set);
}

nil_t set_rehash(set_t *set)
{
    i64_t i, old_size = (*set)->size, key, new_size = old_size * 2,
             *old_keys = (*set)->keys, *new_keys;
    u64_t index, factor;
    hash_t hash = (*set)->hash;
    set_t new_set = set_new(new_size, hash);

    new_keys = new_set->keys;

    for (i = 0; i < new_size; i++)
        new_keys[i] = NULL_I64;

    for (i = 0; i < old_size; i++)
    {
        if (old_keys[i] != NULL_I64)
        {
            key = old_keys[i];
            factor = new_size - 1,
            index = hash(key) & factor;

            while (new_keys[index] != NULL_I64)
                index = (index + 1) & factor;

            new_keys[index] = key;
        }
    }

    heap_free(*set);

    *set = new_set;
}

bool_t set_insert(set_t *set, i64_t key)
{
    while (true)
    {
        i32_t i, size = (*set)->size;
        u64_t factor = size - 1, index = (*set)->hash(key) & factor;
        i64_t *keys = (*set)->keys;

        for (i = index; i < size; i++)
        {
            if (keys[i] == key)
                return false;

            if (keys[i] == NULL_I64)
            {
                keys[i] = key;
                return true;
            }
        }

        set_rehash(set);
    }
}

bool_t set_contains(set_t set, i64_t key)
{
    // i32_t i, size = set->size;
    // u64_t factor = set->size - 1, index = set->hash(key) & factor;
    // i64_t *keys = set->keys;

    // for (i = index; i < size; i++)
    // {
    //     if (keys[i] != NULL_I64)
    //     {
    //         if (set->cmp(keys[i], key) == 0)
    //             return true;
    //     }
    //     else
    //         return false;
    // }

    return false;
}
