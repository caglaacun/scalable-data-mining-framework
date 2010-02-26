#pragma once
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include <stdlib.h>
#include <string>

using namespace std;
using namespace  boost;
namespace CompressionSchemes{
	namespace TestStructure{
		class BitsetGenerator
		{
		public:
			BitsetGenerator(void);
			~BitsetGenerator(void);
			dynamic_bitset<> getPureBitset(int length,bool bitType);
			dynamic_bitset<> getMiddleCompressedBitset(int length,int compressedStart,int span,bool compressedType);
			dynamic_bitset<> getAlternatingBitset(int length);
			dynamic_bitset<> getMiddleAlternatingBitset(int length,int alternatingStart,int span,bool nonAlternatingPart);
			vector<int> getOneBitPositions(int length, int onePercent);
			dynamic_bitset<> getRandomBitStream(int length,int onePercent);
			dynamic_bitset<> buildFromString(string & str);
			bool contains(const int &val,vector<int> & inputVector);

		};
	}
}