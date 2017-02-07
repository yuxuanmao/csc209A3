#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hash manipulation functions in hash_functions.c
void hash(char *hash_val, long block_size);
int check_hash(const char *hash1, const char *hash2, long block_size);

#ifndef MAX_BLOCK_SIZE
    #define MAX_BLOCK_SIZE 1024
#endif

/* Converts hexstr, a string of hexadecimal digits, into hash_val, an an 
 * array of char.  Each pair of digits in hexstr is converted to its 
 * numeric 8-bit value and stored in an element of hash_val.
 * Preconditions: 
 *    - hash_val must have enough space to store block_size elements
 *    - hexstr must be block_size * 2 characters in length
 */

void xstr_to_hash(char *hash_val, char *hexstr, int block_size) {
    for(int i = 0; i < block_size*2; i += 2) {
        char str[3];
        str[0] = hexstr[i];
        str[1] = hexstr[i + 1];
        str[2] = '\0';
        hash_val[i/2] = strtol(str, NULL, 16);
    }
}

// Print the values of hash_val in hex
void show_hash(char *hash_val, long block_size) {
    for(int i = 0; i < block_size; i++) {
        printf("%.2hhx ", hash_val[i]);
    }
    printf("\n");
}


int main(int argc, char **argv) {
    char hash_val[MAX_BLOCK_SIZE] = {'\0'}; // used for input
    char hash_val2[MAX_BLOCK_SIZE] = {'\0'}; // used for second argument
    long block_size;
    int result = 0;

    if(argc < 2 || argc > 3){
        printf("Usage: compute_hash BLCOK_SIZE [ COMPARISON_HASH ]\n");
        exit(0);
    }else{
        block_size = atoi(argv[1]);
    }
    if(block_size < 0 || block_size > MAX_BLOCK_SIZE){
        printf("The block size should be a positive integer less than MAX_BLOCK_SIZE\n");
        exit(1);
    }
    hash(hash_val, block_size);
    printf("\n---- End of Input --------\n");
    printf("The hash value of input = ");
    show_hash(hash_val, block_size);
    if(argc == 3){
        if(strlen(argv[2]) == block_size*2){
            printf("The hash value of argv[2] = ");
            // convert argv to hash value first
            xstr_to_hash(hash_val2, argv[2], block_size);
            show_hash(hash_val2, block_size);
            // check the hash value in two inputs.
            result = check_hash(hash_val, hash_val2, block_size);
        }else{
            printf("You do not have valid hash value to compare (Undefined)\n");
            exit(2);
        }
    }else{
        printf("No need to compare\n");
        exit(3);
    }
    printf("The result of the comparison is %d\n", result);

    return 0;
}

