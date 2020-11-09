#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <time.h> 

int main() 
{ 
  printf("%ld\n",time(0)); 
  time_t t = time(0); 
  printf("%s\n",ctime(&t)); 
  struct tm* pt = localtime(&t); 
  char buf[256]; 
  sprintf(buf,"%d-%d-%d %d:%d:%d",1900+pt->tm_year,1+pt->tm_mon,pt->tm_mday,pt->tm_hour,pt->tm_min,pt->tm_sec); 
  printf("%s\n",buf); 
  return 0; 
}

