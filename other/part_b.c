#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 512 // The maximum length command

int RUN_STATUS = 1;  // flag to determine when to exit program
int WAIT_STATUS = 1; // flag to determine if process should run in the background

void rout(char *file){
    int output = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0600);
    dup2(output, 1);
    close(output);
}
void rin(char *file){
    int input = open(file, O_RDONLY);
    dup2(input, 0);
    close(input);
}

int execute(char *args[]){

    if (strcmp(args[0], "exit") == 0){
        RUN_STATUS = 0;
        return 0;
    }else if (strcmp(args[0], "cd") == 0){
        if (chdir(args[1]) == -1) printf("Directory does not exist\n");
        return 0;
    }

    pid_t pid = fork();
    int status = 0;
    int* exit_code = malloc(sizeof(int));

    if (pid < 0) { 
        printf("Fork Failed...\n");
        exit(0);
    }
    else if (pid == 0) {
        status = execvp(args[0], args);
        if (status < 0) {
            printf("Command not found: %s\n",args[0]);
            exit(0);
        }
    }
    else {
        if (WAIT_STATUS) {
            waitpid(pid, exit_code, 0);
        } else {
            WAIT_STATUS = 0;
        }
        
    }
    rin("/dev/tty");
    rout("/dev/tty");
    return *exit_code;
}


void create_pipe(char *args[]) {
    int fd[2];
    pipe(fd);

    dup2(fd[1], 1);
    close(fd[1]);

    execute(args);

    dup2(fd[0], 0);
    close(fd[0]);
}


char * tokenize(char *input) {
    int j = 0;
    char *tokens = malloc((MAX_LENGTH * 2) * sizeof(char));


    for (int i = 0; i < strlen(input); i++) {
        if (input[i] == '|') {
            tokens[j++] = ' ';
            tokens[j++] = input[i];
            tokens[j++] = ' ';
        }else{
            tokens[j++] = input[i];
        }
    }
    tokens[j] = '\0';

    // add null to the end
    *(tokens + strlen(tokens) - 1) = '\0';

    return tokens;
}


int main(void){
    char *args[MAX_LENGTH];
    while (RUN_STATUS) {
        printf("\033[0;32mAnanya\033[0;37m:\033[0;31m12040180\033[0;35m>>\033[0;37m ");
        fflush(stdout);

        char input[MAX_LENGTH];
        fgets(input, MAX_LENGTH, stdin);

        char *tokens;
        tokens = tokenize(input);

        char *arg = strtok(tokens, " ");
        int i = 0;
        while (arg) {
            if (*arg == '|') {
                args[i] = NULL;
                create_pipe(args);
                i = 0;
            }else if (*arg == '&' && *(arg + 1) == '&') {
                int exit_code = execute(args);
                if (exit_code != 0){
                    exit(0);
                }
                i = 0;
            } else {
                args[i] = arg;
                i++;
            }
            arg = strtok(NULL, " ");
        }
        args[i] = NULL;
        execute(args);
    }
    return 0;
}