#pragma once
#include <stdlib.h>
#include <iostream>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <cstdlib>
#include <ctime>

using namespace boost;
using namespace std;
class GenerateBitSet
{

public:
	GenerateBitSet(void);
	~GenerateBitSet(void);

	dynamic_bitset<> getBitSet(int iSize, int iInterval, bool bInverse);
	dynamic_bitset<> getBitSet(int iSize);//generate dynamic bitset with 1 in random positions
};
