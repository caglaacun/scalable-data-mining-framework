#pragma once
#include "BitsetGenerator.h"
#include <vector>
#include <string>
#include "WAHStructure.h"
#include <iostream>
#include <fstream>


using namespace std;
using namespace CompressedStructure;
using namespace CompressedStructure::TestStructure;

class TimeTest
{
public:
	TimeTest(void);
	~TimeTest(void);
	int testTimeFor(dynamic_bitset<> leftOp,dynamic_bitset<> rightOp,bool andTest);
	vector<int> totalpercentageRun(int length,bool andTest);
	void writeResultsToCSV(vector<int> timeDetails,ofstream & outPut,int length);
	void RunAndTest();

};
