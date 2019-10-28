#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) {
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) {
    // Argument Validation return false if sent NULL values

	if(NULL == ready_queue || NULL == result)
		{
			return false;
		}
	
	// need to know how many blocks there are

	int numBlocks;

// int values for burst time, turn around time, and wait time. I don't see anything in dyn_array which will
// tell me if there is a arrival time for this section. So I am going to assume that the
// arrival time is the order which the blocks showed up in the ready queue. 

// So at first I had set these in a declaration like this int x, y, z = 0; man that took a while to figure out my dumb mistake

	int burstTime = 0;
	int waitTime = 0;
	int turnTime = 0;
	int waitTotal = 0;
	int totalTime = 0;

// set value of numBlocks to the number of pcb's in the ready que with the dyn_array_size function call

	numBlocks = (int)dyn_array_size(ready_queue);

// Place holder variable for when I extract blocks from the front of the que and run their burst time.
// essentially works like a temp variable.
	ProcessControlBlock_t block; //  = (ProcessControlBlock_t*)malloc(sizeof(ProcessControlBlock_t));
	const ProcessControlBlock_t null_block;

// Check to make sure that the number of blocks is a positive number. Don't know how this could not
// be the case, but should the array have zero members, then the times should be zero. 
// Will still return true, this is because it actually shouldn't constitute a failure of the algorithm.

	if(0 >= numBlocks)
		{
			result->average_turnaround_time = 0;
    			result->average_waiting_time = 0;
    			result->total_run_time = 0;
			return true;
		}
	else
		{
			int count;
			for(count = 0; count < numBlocks; count++)
				{
				//wait time starts at 0 for first process, but will need burst added to it 
				//trhough iterations

	
					waitTime +=  burstTime;
				// extract a block from the dyn array into my placeholder variable block
				// Looks like I have three options. 
				// Front,back, or just extract.
				// As this is first come first serve, going to go with the front
					dyn_array_extract_back(ready_queue,(void*)&block);
					
				// Add the extracted blocks listed burst time to the total burst time

			
					burstTime = block.remaining_burst_time;
					totalTime += burstTime;	
					turnTime += waitTime + block.remaining_burst_time;
				
				if(count != 0)
				{
					waitTotal += waitTime;
				}
				
				// Turn Around Time will eventually be burst time divided by the 
				// number of blocks. I think I can do this with just the burst time 

					do
						{
						virtual_cpu(&block);
	
					}while(block.remaining_burst_time);
					block = null_block;	
				//	turnTime = 0;
				// Personal note, doing this in VIM is really a pain in the neck
				// I'm used to an IDE being able to do most of my indentation for me
				// Guess I am just spoiled.
				}
	}

// Now have to send the times into the result set. 
	

	result->average_waiting_time =(float)waitTotal/numBlocks;
	result->average_turnaround_time = (float)turnTime/numBlocks;
	result->total_run_time = (float)totalTime;


	 return true;
}

// Compare function needed for use of dyn array sort
int compare(const void* block1, const void* block2)
{
// Logic for compare function listed out in the include documentation. 

    if(((ProcessControlBlock_t*)block1)->priority > ((ProcessControlBlock_t*)block2)->priority)
    {
        return 1;
    }
    else if (((ProcessControlBlock_t*)block1)->priority < ((ProcessControlBlock_t*)block2)->priority)
    {
        return -1;
    }else if (((ProcessControlBlock_t*)block1)->priority == ((ProcessControlBlock_t*)block2)->priority)
    {
        return 0;
    }

	return 0;
}




bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) {
    // You need to remove this when you begin implementation.
    // replace me with implementation.
  
	if(NULL == ready_queue || NULL == result)
		{
			return false;
		}
	
	// function pointer to compare function
	int (*const compare_ptr)(const void*, const void*) = &compare;
	dyn_array_sort(ready_queue, compare_ptr);

// After the sort function call the dyn_array should be able to follow the code written in first come first serve
// albiet rather than pulling from the back, after sorting, you extract from the front of the ready_queue
	int burstTime = 0;
	int waitTime = 0;
	int turnTime = 0;
	int waitTotal = 0;
	int totalTime = 0;
	
	ProcessControlBlock_t block;
	ProcessControlBlock_t null_block;

	int numBlocks = (int)dyn_array_size(ready_queue);

	if(0 >= numBlocks)
	{
		result->average_turnaround_time = 0;
    		result->average_waiting_time = 0;
    		result->total_run_time = 0;
		return true;
	}

	int count;
	for(count = 0; count < numBlocks; count++)
	{
		waitTime +=  burstTime;
		dyn_array_extract_front(ready_queue,(void*)&block);
		burstTime = block.remaining_burst_time;
		totalTime += burstTime;	
		turnTime += waitTime + block.remaining_burst_time;
	

		if(count != 0)
			{
				waitTotal += waitTime;
			}
		
		do
			{
				virtual_cpu(&block);
	
			}while(block.remaining_burst_time);
			block = null_block;	

	}


	result->average_waiting_time =(float)waitTotal/numBlocks;
	result->average_turnaround_time = (float)turnTime/numBlocks;
	result->total_run_time = (float)totalTime;


	 return true;   
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) {
   

	if(NULL == ready_queue || NULL == result || 0 >(int)quantum)
		{
			return false;
		}

	int turnTime = 0;
	int numBlocks = (int)dyn_array_size(ready_queue);
	int count;
	int ticker = 0;

// Two Process Control Block varaibles - one to read into the other to use as a null to reset the one being read into
	ProcessControlBlock_t block;
	ProcessControlBlock_t null_block;	

	if(0 >= numBlocks)
		{
			result->average_turnaround_time = 0;
    			result->average_waiting_time = 0;
    			result->total_run_time = 0;
			return true;
		}
	
	do
	{


		dyn_array_extract_back(ready_queue,(void*)&block);

		for(count = 0; count < (int)quantum; count++)
		{
			if(0 == block.remaining_burst_time)
				{
					break;
				}
			virtual_cpu(&block);
			ticker++;	
		}
	// If there is still burst time remaining, push the block back onto the array at the front
	// if not, then pull the turn time from the ticker
		if(0 != block.remaining_burst_time)
			{
			
				dyn_array_push_front(ready_queue, (void*)&block);
			}
		else
			{
				turnTime += ticker;	
			}
		block = null_block;


	}while(!dyn_array_empty(ready_queue));

// At first had the darndest time trying to get the wait times correct. Then I remembered basic math and that it doesn't matter the order 
// in which you add or subract things. Wait time is turn time - total time. 	
	result->average_waiting_time = (float)(turnTime - ticker)/numBlocks;
	result->average_turnaround_time = (float)turnTime/numBlocks;
	result->total_run_time = (float)ticker;



	 return true;
}

dyn_array_t *load_process_control_blocks(const char *input_file) {
    // Checking for NULL being passed as an argument. Ironically giving NULL if you get NULL...
	if(NULL == input_file)
		{
			return NULL;
		}


// checking for correct file type - return NULL if file is not of the corect type. Which should 
// be a .bin file according to the testing. searching for ".bin" in input_file should be adequate
// as long as the file name is not something like  "Cor.bin.Dallas.Multi-Pass" it should work just fine
	if(NULL == strstr(input_file, ".bin"))
		{
			return NULL;
		}
//book says system calls all use a "file desciptor".
// Text also implies if the opening the file desciptor was good then it will be a non-negative value
// Should a value less than zero be given - return NULL
	int fd = open(input_file, O_RDONLY);

	if(0 > fd)
		{
			close(fd); // Just in case...
			return NULL;
		}
// Integer variable used to read in from the file descriptor

	
	uint32_t numBlocks; // tried this as pointer first- no avail = (uint32_t*)malloc(sizeof(uint32_t));
   	int count;

	
// PorcessControlBlock pointer. Will allocate memory once I read in how many records are in the file from 
// the file_descriptor

	ProcessControlBlock_t* blocks;
	read(fd, &numBlocks, sizeof(uint32_t));
	blocks = (ProcessControlBlock_t*)malloc(numBlocks * sizeof(ProcessControlBlock_t));

	dyn_array_t* dynArray = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);	

// Step through the ProcessConrolBlock array reading in the data into the structs fields.
// Then push that block onto the dyn_array. First tried using dyn_array_insert but was having trouble 
// with it. Took inspiration from the test on first come first serve and just used dyn_array_push_back
	
	for(count = 0; count < (int)numBlocks ; count++)
		{
			read(fd, &blocks[count].remaining_burst_time, sizeof(uint32_t));
			read(fd, &blocks[count].priority, sizeof(uint32_t)); 
			read(fd, &blocks[count].arrival, sizeof(uint32_t));
			dyn_array_push_back(dynArray, &blocks[count]);
		}

//	free(numBlocks);
	free(blocks);
	close(fd);
	
	
	return dynArray;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) {
    // You need to remove this when you begin implementation.
    // // replace me with implementation.
	
	if(NULL == ready_queue || NULL == result)
		{
			return false;
		}
   
   


    return false;
}
