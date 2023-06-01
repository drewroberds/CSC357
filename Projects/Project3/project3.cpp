#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#define alfred sleep

using namespace std;

char const *exitCommand = "quit";

int i = 0;
int fd[2];
char buffer[1000];
int dupOn = 0;

void resetTimer(int u){
    i = 0;
}

void sendMessage(int w){
    fflush(stdin);
    fflush(stdout);
    dup2(fd[0], 0);
    dupOn = 1;
}

int dupCopy = dup(0);

int main(int argc, char const *argv[])
{
    char *input = (char *) mmap(NULL, 1000, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
//    int *i = (int *) mmap(NULL, 4, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *pidKid = (int *) mmap(NULL, 4, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //create shared memory to store the PID of the child

    signal(SIGUSR1, resetTimer);
    signal(SIGUSR2, sendMessage);

    pipe(fd);

    int pidParent = getpid();

    if(fork() == 0)        //child process
    {
        *pidKid = getpid();
        int count = 0;
        while(strcmp(input, exitCommand) != 0)
        {
            for(i = 0; i < 10 ; i++)
            {
                alfred(1);
            }
            if(i != 11)
            {
                dup2(dupCopy, 0);
                dupOn = 0;
                write(fd[1], "It's been 10 seconds, is everything okay out there?\n", 52);
                kill(pidParent, SIGUSR2);
            }
        }
        exit(0);
    }

    else
    {          //parent process
        while(strcmp(input, exitCommand) != 0)
        {
            fgets(input, 1000, stdin);
            input[strcspn(input, "\n")] = 0;
            kill(*pidKid, SIGUSR1);
            if(dupOn == 1)
            {
                dup2(dupCopy, 0);
                printf("%s\n", input);
            }
            if(dupOn == 0 && strcmp(input, exitCommand) != 0)
            {
                printf("!%s!\n", input);
            }
            dupOn = 0;
        }
    kill(*pidKid, SIGKILL);
    wait(0);
    }

    //close pipe, free memory from mmaps
    close(*fd);
    munmap(input, 1000);
    munmap(pidKid, 4);
    return 0;
}
