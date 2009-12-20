#pragma once
#include <iostream>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>


class GenerateBitSet
{

public:
	GenerateBitSet(void);
	~GenerateBitSet(void);

	boost::dynamic_bitset<> getBitSet(int iSize, int iInterval, bool bInverse);
};
