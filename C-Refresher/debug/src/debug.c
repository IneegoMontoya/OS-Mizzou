#include "../include/debug.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// protected function, that only this .c can use
int comparator_func(const void *a, const void *b) {

	// getting warning from this function: not sure if it is part of what we are supposed
	// to be debugging, but it returns the one pointer minus another, not the values in those pointers.
	
    return (*(uint8_t*)a - *(uint8_t *)b);
}

bool terrible_sort(uint16_t *data_array, const size_t value_count) {
	// argument validation prior to attempting to sort
	// return false if pointer is null or the size is less than or equal to zero
	if(NULL == data_array || 0 >= value_count)
	{
		return false;
	}
	// star was not next to variable declaration making it a pointer. 
	
    uint16_t *sorting_array = malloc(value_count * sizeof(uint16_t));
    for (int i = 0; i < (int)value_count; ++i) {
        sorting_array[i] = data_array[i];
    }
	//missing semi-colon at end of qsort line
    qsort(sorting_array, value_count, sizeof(uint16_t), comparator_func);

    bool sorted = true;
	// missing semicolon in the for loop declaration
	// not sure this second for loop is needed. gonna comment it out and run the test. 
  //  for (int i = 0; i < (int)value_count; ++i) {
    //    sorted &= sorting_array[i] < sorting_array[i + 1];
   // }

    if (sorted) {
	// destination and source are reversed in this mempy call
	// should be memcpy(destination, source, n:charachters)
	// also last argument needs to not only bee the value_count but the value_count* the size of the 
	// data type
        memcpy(data_array, sorting_array, value_count*sizeof(uint16_t));
    }
	//free the sorting array
	free(sorting_array);
    return sorted;
}

