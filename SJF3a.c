#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

/************************************************************************************************ 
		These DEFINE statements represent the workload size of each task and 
		the time quantum values for Round Robin scheduling for each task.
*************************************************************************************************/

#define WORKLOAD1 100000
#define WORKLOAD2 50000
#define WORKLOAD3 25000
#define WORKLOAD4 10000

#define QUANTUM1 90000
#define QUANTUM2 90000
#define QUANTUM3 90000
#define QUANTUM4 90000

struct Node {
	pid_t pid;
	int workload;
	struct Node * next;
	};

/************************************************************************************************ 
					DO NOT CHANGE THE FUNCTION IMPLEMENTATION
*************************************************************************************************/
void myfunction(int param){

	int i = 2;
	int j, k;

	while(i < param){
		k = i; 
		for (j = 2; j <= k; j++)
		{
			if (k % j == 0){
				k = k/j;
				j--;
				if (k == 1){
					break;
				}
			}
		}
		i++;
	}
}
/************************************************************************************************/

void main()
{
	pid_t pid1, pid2, pid3, pid4;
	int running1, running2, running3, running4;
	struct timeval start1, start2, start3, start4, end1, end2, end3, end4;		
		
	pid1 = fork();
	gettimeofday(&start1, 0);
	if (pid1 == 0){
		myfunction(WORKLOAD1);
		exit(0);
	}
	kill(pid1, SIGSTOP);

	pid2 = fork();
	gettimeofday(&start2, 0);
	if (pid2 == 0){
		myfunction(WORKLOAD2);
		exit(0);
	}
	kill(pid2, SIGSTOP);

	pid3 = fork();
	gettimeofday(&start3, 0);
	if (pid3 == 0){
		myfunction(WORKLOAD3);
		exit(0);
	}
	kill(pid3, SIGSTOP);

	pid4 = fork();
	gettimeofday(&start4, 0);
	if (pid4 == 0){
		myfunction(WORKLOAD4);
		exit(0);
	}
	kill(pid4, SIGSTOP);

	/************************************************************************************************ 
		At this point, all  newly-created child processes are stopped, and ready for scheduling.
	*************************************************************************************************/



	/************************************************************************************************
		- Scheduling code starts here
		- Below is a sample schedule. (which scheduling algorithm is this?)
		- For the assignment purposes, you have to replace this part with the other scheduling methods 
		to be implemented.
	************************************************************************************************/

	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;
	
	struct timeval start, end;
	double sTime, eTime;
	
	gettimeofday(&start, 0);
	
	struct Node node4 = {pid4, WORKLOAD4, 0};
	struct Node node3 = {pid3, WORKLOAD3, &node4};
	struct Node node2 = {pid2, WORKLOAD2, &node3};
	struct Node node1 = {pid1, WORKLOAD1, &node2};

	struct Node * first = &node1;
	struct Node * tnode1 = first;
	struct Node * tnode2 = first->next;
	int tempwork;
	pid_t temppid;

	for(int i = 3; i > 0; i--){
		for(int j = 0; j < i; j++){
			if(tnode2->workload < tnode1->workload){
				tempwork = tnode1->workload;
				temppid = tnode1->pid;
				tnode1->workload = tnode2->workload;
				tnode1->pid = tnode2->pid;
				tnode2->workload = tempwork;
				tnode2->pid = temppid;
			}
			tnode1 = tnode1->next;
			tnode2 = tnode2->next;
		}
		tnode1 = first;
		tnode2 = first->next;
	}

	while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0)
	{
		if (running1 > 0){
			kill(first->pid, SIGCONT);
			waitpid(first->pid, &running1, 0);
			gettimeofday(&end1, 0);
			printf("Time 1 = %.6f\n", ((double)(end1.tv_sec) + (double)(end1.tv_usec)/1000000) - ((double)(start1.tv_sec) + (double)(start1.tv_usec)/1000000));
		}
		
		first = first->next;
		
		if (running2 > 0){
			kill(first->pid, SIGCONT);
			waitpid(first->pid, &running2, 0);
			gettimeofday(&end2, 0);
			printf("Time 2 = %.6f\n", ((double)(end2.tv_sec) + (double)(end2.tv_usec)/1000000) - ((double)(start2.tv_sec) + (double)(start2.tv_usec)/1000000));
		}
		
		first = first->next;
		
		if (running3 > 0){
			kill(first->pid, SIGCONT);
			waitpid(first->pid, &running3, 0);
			gettimeofday(&end3, 0);
			printf("Time 3 = %.6f\n", ((double)(end3.tv_sec) + (double)(end3.tv_usec)/1000000) - ((double)(start3.tv_sec) + (double)(start3.tv_usec)/1000000));
		}
		
		first = first->next;
		
		if (running4 > 0){
			kill(first->pid, SIGCONT);
			waitpid(first->pid, &running4, 0);
			gettimeofday(&end4, 0);
			printf("Time 4 = %.6f\n\n", ((double)(end4.tv_sec) + (double)(end4.tv_usec)/1000000) - ((double)(start4.tv_sec) + (double)(start4.tv_usec)/1000000));
		}
	}
	
	gettimeofday(&end, 0);
	
	sTime = (double)(start.tv_sec) + (double)(start.tv_usec)/1000000;
	eTime = (double)(end.tv_sec) + (double)(end.tv_usec)/1000000;

	printf("Total elapsed time = %.12f\n\n", eTime - sTime);

	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/

}

