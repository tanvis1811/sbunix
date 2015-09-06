#include <stdio.h>
#include <string.h>

char *strstr(  char *a,   char *b)
{
    int i, j, k, count=0;
    if(strlen(a)<strlen(b))
        return NULL;
    for(i=0;i<(strlen(a)-strlen(b)+1);i++)
    {
        count=0;
        k=i;
        for(j=0;j<strlen(b);j++)
        {
            if(a[k++]==b[j])
                count++;
        }
        if(count==strlen(b))
            {
                return b;
            }
    }
    return NULL;
}

