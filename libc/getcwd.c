#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>

char* getcwd(char *buf, size_t size)
{	   
        int rt;
        rt=syscall_2(SYS_getcwd,((uint64_t)(buf)),size);
	
	if(rt<0)
		printf("infunctio");
	//buf=NULL;
buf="Nautiyal";
        return buf;
}
