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

/*
 * result is a list:
 *   [0] - value
 *   [1] - bins
 *   [2] - filters
 */
obj_t __group(obj_t x, obj_t y, obj_t z)
{
    u64_t i, l;
    obj_t v, res;

    switch (x->type)
    {
    case TYPE_TABLE:
        l = as_list(x)[1]->len;
        res = vector(TYPE_LIST, l);
        for (i = 0; i < l; i++)
        {
            v = as_list(as_list(x)[1])[i];
            as_list(res)[i] = __group(v, y, z);
        }

        return table(clone(as_list(x)[0]), res);

    default:
        res = vn_list(3, clone(x), clone(y), clone(z));
        res->type = TYPE_GROUPMAP;
        return res;
    }
}

obj_t group_map(obj_t *aggr, obj_t x, obj_t y, obj_t z)
{
    u64_t l;
    i64_t *ids;
    obj_t bins, res;

    if (z)
    {
        l = z->len;
        ids = as_i64(z);
    }
    else
    {
        l = x->len;
        ids = NULL;
    }

    switch (x->type)
    {
    case TYPE_BOOL:
    case TYPE_BYTE:
    case TYPE_CHAR:
        bins = index_group_i8((i8_t *)as_u8(x), ids, l);
        break;
    case TYPE_I64:
    case TYPE_SYMBOL:
    case TYPE_TIMESTAMP:
        bins = index_group_i64(as_i64(x), ids, l);
        break;
    case TYPE_F64:
        bins = index_group_i64((i64_t *)as_f64(x), ids, l);
        break;
    case TYPE_GUID:
        bins = index_group_guid(as_guid(x), ids, l);
        break;
    case TYPE_LIST:
        bins = index_group_obj(as_list(x), ids, l);
        break;
    default:
        throw(ERR_TYPE, "'by' unable to group by: %s", typename(x->type));
    }

    res = __group(y, bins, z);

    *aggr = aggr_first(x, bins, z);
    drop(bins);

    return res;
}

// obj_t group_call(obj_t car, obj_t args, type_t types[], u64_t cnt)
// {
//     u64_t i, j, l;
//     obj_t v, res;

//     l = args->len;

//     res = list(cnt);

//     for (i = 0; i < cnt; i++)
//     {
//         for (j = 0; j < l; j++)
//         {
//             if (types[j] == TYPE_GROUPMAP)
//                 v = at_idx(as_list(args)[j], i);
//             else
//                 v = clone(as_list(args)[j]);

//             stack_push(v);
//         }

//         v = call(car, l);

//         if (is_error(v))
//         {
//             res->len = i;
//             drop(res);
//             return v;
//         }

//         ins_obj(&res, i, v);
//     }

//     return res;
// }

// obj_t filter_call(obj_t car, obj_t args)
// {
//     u64_t i, l;
//     obj_t v;

//     l = args->len;

//     for (i = 0; i < l; i++)
//     {
//         v = clone(as_list(args)[i]);
//         stack_push(v);
//     }

//     return call(car, l);
// }