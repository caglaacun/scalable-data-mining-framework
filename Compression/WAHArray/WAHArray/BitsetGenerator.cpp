#include "StdAfx.h"
#include "BitsetGenerator.h"
#include <iostream>
namespace CompressionSchemes{
	namespace TestStructure{
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

		dynamic_bitset<> BitsetGenerator::getRandomBitStream(int length,int onePercent)
		{
			dynamic_bitset<> result(length);
			vector<int> oneVector = getOneBitPositions(length,onePercent);
			for (int i=0; i < oneVector.size(); i++)
			{
				result[oneVector[i]] = 1;
			}

			return result;
		}

		vector<int> BitsetGenerator::getOneBitPositions(int length, int onePercent)
		{
			vector<int> result;
			int trueBitNumber = (onePercent * length)/100;
			int i = 0;
			while(i < trueBitNumber)
			{		
				int randdomNum = rand();		
				if (!contains(randdomNum,result) && (randdomNum < length) )
				{
					result.push_back(randdomNum);
					i++;
				}

			}
			return result;
		}

		bool BitsetGenerator::contains(const int &val,vector<int> & inputVector)
		{
			bool state = false;
			vector<int>::iterator iteratorType = find(inputVector.begin(),inputVector.end(),val);
			if (iteratorType != inputVector.end())
			{
				state = true;
			}
			return state;
		}

		dynamic_bitset<> BitsetGenerator::buildFromString(string & str)
		{
			dynamic_bitset<> result(str.length());
			int i = str.length()-1;
			string::iterator ite = str.begin();
			while (i >=0)
			{
				if (*(ite) == '1')
				{
					result[i] = 1;
				}
				i--;
				ite++;
			}

			return result;
		}
	}
}