#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/sstring.h"

bool string_valid(const char *str, const size_t length)
{
// Check for NULL string and length longer than zero: return false otherwise
	if(NULL == str || 0>= length)
		{
			return false;
		}
	//int variable for a 'for' loop to traverse str
	int x;
	for(x = 0; x < length; x++)
		{
			if('\0' == *(str))
			{
				return true;
			}
			else
			{
				str++;
			}
		}
	return false;
}

char *string_duplicate(const char *str, const size_t length)
{
// Check for null string, and a valid length. Cannot allocate memory without a corrrect 
	// value of the initial string's length
	if(NULL == str || 0 >= length)
	{
		return NULL;
	}
	// in order to return a local variable have to malloc it to the stack
	char *dupe = malloc(length * sizeof(char));
	// strcpy does all of the copying including the null terminator
	strcpy(dupe, str);
	return dupe;	
}

bool string_equal(const char *str_a, const char *str_b, const size_t length)
{
//Argument validation
	if(NULL == str_a || NULL == str_b || 0 >= length)
		{
			return false;
		}
	// strcmp will return 0 if both strings are equal
	if(0 == strcmp(str_a, str_b))
		{
			return true;
		}
	else
		{
			return false;
		}
}

int string_length(const char *str, const size_t length)
{
if(NULL == str || 0 >= length)
		{
			return -1;
		}
	else
		{
			//strlen function returns the number of charachters not including
			//the null terminator. And will stop counting once it reaches
			//a null terminator
			return strlen(str);
		}
}

int string_tokenize(const char *str, const char *delims, const size_t str_length, char **tokens, const size_t max_token_length, const size_t requested_tokens)
{
	//Argument validation
	if(NULL == str || NULL == delims || 0 >= str_length || NULL == tokens || 0 >= requested_tokens || 0 >= max_token_length)
{
		return 0;
}
	int x = 0;
	char src[30];
	char d[2];
	char* p = malloc(10*sizeof(char));
	int i;
	for(i=0; i < (int)requested_tokens-1; i++)
		{
			if(NULL == (tokens++))
			{
				return -1;
			}
		}
	//copy arguments into local variable using strcpy to use strtok on.
	strcpy(src, str);
	strcpy(d, delims);
	// break off first token with strtok
	
	p = strtok(src, d);
	// Am able to get the tokens, but for some reason cannot seem to remember how to put the values into the token array. 
	tokens = &p;
	while(p != NULL)
		{
			x++;
			p = strtok(NULL, d);
		}	

	return x;
	
}

bool string_to_int(const char *str, int *converted_value)
{
     if(NULL == str || NULL == converted_value)
   		 {
       			 return false;
   		 }
	else if(12 < strlen(str))
		{
	        return false;
    		}
	
	if(strpbrk(str, " "))
    {
        char* p;
        char t[12];
        strcpy(t, str);
        p = strtok(t, " ");
        printf("%s \n", t);
        *converted_value = atoi(p);
        return true;    
    }	
			
   	*converted_value = atoi(str);
    
   	return true;


}
