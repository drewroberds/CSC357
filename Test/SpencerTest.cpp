#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h> 

#define BUFFER_SIZE 2048

int line_received = 0;
int i=0;

void sighandler(int i){
    line_received = 1;
    i = 10;
}

void alarmhandler(int i){
    printf("%s", "inactivity detected");
}

int main(){

    signal(SIGUSR1, sighandler);
    signal(SIGALRM, alarmhandler);

    pid_t pid = fork();

    if(pid == 0){
        // alarm(5);
        // pause();
        // if(line_received){
        //     alarm(5);
        //     line_received = 0;
        //     pause();
        // }
         while(1){
            for(i = 0; i<10; i++){
                printf("%d\n", i);
                sleep(1);
            }
         }

        return 0;
    }
    else{
    //parent process
    char buffer[BUFFER_SIZE];

    while(1){
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "quit") == 0) {
            kill(pid, SIGTERM);
            break;
            }

        printf("!%s!\n", buffer);
        kill(pid, SIGUSR1);
    }

        wait(0);
    }

    return 0;
}