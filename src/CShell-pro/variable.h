#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED


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


#endif // VARIABLE_H_INCLUDED
