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

#ifndef MONAD_H
#define MONAD_H

#include "rayforce.h"

typedef object_t (*unary_t)(object_t);

object_t rf_call_unary(u8_t flags, unary_t f, object_t x);
object_t rf_get_variable(object_t x);
object_t rf_type(object_t x);
object_t rf_count(object_t x);
object_t rf_til(object_t x);
object_t rf_distinct(object_t x);
object_t rf_group(object_t x);
object_t rf_group_remap(object_t x);
object_t rf_sum(object_t x);
object_t rf_avg(object_t x);
object_t rf_min(object_t x);
object_t rf_max(object_t x);
object_t rf_not(object_t x);
object_t rf_iasc(object_t x);
object_t rf_idesc(object_t x);
object_t rf_asc(object_t x);
object_t rf_desc(object_t x);
object_t rf_guid_generate(object_t x);
object_t rf_neg(object_t x);
object_t rf_where(object_t x);
object_t rf_key(object_t x);
object_t rf_value(object_t x);
object_t rf_fread(object_t x);
object_t rf_parse(object_t x);
object_t rf_read_parse_compile(object_t x);

#endif
