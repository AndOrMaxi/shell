#include  "p3100059-p3040185-auebshell1.h"

int main(void)
{
	char  command1[255];
	int rv = 0; //return value	
	
	while (1) 
	{               
		printf("auebshell1> ");  
		   
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
			if (command1[i]=='\n')
			{
				command1[i] = '\0';
			}
			
			rv = execute(command1);
			
			if ( rv == -1)
				return -1;
		}
	}
	
	return 0;
}


int  execute(char *argv)
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
		if (execlp(argv, argv, NULL) < 0) 
		{     
			printf("ERROR: exec failed\n");
			
			return -1;
        }
	}	     
	else //diergasia pateras
	{                                  
		wait(&status);
	}
	
	return 0;
}
