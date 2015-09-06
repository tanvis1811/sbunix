
rootfs/bin/bolo:     file format elf64-x86-64


Disassembly of section .text:

00000000004000b0 <_start>:
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[]);

void _start(void) {
  4000b0:	48 83 ec 08          	sub    $0x8,%rsp
	int argc = 1;
	char* argv[0];
	char* envp[0];
	int res;
	res = main(argc, argv, envp);
  4000b4:	48 89 e2             	mov    %rsp,%rdx
  4000b7:	48 89 e6             	mov    %rsp,%rsi
  4000ba:	bf 01 00 00 00       	mov    $0x1,%edi
  4000bf:	e8 0c 00 00 00       	callq  4000d0 <main>
	exit(res);
  4000c4:	89 c7                	mov    %eax,%edi
  4000c6:	e8 31 00 00 00       	callq  4000fc <exit>
}
  4000cb:	48 83 c4 08          	add    $0x8,%rsp
  4000cf:	c3                   	retq   

00000000004000d0 <main>:
#include <stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[], char* envp[]) {
  4000d0:	48 83 ec 08          	sub    $0x8,%rsp
	printf("%d",5);
  4000d4:	be 05 00 00 00       	mov    $0x5,%esi
  4000d9:	48 8d 3d 74 04 00 00 	lea    0x474(%rip),%rdi        # 400554 <write+0x1b>
  4000e0:	b8 00 00 00 00       	mov    $0x0,%eax
  4000e5:	e8 4d 02 00 00       	callq  400337 <printf>
	//while(2);
	*((char*)0xffffffff80000000+0xb8000)=67;
  4000ea:	c6 04 25 00 80 0b 80 	movb   $0x43,0xffffffff800b8000
  4000f1:	43 
    *((char*)0xffffffff80000000+0xb8001)=0x07;
  4000f2:	c6 04 25 01 80 0b 80 	movb   $0x7,0xffffffff800b8001
  4000f9:	07 
  4000fa:	eb fe                	jmp    4000fa <main+0x2a>

00000000004000fc <exit>:
#include <stdarg.h>
#include <syscall.h>

void exit(int status)
{
	syscall_1((long)(SYS_exit),((long)(status)));
  4000fc:	48 63 ff             	movslq %edi,%rdi
}

static __inline uint64_t syscall_1(uint64_t n, uint64_t a1) {
		 uint64_t ret;
          
	__asm__ volatile("syscall;"
  4000ff:	b8 3c 00 00 00       	mov    $0x3c,%eax
  400104:	0f 05                	syscall 
  400106:	c3                   	retq   
  400107:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
  40010e:	00 00 

0000000000400110 <print_s>:
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#define MAXLINE 100
int print_s(char *input)
{
  400110:	55                   	push   %rbp
  400111:	53                   	push   %rbx
  400112:	48 83 ec 08          	sub    $0x8,%rsp
  400116:	48 89 fd             	mov    %rdi,%rbp
	
	int ps=0;
	while(*input)
  400119:	80 3f 00             	cmpb   $0x0,(%rdi)
  40011c:	74 23                	je     400141 <print_s+0x31>
  40011e:	48 89 fb             	mov    %rdi,%rbx
	{
		write(1,input,1);
  400121:	ba 01 00 00 00       	mov    $0x1,%edx
  400126:	48 89 de             	mov    %rbx,%rsi
  400129:	bf 01 00 00 00       	mov    $0x1,%edi
  40012e:	e8 06 04 00 00       	callq  400539 <write>
		input++;
  400133:	48 ff c3             	inc    %rbx
  400136:	89 d8                	mov    %ebx,%eax
  400138:	29 e8                	sub    %ebp,%eax
#define MAXLINE 100
int print_s(char *input)
{
	
	int ps=0;
	while(*input)
  40013a:	80 3b 00             	cmpb   $0x0,(%rbx)
  40013d:	75 e2                	jne    400121 <print_s+0x11>
  40013f:	eb 05                	jmp    400146 <print_s+0x36>
#include <stdarg.h>
#define MAXLINE 100
int print_s(char *input)
{
	
	int ps=0;
  400141:	b8 00 00 00 00       	mov    $0x0,%eax
		write(1,input,1);
		input++;
		ps++;
	}
	return ps;
}
  400146:	48 83 c4 08          	add    $0x8,%rsp
  40014a:	5b                   	pop    %rbx
  40014b:	5d                   	pop    %rbp
  40014c:	c3                   	retq   

000000000040014d <print_d>:

void print_d(int result,int *printed)
{
  40014d:	53                   	push   %rbx
  40014e:	48 83 ec 30          	sub    $0x30,%rsp
	char ip[20],op[20];
	int a,i=0,x,b=0;
	while(result>=10)
  400152:	83 ff 09             	cmp    $0x9,%edi
  400155:	0f 8e 87 00 00 00    	jle    4001e2 <print_d+0x95>
  40015b:	48 8d 74 24 1c       	lea    0x1c(%rsp),%rsi
}

void print_d(int result,int *printed)
{
	char ip[20],op[20];
	int a,i=0,x,b=0;
  400160:	b9 00 00 00 00       	mov    $0x0,%ecx
	while(result>=10)
	{
		a=result%10;
  400165:	41 b8 67 66 66 66    	mov    $0x66666667,%r8d
  40016b:	89 f8                	mov    %edi,%eax
  40016d:	41 f7 e8             	imul   %r8d
  400170:	c1 fa 02             	sar    $0x2,%edx
  400173:	89 f8                	mov    %edi,%eax
  400175:	c1 f8 1f             	sar    $0x1f,%eax
  400178:	29 c2                	sub    %eax,%edx
  40017a:	8d 04 d5 00 00 00 00 	lea    0x0(,%rdx,8),%eax
  400181:	8d 04 50             	lea    (%rax,%rdx,2),%eax
  400184:	29 c7                	sub    %eax,%edi
		ip[i]=(char)(((int)'0')+a);
  400186:	83 c7 30             	add    $0x30,%edi
  400189:	40 88 3e             	mov    %dil,(%rsi)
		i++;
  40018c:	ff c1                	inc    %ecx
		result=result/10;
  40018e:	89 d7                	mov    %edx,%edi
  400190:	48 ff c6             	inc    %rsi

void print_d(int result,int *printed)
{
	char ip[20],op[20];
	int a,i=0,x,b=0;
	while(result>=10)
  400193:	83 fa 09             	cmp    $0x9,%edx
  400196:	7f d3                	jg     40016b <print_d+0x1e>
		a=result%10;
		ip[i]=(char)(((int)'0')+a);
		i++;
		result=result/10;
	}
	ip[i]=(char)(((int)'0')+result);
  400198:	48 63 c1             	movslq %ecx,%rax
  40019b:	83 c7 30             	add    $0x30,%edi
  40019e:	40 88 7c 04 1c       	mov    %dil,0x1c(%rsp,%rax,1)
	for(x=i;x>=0;x--)
  4001a3:	85 c9                	test   %ecx,%ecx
  4001a5:	79 48                	jns    4001ef <print_d+0xa2>
  4001a7:	eb 11                	jmp    4001ba <print_d+0x6d>
	{
		op[b]=ip[x];
  4001a9:	48 63 d1             	movslq %ecx,%rdx
  4001ac:	0f b6 54 14 1c       	movzbl 0x1c(%rsp,%rdx,1),%edx
  4001b1:	88 10                	mov    %dl,(%rax)
  4001b3:	48 ff c0             	inc    %rax
		ip[i]=(char)(((int)'0')+a);
		i++;
		result=result/10;
	}
	ip[i]=(char)(((int)'0')+result);
	for(x=i;x>=0;x--)
  4001b6:	ff c9                	dec    %ecx
  4001b8:	79 ef                	jns    4001a9 <print_d+0x5c>
	{
		op[b]=ip[x];
		b++;
	}
	char *fin=op;
	while(*fin)
  4001ba:	80 7c 24 08 00       	cmpb   $0x0,0x8(%rsp)
  4001bf:	74 35                	je     4001f6 <print_d+0xa9>
  4001c1:	48 8d 5c 24 08       	lea    0x8(%rsp),%rbx
	{
		write(1,fin,1);
  4001c6:	ba 01 00 00 00       	mov    $0x1,%edx
  4001cb:	48 89 de             	mov    %rbx,%rsi
  4001ce:	bf 01 00 00 00       	mov    $0x1,%edi
  4001d3:	e8 61 03 00 00       	callq  400539 <write>
		fin++;
  4001d8:	48 ff c3             	inc    %rbx
	{
		op[b]=ip[x];
		b++;
	}
	char *fin=op;
	while(*fin)
  4001db:	80 3b 00             	cmpb   $0x0,(%rbx)
  4001de:	75 e6                	jne    4001c6 <print_d+0x79>
  4001e0:	eb 14                	jmp    4001f6 <print_d+0xa9>
		a=result%10;
		ip[i]=(char)(((int)'0')+a);
		i++;
		result=result/10;
	}
	ip[i]=(char)(((int)'0')+result);
  4001e2:	83 c7 30             	add    $0x30,%edi
  4001e5:	40 88 7c 24 1c       	mov    %dil,0x1c(%rsp)
}

void print_d(int result,int *printed)
{
	char ip[20],op[20];
	int a,i=0,x,b=0;
  4001ea:	b9 00 00 00 00       	mov    $0x0,%ecx
  4001ef:	48 8d 44 24 08       	lea    0x8(%rsp),%rax
  4001f4:	eb b3                	jmp    4001a9 <print_d+0x5c>
	{
		write(1,fin,1);
		fin++;
		printed++;
	}
}
  4001f6:	48 83 c4 30          	add    $0x30,%rsp
  4001fa:	5b                   	pop    %rbx
  4001fb:	c3                   	retq   

00000000004001fc <get_hex_value>:
char get_hex_value(int a)
{
	char value='0';
	if(a<10)
	{
		value=(char)(((int)'0')+a);
  4001fc:	8d 57 30             	lea    0x30(%rdi),%edx
  4001ff:	83 ff 09             	cmp    $0x9,%edi
  400202:	b8 30 00 00 00       	mov    $0x30,%eax
  400207:	0f 4e c2             	cmovle %edx,%eax
	}
	if(a==10)
  40020a:	83 ff 0a             	cmp    $0xa,%edi
  40020d:	74 20                	je     40022f <get_hex_value+0x33>
	{
		value='a';
	}
	else if(a==11)
  40020f:	83 ff 0b             	cmp    $0xb,%edi
  400212:	74 21                	je     400235 <get_hex_value+0x39>
	{
		value='b';
	}
	else if(a==12)
  400214:	83 ff 0c             	cmp    $0xc,%edi
  400217:	74 22                	je     40023b <get_hex_value+0x3f>
	{
		value='c';
	}
	else if(a==13)
  400219:	83 ff 0d             	cmp    $0xd,%edi
  40021c:	74 23                	je     400241 <get_hex_value+0x45>
	{
		value='d';
	}
	else if(a==14)
  40021e:	83 ff 0e             	cmp    $0xe,%edi
  400221:	74 24                	je     400247 <get_hex_value+0x4b>
	{
		value='e';
	}
	else if(a==15)
  400223:	83 ff 0f             	cmp    $0xf,%edi
	{
		value='f';
  400226:	ba 66 00 00 00       	mov    $0x66,%edx
  40022b:	0f 44 c2             	cmove  %edx,%eax
  40022e:	c3                   	retq   
	{
		value=(char)(((int)'0')+a);
	}
	if(a==10)
	{
		value='a';
  40022f:	b8 61 00 00 00       	mov    $0x61,%eax
  400234:	c3                   	retq   
	}
	else if(a==11)
	{
		value='b';
  400235:	b8 62 00 00 00       	mov    $0x62,%eax
  40023a:	c3                   	retq   
	}
	else if(a==12)
	{
		value='c';
  40023b:	b8 63 00 00 00       	mov    $0x63,%eax
  400240:	c3                   	retq   
	}
	else if(a==13)
	{
		value='d';
  400241:	b8 64 00 00 00       	mov    $0x64,%eax
  400246:	c3                   	retq   
	}
	else if(a==14)
	{
		value='e';
  400247:	b8 65 00 00 00       	mov    $0x65,%eax
	else if(a==15)
	{
		value='f';
	}
	return value;
}
  40024c:	c3                   	retq   

000000000040024d <print_x>:

void print_x(int result,int *printed)
{
  40024d:	41 54                	push   %r12
  40024f:	55                   	push   %rbp
  400250:	53                   	push   %rbx
  400251:	48 83 ec 30          	sub    $0x30,%rsp
  400255:	89 fb                	mov    %edi,%ebx
	char ip[20],op[20];
	int a,i=0,x,b=2;
	if(result<16)
  400257:	83 ff 0f             	cmp    $0xf,%edi
  40025a:	7f 1a                	jg     400276 <print_x+0x29>
	{
		op[2]='0';
  40025c:	c6 44 24 0a 30       	movb   $0x30,0xa(%rsp)
		a=result%16;
		ip[i]=get_hex_value(a);
		i++;
		result=result/16;
	}
	ip[i]=get_hex_value(result);
  400261:	e8 96 ff ff ff       	callq  4001fc <get_hex_value>
  400266:	88 44 24 1c          	mov    %al,0x1c(%rsp)
	char ip[20],op[20];
	int a,i=0,x,b=2;
	if(result<16)
	{
		op[2]='0';
		b++;
  40026a:	b8 03 00 00 00       	mov    $0x3,%eax
		a=result%16;
		ip[i]=get_hex_value(a);
		i++;
		result=result/16;
	}
	ip[i]=get_hex_value(result);
  40026f:	bd 00 00 00 00       	mov    $0x0,%ebp
  400274:	eb 51                	jmp    4002c7 <print_x+0x7a>

void print_x(int result,int *printed)
{
	char ip[20],op[20];
	int a,i=0,x,b=2;
	if(result<16)
  400276:	41 bc 00 00 00 00    	mov    $0x0,%r12d
		op[2]='0';
		b++;
	}
	while(result>=16)
	{
		a=result%16;
  40027c:	89 d8                	mov    %ebx,%eax
  40027e:	c1 f8 1f             	sar    $0x1f,%eax
  400281:	c1 e8 1c             	shr    $0x1c,%eax
  400284:	8d 3c 03             	lea    (%rbx,%rax,1),%edi
  400287:	83 e7 0f             	and    $0xf,%edi
  40028a:	29 c7                	sub    %eax,%edi
		ip[i]=get_hex_value(a);
  40028c:	e8 6b ff ff ff       	callq  4001fc <get_hex_value>
  400291:	42 88 44 24 1c       	mov    %al,0x1c(%rsp,%r12,1)
  400296:	41 8d 6c 24 01       	lea    0x1(%r12),%ebp
		i++;
		result=result/16;
  40029b:	8d 43 0f             	lea    0xf(%rbx),%eax
  40029e:	85 db                	test   %ebx,%ebx
  4002a0:	0f 49 c3             	cmovns %ebx,%eax
  4002a3:	c1 f8 04             	sar    $0x4,%eax
  4002a6:	89 c3                	mov    %eax,%ebx
  4002a8:	49 ff c4             	inc    %r12
	if(result<16)
	{
		op[2]='0';
		b++;
	}
	while(result>=16)
  4002ab:	83 f8 0f             	cmp    $0xf,%eax
  4002ae:	7f cc                	jg     40027c <print_x+0x2f>
		a=result%16;
		ip[i]=get_hex_value(a);
		i++;
		result=result/16;
	}
	ip[i]=get_hex_value(result);
  4002b0:	89 c7                	mov    %eax,%edi
  4002b2:	e8 45 ff ff ff       	callq  4001fc <get_hex_value>
  4002b7:	48 63 d5             	movslq %ebp,%rdx
  4002ba:	88 44 14 1c          	mov    %al,0x1c(%rsp,%rdx,1)
}

void print_x(int result,int *printed)
{
	char ip[20],op[20];
	int a,i=0,x,b=2;
  4002be:	b8 02 00 00 00       	mov    $0x2,%eax
		i++;
		result=result/16;
	}
	ip[i]=get_hex_value(result);

	for(x=i;x>=0;x--)
  4002c3:	85 ed                	test   %ebp,%ebp
  4002c5:	78 1f                	js     4002e6 <print_x+0x99>
	{
		op[0]='0';
  4002c7:	c6 44 24 08 30       	movb   $0x30,0x8(%rsp)
		op[1]='x';
  4002cc:	c6 44 24 09 78       	movb   $0x78,0x9(%rsp)
		op[b]=ip[x];
  4002d1:	48 63 d0             	movslq %eax,%rdx
  4002d4:	48 63 cd             	movslq %ebp,%rcx
  4002d7:	0f b6 4c 0c 1c       	movzbl 0x1c(%rsp,%rcx,1),%ecx
  4002dc:	88 4c 14 08          	mov    %cl,0x8(%rsp,%rdx,1)
		b++;
  4002e0:	ff c0                	inc    %eax
		i++;
		result=result/16;
	}
	ip[i]=get_hex_value(result);

	for(x=i;x>=0;x--)
  4002e2:	ff cd                	dec    %ebp
  4002e4:	79 e1                	jns    4002c7 <print_x+0x7a>
		op[1]='x';
		op[b]=ip[x];
		b++;
	}
	char *fin=op;
	while(*fin)
  4002e6:	80 7c 24 08 00       	cmpb   $0x0,0x8(%rsp)
  4002eb:	74 1f                	je     40030c <print_x+0xbf>
  4002ed:	48 8d 5c 24 08       	lea    0x8(%rsp),%rbx
	{
		write(1,fin,1);
  4002f2:	ba 01 00 00 00       	mov    $0x1,%edx
  4002f7:	48 89 de             	mov    %rbx,%rsi
  4002fa:	bf 01 00 00 00       	mov    $0x1,%edi
  4002ff:	e8 35 02 00 00       	callq  400539 <write>
		fin++;
  400304:	48 ff c3             	inc    %rbx
		op[1]='x';
		op[b]=ip[x];
		b++;
	}
	char *fin=op;
	while(*fin)
  400307:	80 3b 00             	cmpb   $0x0,(%rbx)
  40030a:	75 e6                	jne    4002f2 <print_x+0xa5>
	{
		write(1,fin,1);
		fin++;
		printed++;
	}
}
  40030c:	48 83 c4 30          	add    $0x30,%rsp
  400310:	5b                   	pop    %rbx
  400311:	5d                   	pop    %rbp
  400312:	41 5c                	pop    %r12
  400314:	c3                   	retq   

0000000000400315 <print_c>:

void print_c(int result,int *printed)
{
  400315:	48 83 ec 18          	sub    $0x18,%rsp
	char s[5];
	s[0]=(char)result;
  400319:	40 88 7c 24 0b       	mov    %dil,0xb(%rsp)
	char *fin=s;
	write(1,fin,1);
  40031e:	48 8d 74 24 0b       	lea    0xb(%rsp),%rsi
  400323:	ba 01 00 00 00       	mov    $0x1,%edx
  400328:	bf 01 00 00 00       	mov    $0x1,%edi
  40032d:	e8 07 02 00 00       	callq  400539 <write>
	printed++;
}
  400332:	48 83 c4 18          	add    $0x18,%rsp
  400336:	c3                   	retq   

0000000000400337 <printf>:

int printf(const char *format, ...)
{    
  400337:	41 56                	push   %r14
  400339:	41 55                	push   %r13
  40033b:	41 54                	push   %r12
  40033d:	55                   	push   %rbp
  40033e:	53                   	push   %rbx
  40033f:	48 83 ec 50          	sub    $0x50,%rsp
  400343:	48 89 74 24 28       	mov    %rsi,0x28(%rsp)
  400348:	48 89 54 24 30       	mov    %rdx,0x30(%rsp)
  40034d:	48 89 4c 24 38       	mov    %rcx,0x38(%rsp)
  400352:	4c 89 44 24 40       	mov    %r8,0x40(%rsp)
  400357:	4c 89 4c 24 48       	mov    %r9,0x48(%rsp)
  40035c:	48 89 fb             	mov    %rdi,%rbx
  
	va_list ap;

	int printed=0;
  40035f:	c7 44 24 04 00 00 00 	movl   $0x0,0x4(%rsp)
  400366:	00 
	char *result_s,s[2];
	int result_d,result_c,result_x;
	va_start(ap, format);
  400367:	c7 44 24 08 08 00 00 	movl   $0x8,0x8(%rsp)
  40036e:	00 
  40036f:	48 8d 84 24 80 00 00 	lea    0x80(%rsp),%rax
  400376:	00 
  400377:	48 89 44 24 10       	mov    %rax,0x10(%rsp)
  40037c:	48 8d 44 24 20       	lea    0x20(%rsp),%rax
  400381:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
	char neg[2],*fin;
	while (*format != 0)
  400386:	0f b6 07             	movzbl (%rdi),%eax
  400389:	84 c0                	test   %al,%al
  40038b:	0f 84 97 01 00 00    	je     400528 <printf+0x1f1>
					    
						write(1,fin,1);
						printed++;
						result_d=result_d*(-1);
					}
					print_d(result_d,&printed);
  400391:	48 8d 6c 24 04       	lea    0x4(%rsp),%rbp
					{
						neg[0]='-';
						fin=neg;
						
					    
						write(1,fin,1);
  400396:	49 89 e5             	mov    %rsp,%r13

					default:
					s[0]='%';
					s[1]=*format;
					s[2]='\0';
					printed=print_s(s);
  400399:	4c 8d 64 24 02       	lea    0x2(%rsp),%r12
	char neg[2],*fin;
	while (*format != 0)
	{
		while(1)
		{
			if(*format=='%')
  40039e:	3c 25                	cmp    $0x25,%al
  4003a0:	0f 85 4d 01 00 00    	jne    4004f3 <printf+0x1bc>
			{
				format++;
				switch(*format)
  4003a6:	0f b6 43 01          	movzbl 0x1(%rbx),%eax
  4003aa:	3c 64                	cmp    $0x64,%al
  4003ac:	74 65                	je     400413 <printf+0xdc>
  4003ae:	3c 64                	cmp    $0x64,%al
  4003b0:	7f 13                	jg     4003c5 <printf+0x8e>
  4003b2:	3c 63                	cmp    $0x63,%al
  4003b4:	0f 84 b5 00 00 00    	je     40046f <printf+0x138>
  4003ba:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
  4003c0:	e9 10 01 00 00       	jmpq   4004d5 <printf+0x19e>
  4003c5:	3c 73                	cmp    $0x73,%al
  4003c7:	74 12                	je     4003db <printf+0xa4>
  4003c9:	3c 78                	cmp    $0x78,%al
  4003cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  4003d0:	0f 84 cc 00 00 00    	je     4004a2 <printf+0x16b>
  4003d6:	e9 fa 00 00 00       	jmpq   4004d5 <printf+0x19e>
				{
					case('s'):
					result_s=va_arg(ap,char *);
  4003db:	8b 44 24 08          	mov    0x8(%rsp),%eax
  4003df:	83 f8 30             	cmp    $0x30,%eax
  4003e2:	73 10                	jae    4003f4 <printf+0xbd>
  4003e4:	89 c2                	mov    %eax,%edx
  4003e6:	48 03 54 24 18       	add    0x18(%rsp),%rdx
  4003eb:	83 c0 08             	add    $0x8,%eax
  4003ee:	89 44 24 08          	mov    %eax,0x8(%rsp)
  4003f2:	eb 0e                	jmp    400402 <printf+0xcb>
  4003f4:	48 8b 54 24 10       	mov    0x10(%rsp),%rdx
  4003f9:	48 8d 42 08          	lea    0x8(%rdx),%rax
  4003fd:	48 89 44 24 10       	mov    %rax,0x10(%rsp)
					printed=print_s(result_s);
  400402:	48 8b 3a             	mov    (%rdx),%rdi
  400405:	e8 06 fd ff ff       	callq  400110 <print_s>
  40040a:	89 44 24 04          	mov    %eax,0x4(%rsp)
					
					break;
  40040e:	e9 dc 00 00 00       	jmpq   4004ef <printf+0x1b8>

					case('d'):
					result_d=va_arg(ap,int);
  400413:	8b 44 24 08          	mov    0x8(%rsp),%eax
  400417:	83 f8 30             	cmp    $0x30,%eax
  40041a:	73 10                	jae    40042c <printf+0xf5>
  40041c:	89 c2                	mov    %eax,%edx
  40041e:	48 03 54 24 18       	add    0x18(%rsp),%rdx
  400423:	83 c0 08             	add    $0x8,%eax
  400426:	89 44 24 08          	mov    %eax,0x8(%rsp)
  40042a:	eb 0e                	jmp    40043a <printf+0x103>
  40042c:	48 8b 54 24 10       	mov    0x10(%rsp),%rdx
  400431:	48 8d 42 08          	lea    0x8(%rdx),%rax
  400435:	48 89 44 24 10       	mov    %rax,0x10(%rsp)
  40043a:	44 8b 32             	mov    (%rdx),%r14d
					if(result_d<0)
  40043d:	45 85 f6             	test   %r14d,%r14d
  400440:	79 1d                	jns    40045f <printf+0x128>
					{
						neg[0]='-';
  400442:	c6 04 24 2d          	movb   $0x2d,(%rsp)
						fin=neg;
						
					    
						write(1,fin,1);
  400446:	ba 01 00 00 00       	mov    $0x1,%edx
  40044b:	4c 89 ee             	mov    %r13,%rsi
  40044e:	bf 01 00 00 00       	mov    $0x1,%edi
  400453:	e8 e1 00 00 00       	callq  400539 <write>
						printed++;
  400458:	ff 44 24 04          	incl   0x4(%rsp)
						result_d=result_d*(-1);
  40045c:	41 f7 de             	neg    %r14d
					}
					print_d(result_d,&printed);
  40045f:	48 89 ee             	mov    %rbp,%rsi
  400462:	44 89 f7             	mov    %r14d,%edi
  400465:	e8 e3 fc ff ff       	callq  40014d <print_d>
					break;
  40046a:	e9 80 00 00 00       	jmpq   4004ef <printf+0x1b8>

					case('c'):
					result_c=va_arg(ap, int );
  40046f:	8b 44 24 08          	mov    0x8(%rsp),%eax
  400473:	83 f8 30             	cmp    $0x30,%eax
  400476:	73 10                	jae    400488 <printf+0x151>
  400478:	89 c2                	mov    %eax,%edx
  40047a:	48 03 54 24 18       	add    0x18(%rsp),%rdx
  40047f:	83 c0 08             	add    $0x8,%eax
  400482:	89 44 24 08          	mov    %eax,0x8(%rsp)
  400486:	eb 0e                	jmp    400496 <printf+0x15f>
  400488:	48 8b 54 24 10       	mov    0x10(%rsp),%rdx
  40048d:	48 8d 42 08          	lea    0x8(%rdx),%rax
  400491:	48 89 44 24 10       	mov    %rax,0x10(%rsp)
					print_c(result_c,&printed);
  400496:	48 89 ee             	mov    %rbp,%rsi
  400499:	8b 3a                	mov    (%rdx),%edi
  40049b:	e8 75 fe ff ff       	callq  400315 <print_c>
					break;
  4004a0:	eb 4d                	jmp    4004ef <printf+0x1b8>

					case('x'):
					result_x=va_arg(ap,int);
  4004a2:	8b 44 24 08          	mov    0x8(%rsp),%eax
  4004a6:	83 f8 30             	cmp    $0x30,%eax
  4004a9:	73 10                	jae    4004bb <printf+0x184>
  4004ab:	89 c2                	mov    %eax,%edx
  4004ad:	48 03 54 24 18       	add    0x18(%rsp),%rdx
  4004b2:	83 c0 08             	add    $0x8,%eax
  4004b5:	89 44 24 08          	mov    %eax,0x8(%rsp)
  4004b9:	eb 0e                	jmp    4004c9 <printf+0x192>
  4004bb:	48 8b 54 24 10       	mov    0x10(%rsp),%rdx
  4004c0:	48 8d 42 08          	lea    0x8(%rdx),%rax
  4004c4:	48 89 44 24 10       	mov    %rax,0x10(%rsp)
					print_x(result_x,&printed);
  4004c9:	48 89 ee             	mov    %rbp,%rsi
  4004cc:	8b 3a                	mov    (%rdx),%edi
  4004ce:	e8 7a fd ff ff       	callq  40024d <print_x>
					break;
  4004d3:	eb 1a                	jmp    4004ef <printf+0x1b8>

					default:
					s[0]='%';
  4004d5:	c6 44 24 02 25       	movb   $0x25,0x2(%rsp)
					s[1]=*format;
  4004da:	88 44 24 03          	mov    %al,0x3(%rsp)
					s[2]='\0';
  4004de:	c6 44 24 04 00       	movb   $0x0,0x4(%rsp)
					printed=print_s(s);
  4004e3:	4c 89 e7             	mov    %r12,%rdi
  4004e6:	e8 25 fc ff ff       	callq  400110 <print_s>
  4004eb:	89 44 24 04          	mov    %eax,0x4(%rsp)
					break;
				}
				format++;
  4004ef:	48 83 c3 02          	add    $0x2,%rbx
			else 
			{
				break;
			}
		}
		if(*format==0)
  4004f3:	80 3b 00             	cmpb   $0x0,(%rbx)
  4004f6:	74 30                	je     400528 <printf+0x1f1>
		{
			break;
		}
		write(1,format,1);
  4004f8:	ba 01 00 00 00       	mov    $0x1,%edx
  4004fd:	48 89 de             	mov    %rbx,%rsi
  400500:	bf 01 00 00 00       	mov    $0x1,%edi
  400505:	e8 2f 00 00 00       	callq  400539 <write>
		format++;
  40050a:	48 8d 53 01          	lea    0x1(%rbx),%rdx
		if(*format!=0)
  40050e:	80 7b 01 00          	cmpb   $0x0,0x1(%rbx)
  400512:	74 14                	je     400528 <printf+0x1f1>
		{
			printed++;
  400514:	ff 44 24 04          	incl   0x4(%rsp)
	int printed=0;
	char *result_s,s[2];
	int result_d,result_c,result_x;
	va_start(ap, format);
	char neg[2],*fin;
	while (*format != 0)
  400518:	0f b6 43 01          	movzbl 0x1(%rbx),%eax
  40051c:	84 c0                	test   %al,%al
  40051e:	74 08                	je     400528 <printf+0x1f1>
		if(*format==0)
		{
			break;
		}
		write(1,format,1);
		format++;
  400520:	48 89 d3             	mov    %rdx,%rbx
  400523:	e9 76 fe ff ff       	jmpq   40039e <printf+0x67>
		}
	}
	va_end(ap);
	
	return printed;
}
  400528:	8b 44 24 04          	mov    0x4(%rsp),%eax
  40052c:	48 83 c4 50          	add    $0x50,%rsp
  400530:	5b                   	pop    %rbx
  400531:	5d                   	pop    %rbp
  400532:	41 5c                	pop    %r12
  400534:	41 5d                	pop    %r13
  400536:	41 5e                	pop    %r14
  400538:	c3                   	retq   

0000000000400539 <write>:
#include <stdarg.h>
#include <syscall.h>
//volatile int sss=1;
ssize_t write(int fd, const void *buf, size_t count)
{ ssize_t ret;
	*((char*)0xffffffff80000000+0xb8000)=70;
  400539:	c6 04 25 00 80 0b 80 	movb   $0x46,0xffffffff800b8000
  400540:	46 
    *((char*)0xffffffff80000000+0xb8001)=0x07;
  400541:	c6 04 25 01 80 0b 80 	movb   $0x7,0xffffffff800b8001
  400548:	07 
//	while(sss);
	ret =syscall_3(((long)(SYS_write)),((long)(fd)),((long)(buf)),((long)(count)));
  400549:	48 63 ff             	movslq %edi,%rdi
static __inline uint64_t syscall_3(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3) {
	
		uint64_t ret;
	//while(1);

	__asm__ volatile(
  40054c:	b8 01 00 00 00       	mov    $0x1,%eax
  400551:	cd 80                	int    $0x80
	return ret;
	
  400553:	c3                   	retq   
