#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>
#include "put.c"
#include "readln.c"

#define NR PIPE_BUF

int containsDol(char *tok) {
	int i, r = 0;
	for(i = 0; i < strlen(tok); i++){
		if(tok[i] == '$')
			r = 1;
	}
	return r;
}

int main(int argc, char *argv[]) {
	char *tmp, *tok, *buf = malloc(PIPE_BUF);
	int i, j, n, status, exit, nrChilds = 0, column[argc-1], execN[argc-1];
	int pfd[2];
	while((n = readln(0, buf, PIPE_BUF)) > 1){
		pipe(pfd);
		if(fork() == 0){
			buf[strlen(buf) - 1] = '\0';
			tmp = strdup(buf);
			for(i = 0,j=0; i < argc; i++){
				if(containsDol(argv[i])){
					execN[j] = i;
					column[j++] = argv[i][1] - '0';
				}
			}
			tok = strtok(tmp, ":");
			for(i = 1,j=0; tok; i++){
				if(i == column[j]){
					strcpy(argv[execN[j++]], tok);
				}
				tok = strtok(NULL,":");
			}
			close(pfd[0]);
			dup2(pfd[1], 1);
			close(pfd[1]);
			execvp(argv[1], argv + 1);
			perror("Command failed");
			_exit(-1);

		}
		wait(&status);
		exit = WEXITSTATUS(status);
		put(buf, exit);
	}

	return 0;
}

