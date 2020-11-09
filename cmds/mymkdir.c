#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
int main(int argc,char* argv[])
{
  if(argc != 2)
  {
    printf("Usage:%s dirname",basename(argv[0]));
    exit(0);
  }
  if(mkdir(argv[1],0770)!=0)
  perror("mkdir"),exit(-1);
 
  printf("mkdir directory  success .\n");
 
   return 0;
} 
