#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


ssize_t read(int fd,  void *buf, size_t count)
{       
         ssize_t ret;
        ret =syscall_3(((uint64_t)(SYS_read)),((uint64_t)(fd)),((uint64_t)(buf)),((uint64_t)(count)));
		//printf("%s",(char*)buf);
        return ret;

}