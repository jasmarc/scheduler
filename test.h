#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                               if (message) return message; } while (0)
extern int tests_run;

typedef struct {
    int arrive;
} thing;