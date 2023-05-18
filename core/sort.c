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

#include "sort.h"
#include "vector.h"

null_t swap(i64_t *a, i64_t *b)
{
    i64_t t = *a;
    *a = *b;
    *b = t;
}

i64_t partition(i64_t array[], i64_t low, i64_t high)
{
    i64_t pivot = array[high];
    i64_t i = (low - 1);

    for (i64_t j = low; j <= high - 1; j++)
    {
        if (array[j] > pivot)
        {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return (i + 1);
}

null_t quick_sort(i64_t array[], i64_t low, i64_t high)
{
    if (low < high)
    {
        i64_t pi = partition(array, low, high);

        quick_sort(array, low, pi - 1);
        quick_sort(array, pi + 1, high);
    }
}

null_t heapify(i64_t arr[], i64_t n, i64_t i)
{
    i64_t largest = i;
    i64_t left = 2 * i + 1;
    i64_t right = 2 * i + 2;

    if (left < n && arr[left] < arr[largest])
        largest = left;

    if (right < n && arr[right] < arr[largest])
        largest = right;

    if (largest != i)
    {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

null_t heap_sort(i64_t arr[], i64_t n)
{
    for (i64_t i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (i64_t i = n - 1; i >= 0; i--)
    {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

null_t insertion_sort(i64_t arr[], i64_t left, i64_t right)
{
    for (i64_t i = left + 1; i <= right; i++)
    {
        i64_t temp = arr[i];
        i64_t j = i - 1;
        while (j >= left && arr[j] < temp)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

i64_t count_out_of_order(i64_t arr[], i64_t n)
{
    i64_t i, out_of_order_count = 0;

    for (i = 0; i < n - 1; i++)
        if (arr[i] > arr[i + 1])
            out_of_order_count++;

    return out_of_order_count;
}

null_t rf_sort_asc(rf_object_t *vec)
{
    i64_t i, len = vec->adt->len, out_of_order;
    i64_t *v = as_vector_i64(vec);

    if (vec->adt->attrs & VEC_ATTR_ASC)
        return;

    if (vec->adt->attrs & VEC_ATTR_DESC)
    {
        for (i = 0; i < len / 2; i++)
            swap(&v[i], &v[len - i - 1]);
        return;
    }

    if (len < 100)
    {
        insertion_sort(v, 0, len - 1);
        return;
    }

    out_of_order = count_out_of_order(v, len);

    // ascending order
    if (out_of_order == 0)
    {
        vec->adt->attrs |= VEC_ATTR_ASC;
        return;
    }

    // descending order
    if (out_of_order == len - 1)
    {
        vec->adt->attrs |= VEC_ATTR_DESC;
        for (i = 0; i < len / 2; i++)
            swap(&v[i], &v[len - i - 1]);
        return;
    }

    // partially ordered
    if (out_of_order > len / 2)
    {
        quick_sort(v, 0, len - 1);
        return;
    }

    heap_sort(v, len);
}

null_t rf_sort_desc(rf_object_t *vec)
{
    i64_t len = vec->adt->len;
    i64_t *v = as_vector_i64(vec);

    if (vec->adt->attrs & VEC_ATTR_DESC)
        return;

    if (vec->adt->attrs & VEC_ATTR_ASC)
    {
        for (i64_t i = 0; i < len / 2; i++)
            swap(&v[i], &v[len - i - 1]);
        return;
    }

    if (len < 100)
    {
        insertion_sort(v, 0, len - 1);
        return;
    }

    quick_sort(v, 0, len - 1);
    // quick_sort(v, 0, len - 1);
    // heap_sort(v, len);
    // tim_sort(v, len);
}
