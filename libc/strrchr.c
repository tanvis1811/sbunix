#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//#include <string.h>


int my_strlen(const char *str)
{  int size =0;
   while(*str++!='\0')
   size++;
  // printf("%d",size);
   return size;

}

char *strrchr( char s[],int c)
{
 char *ptr = NULL;
 int length ;
 length=my_strlen(s);
 s=s+length;

 while(*s--!=c &&length >-1)
 length--;
                if (length>=0)
                    ptr=++s;
        return ptr;
}


