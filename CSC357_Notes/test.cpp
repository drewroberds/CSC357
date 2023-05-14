#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char const *argv[])
{
    for(int i = 0; i<argc ; i++){
        cout << argv[i] << endl;
    }
    return 0;
}
