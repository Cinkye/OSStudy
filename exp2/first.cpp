#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <fcntl.h>
using namespace std;
int main()
{
    pid_t childA = fork();
    pid_t childB;
    if(childA!=0)
        childB = fork();
    if(childA==0)
    {
        cout << "b" << endl;
        exit(0);
    }
    else if(childB==0)
    {
        cout << "c" << endl;
        exit(0);
    }
    else
    {
        cout << "a" << endl;
        exit(0);
    }
}