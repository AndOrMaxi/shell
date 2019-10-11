#include  <stdio.h>
#include  <unistd.h>
#include  <string.h>
#include  <sys/wait.h>
#include  <sys/types.h>
#include  <stdlib.h>
#include <fcntl.h>


void printPrompt(void);
void getCommands(char *command, char **commands, int * count);
void getTokens(char *line, char **argument, int * count, char *dlm);
int  checkLength(char * command);
int  execute(char **argv, int * count);
int  checkToken(char **argv,int *i);
void redirectInput(char * file);
void redirectOutput(char * file, int type);

