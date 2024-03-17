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

#ifndef POOL_H
#define POOL_H

#include "rayforce.h"
#include <pthread.h>

typedef struct task_t
{
    nil_t (*function)(raw_p);
    raw_p arg;
    u64_t len;
} *task_p;

typedef struct result_t
{
    raw_p data;
    u64_t len;
} *result_p;

typedef struct executor_t
{
    u64_t id;
    pthread_t handle;
} *executor_p;

typedef struct pool_t
{
    pthread_cond_t complete; // Condition variable to signal task completion
    executor_p executors;    // Array of executors
    u64_t executors_count;   // Number of executors
    task_p tasks;            // Array of input tasks for executors
    result_p results;        // Array of results from executors
    u64_t tasks_remaining;   // Counter to track remaining tasks
} *pool_p;

pool_p pool_new(u64_t executors_count);
nil_t pool_free(pool_p pool);

#endif // POOL_H
