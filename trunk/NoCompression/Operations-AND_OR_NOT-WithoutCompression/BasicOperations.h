#pragma once
#include <iostream>

#include "boost/dynamic_bitset/dynamic_bitset.hpp"

using namespace std;
using namespace boost;
class BasicOperations
{
public:
	BasicOperations(void);
	~BasicOperations(void);
	dynamic_bitset<> doAndOperation(dynamic_bitset<> firstBitString, dynamic_bitset<> secondBitString);
	dynamic_bitset<> doOrOperation(dynamic_bitset<> firstBitString, dynamic_bitset<> secondBitString);
	dynamic_bitset<> doNotOperation(dynamic_bitset<> BitString);
};
