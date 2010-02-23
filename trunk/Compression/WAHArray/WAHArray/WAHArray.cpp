// WAHArray.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include "WAHStructure.h"
#include "BitsetGenerator.h"
#include <vector>

using namespace std;
using namespace boost;
void RunTest(dynamic_bitset<> &leftoperand,dynamic_bitset<> & rightOperand,int TestNo,bool andTestType);
void pureBitTest();
void testLeftCompressedRightLiteral();
void testDecompress();
bool testAccuracy(dynamic_bitset<> &leftOp, dynamic_bitset<> &rightOp,bool andTest);
void pureBiteTestDecompressionTest();
void printOperands(dynamic_bitset<> & leftOp, dynamic_bitset<> & rightOp);
void testLeftCompressedRightLiteralDecompressionTest();
void testLeftCompressedRightLiteralDecompressionTest2();
void EncodeDecodeTest();
void testRightCompressedLeftLiteralDecompressionTest2();

int _tmain(int argc, _TCHAR* argv[])
{
	//testLeftCompressedRightLiteral();
	//pureBitTest();
	//vectorConversion();
	//testDecompress();
	//pureBiteTestDecompressionTest();
	/*cout << "Test set 1:" << endl;
	testLeftCompressedRightLiteralDecompressionTest();
	cout << "" << endl;*/
	cout << "Test set 2:" << endl;
	//testLeftCompressedRightLiteralDecompressionTest2();
	testRightCompressedLeftLiteralDecompressionTest2();
	//EncodeDecodeTest();
	return 0;
}


void testDecompress()
{	
	cout << "Test decompress : " << endl;
	BitsetGenerator * bg = new BitsetGenerator();
	dynamic_bitset<> bitSet  = bg->getPureBitset(62,false);
	WAHStructure * st = new WAHStructure();
	cout << "Original Bitset : " <<bitSet <<endl;
	st->buildArray(bitSet);
	st->compressWords();
	cout << "Compressed Words : " << endl;
	st->printCompressedStream();
	dynamic_bitset<> bitSet2 = st->decompress();
	cout << "decompressed Structure  : " << bitSet2<<endl;
	bitSet2 = bitSet & bitSet2;
	cout << "Count : " << bitSet2.to_ulong() << endl;

	delete bg,st;
}

bool testAccuracy(dynamic_bitset<> &leftOp, dynamic_bitset<> &rightOp,bool andTest)
{
	WAHStructure * left = new WAHStructure();
	WAHStructure * right = new WAHStructure();
	left->buildArray(leftOp);
	right->buildArray(rightOp);
	left->compressWords();
	right->compressWords();
	dynamic_bitset<> resultBitMap ; 
	cout << "Compressed Word left : "<< endl;
	left->printCompressedStream();
	cout << ""<< endl;
	cout << "Compressed Word right : "<< endl;
	right->printCompressedStream();
	cout << "" << endl;
	WAHStructure * result = NULL;
	if (andTest)
	{
		result = *(left) & *(right);
		resultBitMap = leftOp & rightOp;
	}else
	{
		//
		resultBitMap = leftOp | rightOp;
	}
	cout << "Result : " << endl;
	result->printCompressedStream();
	dynamic_bitset<> decompressedResult = result->decompress();
	//dynamic_bitset<> testAbleBitmap = resultBitMap & decompressedResult;
	bool state = false;
	if (resultBitMap == decompressedResult)
	{
		state = true;
	}
	delete left,right,result;
	return state;
}

void EncodeDecodeTest(){
	BitsetGenerator * bg = new BitsetGenerator();
	dynamic_bitset<> temp = bg->getPureBitset(35,true);
dynamic_bitset<> temp1 = temp; 
	WAHStructure * ws = new WAHStructure();
	cout << "original Bitmap : " << temp << endl;

	ws->buildArray(temp1);
	ws->compressWords();
	ws->printCompressedStream();	

	dynamic_bitset<> decoded = ws->decompress();
	if (decoded == temp)
	{
		cout << "Decompress Success" << endl;
	}else
	{
	cout << "Decoded Bitmap : " << decoded << endl;	
		cout << "Decompress Unsuccess" << endl;
		cout << "Temp Size : " << temp.size() << endl;
		cout << "Decoded Size : " << decoded.size() << endl;
	}
	delete ws,bg;
}

void testLeftCompressedRightLiteralDecompressionTest(){
	cout << "Test : Left Compressed Right Literal" << endl;
	BitsetGenerator * bg = new BitsetGenerator();
	dynamic_bitset<> x = bg->getPureBitset(31 * 2,false);
	dynamic_bitset<> y = bg->getAlternatingBitset(31*2);
	printOperands(x,y);
	bool resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(3*31,31,3*31 -1,false);
	y = bg->getAlternatingBitset(31*3);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
	y = bg->getAlternatingBitset(5*31);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
	y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(5*31,0,4*31 -1,false);
	y = bg->getAlternatingBitset(5*31);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
	y = bg->getMiddleCompressedBitset(6*31,61,4*31 -1,false) & bg->getAlternatingBitset(6*31);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
	y = bg->getMiddleCompressedBitset(6*31,31,4*31 -1,false) & bg->getAlternatingBitset(6*31);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
	y = bg->getMiddleCompressedBitset(6*31,61,5*31 -1,false) & bg->getAlternatingBitset(6*31);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;
	delete bg;
}

void pureBiteTestDecompressionTest()
{
	cout << "Test : Pure Bit Test Decompression Test" << endl;
	BitsetGenerator *bg = new BitsetGenerator();
	dynamic_bitset<> x = bg->getPureBitset(31 * 5,false);
	dynamic_bitset<> y = bg->getPureBitset(31 * 5,false);
	bool test = testAccuracy(x,y,true);
	printOperands(x,y);
	cout << "Test 1" << endl;
	cout << "Accuracy : " << test<<endl;
	cout << ""<< endl;

	x = bg->getPureBitset(31 * 5,false);
	y = bg->getPureBitset(31 * 5,true);
	cout << "Test 2" << endl;
	test = testAccuracy(x,y,true);

	printOperands(x,y);
	cout << "Accuracy : " << test<<endl;
	cout << ""<< endl;

	x = bg->getPureBitset(31 * 5,true);
	y = bg->getPureBitset(31 * 5,true);
	cout << "Test 3" << endl;
	test = testAccuracy(x,y,true);
	printOperands(x,y);
	cout << "Accuracy : " << test<<endl;
	cout << ""<< endl;

	x = bg->getPureBitset(31 * 5,true);
	y = bg->getPureBitset(31 * 5,false);
	cout << "Test 4" << endl;
	test = testAccuracy(x,y,true);
	printOperands(x,y);
	cout << "Accuracy : " << test<<endl;
	cout << ""<< endl;

	delete bg;
}

void testLeftCompressedRightLiteralDecompressionTest2(){
	cout << "Test : Left Compressed Right Literal" << endl;
	BitsetGenerator * bg = new BitsetGenerator();
	dynamic_bitset<> x = bg->getPureBitset(31 * 2,true);
	dynamic_bitset<> y = bg->getAlternatingBitset(31*2);
	cout << "Test : 1"<<endl;
	printOperands(x,y);
	bool resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

cout << "Test : 2"<<endl;
	x = bg->getMiddleCompressedBitset(3*31,31,3*31 -1,true);
	y = bg->getAlternatingBitset(31*3);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

cout << "Test : 3"<<endl;
	x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
	y = bg->getAlternatingBitset(5*31);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	cout << "Test : 4"<<endl;
	x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
	y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	cout << "Test : 5"<<endl;
	x = bg->getMiddleCompressedBitset(5*31,0,4*31 -1,true);
	y = bg->getAlternatingBitset(5*31);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	cout << "Test : 6"<<endl;
	x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
	y = bg->getMiddleCompressedBitset(6*31,61,4*31 -1,true) & bg->getAlternatingBitset(6*31);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	cout << "Test : 7"<<endl;
	x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
	y = bg->getMiddleCompressedBitset(6*31,31,4*31 -1,true) & bg->getAlternatingBitset(6*31);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	cout << "Test : 8"<<endl;
	x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
	y = bg->getMiddleCompressedBitset(6*31,61,5*31 -1,true) & bg->getAlternatingBitset(6*31);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;
	delete bg;
}

void testRightCompressedLeftLiteralDecompressionTest2(){
	cout << "Test : Left Compressed Right Literal" << endl;
	BitsetGenerator * bg = new BitsetGenerator();
	dynamic_bitset<> x = bg->getAlternatingBitset(31*2);
	dynamic_bitset<> y = bg->getPureBitset(31 * 2,true);
	cout << "Test : 1"<<endl;
	printOperands(x,y);
	bool resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	cout << "Test : 2"<<endl;
	x = bg->getAlternatingBitset(31*3);
	y = bg->getMiddleCompressedBitset(3*31,31,3*31 -1,true);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	cout << "Test : 3"<<endl;
	x = bg->getAlternatingBitset(5*31);
	y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	cout << "Test : 4"<<endl;
	x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
	y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,true);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	cout << "Test : 5"<<endl;
	x = bg->getAlternatingBitset(5*31);
	y = bg->getMiddleCompressedBitset(5*31,0,4*31 -1,true);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	cout << "Test : 6"<<endl;
	x = bg->getMiddleCompressedBitset(6*31,61,4*31 -1,true) & bg->getAlternatingBitset(6*31);
	y = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	cout << "Test : 7"<<endl;
	x = bg->getMiddleCompressedBitset(6*31,31,4*31 -1,true) & bg->getAlternatingBitset(6*31);
	y = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	cout << "Test : 8"<<endl;
	x = bg->getMiddleCompressedBitset(6*31,61,5*31 -1,true) & bg->getAlternatingBitset(6*31);
	y = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,true);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;
	delete bg;
}


void printOperands(dynamic_bitset<> & leftOp, dynamic_bitset<> & rightOp)
{
	cout << "Left Operand : " << leftOp << endl;
	cout << "Right Operand : " << rightOp << endl;
}

void pureBitTest()
{
	cout << "Test : Pure Bit Test" << endl;
	BitsetGenerator *bg = new BitsetGenerator();
	dynamic_bitset<> x = bg->getPureBitset(31 * 5,false);
	dynamic_bitset<> y = bg->getPureBitset(31 * 5,false);
	RunTest(x,y,1,true);
	cout << ""<< endl;

	x = bg->getPureBitset(31 * 5,false);
	y = bg->getPureBitset(31 * 5,true);
	RunTest(x,y,2,true);
	cout << ""<< endl;

	x = bg->getPureBitset(31 * 5,true);
	y = bg->getPureBitset(31 * 5,true);
	RunTest(x,y,3,true);
	cout << ""<< endl;

	x = bg->getPureBitset(31 * 5,true);
	y = bg->getPureBitset(31 * 5,false);
	RunTest(x,y,4,true);
	cout << ""<< endl;

	delete bg;
}

void testLeftCompressedRightLiteral(){
	cout << "Test : Left Compressed Right Literal" << endl;
	BitsetGenerator * bg = new BitsetGenerator();
	dynamic_bitset<> x = bg->getPureBitset(31 * 2,false);
	dynamic_bitset<> y = bg->getAlternatingBitset(31*2);
	RunTest(x,y,1,true);
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(3*31,31,3*31 -1,false);
	y = bg->getAlternatingBitset(31*3);
	RunTest(x,y,2,true);
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
	y = bg->getAlternatingBitset(5*31);
	RunTest(x,y,3,true);
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
	y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
	RunTest(x,y,4,true);
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(5*31,0,4*31 -1,false);
	y = bg->getAlternatingBitset(5*31);
	RunTest(x,y,5,true);
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
	y = bg->getMiddleCompressedBitset(6*31,61,4*31 -1,false) & bg->getAlternatingBitset(6*31);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	RunTest(x,y,6,true);
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
	y = bg->getMiddleCompressedBitset(6*31,31,4*31 -1,false) & bg->getAlternatingBitset(6*31);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	RunTest(x,y,7,true);
	cout << ""<< endl;

	x = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
	y = bg->getMiddleCompressedBitset(6*31,61,5*31 -1,false) & bg->getAlternatingBitset(6*31);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	RunTest(x,y,8,true);
	cout << ""<< endl;
	delete bg;
}

void RunTest(dynamic_bitset<> &leftoperand,dynamic_bitset<> & rightOperand,int TestNo,bool andTestType)
{
	cout << "Test No : " << TestNo << endl;
	WAHStructure * leftOp = new WAHStructure();
	WAHStructure * rightOp = new WAHStructure();
	leftOp->buildArray(leftoperand);
	rightOp->buildArray(rightOperand);
	leftOp->compressWords();
	rightOp->compressWords();
	cout << "Print Left Compressed Stream : " << endl;
	leftOp->printCompressedStream();
	cout << "print Right Compressed Stream : " << endl;
	rightOp->printCompressedStream();
	WAHStructure * result = NULL;

	if (andTestType)
	{
		cout << "AND test : " << endl;
		result = *(leftOp) & *(rightOp);
	}else {
		cout << "OR test : " << endl;
		//result = *(leftOp) | *(rightOp);
	}
	cout << "Test Result : " << endl;
	result->printCompressedStream();


	delete leftOp,rightOp,result;	
}