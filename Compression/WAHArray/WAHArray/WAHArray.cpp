// WAHArray.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include "WAHStructure.h"
#include "BitsetGenerator.h"
#include "TestWAHArray.h"
#include <vector>
#include <algorithm>


using namespace std;
using namespace boost;
using namespace boost::detail::dynamic_bitset_count_impl;
using namespace  boost::detail;
void randomBitStreamTester();


int _tmain(int argc, _TCHAR* argv[])
{
	dynamic_bitset<> wer(100);
	
	
	// 	dynamic_bitset<> bitset(32);
	// 	bitset[1] = 1;
	// 	WAHStructure ws;
	// 	ws.compressWords(bitset);
	// 	cout << ws.count() << endl;

	/*TestWAHArray tws;
	tws.testBitCount();*/
	
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