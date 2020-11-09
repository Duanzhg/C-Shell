#include <stdio.h>
int main(int argc, char *argv[])
{
    FILE *fp = fopen(argv[1], "r");  
    int read_ret;
    if(argc < 2)  
    {
        printf("please input correct file!\n");
    }
    if(fp == NULL)  
    {
        printf("open %s failed!\n", argv[1]);
        return -1;
    }
    while(1)  
    {
        read_ret = fgetc(fp);  
        if(feof(fp))  
        {
            printf("read  %s endl\n",argv[1]);  
            break;
        }
        fputc(read_ret,stdout);  
    }
}
