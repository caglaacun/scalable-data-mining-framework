#ifndef OLAP
#define OLAP

#include <stack>
#include <iostream>

#include "DummyDataExtraction.h"
#include "Query.h"
#include "Expression.h"
#include "Symbol.h"

using namespace std;

class Olap
{
public:
	Olap();
	~Olap();
	int count(Query* query);
};
#endif