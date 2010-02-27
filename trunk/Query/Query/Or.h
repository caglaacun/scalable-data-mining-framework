#ifndef OR
#define OR

#include <iostream>

#include "Symbol.h"

using namespace std;

class Or:public Symbol
{
public:
	Or()
	{

	}
	Or(string name)
	{
		this->name=name;
		this->type=TYPEOR;
	}
	~Or()
	{

	}
	string getName()
	{
		return name;
	}
};
#endif