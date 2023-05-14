#include <stdio.h>
#include <iostream>
#include <string.h>

main(){

using namespace std;

int *p;
int u = 10;
p = &u;
*p = 20;


printf("The address of p is now %d\nThe address of u is %d\nThe value of p is %d\nThe value of u is %d", &p, &u, p, u);

return 0;

}