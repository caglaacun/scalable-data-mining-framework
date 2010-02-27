#ifndef AND
#define AND

#include <iostream>

#include "Symbol.h"

using namespace std;

class And:public Symbol
{
public:
	And()
	{

	}
	And(string name)
	{
		this->name=name;
		this->type=TYPEAND;
	}
	~And()
	{

	}
	string getName()
	{
		return name;
	}
	string toString()
	{
		return name;
	}
};
#endif