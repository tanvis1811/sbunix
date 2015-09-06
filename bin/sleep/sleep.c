#include <stdlib.h>
#include <stdio.h>
#include <syscall.h>
#include <string.h>
int main(int argc, char* argv[], char* envp[]) 
{
	char a[10];
	strcpy(a,argv[2]);
	int sleep = atoi(a);
	syscall_1(98,sleep);
	exit(0);
}
