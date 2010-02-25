#include "StdAfx.h"
#include "TestWAHArray.h"
#include "BitsetGenerator.h"
#include <vector>
#include <iostream>

TestWAHArray::TestWAHArray(void)
{
}

TestWAHArray::~TestWAHArray(void)
{
}


void TestWAHArray::testDecompress()
{	
	cout << "Test decompress : " << endl;
	BitsetGenerator * bg = new BitsetGenerator();
	dynamic_bitset<> bitSet  = bg->getPureBitset(62,false);
	WAHStructure * st = new WAHStructure();
	cout << "Original Bitset : " <<bitSet <<endl;
	//st->buildArray(bitSet);
	st->compressWords(bitSet);
	cout << "Compressed Words : " << endl;
	st->printCompressedStream();
	dynamic_bitset<> bitSet2 = st->decompress();
	cout << "decompressed Structure  : " << bitSet2<<endl;
	bitSet2 = bitSet & bitSet2;
	cout << "Count : " << bitSet2.to_ulong() << endl;

	delete bg,st;
}

bool TestWAHArray::testAccuracy(dynamic_bitset<> &leftOp, dynamic_bitset<> &rightOp,bool andTest)
{
	dynamic_bitset<> resultBitMap ; 
if (andTest)
{
	resultBitMap = leftOp & rightOp;
}else{
resultBitMap = leftOp | rightOp;
}

	WAHStructure * left = new WAHStructure();
	WAHStructure * right = new WAHStructure();
	//left->buildArray(leftOp);
	//right->buildArray(rightOp);
	left->compressWords(leftOp);
	right->compressWords(rightOp);
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
	assert(state);
	delete left,right,result;
	return state;
}

void TestWAHArray::EncodeDecodeTest(){
	BitsetGenerator * bg = new BitsetGenerator();
	dynamic_bitset<> temp = bg->getPureBitset(35,true);
	dynamic_bitset<> temp1 = temp; 
	WAHStructure * ws = new WAHStructure();
	cout << "original Bitmap : " << temp << endl;

	//ws->buildArray(temp1);
	ws->compressWords(temp1);
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

void TestWAHArray::testLeftCompressedRightLiteralTestZero(){
	cout << "Test : Left Compressed Right Literal (Zero)" << endl;
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

void TestWAHArray::testRightCompressedLeftLiteralTestZero(){
	cout << "Test : Right Compressed Left Literal (Zero)" << endl;
	BitsetGenerator * bg = new BitsetGenerator();
	dynamic_bitset<> y = bg->getPureBitset(31 * 2,false);
	dynamic_bitset<> x = bg->getAlternatingBitset(31*2);
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

	y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
	x = bg->getAlternatingBitset(5*31);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	y = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
	x = bg->getMiddleCompressedBitset(5*31,31,4*31 -1,false);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	y = bg->getMiddleCompressedBitset(5*31,0,4*31 -1,false);
	x = bg->getAlternatingBitset(5*31);
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	y = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
	x = bg->getMiddleCompressedBitset(6*31,61,4*31 -1,false) & bg->getAlternatingBitset(6*31);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	y = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
	x = bg->getMiddleCompressedBitset(6*31,31,4*31 -1,false) & bg->getAlternatingBitset(6*31);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;

	y = bg->getMiddleCompressedBitset(6*31,31,5*31 -1,false);
	x = bg->getMiddleCompressedBitset(6*31,61,5*31 -1,false) & bg->getAlternatingBitset(6*31);
	//cout <<"Result Size : " <<y.size()<< endl;
	//cout << "Bit stream : " << y << endl;
	printOperands(x,y);
	resul  = testAccuracy(x,y,true);
	cout << "Accuracy : "<< resul<<endl;
	cout << ""<< endl;
	delete bg;
}

void TestWAHArray::pureBiteTestDecompressionTest()
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

void TestWAHArray::testLeftCompressedRightLiteralTestOne(){
	cout << "Test : Left Compressed Right Literal(One)" << endl;
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

void TestWAHArray::testRightCompressedLeftLiteraTestOne(){
	cout << "Test : Right Compressed left Literal (One)" << endl;
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


void TestWAHArray::printOperands(dynamic_bitset<> & leftOp, dynamic_bitset<> & rightOp)
{
	cout << "Left Operand : " << leftOp << endl;
	cout << "Right Operand : " << rightOp << endl;
}

void TestWAHArray::pureBitAndTest()
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

void TestWAHArray::testLeftCompressedRightLiteral(){
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

void TestWAHArray::RunTest(dynamic_bitset<> &leftoperand,dynamic_bitset<> & rightOperand,int TestNo,bool andTestType)
{
	cout << "Test No : " << TestNo << endl;
	WAHStructure * leftOp = new WAHStructure();
	WAHStructure * rightOp = new WAHStructure();
	leftOp->compressWords(leftoperand);
	rightOp->compressWords(rightOperand);
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

void TestWAHArray::andOperatorTest()
{
	//Test contiguous streams entirely consisting of 1's and 0's
	pureBitAndTest();

//LO = 11000000000000...11 RO =001010111000...0 Left operand Compressed(contiguous ones) Right operand not compressed
	testLeftCompressedRightLiteralTestOne();

	//LO = 001010111000...0 RO = 11000000000000...11 Right operand Compressed(contiguous ones) Left operand not compressed
	testRightCompressedLeftLiteraTestOne();

//LO = 10000000000000...11 RO = 001010111000...0 Left operand Compressed(contiguous zeros) Right operand not compressed
	testLeftCompressedRightLiteralTestZero();

//LO = 10000000000000...11 RO = 001010111000...0 Left operand Compressed(contiguous zeros) Right operand not compressed
	testRightCompressedLeftLiteralTestZero();
}

void TestWAHArray::testRandomBitStream(int startingLength,int stopLength,int percentage)
{
BitsetGenerator bg;
dynamic_bitset<> leftOp,rightOp;
dynamic_bitset<> leftOpCopy,rightOpCopy;

for (int length = startingLength; length < stopLength ; length *=10)
{
	cout << "Test Length: "<< length<< endl;
	
	leftOp = bg.getRandomBitStream(length,percentage);	
	//cout << "left operand : "<<leftOp << endl;
	rightOp = bg.getRandomBitStream(length,percentage);
	//cout << "right operand : "<<rightOp << endl;
	testAccuracy(leftOp,rightOp,true);
cout << "Test END: "<< length<< endl;
	cout << ""<< endl;
}

}

void TestWAHArray::testBitCount()
{
	BitsetGenerator bg;
	for (int i = 0; i < 10 ; i++)
	{
		cout <<"Test : "<< i << endl;
		for (int j = 0; j < 110 ; j += 10)
		{
			cout << "Percentage : " << j << endl;
			dynamic_bitset<> bitSet = bg.getRandomBitStream(100*i,j);
			bitCountAccuracy(bitSet);
			cout << "" << endl;
		}
		cout << "" << endl;
	}
	
}
bool TestWAHArray::bitCountAccuracy(dynamic_bitset<>& bitSet)
{
	WAHStructure ws;
	unsigned long long count = (unsigned long long)bitSet.count();
	ws.compressWords(bitSet);
unsigned long long countCompressed = ws.count();
cout << "Bitset count : " << count << endl;
cout << "Compressed word count" << countCompressed<<endl;
assert(countCompressed == count);
return true;
}

