#include "storm.h"
#include "format.h"
#include <stdio.h>
#include "alloc.h"

extern g0 new_scalar_i64(i64 value)
{
    g0 scalar;

    scalar = a0_alloc(sizeof(struct s0));
    scalar->type = -TYPE_I64;
    scalar->i64_value = value;
    return scalar;
}

extern g0 new_vector_i64(i64 *ptr, i64 len)
{
    g0 vector;

    vector = a0_alloc(sizeof(struct s0));
    vector->type = TYPE_I64;
    vector->list_value.ptr = ptr;
    vector->list_value.len = len;
    return vector;
}

extern void g0_free(g0 value)
{
    switch (value->type)
    {
    case TYPE_I64:
    {
        a0_free(value->list_value.ptr);
        break;
    }
    default:
    {
        printf("** Free: Invalid type\n");
        break;
    }
    }
    a0_free(value);
}
