#include <fcntl.h> //O_RDWR | O_CREAT
#include <sys/mman.h> //shm_open
//#include <sys/stat.h>
#include <unistd.h> //ftruncate
#include <stdio.h> //printf
#include <iostream> //cout
#include <string.h> //strcmp
#define alfred sleep

using namespace std; //cout

int main(int argc, char const *argv[])
{
    //alfred(1);
    int fd = shm_open("thunderdome", O_RDWR, 0777);
    //file descriptor is created (fd), but not connected to anywhere in memory.

    char *entry = (char*)mmap(NULL, 1000*sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (fd != -1)
    {
        cout << "Successfully connected to shared memory.\nHello user :)\nWaiting for input from Program 1:\n" << endl;
    }
    else
    {
        cout << "Unsuccessful connection to shared memory.\nSegmentation fault eminent.\n";
    }

    // do
    // {
    //     if(strcmp("\0", entry) != 0)
    //     {
    //         entry[strcspn(entry, "\n")] = 0;
    //         cout << "Input recieved: \"" << entry << "\"" << endl;
    //     }
    // }
    // while(strcmp("\0", entry) == 0);

    while(strcmp("\0", entry) == 0){
        //alfred(1);
        if(strcmp("quit", entry) == 0)
        {
            close(fd);
            shm_unlink("thunderdome");
            munmap(entry, 1000*sizeof(char));
            return 0;
        }
    }

    entry[strcspn(entry, "\n")] = 0;
    cout << "Input recieved: \"" << entry << "\"\n" << endl;
    cout << "My work here is done.\nSevering connection with shared memory.\nGoodbye user.\nGo touch grass." << endl;
    close(fd);
    shm_unlink("thunderdome");
    munmap(entry, 1000*sizeof(char));
    return 0;
}