/*
 * This is my attempt in making a shell terminal
 *
 * General Function
 * 
 *	loop reading comands; when a comand is read execute the read command
 *	if exit is typed it ends the shell
 *	if cd is typed; the current working dir must be changed
 *
 *	the shell must wait till the command called and loaded in the child is finished
 *
	SEE IF YOU CAN ADD SUPPORT FOR THE ARROW KEYS LATER
 * 
 *
 * */
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
//#include <termios.h>
#define STDIN 0
#define STDOUT 1
#define CHARMAX  1002
/*
//This is for reading and interpreting  keyboard
struct termios originter, term;
int getCharFromKeyboard(void){
	int c;
	tcgetattr(STDIN,&originter);
	term=originter;
	term.c_lflag &= ~(ICANON|ECHO);	
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME]=0;	
	tcsetattr(STDIN,TCSANOW,&term);
	c= getchar();
	tcsetattr(STDIN,TCSANOW,&originter);
	return c;
}

int isSpecial(void){
	int c= getCharFromKeyboard();
	
}

void readInput(char** line){ //Implementation with arrow keys
	char* in=malloc(1024*sizeof(char));
	int indx=0;
	int fin=0;
	
	
	
}*/




void printstring(char* st){ 
	for(int i=0; i<strlen(st);i++){
		if(st[i]=='\n') printf("%d : End of String\n",i);
		else printf("%d : %c\n",i,st[i]);
	}
}



void runGivenCmd(char ** parg){
	pid_t pid = fork();
	int rc=0;
	if(pid == -1){
		printf("\033[31m ERROR in FORK \033[0m\n");
		exit(-1);
	}else{
		if(pid==0){
			//printf("cmd: %s \n",parg[0]);
			if((rc=execvp(parg[0],parg))<0){
				printf("\033[31m Could not execute command...\n Error Code: %d \033[0m\n",rc);
			perror("execvp");
			}
			exit(0);
		}else{
			wait(NULL);
			return;
		}
	}	
}

char** parseInput(char* input,char sp, int *argsize){
	

	char* pstart= input;
	//printf("%s",pstart);
	char *token;
	
	int i=0,siz=0;

	//Contar los sp en el string dado
	for(int x=0; x<strlen(input); x++){
		if(input[x]==sp) i++;
	}
	char **pstr = malloc(i * sizeof(char *));
	i=0;
	token = strtok(pstart,&sp);

	while(token!=NULL){
		pstr[i]= token;
		token = strtok(NULL,&sp);
		siz=strlen(pstr[i]);
		pstr[i][siz]='\0';
		
		//printf("Parse string:>%s<\n",pstr[i]);
		i++;
		
	}
	
	return pstr;
}


int customCmd(char** ap){
	int nocmd =2, i, selcmd=0;
	char* customcmds[nocmd];
	int iscmd=0;
	customcmds[0]="exit";
	customcmds[1]="cd"; //Ver si puedo hacerlo
	for(i=0; i<nocmd;i++){ //Elegir el customcmd
		//printstring(ap[0]);
		if(strcmp(customcmds[i],ap[0])==0){
			selcmd = i+1;
			break;
		}
	}
	switch(selcmd){
		case 1:
			printf("Bye!!!!\n");
			iscmd=1;
			exit(0);
		case 2:
		       chdir(ap[1]);
		       iscmd=1;
		       return iscmd;
		default:

		       break;
	}
	return iscmd;


}


int main(){
	const char shpoint[] = "\033[36m scsh>> \033[0m";
	char cmdstr[CHARMAX/2+1]="";
	char** args;
	int argsize=0,siz=0;
	int iscm=0;	
	while(1){
		//We print the shell msg
		write(STDOUT,shpoint,sizeof(shpoint));
		//We read the cmd

		//int c= getCharFromKeyboard();
		//printf("%x",c);
		read(STDIN,cmdstr,CHARMAX/2+1);
		
		siz=strlen(cmdstr);
		for(int i=0; i<siz;i++){//Evitar llevarse de la lectura cosas después de el \n
			if(cmdstr[i]=='\n') cmdstr[i]='\0';
			
		}
		//printstring(cmdstr);
		args=parseInput(cmdstr,' ', &argsize);
		//printf("%d\n",argsize);
		for(int i=0;i<argsize;i++) printf("arg[%d] = %s \n",i,args[i]);
		printf("\n-------------------------------\n");
		//Check if is it one of our own cmd		
		iscm=customCmd(args);
		if(!iscm){
			runGivenCmd(args);		
		}
		iscm=0;
		
		

	}

	return 0;
}
