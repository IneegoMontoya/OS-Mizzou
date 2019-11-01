#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/arrays.h"

// LOOK INTO MEMCPY, MEMCMP, FREAD, and FWRITE

bool array_copy(const void *src, void *dst, const size_t elem_size, const size_t elem_count)
{
    // Checking for valid arguments being sent to the function call. 
    // For the arrays have to make sure they are valid, and for the size_t variables they must always be greater than zero
    // as nothing can have a negative size. 
     if(NULL == src || NULL == dst || 0 >= elem_count || 0 >= elem_size)
    {
        return false;
    }
    else
    {
        // memcpy not only needs to know the source and destination array, but the number of elements and their size to copy.
     memcpy(dst, src, (int)elem_count*(sizeof(elem_size)));
     return true;   
    }

}

bool array_is_equal(const void *data_one, void *data_two, const size_t elem_size, const size_t elem_count)
{
    // Check for valid argumets being sent to the fucntion: If incorrect argmuents are sent return false
     if(NULL == data_one || NULL == data_two || 0 >= elem_count || 0 >= elem_size)
    {
        return false;
    }
    // memcmp function returns an int value: If the two things being compared are equal then it will return zero
    // If the items being compared are not equal: depending on which one is "larger" memcmp will return either a positive or
    // negative value. 
    int test = memcmp(data_one, data_two, elem_count);

    if(0 == test)
    {
        return true;
    }
    else
    {
        return false;
    }

}

ssize_t array_locate(const void *data, const void *target, const size_t elem_size, const size_t elem_count)
{
     if( NULL == data || NULL == target || 0 >= elem_size || 0 >= elem_count )
		{
			return -1;
		}

	size_t x;
	int y;
	// use memcmp 
	for(x = 0; x < elem_count; x++)
		{
			// tried using strncmp as well as memcmp
			// could get correct values in a local IDE but still can't seem to 
			// pass the unit test on this one 
			y = memcmp(data, target, 1);	
			if(0 == y)
			{	
				
				return x;
			//	break;
			}
			data++;
		}
	return -1;
}

bool array_serialize(const void *src_data, const char *dst_file, const size_t elem_size, const size_t elem_count)
{
//checking for valid arguments to the function call. count and size are size_t elements which must
	//be larger than zero. Also checking for NULL values for the data and source along with having a 
	//filename for the destination file.
	if(NULL== src_data || NULL == dst_file || 0 >= elem_size || 0 >= elem_count || 0 >= strlen(dst_file))
		{	
			return false;
		}
	FILE* fp;
	// using a file pointer to open the file in write mode. Checking for valid stream and no errors in the process
	fp = fopen(dst_file, "w");
	if(!fp)
		{	
			return false;
		}
	else if( 0 != ferror(fp))
		{
			return false;
		}
	// Looked at the tests and saw the newline charachter as one of them. Used strbrk to search the file name for the illegal 
	// charachter. Couldn't think of any others which I needed to add to this, but could use this method for validating 
	// there not being any not allowed charachters in the file name. 
	else if( strpbrk(dst_file, "\n"))
		{
			return false;
		}
	else
		{
			// write the data to the destination file
			fwrite(src_data, elem_size, elem_count, fp);
			fclose(fp);
			return true;
		}
}

bool array_deserialize(const char *src_file, void *dst_data, const size_t elem_size, const size_t elem_count)
{
    //Input validation as with the other functions and is basically reverse of serialize
	if(NULL == src_file || NULL == dst_data || 0 >= elem_size || 0 >= elem_count || 0 >= strlen(src_file))
		{
			return false;
		}
	FILE* fp;
	fp = fopen(src_file, "r");
	if(!fp)
		{
			return false;
		}
	else if( 0 != ferror(fp))
		{
			return false;
		}
	else if( strpbrk(src_file, "\n"))
		{
			return false;
		}
	else
		{
			fread(dst_data, elem_size, elem_count, fp);
			fclose(fp);
			return true;
		}

}

