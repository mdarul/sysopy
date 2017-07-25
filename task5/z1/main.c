#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>


const int MAX_PROGS = 32;
const int MAX_ARGS = 8;

void evaluate_line(char *line);
void evaluate_arguments(char *line, char *args[MAX_PROGS][MAX_ARGS], int *progs_amount);
void run_processes(int progs_amount, char *args[MAX_PROGS][MAX_ARGS]);

int main()
{
    char *line = NULL;
    size_t n = 0;
    while(getline(&line, &n, stdin))
    {
        evaluate_line(line);
    }
    return 0;
}

void evaluate_line(char *line)
{
    char *args[MAX_PROGS][MAX_ARGS];
    int progs_amount = 0;
    evaluate_arguments(line, args, &progs_amount);
    run_processes(progs_amount, args);
}

void evaluate_arguments(char *line, char *args[MAX_PROGS][MAX_ARGS], int *progs_amount)
{
    char *commands[MAX_PROGS];

    for(int i=0; i<MAX_PROGS; i++)commands[i] = NULL;
    for(int i=0; i<MAX_PROGS; i++)
        for(int j=0; j<MAX_ARGS; j++)args[i][j] = NULL;

    commands[0] = strtok(line, "\n|");
    (*progs_amount)++;

    while((*progs_amount)<MAX_PROGS && (commands[*progs_amount] = strtok(NULL,"\n|")))
    {
        (*progs_amount)++;
    }

    for(int i=0; i<(*progs_amount); i++)
    {
        args[i][0] = strtok(commands[i], " ");
        if(args[i][0])
        {
            int j=1;
            while(j < MAX_ARGS)
            {
                args[i][j] = strtok(NULL, "\n ");
                if(!args[i][j])break;
                j++;
            }
        }
    }
}

void run_processes(int progs_amount, char *args[MAX_PROGS][MAX_ARGS])
{
    //manual: fds[0] - in, fds[1] - out
    int fds[4];
    fds[0] = fds[1] = fds[2] = fds[3] = -1;
    pid_t *pids = malloc(progs_amount * sizeof(pid_t));

    for (int i = 0; i < progs_amount; i++)
    {
        fds[1] = fds[3];
        fds[0] = fds[2];

        if (pipe(fds + 2) == -1) // in fds[2:3] we have our current fds
        {
            perror("pipe failed");
            exit(1);
        }

        pids[i] = fork(); // create child
        if(pids[i] == 0)
        { // child code
            if(i != 0)
            {
                if(fds[0] != -1) dup2(fds[0], STDIN_FILENO); // copy STDIN_FILENO to fds[0], we change standard input
                if(fds[1] != -1) close(fds[1]);
            }

            if(i < progs_amount-1)
            {
                if(fds[2] != -1) close(fds[2]);
                if(fds[3] != -1) dup2(fds[3], STDOUT_FILENO); // change standard output
            }

            if(execvp(args[i][0], args[i]) == -1)
            {
                printf("In command %d:\n", i);
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        }
        else if(pids[i] < 0)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if(fds[1] != -1) close(fds[1]);
        if(fds[0] != -1) close(fds[0]);
    }
}