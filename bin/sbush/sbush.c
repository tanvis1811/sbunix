#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXLINE 1000

char prompt[100];
int ps_flag =0;
int errno;
int a=0;	

struct path_name
{
        char name[100];
}pn[100];

struct pipe_cmd
{
	char cmd_name[100];	
};	

int check_tilda(char *ip)
{
       if(strstr(ip,"~/")!=NULL)
                return 1;
        return 0;	
}

int check_pipe(char *ip)
{
        if(strstr(ip,"|")!=NULL)
                return 1;
        return 0;
}
int count_pipe(char* input)
{
	int i=0,count=0;
	while(input[i]!='\0')
	{	
		if(input[i]=='|')
			count++;
		i++;
	}
	return count;
}
int check_cd(char *ip)
{
        if((strcmp(ip,"cd"))==0)
                return 1;
        return 0;
}

int check_num_path(char *ip)
{
        int n=1,i=0;
        for(i=0;i<strlen(ip);i++)
        {
                if(ip[i]=='/')
                        n++;
        }
        return n;
}
int check_shell(char path[100])
{
        if (strstr(path,".sh")!=NULL)               
		return 1;
        else return 0;
}
int check_ls( char *input)
{
        if (strstr(input,"ls")!=NULL)
	{
		if(check_pipe(input)==0) 
                	return 1;
	} 
       return 0;
}
int check_pwd(char path[100])
{
        if (strstr(path,"pwd")!=NULL)
                return 1;
        else return 0;
}
int check_ps_variable(char path[100])
{
        if((strstr(path,"PS1"))!=NULL)
                return 1;
        else return 0;
}
int check_path(char path[100])
{	
        if(strstr(path,"PATH")!=NULL)
                return 1;
        else return 0;
}
int check_bin(char path[100])
{	
        if (strstr(path,"/")!=NULL) 
        {        
		if((check_path(path)==1)||(check_pipe(path)==1))
		return 0;
		else return 1;
	}	
	return 0;
}


void for_1pipe(char *ip1,char *ip2,char *envp[])
{
	
	int pipefd[2];
	int pid1,pid2,stat;
	char *myargv[10];
	if(strstr(ip1," ")!=NULL)
	{	
		char *a1={""},*a2={""};

		char *tmp=strrchr(ip1,' ');
		if(tmp!=NULL)
		{
			*tmp='\0';
			a1=tmp+1;
		}
		a2=ip1;
		myargv[0]=a2;
		myargv[1]=a1;
		myargv[2]=NULL;
	}
	else
	{
		myargv[0]=ip1;
		myargv[1]=NULL;
	}
	

	char *argv2[10];
	if(strstr(ip2," ")!=NULL)
	{	
		char *a3={""},*a4={""};
		
		char *tmp=strrchr(ip2,' ');
		if(tmp!=NULL)
		{
			*tmp='\0';
			a3=tmp+1;
			
			
		}
		a4=ip2;
		argv2[0]=a4;
		argv2[1]=a3;
		argv2[2]=NULL;

	}
	else
	{
		argv2[0]=ip2;
		argv2[1]=NULL;
	
	}

	pid1=fork();
	if(pid1==0)
	{
		pipe(pipefd);

		pid2 = fork();

		if (pid2 == 0)
		{	
			close(1);
			dup2(pipefd[1],1);
			close(pipefd[0]);
			execve(myargv[0],myargv,envp);
			exit(0);
		}
		else
		{
			close(0);
			dup2(pipefd[0],0);
			close(pipefd[1]);
			execve(argv2[0], argv2,envp);
			exit(1);
		}
	}
	else
	{
		waitpid(pid1,&stat,0);
		return;
	}
}

void for_2pipe(char *ip1,char *ip2,char *ip3,char *envp[])
{
	int status;
	
	char *myargv1[10];
	if(strstr(ip1," ")!=NULL)
	{	
		char *a1={""},*a2={""};

		char *tmp=strrchr(ip1,' ');
		if(tmp!=NULL)
		{
			*tmp='\0';
			a1=tmp+1;
		}
		a2=ip1;
		myargv1[0]=a2;
		myargv1[1]=a1;
		myargv1[2]=NULL;
	}
	else
	{
		myargv1[0]=ip1;
		myargv1[1]=NULL;	
	}
	
	char *myargv2[10];
	if(strstr(ip2," ")!=NULL)
	{	
		char *a3={""},*a4={""};
		
		char *tmp=strrchr(ip2,' ');
		if(tmp!=NULL)
		{
			*tmp='\0';
			a3=tmp+1;
			
			
		}
		a4=ip2;
		myargv2[0]=a4;
		myargv2[1]=a3;
		myargv2[2]=NULL;
	}
	else
	{
		myargv2[0]=ip2;
		myargv2[1]=NULL;	
	}
	
	char *myargv3[10];
	if(strstr(ip3," ")!=NULL)
	{	
		char *a5={""},*a6={""};

		char *tmp=strrchr(ip3,' ');
		if(tmp!=NULL)
		{
			*tmp='\0';
			a5=tmp+1;
		}
		a6=ip3;
		myargv3[0]=a6;
		myargv3[1]=a5;
		myargv3[2]=NULL;
	}
	else
	{
		myargv3[0]=ip3;
		myargv3[1]=NULL;	
	}
	
	pid_t pid1,pid2,pid3=0,pid4=0;
	pid1=fork();
	if(pid1==0)
	{
		int pipes[4];
		int j=0;	
		pipe(pipes); 		
		pipe(pipes + 2); 	
		pid2=fork();
		if (pid2 == 0)
		{
			dup2(pipes[1], 1);
			for(j=0;j<=3;j++)
			close(pipes[j]);

			execve(myargv1[0], myargv1, envp);
		}
		else
		{
			pid3=fork();
			if (pid3 == 0)
			{

				dup2(pipes[0], 0);
				dup2(pipes[3], 1);

				for(j=0;j<=3;j++)
				close(pipes[j]);
				execve(myargv2[0], myargv2, envp);
			}
			else
			{
				pid4=fork();
				if (pid4 == 0)
				{
					dup2(pipes[2], 0);

					for(j=0;j<=3;j++)
					close(pipes[j]);
					execve(myargv3[0], myargv3, envp);
				}
			}
		}
		for(j=0;j<=3;j++)
		close(pipes[j]);

		
		waitpid(pid2,&status,0);
		waitpid(pid3,&status,0);
		waitpid(pid4,&status,0);		
		exit(0);
	}
	else
	{
		waitpid(pid1,&status,0);
		return;
	}	

}

void for_3pipe(char *ip1,char *ip2,char *ip3,char *ip4,char *envp[])
{
	int status;

	char *myargv1[10];
	if(strstr(ip1," ")!=NULL)
	{	
		char *a1={""},*a2={""};

		char *tmp=strrchr(ip1,' ');
		if(tmp!=NULL)
		{
			*tmp='\0';
			a1=tmp+1;
		}
		a2=ip1;
		myargv1[0]=a2;
		myargv1[1]=a1;
		myargv1[2]=NULL;
	}
	else
	{
		myargv1[0]=ip1;
		myargv1[1]=NULL;	
	}
	
	char *myargv2[10];
	if(strstr(ip2," ")!=NULL)
	{	
		char *a3={""},*a4={""};
		
		char *tmp=strrchr(ip2,' ');
		if(tmp!=NULL)
		{
			*tmp='\0';
			a3=tmp+1;
			
			
		}
		a4=ip2;
		myargv2[0]=a4;
		myargv2[1]=a3;
		myargv2[2]=NULL;
	}
	else
	{
		myargv2[0]=ip2;
		myargv2[1]=NULL;	
	}
	
	char *myargv3[10];
	if(strstr(ip3," ")!=NULL)
	{	
		char *a5={""},*a6={""};

		char *tmp=strrchr(ip3,' ');
		if(tmp!=NULL)
		{
			*tmp='\0';
			a5=tmp+1;
		}
		a6=ip3;
		myargv3[0]=a6;
		myargv3[1]=a5;
		myargv3[2]=NULL;
	}
	else
	{
		myargv3[0]=ip3;
		myargv3[1]=NULL;	
	}

	char *myargv4[10];
	if(strstr(ip4," ")!=NULL)
	{	
		char *a7={""},*a8={""};
		char *tmp = strrchr(ip4,' ');
                if (tmp != NULL)
                {
                        *tmp = '\0';
                        a7=tmp+1;
                }
		a8=ip4;
		myargv4[0]=a8;
		myargv4[1]=a7;
		myargv4[2]=NULL;
	}
	else
	{
		myargv4[0]=ip4;
		myargv4[1]=NULL;	
	}

	pid_t pid1,pid2,pid3=0,pid4=0,pid5=0;
	pid1=fork();
	if(pid1==0)
	{
		int pipes[6];
		int j=0;	
		pipe(pipes); 		
		pipe(pipes + 2); 
		pipe(pipes+4);	
		pid2=fork();
		if (pid2 == 0)
		{
			dup2(pipes[1], 1);
			for(j=0;j<=5;j++)
			close(pipes[j]);

			execve(myargv1[0], myargv1, envp);
		}
		else
		{
			pid3=fork();
			if (pid3 == 0)
			{

				dup2(pipes[0], 0);
				dup2(pipes[3], 1);

				for(j=0;j<=5;j++)
				close(pipes[j]);
				execve(myargv2[0], myargv2, envp);
			}
			else
			{
				pid4=fork();
				if (pid4 == 0)
				{
					dup2(pipes[2], 0);
					dup2(pipes[5],1);
					for(j=0;j<=5;j++)
					close(pipes[j]);
					execve(myargv3[0], myargv3, envp);
				}
				else
				{
					pid5=fork();
					if(pid5 == 0)
					{		
						dup2(pipes[4],0);
						for(j=0;j<=5;j++)
						close(pipes[j]);
						execve(myargv4[0], myargv4, envp);
					}
				}
			}
		}
		for(j=0;j<=5;j++)
		close(pipes[j]);

		
		waitpid(pid2,&status,0);
		waitpid(pid3,&status,0);
		waitpid(pid4,&status,0);
		waitpid(pid5,&status,0);		
		exit(0);
	}
	else
	{
		waitpid(pid1,&status,0);
		return;
	}	

}


void print_shell(int p_count)
{
        char current_dir[1024];
        printf("%s",prompt);
	if( (getcwd(current_dir, sizeof(current_dir))) != NULL)
	{
		printf("%s",current_dir);
		printf("/");
	}
}

int main(int argc, char* argv[], char* envp[]) 
{
	 strcpy(prompt,"<MYSHELL>");
        int p_count=0,stat;
        printf("welcometo%s:",prompt);
       // extern int errno;
        print_shell(p_count);
		
        while(1)
        {
		char cmd[10][MAXLINE];
		int j=0,k=0,cc=0;
                
		char input[MAXLINE] = {""};
     		
        	scanf("%l",input);
		
		while(input[cc]!='\0')
		{
			if(input[cc]==32)
			{
				cmd[k][j]='\0';
				j=0;
				k=k+1;
			}
			else
			{
				cmd[k][j]=input[cc];
				j=j+1;
			}
			cc++;
		}
		cmd[k][j]='\0';
		
		
/*********************************************************Handling cd *******************************************************/
                if(check_cd(cmd[0]))             //cd_function
                {	
			char new_ip[100]={""};
			strcpy(new_ip,cmd[0]);
			
			if((strcmp(new_ip,"cd")==0) && (k==0))
			{	
				int i;
				for( i=0;envp[i]!='\0';i++)
		                {
		                        if((envp[i][0]=='H')&&(envp[i][1]=='O')&&(envp[i][2]=='M')&&(envp[i][3]=='E'))
		                        {
		                                break;
		                        }
		                }				
				char ip3[MAXLINE],ip4[MAXLINE],temp[MAXLINE];
				strcpy(temp,envp[i]);
		                char *tmp = strrchr(temp,'=');
		                if (tmp != NULL)
		                {
		                        *tmp = '\0';
		                        strcpy(ip3, temp);
		                        strcpy(ip4, tmp+1);
		                }	
			
				strcpy(pn[p_count].name,ip4);
				if((chdir(pn[p_count].name))!=0)
                                {
                                        printf("Error in changing directory\n");

                                }
                                print_shell(p_count);
				continue;
			}
			if(strcmp(cmd[1],"~")==0)
			{	
				int i;
				for( i=0;envp[i]!='\0';i++)
		                {
		                        if((envp[i][0]=='H')&&(envp[i][1]=='O')&&(envp[i][2]=='M')&&(envp[i][3]=='E'))
		                        {
		                                break;
		                        }
		                }				
				char ip3[MAXLINE],ip4[MAXLINE],temp[MAXLINE];
				strcpy(temp,envp[i]);
		                char *tmp = strrchr(temp,'=');
		                if (tmp != NULL)
		                {
		                        *tmp = '\0';
		                        strcpy(ip3, temp);
		                        strcpy(ip4, tmp+1);
		                }	
				
				strcpy(pn[p_count].name,ip4);
				if((chdir(pn[p_count].name))!=0)
                                {
                                        printf("Error in changing directory\n");

                                }
                                print_shell(p_count);
				continue;				
			}

		        if(check_tilda(cmd[1]))
			{	
				int i;
				for( i=0;envp[i]!='\0';i++)
		                {
		                        if((envp[i][0]=='H')&&(envp[i][1]=='O')&&(envp[i][2]=='M')&&(envp[i][3]=='E'))
		                        {
		                                break;
		                        }
		                }				
				char ip3[MAXLINE],ip4[MAXLINE],temp[MAXLINE],ip5[MAXLINE];
				strcpy(temp,envp[i]);
		                char *tmp = strrchr(temp,'=');
		                if (tmp != NULL)
		                {
		                        *tmp = '\0';
		                        strcpy(ip3, temp);
		                        strcpy(ip4, tmp+1);
		                }	
				char *tmp1 = strrchr(cmd[1],'~');
		                if (tmp1 != NULL)
		                {
		                        *tmp1 = '\0';
		                        strcpy(ip5, tmp1+1);
		                }	
				
				strcat(ip4,ip5);
				if((chdir(ip4))!=0)
			        {
			                printf("Error in changing directory\n");
			        }
			}

			else if((chdir(cmd[1]))!=0)
                        {	
                                printf("Error in changing directory1\n");
                        }
 			print_shell(p_count);
			continue;                 
                }
/*********************************************************Handling scripts ***************************************************/
                else if (check_shell(cmd[0]))
                {	
			char new_ip[100]={""};
			strcpy(new_ip,cmd[0]);
			char *argv[]={NULL};
			pid_t pid;
			pid=fork();

                        if(pid<0)
                        {       printf("Error forking process in scripts\n");

                        }
                        else if(pid==0)
                        {
                                if((execve(new_ip,argv,envp))<0)
                                {	
                                        printf("Error in exec for scripts\n");
					exit(0);
                                }
                        }
                        else    {
					waitpid(pid,&stat,0);
                                        print_shell(p_count);
                                }
			continue;
                }
/*********************************************************Handling binary files***************************************************/
                else if( check_bin(cmd[0]))
                {	
			char new_ip[100]={""};
	
                        char *argv[]={cmd[0],NULL};
			strcpy(new_ip,cmd[0]);
				
			pid_t pid;
                        pid=fork();
                        if(pid<0)
                        {       printf("Error forking process in binary\n");

                        }
                         if(pid==0)
                        {
                                if((execve(new_ip,argv,envp))<0)
                                {
                                        printf("Error in exec in binary\n");
					exit(0);
                                }
                        }
			else	{
					waitpid(pid,&stat,0);
                                        print_shell(p_count);
				}
			continue;
          	}      
	
/********************************************************Handling PS1 variables******************************************************/
              else if(check_ps_variable(cmd[0]))
                {     
                        char ip1[20],ip2[20];
                        char *tmp = strrchr(cmd[0],'=');
                        if (tmp != NULL)
                        {
                                *tmp = '\0';
                                strcpy(ip1, cmd[0]);
                                strcpy(ip2, tmp+1);
                        }
                        
                        ps_flag=1;
                        strcpy(prompt,ip2);
                        print_shell(p_count);
			continue;
                }
/**************************************************************Handling PATH*******************************************************/
                else if(check_path(cmd[0]))
                {	
		
                        int i=0;
                        char ip1[200]={""},ip2[200]={""},path[200]={""};
                        char *tmp = strrchr(input,'=');
                        if (tmp != NULL)
                        {
                                *tmp = '\0';
                                strcpy(ip1, input);
                                strcpy(ip2, tmp+1);
                        }
                        for( i=0;envp[i]!='\0';i++)
                        {
                                if((envp[i][0]=='P')&&(envp[i][1]=='A')&&(envp[i][2]=='T')&&(envp[i][3]=='H'))
                                {
                                        strcpy(path,envp[i]);
                                }

                        }
                        if(check_path(ip2))
 			{
                                char ip3[20],ip4[20];
                                char *tmp = strrchr(ip2,'H');
                                if (tmp != NULL)
                                {
                                        *tmp = '\0';
                                        strcpy(ip3, input);
                                        strcpy(ip4, tmp+1);
                                        
                                }
                                strcat(path,ip4);
                                printf("%s\n",path);
                        }
                        else
                        {
                                strcpy(path,ip2);
                                printf("%s\n",path);
                        }
                        print_shell(p_count);
			continue;
                }
/********************************************************EXIT**************************************/
		else if((strcmp(cmd[0],"exit")==0))
			{
				exit(0);
			}		
	
/************************************************************Handling pipes*********************************************************/
		 if(check_pipe(cmd[0])||check_pipe(cmd[1]))
		{	
			int c=1;
			while(strlen(cmd[c])!=0)
			{
				strcat(cmd[0]," ");			
				strcat(cmd[0],cmd[c]);
				c++;
			}
			
			int a=1,count,i=0,c_pipe,j=0;
			char ip[5][100]={""},op[5][100]={""};
			c_pipe=count_pipe(input);
			count=c_pipe+1;
			
			while(count)
			{
				char *tmp = strrchr(cmd[0],'|');
				
		                if (tmp != NULL)
		                {	
		                        *tmp = '\0';
					strcpy(ip[a], tmp+1);
					
					a++;
		                }
				count=count-1;
			}
			j=0;
			
			strcpy(op[0],cmd[0]);
			
			
			j=0;i=1;

			for(j=a-1;j>0;j--)
			{
				strcpy(op[i],ip[j]);
			
				i++;
			}
			if(c_pipe==1)
			{
				for_1pipe(op[0],op[1],envp);
			}	

			if(c_pipe==2)
			{
				for_2pipe(op[0],op[1],op[2],envp);
			}
			
			if(c_pipe==3)
			{
				for_3pipe(op[0],op[1],op[2],op[3],envp);
			}
			print_shell(p_count);
			continue;
			
			
			
		}
	
/*********************************************************Handling binary commmands******************************************************/
               else 
                {	
			pid_t pid;
			char new_ip[100]={""};	
			strcpy(new_ip,cmd[0]);

                        int i=0,a=0,c=0;
                        char my_path[100]={""},my_path1[MAXLINE]={""},my_path2[MAXLINE]={""};

                        for( i=0;envp[i]!='\0';i++)
                        {
                                if((envp[i][0]=='P')&&(envp[i][1]=='A')&&(envp[i][2]=='T')&&(envp[i][3]=='H'))
                                {	
                                        break;
                                }
                        }
		
                        strcpy(my_path1,envp[i]);
                        strcpy(my_path2,my_path1);
                        char seq[100][100];
		
                        while(my_path1[a]!='\0')
                        {
                                char *tmp;
                                tmp =(char*) strrchr(my_path2, ':');
                                if (tmp != NULL)
                                {
                                        *tmp = '\0';
                                        strcpy(seq[c], tmp+1);
                                        c=c+1;
                                }
                                a++;
                        }
			
                        for(a=c-1;a>=0;a--)
                        {	
                                strcat(seq[a],"/");				
                                strcat(seq[a],new_ip);
				
                                if((open(seq[a],O_RDONLY))>0)
                                {
                                        strcpy(my_path,seq[a]);
					break;
                                }
                        }
			
		
                        char *argv[]={NULL};
			if(strlen(cmd[1])==0)
			{
				argv[0]=new_ip;
				argv[1]=NULL;
			}
			else if (strlen(cmd[2])==0)
			{
				argv[0]=new_ip;
				argv[1]=cmd[1];
				argv[2]=NULL;
			}
			else if (strlen(cmd[3])==0)
			{
				argv[0]=new_ip;
				argv[1]=cmd[1];
				argv[2]=cmd[2];
				argv[3]=NULL;
			}
			int s = 0;
			if (strcmp(cmd[1],"&") == 0)	
			{	
				s = 1;
			}
		
			if(s)
			{
				print_shell(p_count);
				printf("\n");
			}				
                        pid=fork();
			
                        if(pid<0)
                        {
                                printf("Error in forking process for binary commands\n");
                        }

			
			
                        if(pid==0)
                        {
                                if((execve(my_path,argv,envp))<0)
                                {
					printf("Error in execve for binary commannds\n");
					exit(0);
                                }
                        }
                        else    {       
					
					waitpid(pid,&stat,0);
					if(!s)
                                        {	
						print_shell(p_count);
						
					}
                                }
			continue;
                }

        }
                                //fork
                               //exec
}
