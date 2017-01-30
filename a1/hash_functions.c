#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Complete these two functions according to the assignment specifications
#define BUFFER_SIZE 1024
void hash(char *hash_val, long block_size) {
	char* input;
	input = malloc(sizeof(char)*1024); // give the maximum number of characters can store in input.
	// we need to check whether the input is file name or stdin.
	while(fscanf(stdin, "%s", input) != EOF){
		// convert input to unsigned int representation
		unsigned int* inchar; // list of unsigned int for each character.
		inchar = malloc(sizeof(unsigned int)*strlen(input));
		for(int i=0; i<strlen(input); i++){
			inchar[i] = input[i];
		}

		// xor the input 1 byte by 1 byte
		int j = 0;
		for(int i=0; i<strlen(input); i++){
			if(i >= block_size){
				j=0;
			}
			// we need to change hash_val to unsigned int to compare.
			unsigned int temp = hash_val[j];
			unsigned int result = temp^inchar[i];
			hash_val[j] = result;
			j++;
		}
		free(inchar);
	}
	free(input);
}


int check_hash(const char *hash1, const char *hash2, long block_size) {
    // A place holder so that the file will compile without error.
	// Replace with the correct return value.
	int hash2len = strlen(hash2);
	if(hash2len < block_size){
		printf("this is not valid hash2\n");
		exit(0);
	}else{
		for(int i=0; i<block_size; i++){
			if(hash1[i] != hash2[i]){
				return i;
			}
		}
	}
	return block_size;
}
