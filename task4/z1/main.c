#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum status {
    FORWARD,
    BACKWARD
} status;

status signal_status = FORWARD;

void change_status(int signal)
{
    signal_status = (signal_status == FORWARD ? BACKWARD : FORWARD);
}

void quit(int signal)
{
    printf("Received SIGINT signal (CTRL + Z)\n");
    exit(EXIT_SUCCESS);
}

int main()
{
    signal(SIGINT, quit); //interruption signal that is send by ctrl+c

    struct sigaction signal_action;
    signal_action.sa_handler = change_status;
    sigaction(SIGTSTP, &signal_action, NULL); // SIGTSTP is send by ctrl+z

    char cur_char = 'A';
    while(1)
    {
        usleep(5 * 1e5);
        printf("%c\n", cur_char);
        cur_char = (signal_status == FORWARD ? cur_char + 1 : cur_char -1);
        if(cur_char < 'A')
        {
            cur_char = 'Z';
        }
        if(cur_char > 'Z')
        {
            cur_char = 'A';
        }
    }

    return 0;
}