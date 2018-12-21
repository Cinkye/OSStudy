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
        ofstream out("test.txt",ios::app);
        int fd = open("test.txt",O_RDWR);
        lockf(fd,F_LOCK,10000);
        for(int i = 0;i < 10000;++i)
            {
                out << "b";
            }
        out << endl;
        lockf(fd,F_ULOCK,10000);
        exit(0);
    }
    else if(childB==0)
    {
        ofstream out("test.txt",ios::app);
        int fd = open("test.txt",O_RDWR);
        lockf(fd,F_LOCK,10000);
        for(int i = 0;i < 10000;++i)
        {
            out << "c";
        }
        out << endl;
        lockf(fd,F_ULOCK,10000);
        exit(0);
    }
    else
    {
        ofstream out("test.txt",ios::app);
        int fd = open("test.txt",O_RDWR);
        lockf(fd,F_LOCK,10000);
        for(int i = 0;i < 10000;++i)
        {
            out << "a";
        }
        out << endl;
        lockf(fd,F_ULOCK,10000);
        waitpid(childA,NULL,0);
        waitpid(childB,NULL,0);
        exit(0);
    }
}