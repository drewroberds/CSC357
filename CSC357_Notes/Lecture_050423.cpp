#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>

using namespace std;

/*
execv()





*/

typedef char* charp;
charp **par = *charp[4];

int main(int argc, char const *argv[])
{
char prog[100];
char argv1[100];
strcpy(argv1, "Lecture_050423");
sprintf(prog, "./%s", argv1);

charp *par = new charp[4];
par[2] = NULL;
par[0] = new char [100];
par[1] = new char [100];
strcpy(par[0], argv1);
if(fork() == 0)
    {
    strcpy(par[1], "4");
    execv(prog, par);
    cout << "execv wasn't successful" << endl;
    return 0;
    }
cout << "parent is done." << endl;
return 0;
}
