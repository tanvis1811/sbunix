#include <stdlib.h>
#include<stdio.h>
#include <string.h>
void _start(void) 
{  
	int argc;
	int z;
	char** argv={NULL};
	char* envp[]={"HOME=rootfs/bin/","PATH=/rootfs:bin:/usr/games:/usr/local/games",0};
	
	uint64_t *value;
	
	__asm__ volatile( "movq %%rsp,%0;"

			:"=rsp"(value)
			:
			:"cc","memory"

			);
	while(*value!=9999)
	{
		value++;
	}  
	 z=* ((uint64_t *)value+1 );
	argv=(char**)(value+2);
      
	argc=z;

	

	int res;
	res = main(argc, argv, envp);
	exit(res);
}
