#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int removeMQ(void);

int enter(char *objname, int priority){
	int len, s_qid;
	struct q_entry s_entry;
	
	if( (len = strlen(objname) ) > MAXOBN ){
		warn("invalid priority level");	
	
	return (-1);
}
	
	if( priority > MAXPRIOR || priority < 0 ){
		warn("invalid priority level");
		return (-1);
	}
	
	if( (s_qid = init_queue()) == -1 ){
		warn("invalid priority level");
		return (-1);
	}
	
	
	s_entry.mtype = (long) priority;
	strncpy(s_entry.mtext, objname, MAXOBN);
	
	if(msgsnd(s_qid, &s_entry, len, 0) == -1){
		perror("Msgsnd fails");
		return (-1);
	}
	else
		return (0);
}

int warn (char *s){
	fprintf(stderr, "Warning: %s\n", s);
}

int init_queue (void){
	int queue_id;
	
	if( (queue_id = msgget (ftok("\temp", 9), IPC_CREAT | QPERM) ) == -1 )
		perror("Msgget fails");
	
	return (queue_id);
}

int serve (void){
	int mlen, r_qid;
	struct q_entry r_entry;
	
	if( (r_qid = init_queue() ) == -1 )
		return (-1);
	
	for (;;)
	{
		if ( (mlen = msgrcv (r_qid, &r_entry, MAXOBN, (-1* MAXPRIOR* 0), IPC_NOWAIT) ) == -1 ){
			if(errno == ENOMSG){
				removeMQ();
			}
			return 0;
		}
		else{
			r_entry.mtext[mlen] = '\0';
			proc_obj (&r_entry);
		}
	}
	return 0;
}

int proc_obj (struct q_entry *msg){
	int fd;
	FILE *ff;
	char buf[100];
	
	ff = fopen("./user/temp.txt","a+");
	fprintf(ff,"%s",msg->mtext);
	fclose(ff);
	
}

int removeMQ(void){

	int queueID;
	queueID = msgget(ftok("/temp",9), IPC_CREAT | QPERM);
	if(msgctl(queueID, IPC_RMID, (struct msqid_ds*)0)<0)
		perror("MSG queue remove fails");

	return 0;
}

int removeTemp(void){
	FILE *infile;
	infile = fopen("./user/temp.txt", "rt"); 
	remove("./user/temp.txt");
	return 0;
}

int completeRegister(){
	
	FILE *infile,*infile2,*infile3,*infile4,*fileD; 
	char line_buffer[512],line_buffer2[512],*token;
	char fileDCPY[256];
	char line_number; 
	char prd[100] = "./fileDirectory/";
	char ext[100] = "./user/";
	char userDir[100] = "./userDirectory/";
	char createDir[100];
	char bufcpy[100],*ex;
	
	infile = fopen("./user/temp.txt", "rt"); 
	if (!infile) { 
		printf("Couldn't open file for reading.\n"); 
		
	}
	
	line_number = 0; 
	while (fgets(line_buffer, sizeof(line_buffer), infile) != NULL) { 
	
		++line_number; 
		strcpy(line_buffer2,line_buffer);
		
		if(line_number == 1){

			line_buffer[strlen(line_buffer)-1] = '\0';
			
			strcat(ext,line_buffer);
			infile2 = fopen(ext,"a+");
			
			token = strtok(line_buffer2, ".");
			fprintf(infile2,"%s\n",token);

		}
		else if(line_number == 2){
			line_buffer2[strlen(line_buffer2)-1] = '\0';
			strcpy(bufcpy,line_buffer2);
			ex = ".txt";
			strcat(bufcpy,ex);
			strcpy(fileDCPY,bufcpy);
			infile3 = fopen("./login/login.txt","a+");
			strcat(userDir,line_buffer2);
			strcpy(createDir,"mkdir -p ");
			strcat(createDir,userDir);
			system(createDir);
			
			fprintf(infile3,"%s ",line_buffer2);
			fprintf(infile2,"%s\n",line_buffer2);
		}
		else if(line_number == 3){
			line_buffer2[strlen(line_buffer2)-1] = '\0';
			fprintf(infile3,"%s\n",line_buffer2);
			
			fprintf(infile2,"%s\n",line_buffer2);
		}
		else if(line_number== 4){
			line_buffer2[strlen(line_buffer2)-1] = '\0';
			
			strcat(prd,fileDCPY);
			fileD = fopen(prd,"a+");
			
			fprintf(fileD,"%s\n",line_buffer2);
			fprintf(infile2,"%s\n",line_buffer2);
		}
		else{
			fprintf(infile2,"%s",line_buffer2);
		}
		 
	}
	return 0;

}


