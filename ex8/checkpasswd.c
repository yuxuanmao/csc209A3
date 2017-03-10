/* this solution needs error checking! */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Read a user id and password from standard input, 
   - create a new process to run the validate program
   - send 'validate' the user id and password on a pipe, 
   - print a message 
        "Password verified" if the user id and password matched, 
        "Invalid password", or 
        "No such user"
     depending on the return value of 'validate'.
*/

/* Use the exact messages defined below in your program." */

#define VERIFIED "Password verified\n"
#define BAD_USER "No such user\n"
#define BAD_PASSWORD "Invalid password\n"
#define OTHER "Error validating password\n"


int main(void) {
    char userid[10];
    char password[10];

    /* Read a user id and password from stdin */
    printf("User id:\n");
    scanf("%s", userid);
    printf("Password:\n");
    scanf("%s", password);

	int p[2];

    if(pipe(p)){
        fprintf(stderr, "pipe failed. \n");
        return EXIT_FAILURE;
    }

    pid_t r;
    r = fork();
    if(r > 0){
        close(p[0]);
        // write
        write(p[1], userid, 10);
        write(p[1], password, 10);
        close(p[1]);

        int status;
        if (wait(&status) != -1)  {
            if (WIFEXITED(status)) {
                if(WEXITSTATUS(status) == 0){
                    printf(VERIFIED);
                }else if(WEXITSTATUS(status) == 2){
                    printf(BAD_PASSWORD);
                }else if(WEXITSTATUS(status) == 3){
                    printf(BAD_USER);
                }else if(WEXITSTATUS(status) == 1){
                    printf(OTHER);
                }
            } else {
                printf("[%d] Child exited abnormally\n", getpid());
            } 
        }  
    }else if(r == 0){
        
        close(p[1]);
        dup2(p[0], STDIN_FILENO);
        close(p[0]);
        execlp("./validate", "./validate", NULL);

    }else{
        perror("fork has error");
        exit(0);
    }

    return 0;
}
