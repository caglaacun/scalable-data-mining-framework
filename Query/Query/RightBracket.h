#ifndef RIGHTBRACKET
#define RIGHTBRACKET

#include <iostream>

#include "Symbol.h"

using namespace std;

class RightBracket:public Symbol
{
public:
	RightBracket()
	{

	}
	RightBracket(string name)
	{
		this->name=name;
		this->type=TYPERIGHTBRACKET;
	}
	~RightBracket()
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