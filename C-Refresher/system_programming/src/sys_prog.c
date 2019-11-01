#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <byteswap.h>

#include "../include/sys_prog.h"

// LOOK INTO OPEN, READ, WRITE, CLOSE, FSTAT/STAT, LSEEK
// GOOGLE FOR ENDIANESS HELP

bool bulk_read(const char *input_filename, void *dst, const size_t offset, const size_t dst_size)
{
// input validatin for aruments
	if(NULL == input_filename || NULL == dst || offset > dst_size)
	{
		return false;
	}
	
	if(strpbrk(input_filename, "\n"))
	{
		return false;
	}
	else if(strpbrk(input_filename, "\0"))
	{
		return false;
	}
	int fp;
	// initially tried using the data type uint8_t but it didn't return correct values.
	// regular int gave correct values. 
	int b_read;
	fp = open(input_filename, O_RDONLY, 0);
	if( 0 >= fp)
		{

			return false;
		}
	b_read = read(fp, dst, dst_size);
	if(0 > b_read)
		{
			return false;
		}
	return true;
}

bool bulk_write(const void *src, const char *output_filename, const size_t offset, const size_t src_size)
{
//input validation of the arguments. 
	if(NULL == src || NULL == output_filename)
		{
			return false;
		}
	if(strpbrk(output_filename, "\n"))
		{
			return false;
		}
	else if(strpbrk(output_filename, "\0"))
		{
			return false;
		}
	
	int fd, n;
	fd = open(output_filename, O_WRONLY, 0);
	if(0 > fd)
		{
			return false;
		}
	n = write(fd, src, src_size);
	if(0 > n)
		{
			return false;
		}
	return true;
}


bool file_stat(const char *query_filename, struct stat *metadata)
{
    if(NULL == query_filename || NULL == metadata)
		{	
			return false;
		}
	//	struct stat test;
	
	if(stat(query_filename, metadata) == -1)
		{
			return false;
		}
	else
		{
			return true;
		}

}

bool endianess_converter(uint32_t *src_data, uint32_t *dst_data, const size_t src_count)
{
//Argument validation
	if(NULL == src_data || NULL == dst_data || 0 >= src_count)
        {
            return false;
        }
    //tried this multiple ways but could not get it to work. First was the built in gcc function 
    // bswap32  which is below
      
	*dst_data = __builtin_bswap32(*src_data);
	return true;
/* I also tried this method which I found online, but it didn't seem to give me correct values either
   uint32_t q0, q1, q2, q3;
    
    q0 = (*src_data & 0x000000ff) << 24u;
    q1 = (*src_data & 0x0000ff00) << 8u;
    q2 = (*src_data & 0x00ff0000) >> 8u;
    q3 = (*src_data & 0xff000000) >> 24u;

    *dst_data = q0 | q1 | q2 | q3;

*/
}

