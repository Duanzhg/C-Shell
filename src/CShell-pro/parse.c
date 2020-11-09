#include "parse.h"
#include  <stdlib.h>
extern struct cmd cmdInfo[MAX_CMD_NUM];
extern char cmdStr[MAX_CMD_LENGTH];
extern int cmdNum;

void parseCmds(int n){
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
                return;
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

void parseArgs(){
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
