#ifndef OPERAND
#define OPERAND

#include <iostream>
#include <sstream>

#include "Symbol.h"

using namespace std;

class Operand:public Symbol
{
private:
	size_t attributeIndex;
	size_t bitStringIndex;
public:
	Operand()
	{

	}
	Operand(string name,size_t attributeIndex,size_t bitStringIndex)
	{
		this->name=name;
		this->attributeIndex=attributeIndex;
		this->bitStringIndex=bitStringIndex;
		this->type=TYPEOPERAND;
	}
	~Operand()
	{

	}
	string getName()
	{
		return name;
	}
	size_t getAttributeIndex()
	{
		return attributeIndex;
	}
	size_t getbitStringIndex()
	{
		return bitStringIndex;
	}
	string toString()
	{
		stringstream atind;
		atind << attributeIndex;
		stringstream bits;
		bits << bitStringIndex;
		return name+" "+atind.str()+" "+bits.str();
	}
};
#endif