
#include <iostream>
#include <string>
#include <exception>

#include "Query.h"
#include "Olap.h"

using namespace std;

void main()
{
	try
	{
		Query* q=new Query("id[1,2]");
		Olap olap;
		cout<<olap.count(q)<<endl;
	}
	catch(exception& e)
	{
		cout<<e.what()<<endl;
	}
	system("pause");
}

