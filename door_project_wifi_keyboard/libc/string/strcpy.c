/*
 * strcpy.c
 */

#include <string.h>

char *strcpy(char *dst0, const char *src0)
{
    char *s = dst0;

    while ((*dst0++ = *src0++) != '\0')
        ;
    return s;
}

