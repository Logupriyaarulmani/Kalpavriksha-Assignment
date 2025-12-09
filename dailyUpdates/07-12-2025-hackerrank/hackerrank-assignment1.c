#include <stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>

pid_t ppid;

void child();
void parent();

//Write your code here
int get_child_exit_status()
{
        //Wait for the child to finish and get the status code.
    int status;
    waitpid(-1, &status, 0);   
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return -1;   
}
int main(void)
{  
    ppid = getpid();

    pid_t pid = fork();        

    if (pid == 0) {
        child();              
    }

    if (pid > 0) {
        int code = get_child_exit_status();
        printf("Child exited with status=%d", code);
    }

    return 0;
}

//DO NOT MODIFY CODE BELOW
void child()
{
        pid_t c_pid = getpid();
        if(c_pid == ppid)
        {
                printf("This is not a child\n");
                return;
        }
        printf("This is a child\n");
        int status;
        scanf("%d",&status);
        exit(status);
}