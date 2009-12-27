#include "GenerateBitSet.h"

GenerateBitSet::GenerateBitSet(void)
{
	srand(time(NULL));
}

GenerateBitSet::~GenerateBitSet(void)
{
}

dynamic_bitset<> GenerateBitSet::getBitSet(int iSize, int iInterval, bool bInverse)
{
	dynamic_bitset<> bitSet1(iSize);

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

dynamic_bitset<> GenerateBitSet::getBitSet(int iSize)
{
	dynamic_bitset<> bitSet1(iSize);
	int randPosition = (rand()%35)+1;
	int j=0;

	for (int i=0; i<iSize; i++)
	{
		j++;
		if(j == randPosition)
		{
			bitSet1[i] = 1;
			randPosition = rand()%35+1;
			j=0;
		}
		cout<<bitSet1[i];
	}
	return bitSet1;
}