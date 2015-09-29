#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

int slogin(int sock,int count){
	
	int n,ln_num,counter=0,pcount=0;
	char uname[30],pword[30],buffer[256],ln_buf[256],pbuf[256];
	char *unameCPY;
	char *pwordCPY;
	char *token,*pex;
	char accG[40] = "Access Granted!";
	char pext[30] = "./fileDirectory/";
	char *loginFile = "./login/login.txt";

	
	FILE *fl,*p;
	
	n = send(sock,("Enter Username: " ),255,0);
	if (n < 0) error("Writing error to socket");
	
	bzero(buffer,256);
	n = recv(sock,buffer,255,0);
	if (n < 0) error("Reading error from socket");

	strcpy(uname,buffer);
	
	n = send(sock,("Enter Password: " ),255,0);
	if (n < 0) error("Writing error to socket");
	
	bzero(buffer,256);
	n = recv(sock,buffer,255,0);
	if (n < 0) error("Reading error from socket");

	strcpy(pword,buffer);
	
	fl = fopen(loginFile, "rt"); 
	if (!fl) { 
		printf("Couldn't open file for reading...\n"); 
		return 0; 
	} 
 
	ln_num = 0;

	uname[strlen(uname)-1] = '\0';
	pword[strlen(pword)-1] = '\0';
	
	while (fgets(ln_buf, sizeof(ln_buf), fl) != NULL) { 
		
		++ln_num; 
		
		if((strstr(ln_buf, uname)) != NULL) {
			ln_buf[strlen(ln_buf)-1]='\0';
			token = strtok(ln_buf, " ");
			
			counter = 1;
			pex = ".txt";
			strcat(token,pex);
			strcat(pext,token);
			
   			while( token != NULL ) 
   			{
    				token = strtok(NULL, " ");
   			}
   			
   		}
	
	}

	if(counter == 1){
		strcat(accG,uname);
   		n = send(sock,(accG),255,0);
		if (n < 0) error("Writing error to socket");
		return 0;
	}
	else{
		n = send(sock,("Access Denied!!! Username/Password wrong\n" ),255,0);
		if (n < 0) error("Writing error to socket");
	}
	
	return 0;
	
}
