// WAHArray.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include "WAHStructure.h"
#include "BitsetGenerator.h"
#include "TestWAHArray.h"
#include <vector>
#include <algorithm>
#include <string>
#include "TimeTest.h"


using namespace std;
using namespace boost;
using namespace boost::detail::dynamic_bitset_count_impl;
using namespace  boost::detail;
using namespace CompressedStructure::TestStructure;
using namespace CompressedStructure;
void randomBitStreamTester();




int _tmain(int argc, _TCHAR* argv[])
{
TestWAHArray test;
test.OperatorTest();

return 0;
}


void randomBitStreamTester()
{
	TestWAHArray test;
	int i = 0;
	while ( i < 10)
	{
		for (int j = 0; j < 110 ; j+=10)
		{
			cout << "Lendgth : 100 , Percent : "<<j << endl; 
			test.testRandomBitStream(100,1000,j);
			cout << ""<< endl;
		}

		i++;
	}	
}