#include  "p3100059-p3040185-auebshell3.h"

int main(void)
{
	char  command1[255];
	char  *argv[255];
	int count = 0;
	//char  **argv = (char **) malloc(255*sizeof(char));	
	int rv = 0; //return value
	
	
	while (1) 
	{               
		printf("auebshell3> "); 
		    
		if (fgets(command1, sizeof(command1),stdin) == NULL)//EOF
		{ 
			return 0;
		}
		
		int i = strlen(command1)-1;
		
		if ((i+1) > 255)
		{
			printf("ERROR: Command line out of bounds");
			
			return 0;
		}
		else
		{
			if (command1[i]=='\n')//
			{
				command1[i] = '\0';
			}
				
			gettokens(command1,argv,&count);
			rv = execute(argv, &count);
			count = 0;
			
			if ( rv == -1)
				return -1;
		}	
	}
	
	//free(argv);
	return 0;
}

void gettokens(char *line, char **argument, int * count)
{
	char *token;
	
	token = strtok(line," ");
	*argument++ = token;	
	
	while (token != NULL)
	{
		(*count)++; //anevainei o metrhths afou perastike to prwto
		token = strtok(NULL," ");           
		*argument++ = token;
	}
}

int  execute(char **argv, int * count)
{
	pid_t  pid;
	int    status;
     
	if ((pid = fork()) < 0) //apotyxia ths fork
	{   
		printf("ERROR: Failure at child process\n");
		
		return -1;
	}    	
	else if (pid == 0) //diergasia paidi 
	{  
		if (*count == 1)
		{
			if (execvp(*argv, argv) < 0) 
			{     
				printf("ERROR: exec failed\n");
				
				return -1;
			}
		}
		else 
		{
			int i = 0, counter = 0, flag = 0;
			
			while (argv[i] != NULL)
			{
				if ((!strcmp(argv[i], "<")) || (!strcmp(argv[i], ">")) || (!strcmp(argv[i], ">>")) || (!strcmp(argv[i], "&>")) || (!strcmp(argv[i], "2>")))
				{
					flag=1;
					
					if (!strcmp(argv[i], "<"))
					{
						redirectInput(argv[++i]);
					}
					else if (!strcmp(argv[i], ">"))
					{
						redirectOutput(argv[++i],0);
					}
					else if (!strcmp(argv[i], ">>"))
					{
						redirectOutput(argv[++i],1);
					}
					else if ((!strcmp(argv[i], "&>")) || (!strcmp(argv[i], "2>")))
					{
						redirectOutput(argv[++i],-1);
					}							
				}
				
				i++;
				
				if (!flag)
				{
					counter++;
				}								
			}
			
			if (counter == 1)
			{
				execlp(argv[0],argv[0],NULL);
			}
			else
			{
				char *subarray[255] ;
				memcpy(subarray, &argv[0],counter * sizeof(*argv));
				execvp(*subarray, subarray);
			}
		}		
	}	     
	else //diergasia pateras
	{                                  
		wait(&status);
	}
	
	return 0;
}

void redirectInput(char * file)
{
	int in;
	
	in = open(file,O_RDONLY);
	dup2(in, 0);
	close(in);
}

void redirectOutput(char * file, int type)
{
	int out;
	
	if (type == -1)
	{
		out = open(file,O_WRONLY|O_CREAT,0666);
		dup2(out,2);
	}
	else
	{
		if (type == 1)
		{
			out = open(file,O_WRONLY|O_CREAT|O_APPEND,0666);
		}
		else if (type == 0)
		{
			out = open(file,O_WRONLY|O_CREAT,0666);
		}	
		
		dup2(out,1);
	}
	
	close(out);
}

