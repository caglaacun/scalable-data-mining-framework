#ifndef DATAEX
#define DATAEX

#include <iostream>
#include <boost/dynamic_bitset.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Operand.h"

using namespace std;

typedef boost::dynamic_bitset<> bitstring;

class DataExtraction
{
public:
	DataExtraction()
	{
		srand ( time(NULL) );
		size_t size=10;
		data=vector<bitstring>(1,bitstring(size));

		cout<<"bitstring:";

		for (int i=0;i<size;i++)
		{
			data[0][i]=rand() % 2;
			cout<<data[0][i];
		}
		cout<<endl<<endl;
	}
	bitstring getBitString(string dataSourceID,int column,int bit)
	{
		return data[0];
	}
	bitstring getBitString(Operand* op)
	{
		return data[0];
	}
private:
	vector<bitstring> data;
};
#endif
