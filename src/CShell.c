#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define EXIT 1
#define BUFSIZE 1024
#define TOK_SIZE 64
#define DELIM "\n\r\a\t "


void shLoop();
char* shReadLine();
char** shSplitLine(char* line);
int shExecute(char** args);
int shLanuch(char** args);

int shCd(char** args);
int shHelp(char** args);
int shExit(char** args);
int shNumBuiltins();
char* bulitin[]={
    "cd",
    "help",
    "exit"
};

char (*builtinFunction[])(char**)={
    &shCd,
    &shHelp,
    &shExit

};
int main(){

    shLoop();

    return 0;
}
void shLoop(){
    char* line;
    char** args;
    int status;
    
    do{
        printf(">>>># ");
        line = shReadLine();
        args = shSplitLine(line);
        status = shExecute(args);
        free(line);
        free(args);
    }while(status);

    
}

char* shReadLine(){
    int bufsize = BUFSIZE;
    int position=0;
    char *buffer =(char*) malloc(sizeof(char)*bufsize);
    char c;
    
    
    if(!buffer){
        fprintf(stderr,"allocation error");
        exit(EXIT);
    }

    while(1){
        
        c = getchar();

        if(c==EOF || c=='\n'){
             buffer[position]='\0';
             return buffer;
         }else{
             buffer[position]=c;
          }
         position++;

        if(position >= bufsize){
             bufsize +=BUFSIZE;
             buffer=(char*)realloc(buffer,bufsize);

             if(!buffer){
                fprintf(stderr,"allocation error");
                 exit(EXIT);
             }

         }
    }

}


char** shSplitLine(char* line){
    int bufsize=TOK_SIZE;
    int position =0;
    char* token;
    char** tokens = (char**)malloc(bufsize*sizeof(char*));

    token = strtok(line,DELIM);
    
    while(token !=NULL){
        tokens[position]=token;
        position++;

        
        if(position >=bufsize){
            bufsize +=TOK_SIZE;
            tokens=(char**)realloc(tokens,bufsize*sizeof(char*));
             if(!tokens){
                fprintf(stderr,"allocation error\n");
                 exit(EXIT);
            }
         }

        token = strtok(NULL,DELIM);
    }
    tokens[position]=NULL;
    return tokens;

}

int shExecute(char** args){

    int i;
    if(args[0] ==NULL)
        return 1;

    for(int i=0; i<shNumBuiltins();i++){
        if(strcmp(args[0],bulitin[i]) == 0)
           return (*builtinFunction[i])(args);
    }
    return shLanuch(args);
}

int shLanuch(char** args){
    int pid;
    
    pid = fork();

    if(pid<0){
        printf("fork faild\n");
    }else if(pid == 0){
        if(execvp(args[0],args) == -1){
            perror("lsh");
        }
    }else{
       int wc = wait(NULL);
    }
    return 1;
}

int shNumBuiltins(){
    return sizeof(bulitin)/sizeof(char*);
}
int shCd(char** args){
    return 1;
}

int shExit(char** args){
    return 0;
}

int shHelp(char** args){
    printf("%s\n","啊这");
    return 1;
}
