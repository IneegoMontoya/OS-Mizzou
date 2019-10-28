#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"

// Add and comment your analysis code in this function.
// THIS IS NOT FINISHED.
int main(int argc, char **argv) {
    if (argc < 3) {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }
   

	dyn_array_t* dynArray = load_process_control_blocks(argv[1]);

	ScheduleResult_t result = {0,0,0};

	if(strstr(argv[2], "P"))
	{
		priority(dynArray, &result);
		printf("Priority Scheduling Results \n");
	
	}

	if(strstr(argv[2], "FCFS"))
	{
		first_come_first_serve(dynArray, &result);
		printf("First Come First Serve Scheduling Results \n");
	}

	if(strstr(argv[2], "RR"))
	{	
		char* ptr; 
		round_robin(dynArray, &result, strtoul(argv[3], &ptr, 10));
		printf("Round Robin Scheduling Results \n");
	}

	
	printf("Average wait time: %f \n", result.average_waiting_time);
	printf("Average turn around time: %f \n", result.average_turnaround_time);
	printf("Total time: %f \n", (float)result.total_run_time);

	free(dynArray);
//	free(ptr);

    return EXIT_SUCCESS;
}
