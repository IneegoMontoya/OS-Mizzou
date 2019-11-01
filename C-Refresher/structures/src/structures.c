#include "../include/structures.h"
#include <stdio.h>

int compare_structs(sample_t* a, sample_t* b)
{
	// check the values of the given structs if they are not equal 
	// then return 0
	if(a->a != b->a)
        {
            return 0;
        }
    if(a->b != b->b)
    {
        return 0;
    }
    if (a->c !=  b->c)
    {
        return 0;
    }

    return 1;
}

void print_alignments()
{
	printf("Alignment of int is %zu bytes\n",__alignof__(int));
	printf("Alignment of double is %zu bytes\n",__alignof__(double));
	printf("Alignment of float is %zu bytes\n",__alignof__(float));
	printf("Alignment of char is %zu bytes\n",__alignof__(char));
	printf("Alignment of long long is %zu bytes\n",__alignof__(long long));
	printf("Alignment of short is %zu bytes\n",__alignof__(short));
	printf("Alignment of structs are %zu bytes\n",__alignof__(fruit_t));
}

int sort_fruit(const fruit_t* a,int* apples,int* oranges, const size_t size)
{
	int count;
	for(count = 0; count < (int)size; count++)
	{
		// step through fruit and if type is orange which should be 0 then
		// then icrement oranges, otherwise increment apples
		if(0 == a[count].type)
		{
			*oranges += 1;
		}
		else
		{
			*apples += 1;
		}
	
	}	
	return size;
}

int initialize_array(fruit_t* a, int apples, int oranges)
{
	int total = apples + oranges;
	int count = 0;
	// use local variable and while loop to go through and initialize the types
	// set type to 1 for an apple up to the number of apples
	// otherwise set type to 0 for orange
	while(count < total)
		{
			if(count < (apples))
			{
				a->type = 1;
			}
			else
			{
				a->type = 0;
			}
			count++;
			a++;
		}


	return 0;
}

int initialize_orange(orange_t* a)
{
	// check for NULL pointer, otherwise allocate memory for an orange with malloc
	if(NULL == a)
	{
		return -1;
	}

	a = (orange_t*)malloc(sizeof(orange_t));
	return 0;

}

int initialize_apple(apple_t* a)
{
	//check for NULL pointer, otherwise allocate memory for an apple with malloc
	if(NULL == a)
	{
		return -1;
	}
	a = (apple_t*)malloc(sizeof(apple_t));

	return 0;

}
