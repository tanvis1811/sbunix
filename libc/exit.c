#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>

void exit(int status)
{
	syscall_1((long)(SYS_exit),((long)(status)));
	
}