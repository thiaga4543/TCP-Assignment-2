#include <stdio.h>
#include "queue.h"
#include "qoperate.h"
#include <sys/wait.h>


void completeregister();
int main(int argc, char **argv)
{
	pid_t pid;
	int choice;
	choice = atoi(argv[1]);
	
	if(choice == 1){
		
		switch(pid = fork()){
			case 0:
			serve();
			completeRegister();
			removeTemp();
			return 0;
			break;
			case -1:
			warn("Fork Error"); break;
			default:
			waitpid(pid,0,0);
			return 0;
		}
	}
	else if(choice == 2){
		removeMQ();
	}
	else{
		printf("Choice error[%d]\n", choice);
	}
	
	return 0;

}


