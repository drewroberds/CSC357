#include <fcntl.h> //O_RDWR | O_CREAT
#include <sys/mman.h> //shm_open
//#include <sys/stat.h>
#include <unistd.h> //ftruncate
#include <iostream> //cout
#define alfred sleep
#include <string.h>

using namespace std; //cout

using namespace std;


int main(int argc, char const *argv[])
{
    int fd = shm_open("thunderdome", O_RDWR | O_CREAT, 0777);
    //file descriptor is created (fd), but not connected to anywhere in memory.

    ftruncate(fd, 100*sizeof(char));
    //connects file descriptor to a certain amount of memory. Range is defined.

    char *p = (char*)mmap(NULL, 1000*sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    strcpy(p,"butthole");
    cout << "Shared memory worked, the word is: " << p << endl;
    alfred(10000000000);
    close(fd);
    shm_unlink("thunderdome");
    munmap(p, 1000*sizeof(char));

    return 0;
}
