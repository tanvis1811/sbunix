#include <stdlib.h>
#include <stdio.h>
#include <syscall.h>
#include <string.h>
int main(int argc, char* argv[], char* envp[]) 
{
	if (argc == 3)
	printf("%s\n",argv[2]);
	
	if (argc == 4)
	printf("%s %s\n",argv[2],argv[3]);

	if (argc == 5)
	printf("%s %s %s\n",argv[2],argv[3],argv[4]);
	exit(0);
}
