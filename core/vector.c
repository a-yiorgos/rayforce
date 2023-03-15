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

#include "rayforce.h"
#include "alloc.h"
#include "vector.h"
#include "util.h"

/*
 * Each vector capacity is always factor of 8
 * This allows to avoid storing capacity in vector
 */
#define CAPACITY_FACTOR 8

/*
 * Aligns x to the nearest multiple of a
 */
#define alignup(x, a) (((x) + (a)-1) & ~((a)-1))

/*
 * Calculates capacity for vector of length x
 */
#define capacity(x) (alignup(x, CAPACITY_FACTOR))

/*
 * Appends value to the end of vector (dynamically grows vector if needed)
 */
#define push(vector, type, value)                                                                    \
    i64_t len = (vector)->list.len;                                                                  \
    i64_t cap = capacity(len);                                                                       \
    if (cap == 0)                                                                                    \
        (vector)->list.ptr = rayforce_malloc(CAPACITY_FACTOR * sizeof(type));                        \
    else if (cap == len)                                                                             \
        (vector)->list.ptr = rayforce_realloc((vector)->list.ptr, capacity(len + 1) * sizeof(type)); \
    ((type *)((vector)->list.ptr))[(vector)->list.len++] = (value);

#define pop(vector, type) ((type *)((vector)->list.ptr))[(vector)->list.len--]

/*
 * Attemts to make vector from list if all elements are of the same type
 */
#define flatten(list, vec, fpush, mem)            \
    {                                             \
        value_t *member;                          \
        vec = vector_##mem(0);                    \
                                                  \
        for (u64_t i = 0; i < list.list.len; i++) \
        {                                         \
            member = &as_list(&list)[i];          \
                                                  \
            if (member->type != type)             \
            {                                     \
                value_free(&vec);                 \
                return list;                      \
            }                                     \
                                                  \
            fpush(&vec, member->mem);             \
        }                                         \
    }

extern value_t vector(i8_t type, u8_t size_of_val, i64_t len)
{
    value_t v = {
        .type = type,
        .list = {
            .ptr = NULL,
            .len = len,
        },
    };

    if (len == 0)
        return v;

    v.list.ptr = rayforce_malloc(size_of_val * capacity(len));

    return v;
}

extern null_t vector_i64_push(value_t *vector, i64_t value)
{
    push(vector, i64_t, value);
}

extern i64_t vector_i64_pop(value_t *vector)
{
    return pop(vector, i64_t);
}

extern null_t vector_f64_push(value_t *vector, f64_t value)
{
    push(vector, f64_t, value);
}

extern f64_t vector_f64_pop(value_t *vector)
{
    return pop(vector, f64_t);
}

extern null_t list_push(value_t *list, value_t value)
{
    push(list, value_t, value);
}

extern value_t list_pop(value_t *list)
{
    return pop(list, value_t);
}

/*
 * Try to flatten list in a vector if all elements are of the same type
 */
extern value_t list_flatten(value_t list)
{
    if (list.type != TYPE_LIST)
        return list;

    u64_t len = list.list.len;
    i8_t type;
    value_t vec;

    if (len == 0)
        return list;

    type = as_list(&list)[0].type;

    // Only scalar types can be flattened
    if (type > -1)
        return list;

    switch (type)
    {
    case -TYPE_I64:
        flatten(list, vec, vector_i64_push, i64);
        break;
    case -TYPE_F64:
        flatten(list, vec, vector_f64_push, f64);
        break;
    case -TYPE_SYMBOL:
        flatten(list, vec, vector_i64_push, i64);
        if (vec.type == TYPE_I64)
            vec.type = TYPE_SYMBOL;
        break;
    default:
        return list;
    }

    value_free(&list);

    return vec;
}