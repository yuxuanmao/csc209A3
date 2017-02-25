#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Complete these two functions according to the assignment specifications
char* hash(FILE* f){
	char buffer;
	int count = 0; // used to count how many time we fread
	int j = 0;
	char* hash_val;
	hash_val = malloc(sizeof(char)*9);
	hash_val[0] = '\0';
	if(f == NULL){
		hash_val = "\0\0\0\0\0\0\0\0";
		return hash_val;
	}
	while(fread(&buffer, 1, 1, f) == 1){
		// convert input to unsigned int representation
		unsigned int inchar; // list of unsigned int for each character.
		inchar = buffer;

		// xor the input 1 byte by 1 byte
		
		if(count >= 8){
			j=0;
		}
		// we need to change hash_val to unsigned int to compare.
		unsigned int temp = hash_val[j];
		unsigned int result = temp^inchar;
		hash_val[j] = result;
		j++;
		count++;
	}
	return hash_val;
}

