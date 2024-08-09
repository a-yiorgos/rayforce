/*
 *   Copyright (c) 2024 Anton Kundenko <singaraiona@gmail.com>
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

#include "group.h"
#include "error.h"
#include "ops.h"
#include "util.h"
#include "index.h"
#include "aggr.h"
#include "items.h"
#include "unary.h"
#include "eval.h"
#include "string.h"
#include "hash.h"
#include "pool.h"

obj_p group_map(obj_p val, obj_p index)
{
    u64_t i, l;
    obj_p v, res;

    switch (val->type)
    {
    case TYPE_TABLE:
        l = as_list(val)[1]->len;
        res = list(l);
        for (i = 0; i < l; i++)
        {
            v = as_list(as_list(val)[1])[i];
            as_list(res)[i] = group_map(v, index);
        }

        return table(clone_obj(as_list(val)[0]), res);

    default:
        res = vn_list(2, clone_obj(val), clone_obj(index));
        res->type = TYPE_GROUPMAP;
        return res;
    }
}

obj_p build_partition(u64_t morsels_num, i64_t keys[], u64_t len, hash_f hash, cmp_f cmp, raw_p seed)
{
    u64_t i, j, l, ht_len;
    i64_t idx, *hk;
    obj_p morsels, *ht;

    ht_len = len / morsels_num;
    morsels = list(morsels_num);

    for (i = 0; i < morsels_num; i++)
        as_list(morsels)[i] = NULL_OBJ;

    for (i = 0; i < len; i++)
    {
        j = hash(keys[i], seed) % morsels_num;
        ht = as_list(morsels) + j;
        if (*ht == NULL_OBJ)
            *ht = ht_oa_create(ht_len, TYPE_I64);

        ht_oa_tab_insert_with(ht, keys[i], 0, hash, cmp, seed);
    }

    return morsels;
}

// Some adaptation of the paper:
// https://15721.courses.cs.cmu.edu/spring2016/papers/p743-leis.pdf
obj_p merge_morsels(u64_t radix_bits, obj_p partitions, u64_t len, hash_f hash, cmp_f cmp, raw_p seed)
{
    u64_t i, l, j, ht_len;
    i64_t g, v, *phk, *phv, *mhk;
    obj_p ht, mht;

    l = partitions->len;
    ht_len = len / as_list(partitions)[0]->len;

    // merge morsels from each partition due to radix bits
    ht = ht_oa_create(ht_len, TYPE_I64);

    // merge each morsel of the partition by radix bits
    for (i = 0, g = 0; i < l; i++)
    {
        mht = as_list(as_list(partitions)[i])[radix_bits];
        if (mht == NULL_OBJ)
            continue;
        mhk = as_i64(as_list(mht)[0]);
        ht_len = as_list(mht)[0]->len;

        for (j = 0; j < ht_len; j++)
        {
            if (mhk[j] != NULL_I64)
            {
                // NOTE: assume we never rehash because the length of the hash table is enough
                v = ht_oa_tab_insert_with(&ht, mhk[j], g, hash, cmp, seed);
                if (g == v)
                    g++;
            }
        }
    }

    return vn_list(2, ht, i64(g));
}

obj_p aggr_morsels(obj_p partitions, u64_t radix_bits, i64_t keys[], i64_t vals[], u64_t len, hash_f hash, cmp_f cmp, raw_p seed)
{
    u64_t i, j, l, morsels_num, partition_len;
    i64_t idx, *hk, *hv;
    obj_p local_morsels, ht;

    local_morsels = as_list(partitions)[radix_bits];
    morsels_num = local_morsels->len;

    // initialize morsels values
    // for (i = 0; i < morsels_num; i++)
    //     memset(as_list(as_list(local_morsels)[i]) + 1, 0, sizeof(i64_t) * as_list(local_morsels)[i]->len);

    for (i = 0; i < len; i++)
    {
        j = hash(keys[i], seed) % morsels_num;
        ht = as_list(local_morsels)[j];
        idx = ht_oa_tab_get_with(ht, keys[i], hash, cmp, seed);
        // as_i64(as_list(ht)[1])[idx] += vals[i];
        as_i64(as_list(ht)[1])[idx] += 1;
    }

    return NULL_OBJ;
}

obj_p group_build_index(i64_t keys[], u64_t len, hash_f hash, cmp_f cmp, raw_p seed)
{
    u64_t i, groups, partitions_num, partition_len, morsels_num;
    obj_p morsels, partitions, aggr;
    pool_p pool;

    groups = 0;
    pool = pool_get();
    partitions_num = pool_split_by(pool, len, 0);
    partition_len = len / partitions_num;
    morsels_num = partition_len * sizeof(i64_t) / L1_CACHE_SIZE;

    // build morsels for every partition
    pool_prepare(pool);
    for (i = 0; i < partitions_num - 1; i++)
        pool_add_task(pool, build_partition, 6, morsels_num, keys + i * partition_len, partition_len, hash, cmp, seed);

    pool_add_task(pool, build_partition, 6, morsels_num, keys + i * partition_len, len - (i * partition_len), hash, cmp, seed);

    partitions = pool_run(pool);
    timeit_tick("build partitions");

    // merge morsels from each partition due to radix bits
    pool_prepare(pool);
    for (i = 0; i < morsels_num; i++)
        pool_add_task(pool, merge_morsels, 6, i, partitions, len, hash, cmp, seed);

    morsels = pool_run(pool);
    timeit_tick("merge morsels");

    partitions_num = morsels->len;
    for (i = 0; i < partitions_num; i++)
        groups += as_list(as_list(morsels)[i])[1]->i64;

    // Aggregate
    partitions_num = partitions->len;
    pool_prepare(pool);
    for (i = 0; i < partitions_num - 1; i++)
        pool_add_task(pool, aggr_morsels, 8, partitions, i, keys + i * partition_len, keys + i * partition_len, partition_len, hash, cmp, seed);

    pool_add_task(pool, aggr_morsels, 8, partitions, i, keys + i * partition_len, keys + i * partition_len, len - (i * partition_len), hash, cmp, seed);

    aggr = pool_run(pool);
    timeit_tick("aggr morsels");

    drop_obj(aggr);

    return vn_list(3, i64(groups), morsels, partitions);
}
