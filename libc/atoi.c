#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//#include<string.h>

int is_digit (char c)
{
    int n;

    if (c>='0' && c<='9')
        n=1;
    else
        n=0;
	return n;

}

    int is_space( char c)
{
    int n;
    if (c==32)
    n=1;
     else
        n=0;
     return n;
}




 int atoi(char s[])
 { int i, n, sign;

       for (i = 0; is_space(s[i]); i++)  /* skip white space */           ;

            sign = (s[i] == '-') ? -1 : 1;

        if (s[i] == '+' || s[i] == '-')  /* skip sign */

            i++;

       for (n = 0; is_digit(s[i]);i++)

            n = 10 * n + (s[i] - '0');

        return sign * n;

    }




