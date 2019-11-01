#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "../include/error_handling.h"

int create_blank_records(Record_t **records, const size_t num_records)
{
	//argument validation part 1: checking to make sure that the pointer records points to is valid
	//and or if the number of records (num_records) is not equal to zero. (for the longest I was getting
	//a seg fault and then cast the num_records as an int and it suddenly worked. I hope that is ok
	if(NULL != *records || 0 == (int)num_records)
	{
		return -1;
	}
	// arguments validation part 2: if the pointer that records points to is null
	// AND (not or) the records number is less than zero then return -2
	if( NULL == *records && 0 > (int)num_records)
	{
		return -2;
	}

	*records = (Record_t*) malloc(sizeof(Record_t) * num_records);
	memset(*records,0,sizeof(Record_t) * num_records);
	return 0;	
}

int read_records(const char *input_filename, Record_t *records, const size_t num_records) {

	// argument validation: checking for NULL values and sizes that are 0 or less than 0
	// return -1 if any are incorrect
	if(NULL == input_filename || NULL == records || 0>= (int)num_records)
	{
		return -1;
	}

	int fd = open(input_filename, O_RDONLY);
	// checking to see if file properly opened return -2 if file not open properly
	// fd should have a not NULL value or not less than zero since it is a binary file
	// took out the NULL comparison because I was getting a warning
	if(0 > fd)
	{
		return -2;
	}

	ssize_t data_read = 0;
	// to check for errors reading this must be done in the for loop
	// can be done by checking data_read for 0. as a size_t it should not be zero in any case if
	// properly being read. Having to cast data_read as an int for proper comparison
	for (size_t i = 0; i < num_records; ++i) {
		data_read = read(fd,&records[i], sizeof(Record_t));	
		if( 0 == (int)data_read)
		{
			return -3;
		}
	}
	return 0;
}

int create_record(Record_t **new_record, const char* name, int age)
{
	//argument validation checking for proper age restrictions and name length and 
	//that there is not an NULL name passed.
	//return -1 is any comparisons pop
	if(1 > age || 200 < age || NULL == name || 50 < strlen(name) || strpbrk("\n", name))
	{
		return -1;
	}
	*new_record = (Record_t*) malloc(sizeof(Record_t));
	
	memcpy((*new_record)->name,name,sizeof(char) * strlen(name));
	(*new_record)->name[MAX_NAME_LEN - 1] = 0;	
	(*new_record)->age = age;
	return 0;

}
