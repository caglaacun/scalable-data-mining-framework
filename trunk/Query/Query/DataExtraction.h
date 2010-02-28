#ifndef DATAEX
#define DATAEX

#include <iostream>
#include <boost/dynamic_bitset.hpp>

using namespace std;

typedef boost::dynamic_bitset<> bitstring;

class DataExtraction
{
public:
	DataExtraction()
	{
		data=vector<bitstring>(1,bitstring(3));
	}
	bitstring getBitString(string dataSourceID,int column,int bit)
	{
		return data[0];
	}
private:
	vector<bitstring> data;
};
#endif
