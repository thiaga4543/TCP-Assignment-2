#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define SEMPERM 0600
#define TRUE 1
#define FALSE 0

int initsem(key_t semkey);
int p(int semid);
int v(int semid);
int handlesem(key_t skey);

typedef union _semun{
	int val;
	struct semid_ds *buf;
	ushort *array;
}semun;


void rvobj(int semid){
	if(semctl(semid,0,IPC_RMID,NULL) == -1)
		perror("\n semctl fails");
}
	 


int handlesem(key_t skey){
	int semid;
	pid_t pid = getpid();
	
	if( (semid = initsem(skey)) < 0 )
		exit(1);
	return semid;
}

int initsem(key_t semkey){
	int status = 0, semid;
	semun arg;
	
	if ((semid = semget(semkey,1,SEMPERM | IPC_CREAT | IPC_EXCL)) == -1){
		if(errno == EEXIST){
			semid = semget(semkey,1,0);
			}
	}
	else{
		arg.val = 1;
		status = semctl(semid,0,SETVAL,arg);
		
	
	}
	if(semid == -1 || status == -1){
		perror("Initsem() fails");
		return(-1);
	}
	return(semid);
}

int p(int semid){
	struct sembuf p_buf;
	p_buf.sem_num = 0;
	p_buf.sem_op = -1;
	p_buf.sem_flg = SEM_UNDO;
	
	if(semop(semid,&p_buf,1) == -1){
		perror("P() fails");
		exit(1);
	}
	return(0);
}

int v(int semid){
	struct sembuf v_buf;
	v_buf.sem_num = 0;
	v_buf.sem_op = 1;
	v_buf.sem_flg = SEM_UNDO;
	
	if(semop(semid,&v_buf,1) == -1){
		perror("V() fails");
		exit(1);
	}
	return(0);
}
