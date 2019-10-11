#include  "p3100059-p3040185-auebshell4.h"


int main(void)
{
	char  fullCommand[255];//string me olh thn entolh opws thn plhktrologhse o xrhsths
	char  *commandWithPipe[255];//kommeno to string me diaxwristiko to '|'
	int pipeCount = 0;//plh8os swlhnwsewn 
	//char  **commandWithPipe = (char **) malloc(255*sizeof(char));	
	
	
	while (1) //loop pou termatizei mono an prokypsei sfalma se ektelesh kapoias entolh, h an o xrhsths dwsei EOF
	{               
		printPrompt();//ektypwsh auebshellx>
		
		if (fgets(fullCommand, sizeof(fullCommand),stdin) == NULL)//o xrhsths plhktrologei entolh, an dwsei EOF me ctrl-d, h main kanei exit
			return 0;
				
		if (!checkLength(fullCommand))//ginetai elegxos tou mhkous ths eisodou
			continue;
		
		getCommands(fullCommand, commandWithPipe, &pipeCount);	//elegxos gia yparksh swlhnwsewn
		
		if (execute(commandWithPipe, &pipeCount)==-1)//ektelesh entolhs
			return -1;		
	}
	
	//free(commandWithPipe);
	return 0;
}


void printPrompt(void)
{
	printf("auebshell5> "); 
}

int checkLength(char * command)
{
	int commandLength = strlen(command)-1;
		
		if ((commandLength) > 255)
		{
			printf("ERROR: Command line out of bounds\n");
						
			return 0;
		}
		else
		{
			if (command[commandLength]=='\n')
				command[commandLength] = '\0';
			return 1;
		}
	
}

//synarthsh pou kobei ena pinaka xarakthrwn me diaxwristiko  to dlm
void getTokens(char *line, char **argument, int * count, char *dlm)
{
	char *token;	
	
	token = strtok(line,dlm);
	*argument++ = token;	
	
	while (token != NULL)
	{
		(*count)++; //anevainei o metrhths afou perastike to prwto
		token = strtok(NULL,dlm);           
		*argument++ = token;
	}
}

//diaxwrizei me thn getTokens thn fullCommand se tokens(me diaxwristiko to '|'
void getCommands(char *line, char **argument, int * count)
{
	(*count)=0;
	getTokens(line, argument, count,"|");
	(*count)--;
}

//synarthsh pou trexei tis entoles pou grapsame sto command line
int  execute(char **argv, int * pipes)
{

	int i = 0, counter = 0, flag = 0, argCount=0;
	char *subarray[255] ;	
	char *subarray1[255];
	char *subarray2[255];	

	/*while (argv[i] != NULL)//psaxnw gia swlhnwsh
	{
		if (!strcmp(argv[i], "|"))
		{
			int temp;
			temp = (*count) - i -1;					
			memcpy(subarray1, &argv[0], i * sizeof(*argv));
			memcpy(subarray2, &argv[(int)(i+1)], temp * sizeof(*argv));
			pipeFlag = 1;
			break;
		}
		
		i++;
		
	}//while	*/
	
	i=0;
	pid_t  pid, waitPid;
	int    status;
	

	if (!(*pipes))//den exei swlhnwsh
	{
		printf("mphkaaaa");
		pid = fork();
		if (pid < 0) //apotyxia ths fork
		{   
			printf("ERROR: Failure at child process\n");
			
			return -1;
		}    	
		else if (pid == 0) //diergasia paidi 
		{  
			
				getTokens(argv[0],subarray1, &argCount," ");
				printf("%s \n", subarray1[0]);
				while (subarray1[i] != NULL)
				{
					printf("mphka\n");
					flag = checkToken(subarray1, &i);//elegxos gia anakatey8ynseis
					
					i++;
					
					if (!flag)
					{
						counter++;
					}
				}//while
				
				if (counter == 1)
				{
					execlp(subarray1[0],subarray1[0],NULL);
				}
				else
				{
					
					memcpy(subarray, &subarray1[0],counter * sizeof(*argv));
					execvp(*subarray, subarray);
				}
					
		}//paidi
		else //diergasia pateras
		{                                  
			while (wait(&status) != pid)       
				;
		}
		
		return 0;
	}
	
	
	else
	{//yparxei swlhnwsh

		int status; 
		int stat;		
		int pipefd[2];	

		if (fork()==0)
		{	
			int rv=pipe(pipefd);
			pid = fork();


			if (rv == -1)
			{
				printf("ERROR: Pipe command failed.\n");
				return -1;
			}

			//elegxos oti i fork petyxe.
			if (pid < 0) {
				perror("ERROR: Fork failed.\n");
				return -1;
			}
		
			if ( pid == 0)
			{
				getTokens(argv[0],subarray1, &argCount," ");
				close(pipefd[0]);    // close reading end in the child
				while (subarray1[i] != NULL)
				{
					flag = checkToken(subarray1, &i);//elegxos gia anakatey8ynseis
					
					i++;
					
					if (!flag)
					{
						counter++;
					}
				}//while
				
			
				dup2(pipefd[1], 1);  // send stdout to the pipe
				printf("%d\n", counter);
				close(pipefd[1]); 
				   // this descriptor is no longer needed
				   
				if (counter == 1)
				{
					
					execlp(subarray1[0],subarray1[0],NULL);
				}
				else
				{
					
					memcpy(subarray, &subarray1[0],counter * sizeof(*subarray1));
					execvp(*subarray, subarray);
				}
			//execlp(subarray1[0],subarray1[0],NULL);
			
			}		
			else //diergasia pateras
			{     
				int j=0, flag2=0;                       
				getTokens(argv[1],subarray2, &argCount," ");      
				close(pipefd[1]);  // close the write end of the pipe in the parent
				while (subarray2[j] != NULL)
				{
					flag2 = checkToken(subarray2, &j);//elegxos gia anakatey8ynseis
					
					j++;
					
					if (!flag2)
					{
						counter++;
					}
				}//while
				
		
				dup2(pipefd[0], 0); 
						 
				waitPid = wait(&status);
				if (waitPid == -1) {
					perror("ERROR: Waitpid failed.\n");
					return -1;
				}
				close(pipefd[0]);
				if (counter == 1)
				{
					execlp(subarray2[0],subarray2[0],NULL);
					return -1;
				}
				else
				{
					char *subarray[255] ;
					memcpy(subarray, &subarray2[0],counter * sizeof(*subarray2));
					execvp(*subarray, subarray);
			        return -1;
				}
				//execlp(subarray2[0],subarray2[0],NULL);
			}
			return 0;
		}
		else
		{
			wait(&stat);
		}
		
		return 0;
	}
}







//synarthsh pou elegxei ti eidous anakateu8ynsh exoume
int checkToken(char **argv, int *i)
{
	if ((!strcmp(argv[(*i)], "<")) || (!strcmp(argv[(*i)], ">")) || (!strcmp(argv[(*i)], ">>")) || (!strcmp(argv[(*i)], "&>")) || (!strcmp(argv[(*i)], "2>")))
	{
		
		if (!strcmp(argv[(*i)], "<"))
		{
		       redirectInput(argv[++(*i)]);
			return 1;
		}
		else if (!strcmp(argv[(*i)], ">"))
		{
			redirectOutput(argv[++(*i)],0);
			return 1;
		}
		else if (!strcmp(argv[(*i)], ">>"))
		{
			redirectOutput(argv[++(*i)],1);
			return 1;
		}
		else if ((!strcmp(argv[(*i)], "&>")) || (!strcmp(argv[(*i)], "2>")))
		{
			redirectOutput(argv[++(*i)],-1);
			return 1;
		}
		
	}
	
	return 0;
}

//synarthsh anakateu8ynshs eisodou
void redirectInput(char * file)
{
	int in;
	
	in = open(file,O_RDONLY);
	dup2(in, 0);
	close(in);
}


//synarthsh anakateu8ynshs e3odou
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




