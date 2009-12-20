#include "GenerateBitSet.h"

GenerateBitSet::GenerateBitSet(void)
{
}

GenerateBitSet::~GenerateBitSet(void)
{
}

boost::dynamic_bitset<> GenerateBitSet::getBitSet(int iSize, int iInterval, bool bInverse)
{
	boost::dynamic_bitset<> bitSet1(iSize);

	for (int i=0; i<iSize; i++)
	{
		if((i % iInterval) == 0)
			bitSet1[i] = 1;
	}

	if(bInverse == true)
	{
		for (int i=0; i<iSize; i++)
		{
			bitSet1[i] = ~bitSet1[i];
		}
	}


	return bitSet1;
}