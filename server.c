#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include "inet.h"
#include "clientMenu.h"
#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFSIZE 1024
int i;

void Mainmenu();

char *fifo = "fifofile";

int main(int argc, char** argv){
fd_set master;
fd_set read_fds,write_fds;
struct sockaddr_in myaddr;
struct sockaddr_in remoteaddr;
int fdmax,sockfd,new_sockfd,nbytes,addrlen,j,portNo,pid,ret=-1,fd;
char buffer[BUFSIZE];
char buf[BUFSIZE]="abcd";
int yes=1;
int PID;

FD_ZERO(&master); 
FD_ZERO(&read_fds);
FD_ZERO(&write_fds);

if(mkfifo(fifo,0666) == -1 && errno != EEXIST){
	printf("MKFIFO() error!!!\n");
}

if( (sockfd = socket(AF_INET, SOCK_STREAM, 0) )== -1) {
    printf("\nsocket() error!!!\n");
    exit(1);
}

if(setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(int) ) == -1){
    printf("\nSetsockopt() error!!!\n");
    exit(1);
 }

bzero( (char *)&myaddr, sizeof(myaddr) );
myaddr.sin_family =AF_INET;
//myaddr.sin_addr.s_addr=  inet_addr("10.0.92.22");
myaddr.sin_addr.s_addr= INADDR_ANY;
myaddr.sin_port = htons(SERV_TCP_PORT);
bzero(&(myaddr.sin_zero), 8);




if( bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr))== -1){
    printf("\nbind() error!!!\n");
    exit(1);
}

if((pid=fork())==0){

    if(listen(sockfd,10) == -1){
    printf("\nlisten() error!!!\n");
    exit(1);
}

FD_SET(sockfd,&master);
fdmax = sockfd;

for(;;){
    read_fds = master;
    write_fds = master;
    if(select(fdmax+1,&read_fds,&write_fds,(fd_set *)0,(fd_set *)0) == -1){
        printf("\nselect() error!!!\n");
        exit(1);
    }
    for(i=0;i<=fdmax; i++){
       if( FD_ISSET(i,&read_fds) || FD_ISSET(i,&write_fds)){
            if(i == sockfd) {
                addrlen = sizeof(remoteaddr);
                if( (new_sockfd = accept(sockfd, (struct sockaddr *)&remoteaddr, &addrlen)) == -1) {
                    printf("\naccept() error!!!\n");
                }
                else {
                    FD_SET(new_sockfd,&master);
                    if(new_sockfd> fdmax){
                        fdmax = new_sockfd;
                    }
                    printf("\nNew connection from client: %s\n", inet_ntoa(remoteaddr.sin_addr));
                } 
            }
            else{
                 bzero(buffer,BUFSIZE);
                if( (nbytes = recv(i,buffer, sizeof(buffer), 0)) <= 0){
                    if(nbytes ==0){
                        printf("selectserver: socket %d hung up\n", i);
                    }
                    else{
                        printf("recv(): error\n");
                    }
                    close(i);
                    FD_CLR(i, &master);
                }
                else{

                     FD_CLR(i, &master);
                    if((PID=fork()) == 0){
                       cMenu(i);
                       
                    }
                    
                    if( (fd = open(fifo,O_RDWR))<0)
                    	printf("FIFO fails to open!!!\n");
                 
                    if( read(fd,buf,BUFSIZE-1) <=0){
                    	printf("FIFO reading fails!!!\n");
                   	continue;
                   }
                   ret = atoi(buf);
                   if(ret == 3){
                   	close(i);
                   }
                  bzero(buf,256);
                   ret=-1;
                    
                }
                
                }
            }
        }
    }

}

if(pid >0 ){
	
	while(1){
			system("clear");
			printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n");
			printf("~+~+~+~+~+~ File Repository System +~+~+~+~+~+~\n");
			printf("~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~\n\n");
			printf("Server is running....\n");
			getchar();
			system("clear");
	}
	return 0;

}

}


