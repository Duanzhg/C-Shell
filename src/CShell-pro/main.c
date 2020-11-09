#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include "parse.h"
#include "utils.h"


struct cmd cmdInfo[MAX_CMD_NUM];
char cmdStr[MAX_CMD_LENGTH];
int cmdNum;

int main()
{
        while(1){

        getPrompt();
        for(int i=0;i<cmdNum;i++){
            (cmdInfo+i)->bgExec = 0;
        }
        cmdNum=0;
        int n = getInput();
       fflush(stdin);
        parseCmds(n);
        parseArgs();
        for(int i=0; i<cmdNum;i++){
            struct cmd* pcmd = cmdInfo+i;

            int status = execInner(pcmd);
            if(status){

                int pid = fork();
                if(pid<0){
                    printf("fork failed\n");
                    return -1;
                 }else if(pid == 0){
                    execOuter(pcmd);
                }
                if(!((cmdInfo+i)->bgExec)){
                    waitpid(pid,NULL,0);
                 }
            }
        }
    }
    return 0;
}
