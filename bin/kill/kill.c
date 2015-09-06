#include <stdlib.h>
#include <stdio.h>
#include <syscall.h>
#include <string.h>

int main(int argc, char* argv[], char* envp[]) 
{
	char ip[10];
	if((strcmp(argv[2],"-9") != 0))
	{
		printf("No such command!\n");
		exit(0);
		return 1; 
	}
	strcpy(ip, argv[3]);
	int pid = atoi(ip);
	syscall_1(99,(uint64_t)pid);
	exit(0);
}
