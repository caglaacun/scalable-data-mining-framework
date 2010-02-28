#ifndef EXPRESSION
#define EXPRESSION

#include <iostream>
#include <vector>

#include "Symbol.h"

using namespace std;

class Expression
{
private:
	vector<Symbol*> expression;

public:
	Expression()
	{

	}

	Expression(Symbol* symbol)
	{
		expression.push_back(symbol);
	}

	~Expression()
	{
		//cout<<"delete"<<endl;
	}

	void addSymbolToExpression(Symbol* symbol)
	{
		expression.push_back(symbol);
	}

	Symbol* getSymbolAt(size_t position)
	{
		return expression[position];
	}

	size_t count(){
		return expression.size();
	}

	void print()
	{
		for (int i=0;i<expression.size();i++)
		{
			cout<<expression[i]->toString()<<endl;
		}
	}
};
#endif