#pragma once
#include "WAHStructure.h"
class TestWAHArray
{
public:
	TestWAHArray(void);
	~TestWAHArray(void);
	void RunTest(dynamic_bitset<> &leftoperand,dynamic_bitset<> & rightOperand,int TestNo,bool andTestType);
	void pureBitAndTest();
	void testLeftCompressedRightLiteral();
	void testDecompress();
	bool testAccuracy(dynamic_bitset<> &leftOp, dynamic_bitset<> &rightOp,bool andTest);
	void pureBiteTestDecompressionTest();
	void printOperands(dynamic_bitset<> & leftOp, dynamic_bitset<> & rightOp);
	void testLeftCompressedRightLiteralTestZero();
	void testLeftCompressedRightLiteralTestOne();
	void EncodeDecodeTest();
	void testRightCompressedLeftLiteraTestOne();
	void andOperatorTest();
	void testRightCompressedLeftLiteralTestZero();
	bool bitCountAccuracy(dynamic_bitset<>& bitSet);
	void testRandomBitStream(int startingLength,int stopLength,int percentage);	
	void testBitCount();	
	
};
