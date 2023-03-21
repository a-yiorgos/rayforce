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

#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include "rayforce.h"

/*
 * Points to a actual error position in a source code
 */
typedef struct span_t
{
    u16_t start_line;
    u16_t end_line;
    u16_t start_column;
    u16_t end_column;
} span_t;

typedef struct debuginfo_t
{
    u32_t count;
    span_t span[256];
} debuginfo_t;

extern null_t debuginfo_init(debuginfo_t *debuginfo);
extern u32_t debuginfo_insert(debuginfo_t *debuginfo, span_t span);
extern span_t *debuginfo_get(debuginfo_t *debuginfo, u32_t index);

#endif