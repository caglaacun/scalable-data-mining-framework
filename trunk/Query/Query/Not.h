#ifndef NOT
#define NOT

#include <iostream>

#include "Symbol.h"

using namespace std;

class Not:public Symbol
{
public:
	Not()
	{

	}
	Not(string name)
	{
		this->name=name;
		this->type=TYPENOT;
	}
	~Not()
	{

	}
	string getName()
	{
		return name;
	}
};
#endif