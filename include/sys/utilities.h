#ifndef _UTILITIES_H
#define _UTILITIES_H
#include <sys/defs.h>

void memset(void *dest, uint16_t val, uint32_t len);
void *memcpy(void *dest, void *src, uint64_t count);

void outb(uint16_t port, char value);
unsigned char inb(uint16_t port);
int kscanf(char *add, int limiter);
int strlen(const char *str);
char *strcat(char *s, const char *t);

struct registers
{ 	//uint64_t ds; 
	//uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax; 
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rbp, rdi, rsi, rdx, rcx, rbx, rax;
	
	uint64_t intNo, errCode; 
	uint64_t rip, cs, rflags, rsp, ss; 
};
typedef struct registers registers_t;
#endif
