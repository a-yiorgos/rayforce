#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "../core/storm.h"
#include "../core/format.h"
#include "../core/monad.h"

int main()
{
    int run = 1;
    while (run)
    {
        printf(">");

        char *line = NULL;
        size_t len = 0;
        size_t l = getline(&line, &len, stdin);
        UNUSED(l);

        g0 value = til(123);
        // g0 value = new_scalar_i64(9223372036854775807);
        str buffer;
        Result res = g0_fmt(&buffer, value);
        if (res == Ok)
        {
            printf("%s\n", buffer);
        }
        result_fmt(&buffer, res);
        printf("Result: %s\n", buffer);

        free(line);
        g0_free(value);
    }

    return 0;
}
