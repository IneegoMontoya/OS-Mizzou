#include "../include/allocation.h"
#include <stdlib.h>
#include <stdio.h>


void* allocate_array(size_t member_size, size_t nmember,bool clear)
{	// make a void pointer to return
	void* ptr;
	// argument validation- member_size and nmember must both be greater than zero
	// if they are not return a NULL pointer
	if(0 >= member_size || 0 >= nmember)
	{
		ptr = NULL;
		return ptr;
	}
	// clear is a bool. therefore if clear is flagged, then use calloc to set the values to zero
	// else use malloc
	if(clear)
	{
		ptr = calloc(nmember, sizeof(member_size));
	
	}
	else
	{
		ptr = malloc(nmember*sizeof(member_size));
	}

	return ptr;

}

void* reallocate_array(void* ptr, size_t size)
{
	// argument validation. making sure the pointer is not null and that the new size is not negative or zero
	
	if(NULL == ptr || 0 >= size)
	{
		ptr = NULL;
		return ptr;
	}
	else 
	{
		//reallocation using realloc
		ptr = realloc(ptr, size);
	}
	return ptr;
}

void deallocate_array(void** ptr)
{

	// this one gave me troubles with it being a double pointer. 
	// ended up making a char pointer and setting it to the *ptr value
	// then used free to free up the char pointer
	// I could then set the value of *ptr to NULL
	char* ptr1 = NULL;
	if(ptr)
	{
		ptr1 = *ptr;
	}

	if(ptr1)
	{
		free(ptr1);
		*ptr = NULL;
	}
	return ptr;
}

char* read_line_to_buffer(char* filename)
{
	// file pointer for filestream
	FILE* fp;
	// char pointer to accept the stream
	char* ptr;
	// argument validation - return NULL if bad input
	if(NULL == filename)
	{
		return NULL;
	}
	// malloc the cahr poitner ahead of opening the file
	ptr = (char*)malloc(BUFSIZ*sizeof(char));
	// opening the file in read-only mode
	fp = fopen(filename, "r");
	//pull a line from the fill into the buffer
	fgets(ptr, BUFSIZ, fp);
	// close the file
	fclose(fp);
	//return the char pointer
	return ptr;














}
