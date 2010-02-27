#ifndef QUERY
#define QUERY
#include <iostream>

#include "Expression.h"
#include "Symbol.h"

using namespace std;
class Query
{
private:
	Expression postfixExpression;

	void scan(string query)
	{

	}

	void parse()
	{

	}

	void makePostfixExpression()
	{

	}

public:
	Query()
	{

	}

	Query(string query)
	{
		scan(query);
		parse();
		makePostfixExpression();
	}

	~Query()
	{

	}
};
#endif