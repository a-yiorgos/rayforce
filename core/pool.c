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

#include "pool.h"
#include "heap.h"
#include "util.h"

pool_p pool_new(u64_t executors_count)
{
    pool_p pool = (pool_p)heap_alloc(sizeof(struct pool_t));
    pool->executors = (executor_p)heap_alloc(executors_count * sizeof(struct executor_t));

    return pool;
}

nil_t pool_free(pool_p pool)
{
    heap_free(pool->executors);
    heap_free(pool);
}

// void *thread_function(void *arg)
// {
//     thread_pool_t *pool = (thread_pool_t *)arg;

//     while (1)
//     {
//         pthread_mutex_lock(&pool->lock);

//         while (pool->task_list == NULL && !pool->stop)
//             pthread_cond_wait(&pool->cond, &pool->lock);

//         if (pool->stop)
//         {
//             pthread_mutex_unlock(&pool->lock);
//             break;
//         }

//         task_t *task = pool->task_list;
//         pool->task_list = task->next;
//         pool->tasks_remaining--;
//         int all_tasks_done = pool->tasks_remaining == 0;
//         pthread_mutex_unlock(&pool->lock);

//         (*(task->function))(task->arg);
//         free(task);

//         if (all_tasks_done)
//         {
//             pthread_cond_signal(&pool->complete_cond); // Signal completion of all tasks
//         }
//     }

//     pthread_exit(NULL);
//     return NULL;
// }
