#ifndef LEFTBRACKET
#define LEFTBRACKET

#include <iostream>

#include "Symbol.h"

using namespace std;

class LeftBracket:public Symbol
{
public:
	LeftBracket()
	{

	}
	LeftBracket(string name)
	{
		this->name=name;
		this->type=TYPELEFTBRACKET;
	}
	~LeftBracket()
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