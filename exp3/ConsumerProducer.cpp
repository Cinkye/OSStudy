#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <fstream>
#include <fcntl.h>
#include <pthread.h>
using namespace std;

struct shared_use_st
{
    int write = 0;
	char text[10];//记录写入和读取的文本
    int size = 0;
};

int main()
{
    int upperId = shmget((key_t)1161,sizeof(shared_use_st),0666|IPC_CREAT);
    int lowerId = shmget((key_t)1561,sizeof(shared_use_st),0666|IPC_CREAT);
    if(upperId == -1 || lowerId == -1)
    {
        cout << "Shared memory failed." << endl;
        exit(-1);
    }
    void* upper = NULL;
    void* lower = NULL;
    pthread_mutex_t* mutex;
    pthread_mutex_init(mutex,NULL);
    srand(time(0));
    pid_t producerUpper = fork();
    pid_t consumerLower = 0;
    pid_t consumerUpper = 0;
    pid_t consumerAll = 0;
    if(producerUpper != 0)
        consumerLower = fork();
    if(consumerLower != 0)
        consumerUpper = fork();
    if(consumerUpper != 0)
        consumerAll = fork();
    while(true)
    {
        if(producerUpper == 0)
        {
            pthread_mutex_lock(mutex);
            upper = shmat(upperId,(void*)0,0);
            struct shared_use_st* up = (struct shared_use_st*)upper;
            while(up->write == 1)
                sleep(1);
            up->write = 1;
            if(up->size < 10)
            {
                up->text[up->size] = (char)('A' + rand() % 26);
                up->size++;
            }
            cout << "Producer Upper working" << endl;
            for(int i = 0;i < up->size;++i)
                cout << up->text[i] << " ";
            cout << endl << endl;
            
            up->write = 0;
            shmdt(upper);
            pthread_mutex_unlock(mutex);
            sleep(3);
        }
        else if(consumerLower == 0)
        {
            pthread_mutex_lock(mutex);
            lower = shmat(lowerId,(void*)0,0);
            struct shared_use_st* low = (struct shared_use_st*)lower;
            while(low->write == 1)
                sleep(1);
            low->write = 1;
            if(low->size > 0)
                low->size--;
            cout << "Consumer Lower working" << endl;
            for(int i = 0;i < low->size;++i)
                cout << low->text[i] << " ";
            cout << endl << endl;
            
            low->write = 0;
            shmdt(lower);
            pthread_mutex_unlock(mutex);
            sleep(3);
        }
        else if(consumerUpper == 0)
        {
            pthread_mutex_lock(mutex);
            upper = shmat(upperId,(void*)0,0);
            struct shared_use_st* up = (struct shared_use_st*)upper;
            while(up->write == 1)
                sleep(1);
            up->write = 1;
            if(up->size > 0)
                up->size--;
            cout << "Consumer Upper working" << endl;
            for(int i = 0;i < up->size;++i)
                cout << up->text[i] << " ";
            cout << endl << endl;
            
            up->write = 0;
            shmdt(upper);
            pthread_mutex_unlock(mutex);
            sleep(3);
        }
        else if(consumerAll == 0)
        {
            pthread_mutex_lock(mutex);
            upper = shmat(upperId,(void*)0,0);
            struct shared_use_st* up = (struct shared_use_st*)upper;
            lower = shmat(lowerId,(void*)0,0);
            struct shared_use_st* low = (struct shared_use_st*)lower;
            while(up->write == 1)
                sleep(1);
            up->write = 1;
            while(low->write == 1)
                sleep(1);
            low->write = 1;
            if(up->size > 0 && low->size > 0)
            {
                if(rand() % 2 == 0)
                    low->size--;
                else
                    up->size--;
            }
            else if(up->size > 0)
                up->size--;
            else if(low->size > 0)
                low->size--;
            cout << "Consumer All working" << endl;
            for(int i = 0;i < up->size;++i)
                cout << up->text[i] << " ";
            for(int i = 0;i < low->size;++i)
                cout << low->text[i] << " ";
            cout << endl << endl;
            up->write = 0;
            low->write = 0;
            shmdt(upper);
            shmdt(lower);
            pthread_mutex_unlock(mutex);
            sleep(3);
        }
        else
        {
            pthread_mutex_lock(mutex);
            lower = shmat(lowerId,(void*)0,0);
            struct shared_use_st* low = (struct shared_use_st*)lower;
            while(low->write == 1)
                sleep(1);
            low->write = 1;
            if(low->size < 10)
            {
                low->text[low->size] = (char)('a' + rand() % 26);
                low->size++;
            }
            cout << "Producer Lower working" << endl;
            for(int i = 0;i < low->size;++i)
                cout << low->text[i] << " ";
            cout << endl << endl;
            low->write = 0;
            shmdt(lower);
            pthread_mutex_unlock(mutex);
            sleep(3);
        }
    }
}