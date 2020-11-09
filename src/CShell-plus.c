#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>

#define MAX_ARG_NUM 20
#define MAX_FILE_PATH_LENGTH 255
#define MAX_CMD_NUM 10
#define MAX_CMD_LENGTH  1024
struct cmd{
    struct cmd* next;
    int begin,end;
    int argc;
    char* args[MAX_ARG_NUM];
    int lredir,rredir;
    char fromFile[MAX_FILE_PATH_LENGTH],toFile[MAX_FILE_PATH_LENGTH];
    int bgExec;
};

struct cmd cmdInfo[MAX_CMD_NUM];
char cmdStr[MAX_CMD_LENGTH];
int cmdNum;


void getPrompt();
void init(struct cmd*);
int getInput();
int getItem(char*,char*,int);
int parseCmds(int);
int parseArgs();
int execInner(struct cmd*);
int execOuter(struct cmd*);
int execCmd(struct cmd*);
int main(){

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

void getPrompt(){
    struct passwd *pwd;
    char hostname[10];
    //char dir[1024];
    char permission;
    gethostname(hostname,sizeof(hostname));
    //getcwd(dir,sizeof(dir));
    permission = (getuid()==0) ? '#' : '$';
    pwd = getpwuid(getuid());
    printf("%s@%s%c ",pwd->pw_name,hostname,permission);
}
void init(struct cmd* pcmd){
    pcmd->next = NULL; 
    pcmd->begin = -1;
    pcmd->end = -1;
    pcmd->argc = 0;
    pcmd->rredir = 0;
    pcmd->lredir = 0;

    if(pcmd->bgExec != 1)
    pcmd->bgExec = 0;

    for(int i=0; i < MAX_ARG_NUM;i++)
        pcmd->args[i] = NULL;
}

int getInput(){
    int cmdStrIndex=0;
    int remaining;
    char newline = 1;

    while(newline){
       remaining = MAX_CMD_LENGTH - cmdStrIndex;
       if(remaining <=0){
            printf("Your cmdStr is too long\n");
            return -1;
       }

       fgets(cmdStr+cmdStrIndex,remaining,stdin);
        newline = 0;
        
       while(1){
            if(cmdStr[cmdStrIndex] =='\\' && cmdStr[cmdStrIndex+1] == '\n'){
                newline = 1;
                cmdStr[cmdStrIndex]='\0';
                cmdStrIndex++;
                break;
            }
            else if(cmdStr[cmdStrIndex] == '\n'){
                break;
            }

            cmdStrIndex++;
       }

    }
    
    return cmdStrIndex;
}

int parseCmds(int n){
    char beginCmd = 0;
    //struct cmd* head;
    
    for(int i=0; i<=n;i++){
        switch(cmdStr[i]){
            case '&':
                if(cmdStr[i+1] == '\n' ||cmdStr[i+1] ==';'){
                    cmdStr[i]=' ';
                    cmdInfo[cmdNum].bgExec = 1;
                }
                break;
            case '\t':
                cmdStr[i]=' ';
                break;
            case ';':
                cmdStr[i] = '\0';
                cmdInfo[cmdNum].end=i;
                cmdNum++;
                break;

            case '\n':
                cmdStr[i] = '\0';
                cmdInfo[cmdNum].end = i;
                cmdNum++;
                return 0;
            case ' ':
                break;
            
            default: 
                if(!beginCmd){
                    beginCmd = 1;
                    cmdInfo[cmdNum].begin = i;
                }

        }
    }
}

int getItem(char* dst,char* src,int p){
    int count = 0;
    char ch;
    while(src[++p] == ' ');
    if(src[p] == '\0')
        return -1;
    while(1==1){
        ch=dst[count]=src[p];
        if(ch == ' ' || ch == '>' || ch == '<' || ch == '|' || ch == '\0')
            break;
        count++,p++;
    }
    dst[count]='\0';
   
    return p -1;
}


int parseArgs(){
    int beginItem = 0;
    int begin,end;
    char ch;
    struct cmd* pcmd;
    
    for(int k=0; k<cmdNum;k++){
        beginItem=0;
        begin = cmdInfo[k].begin;
        end = cmdInfo[k].end;
        
        pcmd = &cmdInfo[k];
        init(pcmd);

        for(int i=begin; i<end;i++){
            ch = cmdStr[i];
            if(cmdStr[i] == '>' || cmdStr[i] == '<' || cmdStr[i]=='|'){
                    beginItem = 0;
                    cmdStr[i]='\0';
            }

            if(ch == '>'){
                if(cmdStr[i+1] == '>'){
                    pcmd->rredir = 2;
                    cmdStr[i+1] = ' ';
                }
                else{
                    pcmd->rredir =1;
                }
                
                int tmp = getItem(pcmd->toFile,cmdStr,i);
                i = tmp;

            }else if(ch == '<'){
                if(cmdStr[i+1] == '<'){
                    pcmd->lredir = 2;
                    cmdStr[i+1] = ' ';
                }
                else{
                    pcmd->lredir = 1;
                }
                int tmp = getItem(pcmd->fromFile,cmdStr,i);
                i = tmp;
            }else if( ch == '|'){
                pcmd->end = i;
                pcmd->next = (struct cmd*)malloc(sizeof(struct cmd));
                pcmd = pcmd->next;
                init(pcmd);
            }else if(cmdStr[i] == ' ' || cmdStr[i] == '\0'){
                if(beginItem){
                    beginItem = 0;
                    cmdStr[i] = '\0';
                }
            }else{
                if(pcmd->begin == -1){
                    pcmd->begin = i;
                }
                
                if(!beginItem){
                    beginItem = 1;
                    pcmd->args[pcmd->argc++] = cmdStr + i;
                }
            }
        }
        
        pcmd->end = end;
    }
}

void setIO(struct cmd* pcmd,int rfd,int wfd){
        if( pcmd->rredir > 0){
            int flag;
            if(pcmd->rredir == 1){
                flag = O_WRONLY|O_TRUNC|O_CREAT;
            }
            else{
                flag = O_WRONLY|O_APPEND|O_CREAT;
            }
            int wport = open(pcmd->toFile,flag);
            dup2(wport,STDOUT_FILENO);
            close(wport);
        }

        if(pcmd->lredir > 0){
            int rport = open(pcmd->fromFile,O_RDONLY);
            dup2(rport,STDIN_FILENO);
            close(rport);
        }

        if(rfd != STDIN_FILENO){
            dup2(rfd,STDIN_FILENO);
            close(rfd);
        }

        if(wfd != STDOUT_FILENO){
            dup2(wfd,STDOUT_FILENO);
            close(wfd);
        }
}

int execInner(struct cmd* pcmd){
    if(pcmd->argc == 0){
        return 0;
    }

    if(strcmp(pcmd->args[0],"exit") == 0){
        exit(0);
    }else if(strcmp(pcmd->args[0],"cd") == 0 ){
        struct stat st;
        if(pcmd->args[1]){
            stat(pcmd->args[1],&st);
            if(S_ISDIR(st.st_mode))
                chdir(pcmd->args[1]);
            else{
                printf("%s: No such directory\n",pcmd->args[1]);
                return 0;
            }

        }
    }
    
    return 1;
}

int execOuter(struct cmd* pcmd){
    if(pcmd->next == NULL){
        setIO(pcmd,STDIN_FILENO,STDOUT_FILENO);
        execvp(pcmd->args[0],pcmd->args);
        exit(0);
    }
    int fd[2];

    pipe(fd);
    int rc = fork();

    if(rc < 0){
        printf("fork failed\n");
        return -1;
    } else if(rc == 0){
        close(fd[0]);
        setIO(pcmd,STDIN_FILENO,fd[1]);
        execvp(pcmd->args[0],pcmd->args);
    }else{
        wait(NULL);
        pcmd = pcmd->next;
        close(fd[1]);
        setIO(pcmd,fd[0],STDOUT_FILENO);
        execOuter(pcmd);
    }
}
