#ifndef _STRING_H
#define _STRING_H
#include <stdlib.h>
#ifndef	NULL
#define	NULL	((void *)0)
#endif


//string functions
char* strcpy(char*d,const char*s);
size_t strlen(const char*s);
char *strcat(char *dest, const char *src);
//void *memset(void *str, int c, size_t n);
//void *memcpy(void *dest, const void *src, size_t n);
char *strcat(char *dest, const char *src);
int strcmp(const char *str1, const char *str2);
char *strstr( char *haystack,  char *needle);
char *strrchr(char *string, int c); 
int atoi(char s[]);
int is_space( char c);
int is_digit (char c);
#endif
