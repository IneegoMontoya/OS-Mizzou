#include "../include/bitmap.h"

// data is an array of uint8_t and needs to be allocated in bitmap_create
//      and used in the remaining bitmap functions. You will use data for any bit operations and bit logic
// bit_count the number of requested bits, set in bitmap_create from n_bits
// byte_count the total number of bytes the data contains, set in bitmap_create


bitmap_t *bitmap_create(size_t n_bits)
{
    int size;
    if(0 >= n_bits)
    {
		bitmap_t *bit = NULL;
        return bit;
    }
    else
    {	// important to have enough space for bits in excess of what n_bits/8 would yield
        if(0 == (n_bits%8))
        {
            size = (n_bits/8);
        }
        else
        {
            size = (n_bits/8)+1;
        }
        
        bitmap_t *bit = (bitmap_t*)malloc(sizeof(bitmap_t));
	// used calloc to allocate the array to all zero's
	bit->data = (uint8_t*) calloc(size, sizeof(uint8_t));
	bit->bit_count = n_bits;
        bit->byte_count = size;
        return bit;

	}
}

bool bitmap_set(bitmap_t *const bitmap, const size_t bit)
{
    // this one gave me lots of trouble. Took the longest to get done
	// input validation searches for NULL values of bitmap and that bit is within the 
	// correct range of values : 0-255 
	// website provided was extrememly helpful, but still had to play around with it in seperate IDE to see
	// the behavior of how this worked
 	
   if(NULL == bitmap || 0 > bit || 255 < bit)
    {
        return false;
    }
    int x = (bit)/8;
    int spot = bit%8;
    unsigned int flag = 1;
    flag = flag << spot; 
    bitmap->data[x] = bitmap->data[x] | flag;
  
    return true;
	
}

bool bitmap_reset(bitmap_t *const bitmap, const size_t bit)
{
// same basic structure of sitmap_set except use the inverse of flagg and the & operator to 
	// change the 1 to a zero
	//
	// still use the same argument validation checking for NULL and if the bit input is larger than the size
	// of 255 which is the max amount of bits in a uint8_t
	if(NULL == bitmap || 0 > bit || 255 < bit)
   	{
       		 return false;
	}
	int x = (bit)/8;
	int spot = bit%8;
	unsigned int flag = 1;
	flag = flag << spot;
	flag = ~flag; 
	bitmap->data[x] = bitmap->data[x] & flag;
  
	return true;

}

bool bitmap_test(const bitmap_t *const bitmap, const size_t bit)
{
if(NULL == bitmap || 0 > bit || 255 < bit)
	{
		return false;
	}
	int x = (bit)/8;
	int spot = bit%8;
   	unsigned int flag = 1;
    	flag = flag << spot;
	if(bitmap->data[x] & flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t bitmap_ffs(const bitmap_t *const bitmap)
{
    if(NULL == bitmap)
	{
		return SIZE_MAX;
	}
//Using the bitmap test funciton already written it is easy to step through the bits to find the desired
//value. if the value is not found then return SIZE_MAX is executed.
    size_t high = bitmap->bit_count;
    size_t count = 0;
    while(count < high)
    {
        if(bitmap_test(bitmap, count))
            {
                return count;
            }
        count++;
    }
    return SIZE_MAX;


}

size_t bitmap_ffz(const bitmap_t *const bitmap)
{
if(NULL == bitmap)
    {
	
        return SIZE_MAX;
    }
    size_t high = bitmap->bit_count;
    size_t count = 0;
	//by using the bitmap test function it is easy to check the status of a bit. wimple while
	//loop with an incrimenter returns when the value is found. If the value isn't found a default 
	//return SIZE_MAX will execute
    while(count < high)
    {
        if(!bitmap_test(bitmap, count))
            {
                return count;
            }
        count++;
    }
    return SIZE_MAX;
}

bool bitmap_destroy(bitmap_t *bitmap)
{
// If this is null then no point in freeing it return false
	if(NULL == bitmap)
		{
			return false;
		}
	else
		{
		//first have to free the array inside of the bitmap, then can free the bitmap itself
			free(bitmap->data);
			free(bitmap);
			return true;
		}
}
