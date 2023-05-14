#include <math.h>
#include <iostream>
int PAGE = 4096;
int numPages = (ceil((5000 + 24)/(float)PAGE));
int numBYTES = numPages*PAGE;

using namespace std;

int main(int argc, char const *argv[])
{
    cout << "Number of pages: " << numPages << endl;
    cout << "Number of BYTES needed: " << numBYTES << endl;
    return 0;
}