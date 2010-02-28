#ifndef SCANNEREXCEPTION
#define	SCANNEREXCEPTION

#include <iostream>
#include <exception>

using namespace std;

class OperandSyntaxException: public exception
{
	virtual const char* what() const throw()
	{
		return "Error in operand syntax: use operands such as id[1,2]";
	}
}operandSyntaxException; 

class SymbolException: public exception
{
	virtual const char* what() const throw()
	{
		return "Symbol is not valid";
	}
}symbolException; 

#endif