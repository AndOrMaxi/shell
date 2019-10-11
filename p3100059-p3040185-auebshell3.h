#include  <stdio.h>
#include  <unistd.h>
#include  <string.h>
#include  <sys/wait.h>
#include  <sys/types.h>
#include  <stdlib.h>
#include <fcntl.h>



void gettokens(char *line, char **argument, int * count);
int  execute(char **argv, int * count);
void redirectInput(char * file);
void redirectOutput(char * file, int type);
