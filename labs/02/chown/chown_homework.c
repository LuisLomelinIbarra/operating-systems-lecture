#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <string.h>
#include <grp.h>

int main(int argc, char *argv[]){
	long rc=-1;
	int aux=0;
	int len =strlen(argv[2]);
	uid_t uid = -1;
        gid_t gid = -1;
	char s_uid[len],s_gid[len];
	char *colon;
	struct passwd *pwd;
	struct group *grp;
	if(argc==3)
	if( argv[2][0]!='\0'){
		/*
		 *Averiguar si se tiene grupo; hay que
		 checar si hay un : en el argv[2]
		 si hay hay que dividir y hacer el sys call correcto
		 sino hay que poner -1 a los indicados
		 * */
		colon = strchr(argv[2],':');
		if(colon!=NULL){
			aux = colon - argv[2];
			strncpy(s_uid,argv[2],aux+1);
			s_uid[aux]='\0';
			aux = len-aux;
			strncpy(s_gid,colon+1,aux);
			//printf("USER: %s :\n",s_uid);
			//printf("GROUP: %s :\n",s_gid);
			pwd = getpwnam(s_uid);
			grp = getgrnam(s_gid);
			if(pwd!=NULL){
				uid = pwd->pw_uid;
			//	printf("%s","found user");
			}else{
				printf("%s","User Not Found\n");
			}
			if(grp!=NULL){

				gid = grp->gr_gid;
			//	printf("%s","found group");
			}else{
				printf("%s","Group Not Found");
			}	
				rc = syscall(SYS_chown,argv[1],uid,gid);			
			

		}else{
		//Si no especifican Grupo solo cambiar el owner
		
			pwd= getpwnam(argv[2]);
			//printf("%s\n",argv[2]);
			if(pwd==NULL){
				fprintf(stderr,"Error: User Not Found\n");
				exit(0);
			}else{
				rc = syscall(SYS_chown,argv[1],pwd->pw_uid,-1);
					
			}
		}	
	}
	if(rc<0){
		fprintf(stderr,"Opps there has been an error, Error Number = %d \n",errno);
		exit(0);
	}	
	return 0;
}
