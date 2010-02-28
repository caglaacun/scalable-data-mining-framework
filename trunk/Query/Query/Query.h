#ifndef QUERY
#define QUERY

#include <iostream>
#include <exception>

#include "Expression.h"
#include "Symbol.h"
#include "InfixToPostfix.h"
#include "Scanner.h"

using namespace std;
class Query
{
private:
	Expression* postfixExpression;
	Expression* tokens;

	void scan(string query)
	{
		Scanner s;
		tokens = s.scan(query);
	}

	void parse()
	{

	}

	void makePostfixExpression()
	{
		InfixToPostfix itp;
		postfixExpression = itp.makePostfixExpression(tokens);
	}

public:
	Query()
	{

	}

	Query(string query) throw(exception)
	{
		scan(query);
		parse();
		makePostfixExpression();
	}

	~Query()
	{

	}

	Expression* getPostfixExpression()
	{
		return postfixExpression;
	}
};
#endif