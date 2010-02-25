#pragma once
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include <stdlib.h>

using namespace std;
using namespace  boost;
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
bool contains(const int &val,vector<int> & inputVector);

};
