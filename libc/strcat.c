#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

char *strcat(char *s, const char *t)
{   char *rst=s;
        int i, j;
       i = j = 0;
        while (s[i] != '\0') /* find end of s */
            i++;
        while ((s[i++] = t[j++]) != '\0') /* copy t */
        ;
        return(rst);

}