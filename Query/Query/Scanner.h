#ifndef SCANNER
#define SCANNER

#include <iostream>
#include <cctype>

#include "Expression.h"
#include "Symbol.h"
#include "And.h"
#include "Or.h"
#include "Not.h"
#include "DataSourceId.h"

using namespace std;

class Scanner
{
private:
	Expression tokens;

public:
	~Scanner()
	{

	}
	void scan(string str)
	{
		for (int i=0;i<str.size();i++)
		{
			char character = str[i];
			if(isspace(character))
			{
				continue;
			}
			else if(isalpha(character))
			{
				//cout<<character<<endl;
			}
			else if(character == '&')
			{
				//cout<<character<<endl;
			}
			else if (character == '|')
			{
				//cout<<character<<endl;
			}
			else if (character == '!')
			{
				cout<<character<<endl;
			}
		}
		Symbol* symbol = new And();
		tokens.addSymbolToExpression(symbol);
	}
};
#endif