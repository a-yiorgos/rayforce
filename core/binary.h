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

#ifndef BINARY_H
#define BINARY_H

#include "rayforce.h"

typedef object_t (*binary_t)(object_t, object_t);

object_t rf_call_binary(u8_t flags, binary_t f, object_t x, object_t y);
object_t rf_set_variable(object_t key, object_t val);
object_t rf_dict(object_t x, object_t y);
object_t rf_table(object_t x, object_t y);
object_t rf_rand(object_t x, object_t y);
object_t rf_add(object_t x, object_t y);
object_t rf_sub(object_t x, object_t y);
object_t rf_mul(object_t x, object_t y);
object_t rf_div(object_t x, object_t y);
object_t rf_mod(object_t x, object_t y);
object_t rf_fdiv(object_t x, object_t y);
object_t rf_like(object_t x, object_t y);
object_t rf_eq(object_t x, object_t y);
object_t rf_ne(object_t x, object_t y);
object_t rf_lt(object_t x, object_t y);
object_t rf_gt(object_t x, object_t y);
object_t rf_le(object_t x, object_t y);
object_t rf_ge(object_t x, object_t y);
object_t rf_and(object_t x, object_t y);
object_t rf_or(object_t x, object_t y);
object_t rf_and(object_t x, object_t y);
object_t rf_get(object_t x, object_t y);
object_t rf_find(object_t x, object_t y);
object_t rf_concat(object_t x, object_t y);
object_t rf_filter(object_t x, object_t y);
object_t rf_take(object_t x, object_t y);
object_t rf_in(object_t x, object_t y);
object_t rf_sect(object_t x, object_t y);
object_t rf_except(object_t x, object_t y);
object_t rf_cast(object_t x, object_t y);
object_t rf_group_Table(object_t x, object_t y);
object_t rf_xasc(object_t x, object_t y);
object_t rf_xdesc(object_t x, object_t y);

#endif
