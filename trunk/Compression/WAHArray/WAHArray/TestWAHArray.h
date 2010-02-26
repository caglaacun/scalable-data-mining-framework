#pragma once
#include "WAHStructure.h"
namespace CompressionSchemes{
	namespace TestStructure{
		class TestWAHArray
		{
			
		public:
			enum operation_type{OR,AND};
			TestWAHArray(void);
			~TestWAHArray(void);
			void RunTest(dynamic_bitset<> &leftoperand,dynamic_bitset<> & rightOperand,int TestNo,operation_type type = AND);
			void pureBitTest();
			void pureBitORTest();
			void testLeftCompressedRightLiteral();
			void testDecompress();
			bool testAccuracy(dynamic_bitset<> leftOp, dynamic_bitset<> rightOp,operation_type type = AND);
			void pureBiteTestDecompressionTest();
			void printOperands(dynamic_bitset<> & leftOp, dynamic_bitset<> & rightOp);
			void testLeftCompressedRightLiteralTestZero();
			void testLeftCompressedRightLiteralTestOne();
			void EncodeDecodeTest();
			void testRightCompressedLeftLiteraTestOne();
			void OperatorTest();
			void testRightCompressedLeftLiteralTestZero();
			bool bitCountAccuracy(dynamic_bitset<>& bitSet);
			void testRandomBitStream(int startingLength,int stopLength,int percentage);	
			void testBitCount();
			bool complementAccuracy(dynamic_bitset<> & bitset);
			void testComplement();
			void testComplementStrings(vector<string>  & stringColl);
			void failedCases();
			enum TEST_TYPE{AND_TEST,OR_TEST,NOT_TEST,COUNT_TEST};

		};
	}
}