#include "StdAfx.h"
#include "BitsetGenerator.h"

BitsetGenerator::BitsetGenerator(void)
{
}

BitsetGenerator::~BitsetGenerator(void)
{
}
//bitType = true gives a pure bit map of all one bits
dynamic_bitset<> BitsetGenerator::getPureBitset(int length,bool bitType)
{
	dynamic_bitset<> result(length);
	if (bitType)
	{
		result.flip();
	}
	return result;
}

dynamic_bitset<> BitsetGenerator::getAlternatingBitset(int length)
{
dynamic_bitset<> result(length);
for (int i=0 ; i < result.size(); i++)
{
	if (i%2 == 0)
	{
		result[i]=1;
	}
}
return result;
}

dynamic_bitset<> BitsetGenerator::getMiddleAlternatingBitset(int length, int alternatingStart, int span, bool nonAlternatingPart)
{
dynamic_bitset<> result(length);
for (int i = 0; i < result.size() ; i++)
{
	if ( i < alternatingStart)
	{
		if (nonAlternatingPart)
		{
			result[i] = 1;
		}else if (i <= span)
		{
			if (i%2 == 0)
			{
				result[i] =1;
			}else 
			{
				result[i] = 0;
			}
		}
	}
}
return result;
}

dynamic_bitset<> BitsetGenerator::getMiddleCompressedBitset(int length, int compressedStart, int span, bool compressedType)
{
	dynamic_bitset<> result(length);
	for(int i =0; i < result.size(); i++)
	{
		if (i < compressedStart)
		{
			if (!compressedType)
			{
				result[i] = 1;
			}
		}else if (i <= span)
		{
			if (compressedType)
			{
				result[i] = 1;
			}else{
				result[i] = 0;
			}
		}else{
			if (!compressedType)
			{
				result[i] = 1;
			}
		}
	}
	return result;
}

