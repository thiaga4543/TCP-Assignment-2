#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#include "sem.h" //semaphore for registration


//semaphore for registration
int initsem(key_t semkey);
int p(int semid);
int v(int semid);
int handlesem(key_t skey);



char userN[30];

void createFile(int , char[],char *);
void setFile(int, char[], char[],char *);
void setUsername(int , char[],char *);

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int regist(int sock)
{

	int n,confirmation,choice,semid, reg;
	key_t semkey = 0x200;

    char buffer[256],buffer2[250],clientInfo[256],*txt,filename[256];
    char *uDetails;
    FILE * fp;
    

	int queueID;
	queueID = msgget(ftok("/temp",9), IPC_CREAT | 0666);
	if(msgctl(queueID, IPC_RMID, (struct msqid_ds*)0)<0)
		perror("MSG queue remove fails");
	

		semid = handlesem(semkey);

		//Process before entering registration section
		p(semid); // only one process will entering this
		sleep(1); 
		//Process in registration section
		n = recv(sock,buffer,255,0); // just to prevent logic error

		uDetails = "Name";
    		n = send(sock,("Enter Name: " ),255,0);
    		if (n < 0) error("Writing error to socket");
    		createFile(sock,buffer,uDetails);

		uDetails = "Username";
		n = send(sock,("Username: " ),255,0);
		if (n < 0) error("Writing error to socket");
		setFile(sock,buffer,filename,uDetails);


		uDetails = "Password";
		n = send(sock,("Password: " ),255,0);
		if (n < 0) error("Writing error to socket");
		setFile(sock,buffer,filename,uDetails);
		
		uDetails = "To store username only";
		n = send(sock,("" ),255,0);
		if (n < 0) error("Writing error to socket");
		setFile(sock,buffer,filename,uDetails);
		
		n = send(sock,("Registration Confirmation!!! \n[1] -> Confirm\n[2]-> Cancel \n" ),255,0);
		if (n < 0) error("Writing error to socket");

		bzero(buffer,256);
		n = recv(sock,buffer,10,0);
		if (n < 0)
			error("Reading error from socket");
		confirmation = atoi(buffer);

		char *CMD[3];

		if(confirmation == 1 ){

    			CMD[0] = "/home/thiaga/Desktop/tcp/qServer";
			CMD[1] = "1";
			CMD[2] = NULL;
			if(fork()==0)
				execve(CMD[0], CMD,0);
			//Process leaving registration section
			sleep(2);
			v(semid);
			
			return 0;

    		}
    		else if(confirmation == 2){

    			CMD[0] = "/home/thiaga/Desktop/tcp/qServer";
			CMD[1] = "2";
			CMD[2] = NULL;
			if(fork()==0)
				execve(CMD[0], CMD,0);
			
			return 0;

		}
		
	return 0;
}



void createFile(int sock, char buffer[256],char *uDetails){
    int n;
    char *txt,extension[256],extension2[256];
    FILE * fp;


     bzero(buffer,256);
    n = recv(sock,buffer,255,0);
    if (n < 0) error("Reading error from socket");
  

    buffer[strlen(buffer)-1] = '\0'; //this one to remove newline from client input
    strcpy(extension,buffer); //copy buffer value to set extension
    txt = ".txt";
    strcat(extension,txt); //concatinate for file name


	char *cmd1[3];
	switch(fork()){
		case 0:
		   //to store in msg queue first
		cmd1[0] = "/home/thiaga/Desktop/tcp/qEnter";
		strcat(extension,"\n");
		strcpy(extension2,extension);
		cmd1[1] = extension2;
		cmd1[2] = "10";
		cmd1[3] = NULL;
		execve(cmd1[0], cmd1,0);
		break;
		case -1:
		perror("Fork Error"); break;
	}



}

void setFile(int sock, char buffer[256], char filename[255],char *uDetails){

    int n;
    FILE * fp;
	char *txt,extension[256],uname[256] = "login/";


    bzero(buffer,256);
    n = recv(sock,buffer,255,0);
    if (n < 0) error("Reading error from socket");
  

	char *cmd1[3];
	switch(fork()){
		case 0:
		   //to store in msg queue first

		cmd1[0] = "/home/thiaga/Desktop/tcp/qEnter";
		cmd1[1] = buffer;
		cmd1[2] = "10";
		cmd1[3] = NULL;
		execve(cmd1[0], cmd1,0);
		break;
		case -1:
		warn("Fork Error"); break;

	}

}
