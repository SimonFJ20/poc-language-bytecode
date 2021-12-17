#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void fail(const char* msg)
{
    printf("RuntimeError: %s\n", msg);
    exit(1);
}

void assert(int condition, const char* msg)
{
    if (!condition)
        fail(msg);
}

