#include <iostream>
using namespace std;
int main(int argc,char** argv)
{
	enum months = {January,February,March,April,May,June,July,August,September,October,November,December};
	string a = "cal ";
	string v[argc-1];
	for(int i = 0;i < argc-1;++i)
		v[i] = argv[i+1];
	string s = a;
	for(int i = 0;i < argc-1;++i)
	{
		s += " ";
		s += v[i];
	}
	system(s.c_str());
	return 0;
}
