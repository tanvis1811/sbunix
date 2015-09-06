#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>
//volatile int sss=1;
ssize_t write(int fd, const void *buf, size_t count)
{ ssize_t ret;
	//*((char*)0xffffffff80000000+0xb8000)=71;
   // *((char*)0xffffffff80000000+0xb8001)=0x07;
	//while(1);
	ret =syscall_3(((uint64_t)(SYS_write)),((uint64_t)(fd)),((uint64_t)(buf)),((uint64_t)(count)));
	return ret;
	
}