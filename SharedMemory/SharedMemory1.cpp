#include <fcntl.h> //O_RDWR | O_CREAT
#include <sys/mman.h> //shm_open
//#include <sys/stat.h>
#include <unistd.h> //ftruncate
#include <iostream> //cout
#define alfred sleep
#include <string.h> //strcmp

using namespace std; //cout

char const *exitCommand = "quit\n";

int main(int argc, char const *argv[])
{
    char emptyArray[10000];

    int fd = shm_open("thunderdome", O_RDWR | O_CREAT, 0777);
    //file descriptor is created (fd), but not connected to anywhere in memory.

    ftruncate(fd, 100*sizeof(char));
    //connects file descriptor to a certain amount of memory. Range is defined.

    char *entry = (char*)mmap(NULL, 1000*sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    cout << "Shared memory initialized.\nPlease enter a message to send to Program 2: ";
    while(strcmp(entry, exitCommand) != 0)
    {
        fgets(entry, 1000, stdin);
        if(strcmp(entry, exitCommand) != 0)
        {
            cout << "Message sent." << endl;
            cout << "Connection with Program 2 has been severed, please type \"quit\" to end program.\n";
        }
    }

    close(fd);
    shm_unlink("thunderdome");
    munmap(entry, 1000*sizeof(char));

    return 0;
}
