#ifndef OLAP
#define OLAP

#include <stack>

#include "DummyDataExtraction.h"
#include "Query.h"
#include "Expression.h"
#include "Symbol.h"

class Olap
{
public:
	int count(Query* query)
	{

		DataExtraction dataExtractor;
		bitstring bitstring1,bitstring2;
		bitstring result;
		Expression* postfixExpression = query->getPostfixExpression();
		stack<Symbol*> operandStack;

		for (int i=0;i<postfixExpression->count();i++)
		{

			Symbol* inputSymbol = postfixExpression->getSymbolAt(i);

			if(inputSymbol->getType()==TYPEOPERAND||inputSymbol->getType()==TYPENOT)
			{
				operandStack.push(inputSymbol);
			}
			else
			{
				if (!operandStack.empty())
				{
					Operand* operand2 = dynamic_cast<Operand*>(operandStack.top());
					operandStack.pop();
					bitstring2 = dataExtractor.getBitString(operand2);
					if (!operandStack.empty())
					{
						Symbol* possibleNot = operandStack.top();
						if (possibleNot->getType()==TYPENOT)
						{
							operandStack.pop();
							bitstring2=~bitstring2;
						}
					}
					if (!operandStack.empty())
					{
						Operand* operand1 = dynamic_cast<Operand*>(operandStack.top());
						operandStack.pop();
						bitstring1 = dataExtractor.getBitString(operand1);
						if (!operandStack.empty())
						{
							Symbol* possibleNot = operandStack.top();
							if (possibleNot->getType()==TYPENOT)
							{
								operandStack.pop();
								bitstring1=~bitstring1;
							}
						}
					}
				}
				if (inputSymbol->getType()==TYPEAND)
				{
					result=bitstring1&bitstring2;
				}
				else if (inputSymbol->getType()==TYPEOR)
				{
					result=bitstring1|bitstring2;
				}
			}
		}
		while(!operandStack.empty())
		{
			Operand* operand = dynamic_cast<Operand*>(operandStack.top());
			operandStack.pop();
			result = dataExtractor.getBitString(operand);
			if (!operandStack.empty())
			{
				Symbol* possibleNot = operandStack.top();
				if (possibleNot->getType()==TYPENOT)
				{
					operandStack.pop();
					result=~result;
				}
			}
		}
		return result.count();
	}
};
#endif