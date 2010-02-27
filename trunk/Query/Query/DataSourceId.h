#ifndef ID
#define ID

#include <iostream>

#include "Symbol.h"

using namespace std;

class DataSourceId:public Symbol
{
private:
	size_t attributeIndex;
	size_t bitStringIndex;
public:
	DataSourceId()
	{

	}
	DataSourceId(string name,size_t attributeIndex,size_t bitStringIndex)
	{
		this->name=name;
		this->attributeIndex=attributeIndex;
		this->bitStringIndex=bitStringIndex;
		this->type=TYPEDATASOURCEID;
	}
	~DataSourceId()
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
};
#endif