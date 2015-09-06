
#include<stdio.h>
//#include<sys/utilities.h>
//#include<sys/defs.h>
#include<stdlib.h>
#include <string.h>
#include <syscall.h>
//char *a;
//int b;

int main(int argc, char* argv[], char* envp[]) 
{

	//getcwd
	char buf[100];
	uint64_t size=8;
	uint64_t ret =(uint64_t)syscall_2(((uint64_t)(79)),((uint64_t)(buf)),((uint64_t)(size)));
	//char* name=(char*)buf;
	
	ret--;	
	struct dirent* dp;
	struct file_list* dirp;
	dirp = (struct file_list*)opendir(buf);

	while (dirp) 
	{
   
		if ((dp = readdir(dirp)) != NULL) 
		{
			printf("%s    ",dp->d_name);
			printf("\n");
		} 
		else
		{
			break;
		}


	}
	exit(0);
}
