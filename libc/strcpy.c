#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

char *strcpy(char *dest, const char *src)
{
char *point= dest;

//return point;

while ((*dest++ = *src++) != '\0')          
	;
return point;



}
