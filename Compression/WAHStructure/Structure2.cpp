#include "Structure2.h"
#include "Math.h"
#include <iostream>

using namespace std;
Structure2::Structure2(void)
{
}

Structure2::~Structure2(void)
{
	delete mainArray;
}

void Structure2::buildArray(boost::dynamic_bitset<> bitMap)
{
	count = bitMap.count();
	mainArraySize = floor(((float)bitMap.size()/(float)(WORD_SIZE-1)));
	mainArray = new int[mainArraySize];
	int residue = bitMap.size() - mainArraySize * (WORD_SIZE-1);

	if (residue > 0)
	{
		activeWord = boost::dynamic_bitset<> (residue);
		int k=0;
		for (int i = bitMap.size()-residue; i < bitMap.size() ; i++)
		{
			activeWord[k++] = bitMap[i];
		}		
		bitMap.resize(mainArraySize*(WORD_SIZE-1));
	}
	boost::dynamic_bitset<> temp(32);
	int mainArrayIndex= 0;
	int counter = 0;
	for (int i = 0; i < bitMap.size() ;)
	{		
		for (int j=0; j < WORD_SIZE-1; j++)
		{
			temp[j]= bitMap[i];	

			i++;
		}

		mainArray[mainArrayIndex++] = temp.to_ulong();		
	}
}
void Structure2::printArray()
{
	for (int i=0;i < mainArraySize ; i++)
	{
		cout<<"Index  "<<i<<": "<<mainArray[i]<<endl;
	}
	if (activeWord.size() > 0)
	{
		cout<<"Active Word : " << activeWord<< endl;
	}
}

void Structure2::compressWords()
{
	compressedStream = boost::dynamic_bitset<> (mainArraySize*WORD_SIZE);
	int i = 0;
	int tempIndex = 0;
	int oneCount = 0;
	int zeroCount = 0;
	while(i < mainArraySize){
		if (mainArray[i] == 0)
		{
			zeroCount++;
			if (oneCount > 1)
			{
				tempIndex = setOneCount(oneCount,compressedStream,tempIndex);
				oneCount = 0;
			}else if (oneCount == 1)
			{
				tempIndex = setLiteral(MAX_WORD_VALUE,compressedStream,tempIndex);
				oneCount = 0;
			}

		}else if(mainArray[i] == MAX_WORD_VALUE){
			oneCount++;
			if (zeroCount > 1)
			{
				tempIndex = setZeroCount(zeroCount,compressedStream,tempIndex);
				zeroCount = 0;
			}else if (zeroCount == 1)
			{
				tempIndex = setLiteral(0,compressedStream,tempIndex);
				zeroCount = 0;
			}
		}else if (oneCount != 0)
		{
			if (oneCount == 1)
			{
				tempIndex = setLiteral(MAX_WORD_VALUE,compressedStream,tempIndex);
				oneCount =0;
			}else if(oneCount > 1){
				tempIndex = setOneCount(oneCount,compressedStream,tempIndex);
				oneCount =0;
			}

			tempIndex = setLiteral(mainArray[i],compressedStream,tempIndex);
		}else if (zeroCount != 0)
		{
			if (zeroCount == 1)
			{
				tempIndex = setLiteral(0,compressedStream,tempIndex);
				zeroCount =0;	
			}else if (zeroCount > 1)
			{				
				tempIndex = setZeroCount(zeroCount,compressedStream,tempIndex);
				zeroCount =0;
			}

			tempIndex = setLiteral(mainArray[i],compressedStream,tempIndex);

		}else{
			tempIndex = setLiteral(mainArray[i],compressedStream,tempIndex);
		}
		i++;
	}
	if (oneCount == 1)
	{
		tempIndex = setLiteral(MAX_WORD_VALUE,compressedStream,tempIndex);
		oneCount = 0;

	}else if (oneCount > 1)
	{
		tempIndex = setOneCount(oneCount,compressedStream,tempIndex);
		oneCount = 0;
	}else if (zeroCount == 1)
	{
		tempIndex = setLiteral(0,compressedStream,tempIndex);
		zeroCount =0;
	}
	else if (zeroCount > 1)
	{
		tempIndex = setZeroCount(zeroCount,compressedStream,tempIndex);
		zeroCount =0;
	}
	compressedStream.resize(tempIndex);	
	delete mainArray;
}

int Structure2::setOneCount(int count,boost::dynamic_bitset<> & compressed,int startIndex)
{
	unsigned long newCount= (unsigned long)count;
	boost::dynamic_bitset<> temp(WORD_SIZE,newCount);
	temp[(WORD_SIZE-1)] = 1;
	temp[(WORD_SIZE-2)] = 1;
	for (int i = 0; i < (WORD_SIZE); i++)
	{		
		compressed[startIndex] = temp[i];
		startIndex++;
	}
	return startIndex;
} 
int Structure2::setZeroCount(int count,boost::dynamic_bitset<> & compressed,int startIndex)
{
	unsigned long newCount= (unsigned long)count;
	boost::dynamic_bitset<> temp(WORD_SIZE,newCount);
	temp[(WORD_SIZE-1)] = 1;	
	for (int i = 0; i < (WORD_SIZE); i++)
	{	compressed[startIndex] = temp[i];
	startIndex++;
	}
	return startIndex;
} 

int Structure2::setLiteral(int literalVal,boost::dynamic_bitset<> & compressed,int startIndex)
{
	unsigned long newCount= (unsigned long)literalVal;
	boost::dynamic_bitset<> temp(WORD_SIZE,newCount);

	for (int i = 0; i < (WORD_SIZE); i++)
	{	
		compressed[startIndex] = temp[i];
		startIndex++;
	}
	return startIndex;
}

void Structure2::printCompressedStream()
{
	if (compressedStream.size() > 0)
	{
		cout << "Compressed : "<< compressedStream <<endl;
	}
	if (activeWord.size() > 0 )
	{
		cout << "Active word : "<< activeWord <<endl;
	}

}

void Structure2::printCount()
{
	cout << "Count : "<<count <<endl;
}

void Structure2::flip()
{
	if (activeWord.size() > 0)
	{
		activeWord.flip();
	}
	if (compressedStream.size() > 0)
	{
		for (int i = compressedStream.size()-1 ; i >=0 ;)
		{
			if (compressedStream[i] ==1)
			{
				compressedStream[i-1].flip();
				i -= WORD_SIZE;
			}else{
				i --;
				for (int j = 0; j < (WORD_SIZE -1); j++ )
				{
					compressedStream[i].flip();
					i--;
				}
			}
		}
	}
	count = mainArraySize * (WORD_SIZE -1) + activeWord.size() - count;
}

Structure2 * Structure2::operator &(Structure2 & structure)
{
	boost::dynamic_bitset<> result(WORD_SIZE*mainArraySize);
	// 	boost::dynamic_bitset<> candidateBitMap;
	// 	if (compressedStream.size() >= structure.getCompressedWord().size())
	// 	{
	// 		candidateBitMap= compressedStream;
	// 	}else
	// 	{
	// 		candidateBitMap = structure.getCompressedWord();
	// 	}

	bool resize = false;
	bool oneBlock = false;
	int leftOperandLastBit = 0;
	int rightOperandLastBit = 0;
	int zeroStartBlock = 0;
	int zeroEndBlock = 0;
	int oneStartBlock = 0;
	int oneEndBlock = 0;
	boost::dynamic_bitset<> rightOperand = structure.getCompressedWord();
	boost::dynamic_bitset<> leftOperand = compressedStream;
	int rightOperandIndex = 0;
	int leftOperandIndex = 0;
	int resultBitMapIndex = 0;
	bool leftOperandZero = false;
	bool rightOperandZero = false;
	if (rightOperand == leftOperand)
	{
		result = leftOperand;
		result.resize(leftOperand.size());
		resize = true;
	}else{
		while(leftOperandIndex <= compressedStream.size())
		{
			if ((leftOperandLastBit == rightOperandLastBit))
			{
				if ((leftOperand.size() >leftOperandIndex &&leftOperand[leftOperandIndex+WORD_SIZE-1] == 0) && (rightOperand.size()> rightOperandIndex && rightOperand[rightOperandIndex+WORD_SIZE-1]==0) )
				{
					for (int i = 0; i < WORD_SIZE; i++)
					{
						result[resultBitMapIndex++] = leftOperand[leftOperandIndex++] & rightOperand[rightOperandIndex++];
						leftOperandLastBit ++;
						rightOperandLastBit++;
					}
				}else if (leftOperand[leftOperandIndex+WORD_SIZE-1]== 0 && rightOperand[rightOperandIndex+WORD_SIZE-1]==1)
				{
					if (rightOperand[rightOperandIndex + WORD_SIZE -2] ==0)
					{
						int span = getValue(rightOperand,rightOperandIndex,WORD_SIZE-2);
						rightOperandLastBit += span * WORD_SIZE;					
						for (int i = 0 ; i < WORD_SIZE; i++)
						{
							result[resultBitMapIndex++] = rightOperand[rightOperandIndex++];
						}
					}else if (rightOperand[rightOperandIndex + WORD_SIZE -2] ==1)
					{
						int span = getValue(rightOperand,rightOperandIndex,WORD_SIZE-2);
						rightOperandLastBit += span * WORD_SIZE;					
						rightOperandIndex += WORD_SIZE;
						oneBlock = true;
					}

				}else if (leftOperand.size() > leftOperandIndex &&rightOperand.size() > rightOperandIndex && leftOperand[leftOperandIndex+WORD_SIZE-1]== 1 && rightOperand[rightOperandIndex+WORD_SIZE-1]==0)
				{
					if (leftOperand[leftOperandIndex + WORD_SIZE -2] ==0)
					{
						int span = getValue(leftOperand,leftOperandIndex,WORD_SIZE-2);
						leftOperandLastBit += span * WORD_SIZE;					
						for (int i = 0 ; i < WORD_SIZE; i++)
						{
							result[resultBitMapIndex++] = leftOperand[leftOperandIndex++];
						}
					}else	if (leftOperand[leftOperandIndex + WORD_SIZE -2] ==1)
					{
						int span = getValue(leftOperand,leftOperandIndex,WORD_SIZE-2);
						leftOperandLastBit += span * WORD_SIZE;					
						leftOperandIndex += WORD_SIZE;
						oneBlock = true;
					}

				}else if (leftOperand.size() > leftOperandIndex &&rightOperand.size() > rightOperandIndex && leftOperand[leftOperandIndex+WORD_SIZE-1]== 1 && rightOperand[rightOperandIndex+WORD_SIZE-1]==1)
				{
					if (leftOperand[leftOperandIndex+WORD_SIZE-2]== 0 &&  rightOperand[rightOperandIndex+WORD_SIZE-2]==0)
					{
						int leftSpan = getValue(leftOperand,leftOperandIndex,WORD_SIZE-2);
						int rightSpan = getValue(rightOperand,rightOperandIndex,WORD_SIZE-2);
						if (leftSpan > rightSpan)
						{
							leftOperandLastBit += leftSpan*WORD_SIZE;
							for (int i =0; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = leftOperand[leftOperandIndex++];
							}
						}else if (leftSpan == rightSpan)
						{
							leftOperandLastBit += leftSpan*WORD_SIZE ;
							rightOperandLastBit += leftSpan*WORD_SIZE;
							for (int i =0; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = leftOperand[leftOperandIndex++];
								rightOperandIndex++;
							}
						}else if (leftSpan < rightSpan)
						{

							rightOperandLastBit += rightSpan*WORD_SIZE;
							for (int i =0; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = leftOperand[rightOperandIndex++];							
							}
						}

					}else if (leftOperand[leftOperandIndex+WORD_SIZE-2]== 1 &&  rightOperand[rightOperandIndex+WORD_SIZE-2]==1)
					{
						int leftSpan = getValue(leftOperand,leftOperandIndex,WORD_SIZE-2);
						int rightSpan = getValue(rightOperand,rightOperandIndex,WORD_SIZE-2);
						if (leftSpan > rightSpan)
						{
							leftOperandLastBit += leftSpan*WORD_SIZE;
							leftOperandIndex += WORD_SIZE;
							oneBlock = true;

						}else if (leftSpan == rightSpan)
						{
							leftOperandLastBit += leftSpan*WORD_SIZE ;
							rightOperandLastBit += leftSpan*WORD_SIZE;
							for (int i = 0 ; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = leftOperand[leftOperandIndex++];
								rightOperandIndex++;
							}
						}else if (leftSpan < rightSpan)
						{
							rightOperandLastBit += rightSpan*WORD_SIZE;
							rightOperandIndex += WORD_SIZE;
							oneBlock = true;
						}

					}

				}
				if(leftOperandIndex >= leftOperand.size() && rightOperandIndex >= rightOperand.size()){
					break;
				}

			} 
			else
			{

				if(rightOperandLastBit < leftOperandLastBit)
				{
					while(rightOperandLastBit < leftOperandLastBit)
					{
						if (oneBlock && rightOperand[rightOperandIndex + WORD_SIZE -1] == 0)
					{
						for (int i = 0 ; i < WORD_SIZE ; i++)
						{
							result[resultBitMapIndex++] = rightOperand[rightOperandIndex++];
							rightOperandLastBit ++;
						}						
					}else if (oneBlock && rightOperand[rightOperandIndex + WORD_SIZE -1] == 1)
					{
						int span = getValue(rightOperand,rightOperandIndex,WORD_SIZE -1);
						for (int i = 0 ; i < WORD_SIZE ; i++)
						{
							result[resultBitMapIndex++] = rightOperand[rightOperandIndex++];
							
						}
						rightOperandLastBit += span*WORD_SIZE;
					}else if (rightOperand[rightOperandIndex + WORD_SIZE -1] == 0)
						{
							rightOperandIndex += WORD_SIZE;
							rightOperandLastBit += WORD_SIZE;
						}else if (rightOperand[rightOperandIndex + WORD_SIZE -1] == 1)
						{
							int span = getValue(rightOperand,rightOperandIndex,WORD_SIZE -1);
							rightOperandIndex += WORD_SIZE;
							rightOperandLastBit += span*WORD_SIZE;
						}

					}

				}
				else if (rightOperandLastBit > leftOperandLastBit)
				{
					while(rightOperandLastBit > leftOperandLastBit)
					{
						if (oneBlock && leftOperand[leftOperandIndex + WORD_SIZE -1] == 0)
						{
							for (int i = 0 ; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = rightOperand[leftOperandIndex++];
								leftOperandLastBit ++;
							}
						}else if (oneBlock && leftOperand[leftOperandIndex + WORD_SIZE -1] == 1)
						{
							int span = getValue(leftOperand,leftOperandIndex,WORD_SIZE -1);
							for (int i = 0 ; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = rightOperand[leftOperandIndex++];
								
							}
							leftOperandLastBit += span*WORD_SIZE;
						}else if (leftOperand[leftOperandIndex + WORD_SIZE -1] == 0)
						{
							leftOperandIndex += WORD_SIZE;
							leftOperandLastBit += WORD_SIZE;
						}else if (leftOperand[leftOperandIndex + WORD_SIZE -1] == 1)
						{
							int span = getValue(leftOperand,leftOperandIndex,WORD_SIZE -1);
							leftOperandIndex += span*WORD_SIZE;
							leftOperandLastBit += span*WORD_SIZE;
						}

					}
				}
				if(leftOperandIndex >= leftOperand.size() && rightOperandIndex >= rightOperand.size()){
					break;
				}
			}
		}
	}
	if (!resize)
	{
		result.resize(resultBitMapIndex);
	}
	boost::dynamic_bitset<> operandActive = structure.getActiveWord();	
	boost::dynamic_bitset<> resultActive = operandActive & activeWord;

	Structure2 * st = new Structure2();

	st->setActiveWord(resultActive);
	st->setCompressedStream(result);


	return st;
}

Structure2 * Structure2::operator |(Structure2 & structure)
{
	boost::dynamic_bitset<> result(WORD_SIZE*mainArraySize);
	// 	boost::dynamic_bitset<> candidateBitMap;
	// 	if (compressedStream.size() >= structure.getCompressedWord().size())
	// 	{
	// 		candidateBitMap= compressedStream;
	// 	}else
	// 	{
	// 		candidateBitMap = structure.getCompressedWord();
	// 	}

	bool resize = false;
	bool zeroBlock = false;
	int leftOperandLastBit = 0;
	int rightOperandLastBit = 0;
	int zeroStartBlock = 0;
	int zeroEndBlock = 0;
	int oneStartBlock = 0;
	int oneEndBlock = 0;
	boost::dynamic_bitset<> rightOperand = structure.getCompressedWord();
	boost::dynamic_bitset<> leftOperand = compressedStream;
	int rightOperandIndex = 0;
	int leftOperandIndex = 0;
	int resultBitMapIndex = 0;
	bool leftOperandZero = false;
	bool rightOperandZero = false;
	if (rightOperand == leftOperand)
	{
		result = leftOperand;
		result.resize(leftOperand.size());
		resize = true;
	}else{
		while(leftOperandIndex <= compressedStream.size())
		{
			if ((leftOperandLastBit == rightOperandLastBit))
			{
				if ((leftOperand.size() >leftOperandIndex &&leftOperand[leftOperandIndex+WORD_SIZE-1] == 0) && (rightOperand.size()> rightOperandIndex && rightOperand[rightOperandIndex+WORD_SIZE-1]==0) )
				{
					for (int i = 0; i < WORD_SIZE; i++)
					{
						result[resultBitMapIndex++] = leftOperand[leftOperandIndex++] & rightOperand[rightOperandIndex++];
						leftOperandLastBit ++;
						rightOperandLastBit++;
					}
				}else if (leftOperand[leftOperandIndex+WORD_SIZE-1]== 0 && rightOperand[rightOperandIndex+WORD_SIZE-1]==1)
				{
					if (rightOperand[rightOperandIndex + WORD_SIZE -2] ==1)
					{
						int span = getValue(rightOperand,rightOperandIndex,WORD_SIZE-2);
						rightOperandLastBit += span * WORD_SIZE;					
						for (int i = 0 ; i < WORD_SIZE; i++)
						{
							result[resultBitMapIndex++] = rightOperand[rightOperandIndex++];
						}
					}else if (rightOperand[rightOperandIndex + WORD_SIZE -2] ==0)
					{
						int span = getValue(rightOperand,rightOperandIndex,WORD_SIZE-2);
						rightOperandLastBit += span * WORD_SIZE;					
						rightOperandIndex += WORD_SIZE;
						zeroBlock = true;
					}

				}else if (leftOperand.size() > leftOperandIndex &&rightOperand.size() > rightOperandIndex && leftOperand[leftOperandIndex+WORD_SIZE-1]== 1 && rightOperand[rightOperandIndex+WORD_SIZE-1]==0)
				{
					if (leftOperand[leftOperandIndex + WORD_SIZE -2] ==1)
					{
						int span = getValue(leftOperand,leftOperandIndex,WORD_SIZE-2);
						leftOperandLastBit += span * WORD_SIZE;					
						for (int i = 0 ; i < WORD_SIZE; i++)
						{
							result[resultBitMapIndex++] = leftOperand[leftOperandIndex++];
						}
					}else	if (leftOperand[leftOperandIndex + WORD_SIZE -2] ==0)
					{
						int span = getValue(leftOperand,leftOperandIndex,WORD_SIZE-2);
						leftOperandLastBit += span * WORD_SIZE;					
						leftOperandIndex += WORD_SIZE;
						zeroBlock = true;
					}

				}else if (leftOperand.size() > leftOperandIndex &&rightOperand.size() > rightOperandIndex && leftOperand[leftOperandIndex+WORD_SIZE-1]== 1 && rightOperand[rightOperandIndex+WORD_SIZE-1]==1)
				{
					if (leftOperand[leftOperandIndex+WORD_SIZE-2]== 1 &&  rightOperand[rightOperandIndex+WORD_SIZE-2]==1)
					{
						int leftSpan = getValue(leftOperand,leftOperandIndex,WORD_SIZE-2);
						int rightSpan = getValue(rightOperand,rightOperandIndex,WORD_SIZE-2);
						if (leftSpan > rightSpan)
						{
							leftOperandLastBit += leftSpan*WORD_SIZE;
							for (int i =0; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = leftOperand[leftOperandIndex++];
							}
						}else if (leftSpan == rightSpan)
						{
							leftOperandLastBit += leftSpan*WORD_SIZE ;
							rightOperandLastBit += leftSpan*WORD_SIZE;
							for (int i =0; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = leftOperand[leftOperandIndex++];
								rightOperandIndex++;
							}
						}else if (leftSpan < rightSpan)
						{

							rightOperandLastBit += rightSpan*WORD_SIZE;
							for (int i =0; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = leftOperand[rightOperandIndex++];							
							}
						}

					}else if (leftOperand[leftOperandIndex+WORD_SIZE-2]== 0 &&  rightOperand[rightOperandIndex+WORD_SIZE-2]==0)
					{
						int leftSpan = getValue(leftOperand,leftOperandIndex,WORD_SIZE-2);
						int rightSpan = getValue(rightOperand,rightOperandIndex,WORD_SIZE-2);
						if (leftSpan > rightSpan)
						{
							leftOperandLastBit += leftSpan*WORD_SIZE;
							leftOperandIndex += WORD_SIZE;
							zeroBlock = true;

						}else if (leftSpan == rightSpan)
						{
							leftOperandLastBit += leftSpan*WORD_SIZE ;
							rightOperandLastBit += leftSpan*WORD_SIZE;
							for (int i = 0 ; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = leftOperand[leftOperandIndex++];
								rightOperandIndex++;
							}
						}else if (leftSpan < rightSpan)
						{
							rightOperandLastBit += rightSpan*WORD_SIZE;
							rightOperandIndex += WORD_SIZE;
							zeroBlock = true;
						}

					}

				}
				if(leftOperandIndex >= leftOperand.size() && rightOperandIndex >= rightOperand.size()){
					break;
				}

			} 
			else
			{

				if(rightOperandLastBit < leftOperandLastBit)
				{
					while(rightOperandLastBit < leftOperandLastBit)
					{
						if (zeroBlock && rightOperand[rightOperandIndex + WORD_SIZE -1] == 0)
						{
							for (int i = 0 ; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = rightOperand[rightOperandIndex++];
								rightOperandLastBit ++;
							}						
						}else if (zeroBlock && rightOperand[rightOperandIndex + WORD_SIZE -1] == 1)
						{
							int span = getValue(rightOperand,rightOperandIndex,WORD_SIZE -1);
							for (int i = 0 ; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = rightOperand[rightOperandIndex++];

							}
							rightOperandLastBit += span*WORD_SIZE;
						}else if (rightOperand[rightOperandIndex + WORD_SIZE -1] == 0)
						{
							rightOperandIndex += WORD_SIZE;
							rightOperandLastBit += WORD_SIZE;
						}else if (rightOperand[rightOperandIndex + WORD_SIZE -1] == 1)
						{
							int span = getValue(rightOperand,rightOperandIndex,WORD_SIZE -1);
							rightOperandIndex += WORD_SIZE;
							rightOperandLastBit += span*WORD_SIZE;
						}

					}

				}
				else if (rightOperandLastBit > leftOperandLastBit)
				{
					while(rightOperandLastBit > leftOperandLastBit)
					{
						if (zeroBlock && leftOperand[leftOperandIndex + WORD_SIZE -1] == 0)
						{
							for (int i = 0 ; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = rightOperand[leftOperandIndex++];
								leftOperandLastBit ++;
							}
						}else if (zeroBlock && leftOperand[leftOperandIndex + WORD_SIZE -1] == 1)
						{
							int span = getValue(leftOperand,leftOperandIndex,WORD_SIZE -1);
							for (int i = 0 ; i < WORD_SIZE ; i++)
							{
								result[resultBitMapIndex++] = rightOperand[leftOperandIndex++];

							}
							leftOperandLastBit += span*WORD_SIZE;
						}else if (leftOperand[leftOperandIndex + WORD_SIZE -1] == 0)
						{
							leftOperandIndex += WORD_SIZE;
							leftOperandLastBit += WORD_SIZE;
						}else if (leftOperand[leftOperandIndex + WORD_SIZE -1] == 1)
						{
							int span = getValue(leftOperand,leftOperandIndex,WORD_SIZE -1);
							leftOperandIndex += span*WORD_SIZE;
							leftOperandLastBit += span*WORD_SIZE;
						}

					}
				}
				if(leftOperandIndex >= leftOperand.size() && rightOperandIndex >= rightOperand.size()){
					break;
				}
			}
		}
	}
	if (!resize)
	{
		result.resize(resultBitMapIndex);
	}
	boost::dynamic_bitset<> operandActive = structure.getActiveWord();	
	boost::dynamic_bitset<> resultActive = operandActive & activeWord;

	Structure2 * st = new Structure2();

	st->setActiveWord(resultActive);
	st->setCompressedStream(result);


	return st;
}

void Structure2::setCompressedStream(boost::dynamic_bitset<> & x)
{
	compressedStream = x;
}

void Structure2::setActiveWord(boost::dynamic_bitset<> & x)
{
	activeWord = x;
}

boost::dynamic_bitset<> Structure2::getActiveWord()
{
	return activeWord;
}

int Structure2::getValue(boost::dynamic_bitset<> & bitMap,int startIndex,int offset)
{

	boost::dynamic_bitset<> temp(offset);
	for (int i =0; i < offset; i++)
	{
		temp[i] = bitMap[startIndex + i];
	}

	return (int)temp.to_ulong();
}

boost::dynamic_bitset<> Structure2::getCompressedWord()
{
	return compressedStream;
}

int Structure2::getSpaceUtilisation()
{
	int compressedSize  = 0;
	int active = 0;

	compressedSize = 24 + (int)(compressedStream.size()/8);	
	active= (int)(activeWord.size()/8)+24;

	return (active + compressedSize);
}
