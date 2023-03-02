#include "storm.h"
#include "alloc.h"

g0 til(i64 count)
{
    i64 *vec;

    vec = (i64 *)a0_alloc(count * sizeof(i64));
    for (i64 i = 0; i < count; i++)
    {
        vec[i] = i;
    }
    return new_vector_i64(vec, count);
}