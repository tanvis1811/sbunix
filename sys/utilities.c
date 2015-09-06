#include <sys/utilities.h>

void memset(void *dest, uint16_t val, uint32_t len)
{
	uint16_t *temp = (uint16_t *)dest;
	for ( ; len != 0; len--) *temp++ = val;
}

void outb(uint16_t port, char value)
{
	__asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

unsigned char inb(uint16_t port)
{
	unsigned char value;
	__asm__ __volatile__("inb %1, %0" : "=a"(value) : "dN"(port));
	return value;
}

void *memcpy(void *dst,void *src, uint64_t count)
{
	char *dest= dst;
	char *source= src;
	while(count--)
	{
		*dest=*source;
		dest++;
		source++;
	}
	return 0;
}

int strlen(const char *str)
{  int size =0;
   while(*str++!='\0')
   size++;
  // printf("%d",size);
   return size;

}

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
