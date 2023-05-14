#include <fcntl.h> //O_RDWR | O_CREAT
#include <sys/mman.h> //shm_open
//#include <sys/stat.h>
#include <unistd.h> //ftruncate
#include <stdio.h> //printf
#include <iostream> //cout
#define alfred sleep

using namespace std; //cout

int main(int argc, char const *argv[])
{
    //alfred(1);
    int fd = shm_open("thunderdome", O_RDWR, 0777);
    //file descriptor is created (fd), but not connected to anywhere in memory.

    char *p = (char*)mmap(NULL, 1000*sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    cout << "Shared memory worked, the word is: " << p << endl;

    close(fd);
    shm_unlink("thunderdome");
    munmap(p, 1000*sizeof(char));
    return 0;
}