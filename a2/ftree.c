#include <stdio.h>
// Add your system includes here.
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include "ftree.h"
#include "hash.h"


/*
 * Returns the FTree rooted at the path fname.
 */
struct TreeNode *generate_ftree(const char *fname) {
    // Your implementation here.
    struct TreeNode* top;
    top = malloc(sizeof(struct TreeNode));
    struct stat* buf;
    buf = malloc(sizeof(struct stat));
    if(lstat(fname, buf) != 0){
    	// checking the error of lstat
    	perror("lstat");
    	exit(1);
    }


    if(S_ISREG(buf->st_mode) != 0 || S_ISLNK(buf->st_mode) != 0){
    	// this is file or link
    	top->fname = malloc(sizeof(char)*strlen(fname)+1);
    	top->fname[0] = '\0';
    	strcpy(top->fname, fname);

    	// setting the content of struct TreeNode
    	top->permissions = (buf->st_mode)&0777;	
    	top->contents = NULL;
    	FILE* fp;
    	fp = fopen(fname, "r");
    	// if fp is NULL we will get 00000000 for the hash
    	top->hash = hash(fp);
    	top->next = NULL;
    	free(buf);
    	if(fp != NULL){
    		// we close file only if the fopen succeeded to open the file
    		fclose(fp);
    	}
    	return top;
    }else if(S_ISDIR(buf->st_mode) != 0){
		// the buf is directory
		top->fname = malloc(sizeof(char)*strlen(fname)+1);
    	strcpy(top->fname, generate_name(fname));
    	top->permissions = (buf->st_mode)&0777;
    	top->hash = NULL;
    	top->contents = NULL;
    	top->next = NULL;
    	// before open the direcotry we need to check permissions
    	if(top->permissions == 000){
    		// if the user has no permission to access to the content of the directory
    		// we do not need to open directory anymore.
    		return top;
    	}
    	// we create a pointer to struct TreeNode* so that we can keep node of root
    	struct TreeNode** nxtFLE;
    	nxtFLE = &(top);
		DIR* dirp;
		if((dirp = opendir(fname)) == NULL){
			// did not succeed to open directory
			perror("opendir");
			exit(2);
		}
		struct dirent* dp;
		dp = malloc(sizeof(struct dirent));
		int errno = 0; // setting the errno value
		// iterate till we find the end of directory or error occurs
	    do {
	        errno = 0;
	        if ((dp = readdir(dirp)) != NULL && dp->d_name[0] != '.') {
				// looking the inside of directory
	        	// we manually creates a valid path so that lstat can open
	        	char* path;
	        	path = malloc(sizeof(char)*(strlen(fname) + 1 + strlen(dp->d_name) + 2));
	        	path[0] = '\0';
	        	strncat(path, fname, strlen(fname));
	        	path[strlen(fname)] = '/';
	        	path[strlen(fname)+1] = '\0';
	        	strncat(path, dp->d_name, strlen(dp->d_name));
	        	
	        	// we find th parent node's name of the current node
	        	char* parent_path = generate_name(fname);
	        	// check if the prevuious node 
	        	nxtFLE = contents_or_next(*nxtFLE, parent_path);;
        		*nxtFLE = malloc(sizeof(struct TreeNode*));
        		*nxtFLE = generate_ftree(path);

	        	if(dp->d_type != DT_DIR){
	        		// this is file or link
	        		// then we need to generate the file name from the full path
	        		strcpy((*nxtFLE)->fname, generate_name(path));	
	        	}
	        	free(path);
	        }
	    } while (dp != NULL);
	    if(errno != 0){
	    	// did not scceed to read directory
	    	perror("readdir");
	    	exit(3);
	    }else{
	    	closedir(dirp);
	    }
	    free(dp);
	    free(buf);
	    return top;
	}
    return NULL;
}

/*
 * HELP FUNCTION
 * generate the leaf directory or file in the given path and return it 
 */
char* generate_name(const char* path){
	char* result;
	int key = -1; // used to point the last "/" in path
	for(int i = strlen(path)-1; i >= 0; i--){
		if(path[i] == '/'){
			key = i;
			break;
		}
	}
	result = malloc(sizeof(char)*(strlen(path)-key));
	int j = 0;
	for(int i = key+1; i <= strlen(path); i++){
		result[j] = path[i];
		j++;
	}
	return result;

}

/*
 This is the helper function that is used to determine the current TreeNode
 will point to contents or next TreeNode
*/
struct TreeNode** contents_or_next(struct TreeNode* current, char* previous){
	if(strstr(previous, current->fname) != NULL && current->hash == NULL){
		// if the parent node is a direcotry we use contents
		return &(current->contents);
	}else{
		return &(current->next);
	}
}


/*
 * Prints the TreeNodes encountered on a preorder traversal of an FTree.
 */
void print_ftree(struct TreeNode *root) {
    // Here's a trick for remembering what depth (in the tree) you're at
    // and printing 2 * that many spaces at the beginning of the line.
    static int depth = 0;
    printf("%*s", depth * 2, "");
    // -----------------------------------------------
    if(root != NULL){
    	depth++;
    	if(root->hash == NULL){
    		
    		printf("==== %s (%o) ====\n", root->fname, root->permissions);
    		if(root->next == NULL && root->contents == NULL){
    			depth--;
    		}

    	}else{
    		printf("%s (%o) \n", root->fname, root->permissions);
    		if(root->next == NULL ){
    			depth--;
    		}
    	}
    	if(root->contents != NULL){
    		print_ftree(root->contents);
    		if(root->next == NULL && root->contents != NULL){
				depth--;
    			
    		}
    	}
    	if(root->next != NULL){
    		depth--;

    		print_ftree(root->next);

    	}

    }
    

}
