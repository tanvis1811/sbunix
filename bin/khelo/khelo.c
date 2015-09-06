#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXLINE 1000


int main(int argc, char* argv[], char* envp[])
{
	char hello[100];
	char a;
	scanf("%c",&a);
	printf("char:%c\n",a);
	scanf("%s\n",hello);
	printf("string:%s\n",hello);
	while(1);
}