#include  "p3100059-p3040185-auebshell5.h"


int main(void)
{
	char  fullCommand[255];//string me olh thn entolh opws thn plhktrologhse o xrhsths
	char  *commandWithPipes[255];//kommeno to string me diaxwristiko to '|'
	int pipeCount = 0;//plh8os swlhnwsewn 
	//char  **commandWithPipes = (char **) malloc(255*sizeof(char));	
	
	
	while (1) //loop pou termatizei mono an prokypsei sfalma se ektelesh kapoias entolh, h an o xrhsths dwsei EOF
	{               
		printPrompt();//ektypwsh auebshellx>
		
		if (fgets(fullCommand, sizeof(fullCommand),stdin) == NULL)//o xrhsths plhktrologei entolh, an dwsei EOF me ctrl-d, h main kanei exit
			return 0;
				
		if (!checkLength(fullCommand))//ginetai elegxos tou mhkous ths eisodou
			continue;
		
		getCommands(fullCommand, commandWithPipes, &pipeCount);	//elegxos gia yparksh swlhnwsewn
		
		if (execute(commandWithPipes, &pipeCount)==-1)//ektelesh entolhs
			return -1;		
	}
	
	//free(commandWithPipes);
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
int  execute(char **command, int * pipes)
{

	int i = 0;//metrhths gia na diatrexoume tous pinakes 
	int counter = 0; //katametraei to plh8os ton orismatwn ths entolhs pros ektelesh
	int redirections = 0;//paramenei mhdenikh oso den entopizontai anakatey8ynseis
	int argCount=0;// metrhths plh8ous orismatwn plhrous entolhs
	int j=0;//metrhths gia na diatrexoume tous pinakes 		
	char *commandWithArgs[255];//proswrinos pinakas ston opoion ekxwreitai h stoixeiwdhs entolh pros ektelesh me ta orismata ths
	char *commandWithArgsAndRed[255];//proswrinos pinakas ston opoion ekxwreitai h entolh pros ektelesh me ta orismata ths kai tis anakatey8ynseis
	pid_t  pid;
	int    status;
	
	if (!(*pipes))//an einai entolh xwris swlhnwseis
	{
		pid = fork();
		if (pid < 0) //apotyxia ths fork
		{   
			printf("ERROR: Failure at child process\n");
			
			return -1;
		}    	
		else if (pid == 0) //diergasia paidi 
		{  
			
				getTokens(command[0],commandWithArgsAndRed, &argCount," ");//apo8hkeyw thn entolh me tis parametrous tis sto
				
				i=0;
				
				while (commandWithArgsAndRed[i] != NULL)
				{
					redirections = checkToken(commandWithArgsAndRed, &i);//elegxos gia anakatey8ynseis
					
					i++;
					
					if (!redirections)
					{
						counter++;
					}
				}//while
				
				if (counter == 1)//monolektikh entolh
				{
					execlp(commandWithArgsAndRed[0],commandWithArgsAndRed[0],NULL);
				}
				else
				{
					
					memcpy(commandWithArgs, &commandWithArgsAndRed[0],counter * sizeof(*commandWithArgsAndRed));
					execvp(*commandWithArgs, commandWithArgs);
				}
					
		}//paidi
		else //diergasia pateras
		{                                  
			wait(&status);
		}
		
		return 0;
	}	
	else//yparxei swlhnwsh
	{		
		int pipeFds[2*(*pipes)];//akra swlhnwsewn, dyo gia ka8e pipe		
		
		for(i=0;i<(*pipes);i++)//dhmiourgia twn pipes
		{
			pipe(pipeFds+2*i);//pipeFds[0] kai pipeFds[1] gia to prwto pipe,pipeFds[2] kai pipeFds[3] gia to deytero pipe, kai outw ka8ekshs
		}

		while(command[j]!=NULL)//diatrexoume ton pinaka twn entolwn anamesa sta '|'
		{
			getTokens(command[j],commandWithArgsAndRed, &argCount," ");//xwrizoume thn entolh me diaxwristh to ' '
			//oses oi entoles anamesa stis swlhnwseis, tosa paidia 8a dhmiourgh8oun
			if (fork() == 0)//diergasia paidi pou 8a ektelesei thn entolh j k 8a perasei to apotelesma ths ws eisodo sthn entolh j+1
			{		
				i=0;
				counter=0;
				
				while (commandWithArgsAndRed[i] != NULL)//elegxoume gia anakatey8ynseis
				{
					redirections = checkToken(commandWithArgsAndRed, &i);//elegxos gia anakatey8ynseis
					
					i++;
					
					if (!redirections)
					{
						counter++;
					}
				}//while
			
				if(j!=0)
					dup2(pipeFds[(j-1)*2], 0);
		
				if(j!=(*pipes))
					dup2(pipeFds[j*2+1], 1);
			
				for(i=0;i<2*(*pipes);i++)
					close(pipeFds[i]);
					
				if (counter == 1)//monolektikh entolh
				{
					execlp(commandWithArgsAndRed[0],commandWithArgsAndRed[0],NULL);
				}
				else
				{					
					memcpy(commandWithArgs, &commandWithArgsAndRed[0],counter * sizeof(*commandWithArgsAndRed));
					execvp(*commandWithArgs, commandWithArgs);
				}
			}//child
			
			j++;
		}//while  
		
		//o pateras olwn kleinei ta akra anagnwshs k eggrafhs twn pipes kai perimenei ola ta paidia na teleiwsoun
		for(i=0;i<2*(*pipes);i++)
			close(pipeFds[i]);
		for (i = 0; i <= (*pipes); i++)
			wait(&status); 
	
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
		else if ((!strcmp(argv[(*i)], "&>")) || (!strcmp(argv[(*i)], "2>")))//anakatey8ynsh sfalmatwn
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
	
	in = open(file,O_RDONLY);//anoigma arxeiou mono gia anagnwsh
	dup2(in, 0);
	close(in);
}


//synarthsh anakateu8ynshs e3odou
void redirectOutput(char * file, int type)
{
	int out;
	
	if (type == -1)
	{
		out = open(file,O_WRONLY|O_CREAT,0666);//anoigma arxeiou mono gia eggrafh kai an den yparxei, dhmiourgeitai(antikatastash periexomenwn)
		dup2(out,2);
	}
	else
	{
		if (type == 1)
		{
			out = open(file,O_WRONLY|O_CREAT|O_APPEND,0666);//anoigma arxeiou gia eggrafh xwris na sbhstoun ta prohgoumena periexomena
		}
		else if (type == 0)
		{
			out = open(file,O_WRONLY|O_CREAT,0666);
		}
			
		dup2(out,1);
	}
	
	close(out);
}
