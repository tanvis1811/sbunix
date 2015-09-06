#include <stdlib.h>
#include <stdio.h>
#include <syscall.h>
int main(int argc, char* argv[], char* envp[]) 
{
	
	syscall_0(100);
	exit(0);
}
