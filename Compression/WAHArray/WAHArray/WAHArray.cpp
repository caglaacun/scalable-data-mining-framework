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


using namespace std;
using namespace boost;
using namespace boost::detail::dynamic_bitset_count_impl;
using namespace  boost::detail;
using namespace CompressionSchemes::TestStructure;
using namespace CompressionSchemes;
void randomBitStreamTester();




int _tmain(int argc, _TCHAR* argv[])
{
TestWAHArray test;
BitsetGenerator bg1;
//string str = "111111111111111111111111111111100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001111111111111111111111111111111";
//dynamic_bitset<> bs1 =bg1.buildFromString(str);
//string str2 = "010101010101010101010101010101010101010101010101010101010101010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001010101010101010101010101010101";
//dynamic_bitset<> bs2 =bg1.buildFromString(str2);
//test.testAccuracy(bs1.flip(),bs2.flip());
//test.testAccuracy(bs1,bs2,TestWAHArray::OR);
 //	test.testLeftCompressedRightLiteral();
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