#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include "clogin.h"
#include "inet.h"

#define BUFSIZE 1024

void reg(int , char [],int );
void mainmenu();
void mainmenu2();

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main (int argc,char *argv[]){
int sockfd,count=0,n,i,portNo;
 char choice[10];
char buffer[BUFSIZE+1];
struct sockaddr_in serv_addr;

    //signal for registration
sigset_t set1;
sigfillset(&set1);

bzero( (char *) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family =AF_INET;
serv_addr.sin_port = htons(SERV_TCP_PORT);
inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

if( (sockfd = socket(AF_INET, SOCK_STREAM,0)) <0)
    perror("\nsoccket() error\n");

if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0){
      perror("\nconnect() error\n");
}
else{	
	system("clear");
	printf("\nConnection with server: %s...\n", inet_ntoa(serv_addr.sin_addr));

    n = send(sockfd," ",30,0);

        mainmenu();
    	printf("Enter option --> ");
        fgets(choice,10,stdin);

        	n = send(sockfd,choice,strlen(choice),0);
        	if (n < 0)
         		error("Writing error to socket");

        while(1){
        	if(atoi(choice) == 1 && count == 0){
        		++count;
    
			printf("Please Wait!!!\n");
			sigprocmask(SIG_BLOCK, &set1, (void *)0);
	 		for(i=0;i<5;i++){
	 			reg(sockfd,buffer,i);
 			}
			sigprocmask (SIG_UNBLOCK, &set1, (void *)0);
			
			//printf("\nUser Registration Successfull\n\n");

	 		 bzero(choice,256);
	 		mainmenu2();
    			printf("Enter option --> ");
    			fgets(choice,10,stdin);
    			continue;
        	}
        	else if(atoi(choice) == 2){

        		if(count == 1){
        			
        			char *UNAME;
        			const char delimiter[2] = "0";

        			n = send(sockfd,choice,strlen(choice),0);
        			if (n < 0)
         			error("Writing error to socket");
        			
        			char *rtn = clogin(sockfd);

        			if(strlen(rtn) > 1){
        				UNAME = strtok(rtn, delimiter);
        				inAccess(sockfd,UNAME);
        				}
        			else{
        				exit(0);
        			}

        			if(strcmp(rtn,"3")==0){
        				mainmenu2();
    					printf("Enter option --> ");
    					fgets(choice,10,stdin);
        			}
        			else
        				exit(0);

        		}
        		else{
        			char *UNAME;
        			const char delimiter[2] = "0";
        			
        			char *rtn =clogin(sockfd);
        			
        			if(strlen(rtn) > 1){
        				UNAME = strtok(rtn, delimiter);
        				inAccess(sockfd,UNAME);
        				}
        			else{
        				exit(0);
        			}

        			if(strcmp(rtn,"3")==0){
        				mainmenu2();
    					printf("Enter option --> ");
    					fgets(choice,10,stdin);
        			}
        			else
        				exit(0);
        		}

        	}
        	else if(atoi(choice) == 0){

        		n = send(sockfd,choice,1,0);
        		if (n < 0)
         			error("Writing error to socket");
        		sleep(2);
        		exit(0);

        	}
		else{
			if(count == 0){
       				mainmenu();
    				printf("Enter option --> ");
    				fgets(choice,10,stdin);
    			}
    			if(count != 0){
       				mainmenu2();
    				printf("Enter option --> ");
    				fgets(choice,10,stdin);
    			}
       	 	}
    	}
}

return 0;
}

int inAccess(int sockfd, char *UNAME){

	char choice[10],buffer[255], Uname[50],Uname2[50];
	int n,i;
	FILE *accessFile;
char line_buffer[1024];

	system("clear");
	printf("\n~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
	printf("[5] --> Create Directory\n");
	printf("[6] --> Copy Directory\n");
	printf("[7] --> Delete Directory\n");
	printf("[8] --> Upload File\n");
	printf("[9] --> Download FIle\n");
	printf("[0] --> Logout\n");
	printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
	
	strcpy(Uname,UNAME);
	
	printf("Enter option --> ");
	fgets(choice,10,stdin);
	
	if(atoi(choice) == 5){
		n = send(sockfd,choice,1,0);
		if (n < 0)
			error("Writing error to socket");
			
		system("clear");
		
		char buffDirName[255],buffDirPermission[10];
		
		printf("\n~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
		printf("~+~+~+~+~+~+~+ Create Directory +~+~+~+~+~+~+~+\n");
		printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
		printf("Please enter directory name: ");
		fgets(buffDirName,255,stdin);
		
		//this is for send username to create directory
		n = send(sockfd,buffDirName,255,0);
		strcpy(Uname2,"U"); //to eliminate first charater in username 
		strcat(Uname2,Uname);
		n = send(sockfd,Uname2,255,0);
		
		bzero(buffer,256);
		n = recv(sockfd,buffer,256,0);
		printf("\n%s\n", buffer);
		sleep(3);
		system("clear");
		inAccess(sockfd, UNAME);
		
	}
	else if(atoi(choice) == 6){
		n = send(sockfd,choice,1,0);
		if (n < 0)
			error("Writing error to socket");
			
		system("clear");
		
		char buffDirName[255],buffDirPermission[10];
		
		printf("\n~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
		printf("~+~+~+~+~+~+~+~ Copy Directory ~+~+~+~+~+~+~+~+\n");
		printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
		printf("List of directory(s) :-\n");
		n = send(sockfd,Uname,255,0);
		
		while(n!=3){
			if(n!=3){
				bzero(buffer,256);
				n = recv(sockfd,buffer,256,0);
				printf("--> %s", buffer);
			}
		}
		printf("\n\nPlease enter directory name to copy: ");
		fgets(buffDirName,255,stdin);
		n = send(sockfd,buffDirName,255,0);
		
		bzero(buffer,256);
		n = recv(sockfd,buffer,256,0);
		printf("\n%s\n", buffer);
		if(n==18){
			printf("Please enter correct directory name.\n");
		}
		sleep(3);
		system("clear");
		inAccess(sockfd, UNAME);
		
	}
	else if(atoi(choice) == 7){
		n = send(sockfd,choice,1,0);
		if (n < 0)
			error("Writing error to socket");
			
		system("clear");
		
		char buffDirName[255],buffDirPermission[10];
		
		printf("\n~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
		printf("~+~+~+~+~+~+~+ Delete Directory +~+~+~+~+~+~+~+\n");
		printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
		printf("List of directory(s) :-\n");
		n = send(sockfd,Uname,255,0);
		
		while(n!=3){
			if(n!=3){
				bzero(buffer,256);
				n = recv(sockfd,buffer,256,0);
				printf("--> %s", buffer);
			}
		}
		printf("\n\nPlease enter directory name to delete: ");
		fgets(buffDirName,255,stdin);
		n = send(sockfd,buffDirName,255,0);
		
		bzero(buffer,256);
		n = recv(sockfd,buffer,256,0);
		printf("\n%s\n", buffer);
		if(n==18){
			printf("Please enter correct directory name.\n");
		}
		
		sleep(3);
		system("clear");
		inAccess(sockfd, UNAME);
		
	}
	else if(atoi(choice) == 8){
		n = send(sockfd,choice,1,0);
		if (n < 0)
			error("Writing error to socket");
			
		system("clear");
		
		char fileChoice[5],buffDirName[255];
		FILE *fl;
		char uploadDir[100] = "./upload/";
		char linebuffer[1024];
			
			
		printf("\n~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
		printf("~+~+~+~+~+~+~+~+~ Upload File +~+~+~+~+~+~+~+~+\n");
		printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
		printf("List of files to upload :-\n\n");
		
		printf("[1] File 1: upload1.txt\n");
		printf("[2] File 2: upload2.txt\n");
		printf("[3] File 3: upload3.txt\n");
		printf("[4] File 4: upload4.txt\n");
		printf("[5] File 5: upload5.txt\n\n");
		
		printf("Enter file number to upload: ");
		fgets(fileChoice,5,stdin);
		
		printf("\nList of directory(s) :-\n");
		
		n = send(sockfd,Uname,255,0);
		
		while(n!=3){
			if(n!=3){
				bzero(buffer,256);
				n = recv(sockfd,buffer,256,0);
				printf("--> %s", buffer);
			}

		}
		
		printf("\n\nPlease enter directory name to upload file: ");
		fgets(buffDirName,255,stdin);
		n = send(sockfd,buffDirName,255,0);
		
		if(atoi(fileChoice) == 1){
			n = send(sockfd,"upload1.txt",255,0);
			bzero(buffer,256);
			n = recv(sockfd,buffer,256,0);
			if(n==18){
				printf("Please enter correct directory name.\n");
			}
			else{
				printf("\n%s\n", buffer);
				fl = fopen("./upload/upload1.txt","r+");
				while (fgets(linebuffer, sizeof(linebuffer), fl) != NULL) {
						send(sockfd,linebuffer,BUFSIZE,0);
						bzero(buffer, sizeof(line_buffer));
						recv(sockfd,linebuffer, BUFSIZE,0);
				}
				fclose(fl);
				send(sockfd," ",1,0);
				bzero(buffer,256);
				n = recv(sockfd,buffer,256,0);
				printf("%s\n", buffer);
				
			}
			sleep(2);
			system("clear");
			inAccess(sockfd, UNAME);
			
		}
		else if(atoi(fileChoice) == 2){
			n = send(sockfd,"upload2.txt",255,0);
			bzero(buffer,256);
			n = recv(sockfd,buffer,256,0);
			if(n==18){
				printf("Please enter correct directory name.\n");
			}
			else{
				printf("\n%s\n", buffer);
				fl = fopen("./upload/upload2.txt","r+");
				while (fgets(linebuffer, sizeof(linebuffer), fl) != NULL) {
						send(sockfd,linebuffer,BUFSIZE,0);
						bzero(buffer, sizeof(line_buffer));
						recv(sockfd,linebuffer, BUFSIZE,0);
				}
				fclose(fl);
				send(sockfd," ",1,0);
				bzero(buffer,256);
				n = recv(sockfd,buffer,256,0);
				printf("%s\n", buffer);
			}
			sleep(2);
			system("clear");
			inAccess(sockfd, UNAME);
			
		}
		else if(atoi(fileChoice) == 3){
			n = send(sockfd,"upload3.txt",255,0);
			bzero(buffer,256);
			n = recv(sockfd,buffer,256,0);
			if(n==18){
				printf("Please enter correct directory name.\n");
			}
			else{
				printf("\n%s\n", buffer);
				fl = fopen("./upload/upload3.txt","r+");
				while (fgets(linebuffer, sizeof(linebuffer), fl) != NULL) {
						send(sockfd,linebuffer,BUFSIZE,0);
						bzero(buffer, sizeof(line_buffer));
						recv(sockfd,linebuffer, BUFSIZE,0);
				}
				fclose(fl);
				send(sockfd," ",1,0);
				bzero(buffer,256);
				n = recv(sockfd,buffer,256,0);
				printf("%s\n", buffer);
			}
			sleep(2);
			system("clear");
			inAccess(sockfd, UNAME);
			
		}
		else if(atoi(fileChoice) == 4){
			n = send(sockfd,"upload4.txt",255,0);
			bzero(buffer,256);
			n = recv(sockfd,buffer,256,0);
			if(n==18){
				printf("Please enter correct directory name.\n");
			}
			else{
				printf("\n%s\n", buffer);
				fl = fopen("./upload/upload4.txt","r+");
				while (fgets(linebuffer, sizeof(linebuffer), fl) != NULL) {
						send(sockfd,linebuffer,BUFSIZE,0);
						bzero(buffer, sizeof(line_buffer));
						recv(sockfd,linebuffer, BUFSIZE,0);
				}
				fclose(fl);
				send(sockfd," ",1,0);
				bzero(buffer,256);
				n = recv(sockfd,buffer,256,0);
				printf("%s\n", buffer);
				
			}
			sleep(2);
			system("clear");
			inAccess(sockfd, UNAME);
			
		}
		else if(atoi(fileChoice) == 5){
			n = send(sockfd,"upload5.txt",255,0);
			bzero(buffer,256);
			n = recv(sockfd,buffer,256,0);
			if(n==18){
				printf("Please enter correct directory name.\n");
			}
			else{
				printf("\n%s\n", buffer);
				fl = fopen("./upload/upload5.txt","r+");
				while (fgets(linebuffer, sizeof(linebuffer), fl) != NULL) {
						send(sockfd,linebuffer,BUFSIZE,0);
						bzero(buffer, sizeof(line_buffer));
						recv(sockfd,linebuffer, BUFSIZE,0);
				}
				fclose(fl);
				send(sockfd," ",1,0);
				bzero(buffer,256);
				n = recv(sockfd,buffer,256,0);
				printf("%s\n", buffer);
				
			}
			sleep(2);
			system("clear");
			inAccess(sockfd, UNAME);
			
		}
		else{
			printf("\nWrong file choice! Enter Correct filename.\n");
			sleep(2);
			system("clear");
			inAccess(sockfd, UNAME);
		}

	}
	else if(atoi(choice) == 9){
		n = send(sockfd,choice,1,0);
		if (n < 0)
			error("Writing error to socket");
			
		system("clear");
		
		char fileChoice[5],buffDirName[255];
		FILE *fl;
		char downloadDir[100] = "./download/";
		char linebuffer[1024];
			
		printf("\n~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
		printf("~+~+~+~+~+~+~+~+ Download File ~+~+~+~+~+~+~+~+\n");
		printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
		
		printf("\nList of File(s) :-\n");
		
		n = send(sockfd,Uname,255,0);
		
		int fileCount=0;
		while(n!=11){
			if(n!=11){
				++fileCount;
				bzero(buffer,256);
				n = recv(sockfd,buffer,256,0);
				printf("--> [%d] %s",fileCount, buffer);
			}
		}
		
		printf("\nEnter file number to download: ");
		fgets(fileChoice,5,stdin);
		
		if(atoi(fileChoice) < 1 || atoi(fileChoice) > fileCount){
			printf("\nWrong file choice! Enter Correct file number.\n");
			sleep(2);
			system("clear");
			inAccess(sockfd, UNAME);
		}
		else{
			n = send(sockfd,fileChoice,255,0);
			printf("\nDownloading...!\n");
			char downloadDir[100] = "./download/";
			
			bzero(linebuffer, 500);
			n= recv(sockfd,linebuffer, 500,0); //receive folder name for setting download location
			linebuffer[strlen(linebuffer)-1] = '\0';
			
			strcat(downloadDir,linebuffer);
			strcat(downloadDir,"-");
			
			bzero(linebuffer, 500);
			n= recv(sockfd,linebuffer, 500,0); //receive file name for setting download location
			linebuffer[strlen(linebuffer)-1] = '\0';
			
			
			strcat(downloadDir,linebuffer);
			
			fl = fopen(downloadDir,"a+");
			if (!fl) { 
				printf("Couldn't open file for create or append.\n"); 
			}
			while(1){
					bzero(linebuffer, 500);
					n= recv(sockfd,linebuffer, 500,0);
					if(n==1)
						break;
					fprintf(fl,"%s",linebuffer);
					send(sockfd, linebuffer,500,0);
					fclose(fl);
					fl = fopen(downloadDir,"a+");
			}
			fclose(fl);
			printf("\nDownload Successfull...\n");
			sleep(2);
			system("clear");
			inAccess(sockfd, UNAME);
		}
		
	}
	else if(atoi(choice) == 0){
		n = send(sockfd,choice,1,0);
		if (n < 0)
			error("Writing error to socket");
	}
	else
	{
		printf("Wrong choice!\n");
		inAccess(sockfd, UNAME);
	}
	
	return 0;
}

void reg(int sockfd, char buffer[256],int i){

	int n;
	char buff[10];
	bzero(buffer,256);
        n = recv(sockfd,buffer,255,0);

    	if (n < 0)
         	error("Reading error from socket");
  	printf("%s",buffer);

	bzero(buffer,256);
	if(i==3){
		strcpy(buff," ");
		n = send(sockfd,buff,strlen(buff),0);

		if (n < 0)
         		error("Writing error to socket");
	}
	else{
		fgets(buffer,255,stdin);
		n = send(sockfd,buffer,strlen(buffer),0);
		if (n < 0)
			error("Writing error to socket");
	}

}

void mainmenu(){
        printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
        printf("~+~+~+~+~+~ File Repository System +~+~+~+~+~+~\n");
        printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
        printf("[1] --> Registration\n");
        printf("[2] --> Login\n");
        printf("[0] --> Exit\n");
        printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
}
void mainmenu2(){
        printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
        printf("~+~+~+~+~+~ File Repository System +~+~+~+~+~+~\n");
        printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
        printf("[2] --> Login\n");
        printf("[0] --> Exit\n");
        printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
}
