#ifndef EXPRESSION
#define EXPRESSION

#include <iostream>
#include <queue>

#include "Symbol.h"

using namespace std;

class Expression
{
private:
	queue<Symbol*> expression;

public:
	Expression()
	{

	}

	Expression(Symbol* symbol)
	{
		expression.push(symbol);
	}

	~Expression()
	{

	}

	void addSymbolToExpression(Symbol* symbol)
	{
		expression.push(symbol);
	}

	Symbol* removeSymbolFromExpressionFront()
	{
		Symbol* symbol = expression.front();
		expression.pop();
		return symbol;
	}

	size_t symbolCount(){
		return expression.size();
	}
};
#endif