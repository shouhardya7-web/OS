#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int parse(char *line, char **argv) {
    int argc = 0;
    char *tok = strtok(line, " \t\n");

    while(tok && argc < MAX_ARGS - 1) {
        argv[argc++] = tok;
        tok = strtok(NULL, " \t\n");
    }

    argv[argc] = NULL;
    return argc;
}

int main(void) {

    char line[MAX_LINE];
    char *argv[MAX_ARGS];

    while(1) {

        printf("mysh > ");
        fflush(stdout);

        if(!fgets(line, sizeof(line), stdin))
            break;

        int argc = parse(line, argv);

        if(argc == 0)
            continue;

        if(strcmp(argv[0], "exit") == 0)
            break;

        pid_t pid = fork();

        if(pid == 0) {

            execvp(argv[0], argv);

            perror("execvp");
            exit(1);
        }

        else if(pid > 0) {

            int status;
            waitpid(pid, &status, 0);
        }

        else {
            perror("fork");
        }
    }

    return 0;
}
