#include<sys/utilities.h>
#include<sys/defs.h>
#include<sys/sbunix.h>
//#include<stdlib.h>
#define DIRECTORY_TYPE 5
#define FILE_TYPE 0

extern char _binary_tarfs_start;
extern char _binary_tarfs_end;

struct posix_header_ustar {
	char name[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char checksum[8];
	char typeflag[1];
	char linkname[100];
	char magic[6];
	char version[2];
	char uname[32];
	char gname[32];
	char devmajor[8];
	char devminor[8];
	char prefix[155];
	char pad[12];
};

struct files_list
{
	char name[256];
	int fd; //file descriptor
	void* addr;
	uint64_t size;
	int offset;
	struct files_list* next;
	char typeflag[1];
	long d_ino;
	uint64_t d_off;
	unsigned short d_reclen;
	
	
};


typedef struct posix_header_ustar HEAD_USTAR;

uint64_t atoi(char *str); 
void strcpy(char*d,const char*s);
int strcmp(char *filename,const char* name);
int strlen(const char *str);
uint64_t pow(int base,int power);
uint64_t octalToDecimal(uint64_t num);
void part(char* str,char* a);
void* lookup(char *file_name);
//void parse(char* str,char* a);

void init_tarfs();
int open(char *filename);

void parser(char* str,char* a);
//size_t read(char *filename,size_t count,void* buf);
uint64_t kopendir(char *name);
uint64_t kreaddir(uint64_t addr);

uint64_t kgetdents(uint64_t fd, uint64_t dirp, uint64_t count);
int pathlook(char* filename); 


