#include <iostream>
#include <map>

#ifndef REFCNTR_HXX
#define REFCNTR_HXX

using namespace std;

class RefCntr
{
public:
	static void Inc(string name);
	static void Dec(string name);
	static int Get(string name);
private:
	static map<string,int> refCountMap;
};

#endif
