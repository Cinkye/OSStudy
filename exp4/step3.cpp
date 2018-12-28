#include <iostream>
#include <fstream>
#define PAGE_NUMBER 32
#define INSTRUCTION_NUMBER 320
#define DATA_RANGE 32
using namespace std;

void displayMemory(int* memory)
{
    for(int i = 0;i < PAGE_NUMBER;++i)
    {
        cout << memory[i] << " ";
    }
    cout << endl;
}

int FIFO(int* num)
{
    int cnt = 0;
    int out = 0;
    int hit = 0;
    int memory[PAGE_NUMBER];
    for(int i = 0;i < PAGE_NUMBER;++i)
        memory[i] = -1;
    for(int i = 0;i < INSTRUCTION_NUMBER;++i)
    {
        for(int j = 0;j < PAGE_NUMBER;++j)
            if(memory[j] == num[i])
            {
                cnt++;
                hit = 1;
                break;
            }
        if(hit == 0)
        {
            memory[out] = num[i];
            out = (out + 1) % PAGE_NUMBER;
        }
        hit = 0;
        //displayMemory(memory);
    }
    return cnt;
}

int LRU(int* num)
{
    int cnt = 0;
    int out = 0;
    int hit = 0;
    int memory[PAGE_NUMBER];
    for(int i = 0;i < PAGE_NUMBER;++i)
        memory[i] = -1;
    for(int i = 0;i < INSTRUCTION_NUMBER;++i)
    {
        for(int j = 0;j < PAGE_NUMBER;++j)
            if(memory[j] == num[i])
            {
                cnt++;
                hit = 1;
                if(j == out)
                    out = (out + 1) % PAGE_NUMBER;
                else if(j < out)
                {
                    int tmp = memory[j];
                    for(int k = j;k < out -1;++k)
                        memory[k] = memory[k+1];
                    memory[out-1] = tmp;
                }
                else
                {
                    int tmp = memory[PAGE_NUMBER-1];
                    for(int k = PAGE_NUMBER-1;k > out;--k)
                        memory[k] = memory[k-1];
                    memory[out] = tmp;
                    out++;
                }
                break;
            }
        if(hit == 0)
        {
            memory[out] = num[i];
            out = (out + 1) % PAGE_NUMBER;
        }
        hit = 0;
        //displayMemory(memory);
    }
    return cnt;
}

int NRU(int* num)
{
    int cnt = 0;
    int hit = 0;
    int clk = PAGE_NUMBER * 1.5;
    int memory[PAGE_NUMBER];
    int used[PAGE_NUMBER] = {0};
    for(int i = 0;i < PAGE_NUMBER;++i) 
    {
        memory[i] = -1;
        used[i] = 0;
    }
    for(int i = 0;i < INSTRUCTION_NUMBER;++i)
    {
        if(i % clk == 0)
            for(int j = 0;j < PAGE_NUMBER;++j)
                used[j] = 0;
        for(int j = 0;j < PAGE_NUMBER;++j)
        {
            if(memory[j] == num[i])
            {
                cnt++;
                used[j] = 1;
                hit = 1;
                break;
            }
        }
        if(hit == 0)
        {
            int tmp = 0;
            for(int j = 0;j < PAGE_NUMBER;++j)
                if(used[j] == 0)
                {
                    tmp = j;
                    break;
                }
            memory[tmp] = num[i];
            used[tmp] = 1;
        }
        hit = 0;
        //displayMemory(memory);
    }
    return cnt;
}

int freePage(int* memory)
{
    for(int i = 0;i < PAGE_NUMBER;++i)
        if(memory[i] == -1)
            return i;
    return -1;
}

int min(int* memory,int* frequency)
{
    int m = 0;
    for(int i = 0;i < PAGE_NUMBER;++i)
        if(frequency[memory[i]] < frequency[memory[m]])
            m = i;
    return m;
}

int OPT(int* num)
{
    int cnt = 0;
    int hit = 0;
    
    int memory[PAGE_NUMBER];
    int frequency[DATA_RANGE]={0};
    for(int i = 0;i < PAGE_NUMBER;++i)
    {
        memory[i] = -1;
    }
    for(int i = 0;i < INSTRUCTION_NUMBER;++i)
    {
        frequency[num[i]]++;
    }
    for(int i = 0;i < INSTRUCTION_NUMBER;++i)
    {
        for(int j = 0;j < PAGE_NUMBER;++j)
        {
            if(memory[j] == num[i])
            {
                hit = 1;
                cnt++;
                break;
            }
        }
        if(hit == 0)
        {
            int free = freePage(memory);
            if(free == -1)
                free = min(memory,frequency);
            memory[free] = num[i];
        }
        hit = 0;
        //displayMemory(memory);
    }
    return cnt;
}

int main()
{
    int num[INSTRUCTION_NUMBER];
    fstream in("data.txt",ios::in);
    for(int i = 0;i < INSTRUCTION_NUMBER;++i)
    {
        string tmp;
        getline(in,tmp);
        num[i] = stoi(tmp);
    }
    cout << FIFO(num) << endl;
    cout << LRU(num) << endl;
    cout << OPT(num) << endl;
    cout << NRU(num) << endl;
}
