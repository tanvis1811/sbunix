#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//#include <string.h>


int strlen(const char *str)
{  int size =0;
   while(*str++!='\0')
   size++;
  // printf("%d",size);
   return size;

}

