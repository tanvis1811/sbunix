#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int strcmp(char *s, const char *t)
{
       int i;
       for (i = 0; s[i] == t[i]; i++)
        if (s[i] == '\0')
        return 0;
       return s[i] - t[i];
       }
