#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

char* clogin(int sockfd){
	
	int n,ret;
	char buffer[256],bufcpy1[256],bufcpy3[256],bufcpy2[256],tokenBuf[40],retC[30]="0";
	char *token;
	const char delimiter[2] = "!"; //for split username with accessGrandted reply from server
	
	bzero(buffer,256);
	n = recv(sockfd,buffer,255,0);

	if (n < 0) 
		perror("Reading error from socket\n");
	printf("%s--->> ",buffer);
	
	bzero(buffer,256);
	fgets(buffer,255,stdin);
	
	n = send(sockfd,buffer,strlen(buffer),0);

	if (n < 0) 
		perror("Writing error to socket\n");
	
	bzero(buffer,256);
	n = recv(sockfd,buffer,255,0);

	if (n < 0) 
		perror("Reading error from socket\n");
	printf("%s--->> ",buffer);
	
	bzero(buffer,256);
	fgets(buffer,255,stdin);
	
	n = send(sockfd,buffer,strlen(buffer),0);

	if (n < 0) 
		perror("Writing error to socket\n");
	
	bzero(buffer,256);
	n = recv(sockfd,buffer,255,0);
	
	if (n < 0) 
		perror("Reading error from socket\n");
	
	strcpy(bufcpy3,buffer);
	strcpy(bufcpy1, "Access Granted");
	
	
   	token = strtok(buffer, delimiter);
   	
   	ret = strcmp(token,bufcpy1);
  	
  	while( token != NULL ) 
   	{
    		strcpy(tokenBuf,token);
   	   token = strtok(NULL, delimiter);
   	   
  	 }
	
	strcat(retC,tokenBuf);
	if(ret == 0){
		printf("\t\t%s",bufcpy1);
		return retC;
	}
	else{
		printf("\n%s\n",bufcpy3);
		return "2";
	}
	
	
}
