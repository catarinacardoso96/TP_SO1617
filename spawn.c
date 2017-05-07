#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

ssize_t readln(int fildes, char *buf, size_t nbyte){
	int i = 0;
	int rd;
	while((i<nbyte) && (rd=read(fildes,buf+i,1)) > 0 && *(buf+i) != '\n'){
		i++;
	}
	return ++i;
}

int containsDol(char *tok) {
	int i, r = 0;
	for(i = 0; i < strlen(tok); i++){
		if(tok[i] == '$')
			r = 1;
	}
	return r;
}

int main(int argc, char *argv[]) {
	char *tmp, *tok, *buf = malloc(1024);
	int i, j, n, column[argc-2], execN[argc-2];
	while((n = readln(0, buf, 1024)) > 1){
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
			if(execvp(argv[1], argv + 1) == -1){
				perror("Command failed");
				_exit(-1);
			}
			_exit(0);
		}
	}
	return 0;
}

