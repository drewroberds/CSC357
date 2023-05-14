#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>

using namespace std;

/*

CODE ORGANIZATION:

*K&R*
if(----){
    code
}

*whitesmith*
if(---)
    {
    code
    }

*allman*
if(---)
{
    code
}

-------------------------------------------------------------

To share memory between programs: identifier - namespace

if processes are independant of each other, use parallelism: parent/child forks

The effictiveness of adding more cores has a limit. At 32 cores the effectiveness reaches an
asymptotic limit as it climbs in effectiveness. Anything past this is not neccisarily more effective.

If you split a program into multiple parts, you have to keep track of:
n = how many parts it is split into
o = which part the current program is working on

ex:
n=2 -> n, id(o.., n..)

MPI:
fork()xn
    execv(---);

if n = 4, program name = calc:
    execv("./calc_4_0");
    execv("./calc_4_1");
    execv("./calc_4_2");
    execv("./calc_4_3");

execv acts like a command line input?

---------------------------------------------------------

generate file descripter

int fd = shm_open("name!", O_RDWR | O_CREAT, 0777);
shm_unlink("name!"); --- always close shared memory, if you don't: memory will exist even after close

arg1 = name
arg2 = permissions: read and write | create
arg3 = ????

ftruncate(fd, size)
    #include <unistd.h>


arg1 = file descripter
size = size of fd

typedef unisgned char BYTE;

BYTE *b = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

for all id > 0:
    int fd = shm_open(...., O_RDWR, 0777); --- Links
    BYTE *b = mmap(.........);

---------------------------------------------------------------

calc program: 

main()
n = ...
id = ...
if(id == 0)
    {

    }
else
    {
        sleep(1);

    }

-------------------------

making sure processes finish before other processes use their data:

M = A * B
gather()    ---- will wait until processes finishes

A = M * B

*/



int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
