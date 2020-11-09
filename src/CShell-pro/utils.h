#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include "variable.h"

void getPrompt();
void init(struct cmd*);
int getInput();
int getItem(char*,char*,int);
int execInner(struct cmd*);
int execOuter(struct cmd*);
int execCmd(struct cmd*);

#endif // UTILS_H_INCLUDED
