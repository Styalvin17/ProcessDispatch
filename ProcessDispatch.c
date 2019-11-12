#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<limits.h>

struct process{
	char name[15];// process name
	int arrival_time;// process arrive time
	int burst_time;// process CPU burst time
	int completion_time;// process complete time
	int turn_around_time;// process turn around time
	int waiting_time;// process warting time
	int response_time;// process response time
	int remaining_time;// process remaining time
	int priority;// process priority
};

int idle_time;
int total_burst;
void read(struct process *prcs, int num_of_process){
	int process_amount = 0;
	FILE *fpRead=fopen("input.txt","r");
	fscanf(fpRead,"%d",&process_amount); 
	num_of_process = process_amount;
//	printf("%d\n", num_of_process);
	
	int i=0;
	for(i=0;i<process_amount;i++){  
        fscanf(fpRead,"%s",&prcs[i].name);
        fscanf(fpRead,"%d",&prcs[i].arrival_time);
        fscanf(fpRead,"%d",&prcs[i].burst_time);
        fscanf(fpRead,"%d",&prcs[i].priority);
        printf("%s %d %d %d\n", prcs[i].name,prcs[i].arrival_time,prcs[i].burst_time,prcs[i].priority);
    }  
	
//	for(i=0;i<process_amount;i++){
//		scanf("%s%d%d%d", &prcs[i].name,&prcs[i].arrival_time,&prcs[i].burst_time,&prcs[i].priority);
//		scanf("%s %d %d %d", prcs[i].name,prcs[i].arrival_time,prcs[i].burst_time,prcs[i].priority);
//	}
	
	prcs->completion_time = prcs->turn_around_time = prcs->waiting_time = prcs->response_time = 0;		// initiate all types of time 
	prcs->remaining_time = prcs->burst_time;
	total_burst += prcs->burst_time;
	printf("\n");
}

void printdata(int num_of_process, struct process prscs[num_of_process]){
	float avg_waiting_time = 0;
	float avg_turn_around_time = 0;
	float utilization = 0;
	float total_waiting_time = 0;
	float total_turn_around_time = 0;
	
	int i=0;
	for(i=0;i<num_of_process;i++){
		//add up the total waiting time and the total turn around time
		total_waiting_time += prscs[i].waiting_time;
		total_turn_around_time += prscs[i].turn_around_time;
	}
	
	//calculate the average waiting time which is: avg_waiting_time = total_waiting_time/process Amount
	avg_waiting_time = total_waiting_time/num_of_process;
	printf("Average waiting time : %.2f\n", avg_waiting_time);
	//calculate the average turn around time which is: avg_turn_around_time = total_turn_around_time/process amount
	avg_turn_around_time = total_turn_around_time/num_of_process;
	printf("Average turn-around time: %.2f\n", avg_turn_around_time);
}

// this is the function to reset the remaining time to let other algorithms dispatch
void resetRemainingTime(int num_of_process, struct process prscs[num_of_process]){
	int i=0;
	for(i=0;i<num_of_process; i++){
		//remaining time will again be the original burst time
		prscs[i].remaining_time = prscs[i].burst_time;
	}
}
//judge whether all the processes are finished
int allDone(int num_of_process, int done[num_of_process]){
	int i=0;
    for(i=0;i<num_of_process;i++){
    	//if there is one process that is not done, break the loop directly using return 0;
        if(done[i]==0) return 0;
    }   
    return 1;
}

int firstJob(int num_of_process, struct process prscs[num_of_process], int time_elapsed, int done[num_of_process]){//to find the first job which entered the CPU
	//the final index that will be returned;
	int jobIndex = -1;
	//firstly set the earliest arrival time to a very large int "INT_MAX"
	int earliest = INT_MAX;
	int i=0;
	//loop all the processes to find the earliest entered process
	for(i=0;i < num_of_process; i++){
		//judge that the process is not done.Also the arrival time is less than the earliest arrival time and the elapsed time
		if(!done[i] && prscs[i].arrival_time <= earliest && prscs[i].arrival_time <= time_elapsed){
			//then the index is temporarily i before the loop is ended
			jobIndex = i;
			//refresh the earliest arrival time to be the current process' arrival time
			earliest = prscs[i].arrival_time;
			
		}
	}
	//after the loop, the final jobIndex will be the earliest arrived job and will be returned
	return jobIndex;
}

void FCFS(int num_of_process, struct process prscs[num_of_process]){
	int done[num_of_process];
	//the array to signify whether the process has been done
	int j=0;
	//initiate the done array
	for(j=0;j<num_of_process;j++) 
		done[j] = 0;
	//the parameter which signifies the time that has elapsed
	int time_elapsed = 0;
	//job index
	int job;
	//loop until all the prcesses are done
	while(!allDone(num_of_process, done)){
		//get the first job index
		job = firstJob(num_of_process, prscs, time_elapsed, done);
		//if there is no job now
		if(job == -1) {
			//time passes and continue
			time_elapsed++;
			continue;
		}
		//output the outcome
		printf("%d:%s\n", time_elapsed, prscs[job].name);
		//time will elapse the amount of the burst time of that process
		time_elapsed += prscs[job].burst_time;
		//update the completion time to be the total time elapsed when the process is finished
		prscs[job].completion_time = time_elapsed;
		//update the turn around time to be the time completed minus the time arrived
		prscs[job].turn_around_time = prscs[job].completion_time - prscs[job].arrival_time;
		//update the waiting time to be the total turn around time minus the burst time
		prscs[job].waiting_time = prscs[job].turn_around_time - prscs[job].burst_time;
		//update the response time to be the same as waiting time
		prscs[job].response_time = prscs[job].waiting_time;
		//signify that this job is done
		done[job] = 1;
	}
}

//to find the shortest job
int shortestJob(int num_of_process, struct process prscs[num_of_process], int elapsed_time, int done[num_of_process]){
	//shortest burst time is initiated to be a large number
	int shortest_burst = INT_MAX;
	//shortest job index is initiated to be -1
	int shortest_index = -1;
	int i=0;
	//loop all the processes
	for(i=0; i<num_of_process; i++){
		//if the process is arrived before the current total elapsed time and its burst time is shorter than the current shortest burst time and it is not done yet
		if(prscs[i].arrival_time <= elapsed_time && prscs[i].burst_time < shortest_burst && !done[i]){
			//then the shortest job is the current index
			shortest_index = i;
			//refresh the shortest burst time to be the current job's burst time
			shortest_burst = prscs[i].burst_time;
		}
	}
	//after the loop, the function will find the shortest job that has not been done
	return shortest_index;
}

void SJF(int num_of_process, struct process prscs[num_of_process]){
	//all the parameters is the same
	int time_elapsed = 0;
	int done[num_of_process];
	int i=0;
	for(i=0; i<num_of_process; i++) 
		done[i] = 0;
	int job;
	//if not all processes are done
	while(!allDone(num_of_process, done)){
		//get the shortest job which is not done
		job = shortestJob(num_of_process, prscs, time_elapsed, done);
		if(job == -1){
			time_elapsed++;
			continue;
		}
		
		printf("%d:%s\n", time_elapsed, prscs[job].name);
		//the current job's burst time will be added to the total time elapsed
		time_elapsed += prscs[job].burst_time;
		//update the completion time to be the current time elapsed
		prscs[job].completion_time = time_elapsed;
		//update the turn around time to be the completion time minus arrival time
		prscs[job].turn_around_time = prscs[job].completion_time - prscs[job].arrival_time;
		//update the waiting time to be the turn around time minus the burst time
		prscs[job].waiting_time = prscs[job].turn_around_time - prscs[job].burst_time;
		//the reponse time is the same as the waiting time
		prscs[job].response_time = prscs[job].waiting_time;
		//it's done
		done[job] = 1;
		
	}
}
//the last parameter is the time quantum/slice for the round-robin(which is 5 according to the current demand)
void roundRobin(int num_of_process, struct process prscs[num_of_process], int time_quantum){
	//the parameters are the same as former
	int done[num_of_process];
	int j=0;
	for(j=0;j<num_of_process;j++) 
		done[j] = 0;
	int time_elapsed = 0;
	int i=0;
	//loop until all the processes are done
	while(!allDone(num_of_process, done)){
		for(i=0;i<num_of_process;i++){
			//check if the process has arrived and incomplete
			if(prscs[i].arrival_time <= time_elapsed && !done[i]){ 
				//the process has arrived and check if there is any time remained 
				if(prscs[i].remaining_time > 0){
					//check if it's executed for the first time, calculate the current response time which is the current total time elapsed minus arrival time.
					if(prscs[i].remaining_time == prscs[i].burst_time){
						prscs[i].response_time = time_elapsed - prscs[i].arrival_time;
					}
					//check if it requries less than or greater than the time quantum
					if(prscs[i].remaining_time <= time_quantum){
						//the prscs[i] will be comepleted here and calulate it's time information and set it done
						time_elapsed += prscs[i].remaining_time;
						printf("%d:%s\n", time_elapsed, prscs[i].name);
						//it's completion time is the current total time elapsed
						prscs[i].completion_time = time_elapsed;
						//it's completed so the remaining time is 0
						prscs[i].remaining_time = 0;
						//set it done
						done[i] = 1;
						//the turn around time is the completion time minus the arrival time
						prscs[i].turn_around_time = prscs[i].completion_time - prscs[i].arrival_time;
						//the waiting time is the turn around time minus the burst time
						prscs[i].waiting_time = prscs[i].turn_around_time - prscs[i].burst_time;
					}
					else{
						//there's remaining time to execute, but the process wont be done which means the remaining time is greater than the time quantum
						//step one more quantum forward
						time_elapsed += time_quantum;
						//this process' remaining time will minus one time quantum
						prscs[i].remaining_time -= time_quantum;
					}
				}
			}
		}
	}
}

//this is the function to get the job that has the highest priority
int mostPriorJob(int num_of_process, struct process prscs[num_of_process], int time_elapsed, int done[num_of_process]){
	//set the prior job index to be -1 at first
	int priorIndex = -1;
	//the most prior is the largest integer at first
	int highest_priority = INT_MAX;
	int i=0;
	//loop all the processes
	for(i=0; i < num_of_process; i++){
		//if is's not done and it's arrived and it's priority is higher than the current highest priority
		if(!done[i] && prscs[i].arrival_time <= time_elapsed && prscs[i].priority < highest_priority){
			//then the current job with highest priority is updated
			priorIndex = i;
			//the highest priority is the current job's priority
			highest_priority = prscs[i].priority;
		}
	}
	//return the current job index with the highest priority
	return priorIndex;
}

void nonPreemptive_Priority(int num_of_process, struct process prscs[num_of_process]){
//	idle_time = 0;
	//the parameters are the same
	int done[num_of_process];
	int i=0;
	for(i=0; i<num_of_process;i++)
		done[i] = 0;
	int time_elapsed = 0;
	int job = -1;
	
	//loop until all the processes are done
	while(!allDone(num_of_process, done)){
		//get the most prior job
		job = mostPriorJob(num_of_process, prscs, time_elapsed, done);
		if(job==-1){
//			idle_time++;
			time_elapsed++;
			continue;
		}
		//elapse the current job's burst time
		time_elapsed += prscs[job].burst_time;
		printf("%d:%s\n", time_elapsed, prscs[job].name);
		//the completion time is the current elapsed time
		prscs[job].completion_time = time_elapsed;
		//the turn_around time is the completion time minus the arrival time
		prscs[job].turn_around_time = prscs[job].completion_time - prscs[job].arrival_time;
		//the waiting time is the turn around time miuns the burst time
		prscs[job].waiting_time = prscs[job].turn_around_time - prscs[job].burst_time;
		//set it done
		done[job] = 1;
		//the response time is the same as the waiting time
		prscs[job].response_time = prscs[job].waiting_time;
	}
}

//to get the job with the shortest remaining time
int SRTJob(int num_of_process, struct process prscs[num_of_process], int time_elapsed, int done[num_of_process]){
	//the job index is initiated
	int jobIndex = -1;
	//the shortest remain time is initiated
	int shortestRemainTime = INT_MAX;
	int i=0;
	//loop all the processes
	for(i=0; i<num_of_process; i++){
		//if the job is not done and it's arrived and it's remaining time is smaller than the current shortest reamaining time
		if(!done[i] && prscs[i].arrival_time <= time_elapsed && prscs[i].remaining_time < shortestRemainTime){
			//update the shortest remaining time
			shortestRemainTime = prscs[i].remaining_time;
			//update the job index which has the shortest remaining time
			jobIndex = i;
		}
	}
	return jobIndex;
}
void SRTF(int num_of_process, struct process prscs[num_of_process]){
	//the parameters are the same
	int done[num_of_process];
	int i=0;
	for(i=0;i < num_of_process; i++) 
		done[i] = 0;
	int time_elapsed = 0;
	int job = -1;
	//loop until all the processes are done
	while(!allDone(num_of_process, done)){
		//get the job with the shortest remaining time
		job = SRTJob(num_of_process, prscs, time_elapsed, done);
		if(job == -1){
			time_elapsed++;
			continue;
		}
		if(time_elapsed%5==0)
			printf("%d:%s\n", time_elapsed, prscs[job].name);
		//if this is the first time for the job to execute
		if(prscs[job].remaining_time == prscs[job].burst_time){
			//then the response time will be the current total elapsed time minus the job's arrival time
			prscs[job].response_time = time_elapsed - prscs[job].arrival_time;
		}
		time_elapsed++;
		//remaining time minus 1 for one execution
		prscs[job].remaining_time -= 1;
		//if the remaining time is 0 which means the job is done
		if(prscs[job].remaining_time == 0){
			//the completion time is the current time elapsed
			prscs[job].completion_time = time_elapsed;
			//update the turn around time to be the completion time subtract the arrival time
			prscs[job].turn_around_time = prscs[job].completion_time - prscs[job].arrival_time;
			//update the waiting time to be the turn around time subtract the burst time
			prscs[job].waiting_time = prscs[job].turn_around_time - prscs[job].burst_time;
			//set it done
			done[job] = 1;
		}
	}
}

int main(){
	int num_of_process;
	//the time quantum is 5 according to the requirment
	int time_quantum = 5;
	FILE *fpRead=fopen("input.txt","r");
	//read in the process amount
	fscanf(fpRead,"%d",&num_of_process);
	//the processes control block
	struct process processes[num_of_process];

	//read in the processes information
	read(processes, num_of_process);
	
	//FCFS output
	printf("First Come first Served output: \n ");
	FCFS(num_of_process, processes);
	printdata(num_of_process, processes);
	//reset the remaining time
	resetRemainingTime(num_of_process, processes);

	//SJF output 
	printf("\nShortest Job First output: \n");
	SJF(num_of_process, processes);
	printdata(num_of_process, processes);
	//reset the remaining time
	resetRemainingTime(num_of_process, processes);
	
	//RR output
	printf("\nRound Robin output: \n");
	roundRobin(num_of_process, processes, time_quantum);
	printdata(num_of_process, processes);
	//reset the remaining time
	resetRemainingTime(num_of_process, processes);
	
	//Priority output
	printf("\nNon-Preemptive Priority output: \n");
	nonPreemptive_Priority(num_of_process, processes);
	printdata(num_of_process, processes);
	//reset the remaining time
	resetRemainingTime(num_of_process, processes);
	
	//SRT output
	printf("\nShortest Remaining Time output: \n");
	SRTF(num_of_process, processes);
	printdata(num_of_process, processes);
}

