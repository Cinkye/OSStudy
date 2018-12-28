#include <iostream>
using namespace std;
int main()
{
    srand(time(0));
    for(int i = 0;i < 80;++i)
    {
        int j = rand() % 319;
        cout << (j + 1) / 10 << endl;
        j = rand() % (j + 1);
        cout << j / 10 << endl;
        cout << (j + 1) /10 << endl;
        j = rand() % (320 - j - 1) + j + 2;
        cout << j / 10 << endl;
    }
}