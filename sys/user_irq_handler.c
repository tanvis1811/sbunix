#include <sys/defs.h>
#include <sys/utilities.h>
#include <sys/sbunix.h>
#include <sys/tarfs.h> 
//#include <sys/fileio.h>	
#include <sys/user_irq_handler.h>
#include <sys/task.h>
//extern struct files_list* flist_head;
#include <sys/memory.h>	
#include <sys/timer.h>
#define VADDR 0xFFFFFFFF


char pwd[200]="rootfs/";

uint64_t user_irq_handler(registers_t *regs)
{  
	int n = regs->rax;
	uint64_t ret,buf;
	int fd;
	int count;
	if(n == 0)
	{
		scheduling = 1;
		fd=regs->rdi;
		buf = regs->rsi;
		count = regs->rdx;
		if(fd==0)
		{
			kscanf((char*)buf,count);
			scheduling = 0;
			return (uint64_t)(strlen((char*)buf));
		}
		
	}

	if(n == 1)
	{
		buf=regs->rsi;
		count = regs->rdx;
		printf("%c",*(char*)buf);
		return (uint64_t)count;
	}
	else if(n==12)    //brk malloc
	{
		uint64_t ret=ksbrk((uint64_t)(regs->rdi));
		regs->rax=ret;
		return ret;
	}

	else if(n==15)    //opendir
	{
		struct files_list* dir=(struct files_list*)kopendir((char*)(regs->rdi));
		regs->rax=(uint64_t)(struct files_list*)dir;
		return (uint64_t)dir;
	}	



	else if(n==16)	  //readdir
	{
		uint64_t dent=kreaddir((uint64_t)(regs->rdi));
		regs->rax=dent;
		return dent;

	}
	if(n==79)//getcwd
	{   
		buf=regs->rdi;
		count=regs->rsi; 
		strcpy((char*)buf,pwd);
		//while(1);
		regs->rax=4;
		return 1;
	}
	
	if(n==80) //cchdir
	{  
		buf=regs->rdi;
		if(strcmp((char*)buf,"..")==0)
		{
			int len=strlen(pwd);
			int count=0;
			while(count<2)
			{ 
				if(pwd[len-1]=='/')
				count++;
				len--;
			}
			pwd[len+1]='\0'; 
			regs->rax=0;
			return 0;
		}	
		int k=strlen((char*)buf);
		if(*(char*)(buf+k-1)!='/')
		buf=(uint64_t)strcat((char*)buf,"/");
		
		if(pathlook((char*)buf))
		{	
			strcpy(pwd,(char*)buf);
			regs->rax=0;
			return 0;
		}
		char path[100];
		strcpy(path,pwd);
		buf=(uint64_t)strcat(path,(char*)buf);

		if(pathlook((char*)buf))
		{	
			strcpy(pwd,(char*)buf);
			regs->rax=0;
			return 0;
		}

		return 0;
	}
		
	/* Handler for getpid*/
	if (n == 39)
	{
		regs->rax = getpid();
	}

	/* Handler for getppid*/
	if (n == 110)
	{
		regs->rax = getppid();
	}

	/* Handler for fork*/
	if(n == 57)
	{
		 
		 int a = fork();
		regs->rax = a;
		return a;
		
	}
	/* Handler for execve*/
	if(n == 59)
	{
		uint64_t filename = regs->rdi;
		uint64_t argv = regs->rsi;
		uint64_t envp = regs->rdx;
		int a = execve((char *)filename,(char**)argv,(char**)envp);
		regs->rax = a;
		return a;
	}
	/* Handler for exit*/
	if(n == 60)
	{
		exit();
		return 1;	
	}
	/* Handler for waitpid*/
	if(n == 61)
	{
		waitpid();
		return 1;	
	}
	/* Handler for sleep*/
	if(n == 98)
	{
		uint64_t sleep = regs->rdi;
		struct task *temp = running;
		temp->task_state = WAITING;
		temp->sleep = 1;
		int pid = temp->ppid;
		temp->time_to_sleep = sleep;
		while(temp->next->pid != pid)
			temp = temp->next;
		temp->task_state = WAITING;
		temp->sleep = 1;
		temp->time_to_sleep = sleep;		
		scheduler();
	}
	/* Handler for kill*/
	if(n == 99)
	{
		uint64_t pid = regs->rdi;
		struct task *temp = running;
		while(temp->next->pid != pid)
		{	
			temp = temp->next;
		}
		temp->next = temp->next->next;
		scheduler();
		return 1; 
	}
	/* Handler for PS*/
	if(n == 100)
	{
		struct task *temp = running;
		printf("Process:%s  PID:%d\n",temp->name,temp->pid);
		while(temp->next != running)
		{	
			printf("Process:%s  PID:%d\n",temp->next->name,temp->next->pid);
			temp = temp->next;
		}
		return 1; 
	}

	ret=1;
	return (uint64_t)ret;
}
