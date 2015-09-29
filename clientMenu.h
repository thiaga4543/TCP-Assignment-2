#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include "register.h"
#include "slogin.h"

char *fif = "fifofile";

int cMenu(int sock)
{
	int n,choice,pid,rturn,i,count=0,fd;
	char buffer[256],buf[256];
	
	char line_buffer[1024];
	int childpid;
		
	if( (fd = open(fif, O_WRONLY | O_NONBLOCK)) <0)
		printf("FIFO open fail\n");
		
	write(fd,"0",2);

	bzero(buffer,256);
	n = recv(sock,buffer,1,0);
	if (n < 0){
		error("Reading error from socket");
		exit(0);
	}
	choice = atoi(buffer);
	
	for(;;){
		if(choice == 1){
				++count;
				 regist(sock);
		}
		else if(choice == 2){
			if(count == 1){
				slogin(sock,count);
			}
			else{
				bzero(buffer,256);
				n = recv(sock,buffer,255,0);
				
				slogin(sock,count);
				
			}
			
		}
		else if(choice == 0){
			printf("Client from PC %d disconnected!!!--\n", sock);
			
			close(sock);
			write(fd,"3",2);
			break;
			
		}
		else if(choice == 5){
			char fileDir[100] = "./userDirectory/";
			char listDir[100] = "./fileDirectory/";
			char createDir[20],dirUname[255];
			FILE *dirList;
			
			bzero(buffer,256);
			n = recv(sock,buffer,256,0);
			n = recv(sock,dirUname,256,0);
			
			strcat(listDir,dirUname);
			strcat(fileDir,dirUname);
			
			strcat(fileDir,"/");
			strcat(listDir,".txt");
			dirList = fopen(listDir,"a+");
			if (!dirList) { 
				printf("Couldn't open file for append.\n"); 
			}
			fprintf(dirList,"%s",buffer); 
			
			strcat(fileDir,buffer);
			
			strcpy(createDir,"mkdir -p ");
			strcat(createDir,fileDir);
			
			system(createDir);
			fclose(dirList);
			
			n = send(sock,"Directory created!",256,0);	
			if (n < 0){
				error("Reading error from socket");
			}
			
		}
		else if(choice == 6){
			FILE *dirname,*fl;
			char fileDir[100] = "./userDirectory/";
			char listDir[100] = "./fileDirectory/";
			char createDir[20],dirUname[255],bufDir[256],fileCopy[256];
			char line_buffer[512];
			char line_number;
			char *ret,*ret2; 
			
			
			n = recv(sock,dirUname,256,0);
			strcat(listDir,dirUname);
			strcat(listDir,".txt");
			
			dirname = fopen(listDir, "rt"); 
			if (!dirname) { 
				printf("Couldn't open file for reading.\n"); 
			}
			
			line_number = 0; 
			while (fgets(line_buffer, sizeof(line_buffer), dirname) != NULL) { 
				++line_number; 
				if(line_number != 1){
					n = send(sock,line_buffer,256,0);
				}
			}
			n = send(sock,"END",3,0);
			fclose(dirname);
			
			//to check directory before copy it
			dirname = fopen(listDir, "a+"); 
			if (!dirname) { 
				printf("Couldn't open file for reading.\n"); 
			}
			n = recv(sock,bufDir,256,0);
			line_number = 0; 
			bzero(line_buffer,512);
			while (fgets(line_buffer, sizeof(line_buffer), dirname) != NULL) { 
				++line_number; 
				
				if(line_number != 1){
					ret = strstr(line_buffer,bufDir);
					
					if(ret != NULL){ 
						strcat(fileDir,dirUname);
						strcat(fileDir,"/");
						strcat(fileDir,ret);
						strcpy(fileCopy,fileDir);
						fileCopy[strlen(fileCopy)-1] = '\0';
						strcat(fileCopy,"-copy");

						strcpy(createDir,"cp -r ");
						fileDir[strlen(fileDir)-1] = '\0';
						strcat(createDir,fileDir);
						strcat(createDir," ");
						strcat(createDir,fileCopy);
		
						system(createDir);
						
						ret[strlen(ret)-1] = '\0';
						strcat(ret,"-copy");
						fprintf(dirname,"%s\n",ret);
						
						n = send(sock,"Directory is copied!!!",25,0);
						//directory that client mention is copied 
						
					}
				}
			}
			fclose(dirname);
			if(ret == NULL)
				n = send(sock,"Invalid directory!",18,0);
		}
		else if(choice == 7){
			FILE *dirname,*fl;
			char fileDir[100] = "./userDirectory/";
			char listDir[100] = "./fileDirectory/";
			char createDir[20],dirUname[255],bufDir[256];
			char line_buffer[512];
			char line_number;
			char *ret; 
			
			
			n = recv(sock,dirUname,256,0);
			strcat(listDir,dirUname);
			strcat(listDir,".txt");
			
			dirname = fopen(listDir, "rt"); 
			if (!dirname) { 
				printf("Couldn't open file for reading.\n"); 
			}
			
			line_number = 0; 
			while (fgets(line_buffer, sizeof(line_buffer), dirname) != NULL) { 
				++line_number; 
				if(line_number != 1){
					n = send(sock,line_buffer,256,0);
				}
			}
			n = send(sock,"END",3,0);
			fclose(dirname);
			
			//to check directory before delete it
			dirname = fopen(listDir, "rt"); 
			if (!dirname) { 
				printf("Couldn't open file for reading.\n"); 
			}
			n = recv(sock,bufDir,256,0);
			line_number = 0; 
			bzero(line_buffer,512);
			while (fgets(line_buffer, sizeof(line_buffer), dirname) != NULL) { 
				++line_number; 
				
				if(line_number != 1){
					ret = strstr(line_buffer,bufDir);
					
					if(ret != NULL){ 
						strcat(fileDir,dirUname);
						strcat(fileDir,"/");
						strcat(fileDir,ret);
						strcpy(createDir,"rm -rf ");
						strcat(createDir,fileDir);
						system(createDir);
						
						n = send(sock,"Directory is deleted!!!",25,0);
						//directory that client mention is deleted 
						
					}
					else
					{
						removeDir(listDir,line_buffer);
					}
					
				}
				else{
					removeDir(listDir,line_buffer);
				}
			}
			fclose(dirname);
			remove(listDir);
    			//rename the file replica.c to original name
   			rename("replica.c", listDir);
			if(ret == NULL)
				n = send(sock,"Invalid directory!",18,0);
		}
		else if(choice == 8){
			FILE *dirname,*fl,*fl2,*fl3;
			char uploadFullPath[255];
			char listDir[100] = "./fileDirectory/";
			char createDir[20],dirUname[255],bufDir[256],uploadName[50],fileList[50],fileListPath[50];
			char line_buffer[512],buff[1024],downloadList[50];
			char line_number;
			char *ret; 
			int foundDir=0;
			
			n = recv(sock,dirUname,256,0);
			strcat(listDir,dirUname);
			strcat(listDir,".txt");
			
			dirname = fopen(listDir, "rt"); 
			if (!dirname) { 
				printf("Couldn't open file for reading.\n"); 
			}
			
			line_number = 0; 
			while (fgets(line_buffer, sizeof(line_buffer), dirname) != NULL) { 
				++line_number; 
				if(line_number != 1){
					n = send(sock,line_buffer,256,0);
				}
			}
			n = send(sock,"END",3,0);
			fclose(dirname);
			
			//to check directory before delete it
			dirname = fopen(listDir, "rt"); 
			if (!dirname) { 
				printf("Couldn't open file for reading.\n"); 
			}
			n = recv(sock,bufDir,256,0);
			n = recv(sock,uploadName,256,0); //to store upload filename
			line_number = 0; 
			bzero(line_buffer,512);
			while (fgets(line_buffer, sizeof(line_buffer), dirname) != NULL) { 
				++line_number; 
				
				if(line_number != 1){
					ret = strstr(line_buffer,bufDir);
					
					if(ret != NULL){
						foundDir=1;
						char fDir[200] = "./userDirectory/"; 
						strcat(fDir,dirUname);
						strcat(fDir,"/");
						strcpy(fileList,fDir);
						strcpy(fileListPath,fDir);
						ret[strlen(ret)-1] = '\0';
						strcat(fDir,ret);
						strcat(fDir,"/");
						
						strcat(fileList,"filelist.txt");
						fl2 = fopen(fileList,"a+");
						if (!fl2) { 
							printf("Couldn't open file for create or append.\n"); 
						}
						strcpy(downloadList,uploadName);
						strcat(downloadList," is at folder - ");
						strcat(downloadList,ret);
						fprintf(fl2,"%s\n",downloadList);
						fclose(fl2);
						strcat(fDir,uploadName);
						strcpy(uploadFullPath,fDir);
						strcat(fileListPath,"filelistPath.txt");
						fl3 = fopen(fileListPath,"a+");
						if (!fl3) { 
							printf("Couldn't open file for create or append.\n"); 
						}
						fprintf(fl3,"%s\n",uploadFullPath);
						fclose(fl3);

						break;
					}
				}
			}
			fclose(dirname);
			
			if(foundDir==1){
				n = send(sock,"File uploading...",25,0);
				
				fl = fopen(uploadFullPath,"a+");
				if (!fl) { 
					printf("Couldn't open file for create or append.\n"); 
				}
			
				do{
						bzero(buff, 1024);
						n= recv(sock,buff, 1024,0);
						if(n==1)
							break;
						fprintf(fl,"%s",buff);
						send(sock, buff,1024,0);
						fclose(fl);
						fl = fopen(uploadFullPath,"a+");
				}while(n!=1);
				n = send(sock,"Successfull uploaded...",50,0);
			}
			else{
				n = send(sock,"Invalid directory!",18,0);
			}
		}
		else if(choice == 9){
			FILE *dirname,*fl,*fl2;
			char uploadFullPath[255],FDir[200] = "./userDirectory/",FDir2[200] = "./userDirectory/";
			char listDir[100] = "./fileDirectory/";
			char createDir[20],dirUname[255],bufDir[256],uploadName[50],fileList[50];
			char line_buffer[512],line_buffer2[512],buff[500],linebuffer[512],splitTok[20];
			char line_number;
			const char splitToken[2] = "/";
			char *ret,*token; 
			int foundDir=0;
			
			n = recv(sock,dirUname,256,0);
			strcat(FDir,dirUname);
			strcat(FDir2,dirUname);
			strcat(FDir,"/filelist.txt");
			strcat(FDir2,"/filelistPath.txt");
			
			dirname = fopen(FDir, "rt"); 
			if (!dirname) { 
				printf("Couldn't open file for reading.\n"); 
			}
			
			line_number = 0; 
			while (fgets(line_buffer, sizeof(line_buffer), dirname) != NULL) { 
					n = send(sock,line_buffer,256,0);
			}
			n = send(sock,"END OF FILE",11,0);
			fclose(dirname);
			
			bzero(buff, 500);
			n= recv(sock,buff, 500,0);
			
			dirname = fopen(FDir2, "rt"); 
			if (!dirname) { 
				printf("Couldn't open file for reading.\n"); 
			}
			
			line_number = 0; 
			while (fgets(line_buffer, sizeof(line_buffer), dirname) != NULL) { 
				++line_number; 
				if(line_number == atoi(buff)){
					strcpy(line_buffer2,line_buffer);
					
					//to get filename
					token = strtok(line_buffer, splitToken);
					while( token != NULL ) 
					{
						strcpy(splitTok,token);
						token = strtok(NULL, splitToken);
					}
					
					strcat(dirUname,"x");
					send(sock,dirUname,500,0); //send folder name for download purpose
					send(sock,splitTok,500,0); //send file name for download purpose
					
					line_buffer2[strlen(line_buffer2)-1] = '\0';
					fl = fopen(line_buffer2,"r+");
					if (!fl) { 
						printf("Couldn't open file for reading..\n"); 
					}
					while (fgets(linebuffer, sizeof(linebuffer), fl) != NULL) {
							send(sock,linebuffer,500,0);
							bzero(buffer, sizeof(linebuffer));
							recv(sock,linebuffer, 500,0);
					}
					fclose(fl);
					send(sock," ",1,0);
					break;
				}
			}
			fclose(dirname);
		}
		
		
		bzero(buffer,256);
		n = recv(sock,buffer,256,0);	
		if (n < 0){
			error("Reading error from socket");
		}
		choice = atoi(buffer);
		
	}
	write(fd,"3",2);
}


//remove Dir
int removeDir(char listDir[],char content[]){
	FILE *fl, *fl2;
	char ch;
	char ln_buf[100];
	int delete_line, temp = 1,line=0;

    //open new file in write mode
    fl2 = fopen("replica.c", "a+");
    fprintf(fl2,"%s",content); 
    
    fclose(fl2);

    return 0;

}
