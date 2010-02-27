#ifndef SYMBOL
#define SYMBOL

#include <iostream>

using namespace std;

#define TYPEAND 0
#define TYPEOR 1
#define TYPENOT 2
#define TYPEDATASOURCEID 3

class Symbol abstract
{
protected:
	string name;
	size_t type;
public:
	virtual string getName()=0;
	size_t getType()
	{
		return type;
	}
};
#endif