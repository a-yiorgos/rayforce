#include "storm.h"
#include <stdlib.h>

g0 til(i64 count)
{
    i64 *vec = (i64 *)malloc(count * sizeof(i64));
    for (i64 i = 0; i < count; i++)
    {
        vec[i] = i;
    }
    return new_vector_i64(vec, count);
}