#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <fcntl.h>
#include <signal.h>
using namespace std;
static pid_t childA;
static pid_t childB;
void killParent(int num)
{
    waitpid(childA,NULL,0);
    waitpid(childB,NULL,0);
    cout << "Parent Process is Killed!" << endl;
    exit(0);
}
void killChild(int num)
{

    kill(childA,SIGINT);
    kill(childB,SIGINT);
    killParent(0);
}
void childAExit(int num)
{
    cout << "Child Processll is Killed by Parent!" << endl;
    exit(0);
}
void childBExit(int num)
{
    cout << "Child Processl2 is Killed by Parent!" << endl;
    exit(0);
}
int main()
{
    childA = fork();
    if(childA!=0)
        childB = fork();
    if(childA == 0)
    {
        signal(SIGINT,childAExit);
        while(1)
        {
            sleep(1);
        }
    }
    else if(childB == 0)
    {
        signal(SIGINT,childBExit);
        while(1)
        {
            sleep(1);
        }
    }
    else
    {
        signal(SIGINT,killChild);
        while(1)
        {
            sleep(1);
        }
        return 0;
    }
}