#include "StdAfx.h"
#include "TimeTest.h"

TimeTest::TimeTest(void)
{
}

TimeTest::~TimeTest(void)
{
}

int TimeTest::testTimeFor(dynamic_bitset<> leftOp, dynamic_bitset<> rightOp, bool andTest)
{
	WAHStructure  * leftWs = new WAHStructure();
	leftWs->CompressWords(leftOp);
	WAHStructure  * rightWs = new WAHStructure();
	rightWs->CompressWords(rightOp);
	WAHStructure * compr = NULL;
	clock_t begin,end;
	if (andTest)
	{
		begin = clock() * CLK_TCK;
		compr = *(leftWs) & *(rightWs);
		end = clock()* CLK_TCK;
	}else{
		begin = clock();
		compr = *(leftWs) | *(rightWs);
		end = clock();
	}
	delete leftWs,rightWs,compr;
	leftWs = NULL;rightWs = NULL;compr = NULL;
	return (end - begin);

}

void TimeTest::writeResultsToCSV( vector<int> timeDetails,ofstream & outPut,int length )
{
	vector<int>::iterator iter = timeDetails.begin();
	outPut<< "Length : "<< length << endl;
	cout <<  "Length : "<< length << endl;
	for (size_t i = 0; i < timeDetails.size(); i++)
	{
		outPut << (i * 10)<< " , " << *(iter)<< endl;
	}
	outPut << ""<<endl;
}
void TimeTest::RunAndTest()
{
	ofstream output("AND Test.txt");
	for (int i = 5000000; i < 10000000 ; i += 500000)
	{
		vector<int> result = totalpercentageRun(i,true);
		writeResultsToCSV(result,output,i);
		output.flush();
	}

	output.close();
}

vector<int> TimeTest::totalpercentageRun(int length,bool andTest)
{
	vector<int> result;
	BitsetGenerator bg;
	//for (int i = 0 ; i <= 100 ; i+=10)
	//{
	//	dynamic_bitset<> leftOp = bg.getRandomBitStream(length,i);
	//	dynamic_bitset<> rightOp = bg.getRandomBitStream(length,i);
	//	int time = testTimeFor(leftOp,rightOp,andTest);
	//	result.push_back(time);
	//}

	for (int i = 0 ; i <= 0 ; i++)
	{
		dynamic_bitset<> leftOp = bg.getRandomBitStream(length,i);
		dynamic_bitset<> rightOp = bg.getRandomBitStream(length,i);
		int time = testTimeFor(leftOp,rightOp,andTest);
		result.push_back(time);
	}

	return result;
}

