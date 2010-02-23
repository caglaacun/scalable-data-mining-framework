#pragma once
#include "boost/dynamic_bitset/dynamic_bitset.hpp"

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

};
