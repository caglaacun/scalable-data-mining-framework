#include "StdAfx.h"
#include "WAHStructure.h"
#include "Math.h"
#include <iostream>
using namespace std;
namespace CompressionSchemes{
	WAHStructure::WAHStructure(void)
	{
	}
	WAHStructure::~WAHStructure(void)
	{
	}
	void WAHStructure::buildArray(boost::dynamic_bitset<>& bitMap)
	{
		//count = bitMap.count();
		mainArraySize = floor(((float)bitMap.size()/(float)(WORD_SIZE-1)));
		mainArray = new int[mainArraySize];
		activeWordSize = bitMap.size() - mainArraySize * (WORD_SIZE-1);

		if (activeWordSize > 0)
		{
			boost::dynamic_bitset<> activeWordBitSet (activeWordSize);		
			int k=0;
			for (int i = bitMap.size()-activeWordSize; i < bitMap.size() ; i++)
			{
				activeWordBitSet[k++] = bitMap[i];
			}		
			bitMap.resize(mainArraySize*(WORD_SIZE-1));
			activeWord = activeWordBitSet.to_ulong();
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
	unsigned long long WAHStructure::count()
	{
		vector<unsigned long int>::iterator value_iterator = compressedStreamVector.begin();
		unsigned long long countVal = 0;
		while(value_iterator != compressedStreamVector.end())
		{
			unsigned long int temp_value = *(value_iterator);
			int startBit = getStartBitValue(temp_value);
			if (LITERAL_WORD == startBit)
			{
				countVal += getLiteralCount(temp_value);
			}else if (ONE_GAP_WORD == startBit)
			{
				countVal += ((WORD_SIZE -1)*(temp_value - ONE_GAP_START_FLAG));
			}
			value_iterator++;
		}

		if (ActiveWordSize() > 0)
		{
			unsigned long active  = ActiveWord();
			countVal += getLiteralCount(active);
		}
		return countVal;
	}


	unsigned long int WAHStructure::getLiteralCount(unsigned long int &bitLiteral)
	{
		unsigned long int result = 0;
		typedef const unsigned char type_byte;
		type_byte * bytePtr = static_cast<type_byte *>(static_cast<const void *>(&bitLiteral));
		int length = sizeof(bitLiteral);
		while(length)
		{
			result += boost::detail::dynamic_bitset_count_impl::count_table<>::table[*(bytePtr)];
			bytePtr++;
			length--;
		}

		return result;
	}

	void WAHStructure::printArray()
	{
		for (int i=0;i < mainArraySize ; i++)
		{
			cout<<"Index  "<<i<<": "<<mainArray[i]<<endl;
		}
		if (activeWord > 0)
		{
			cout<<"Active Word : " << activeWord<< endl;
		}
	}

	void WAHStructure::compressWords( boost::dynamic_bitset<>& bitMap )
	{	
		int i = 0;
		int tempIndex = 0;
		int oneCount = 0;
		int zeroCount = 0;

		buildArray(bitMap);
		while(i < mainArraySize){
			if (mainArray[i] == 0)
			{
				zeroCount++;
				if (oneCount > 1)
				{
					setOneCount(oneCount,compressedStreamVector);
					oneCount = 0;
				}else if (oneCount == 1)
				{
					setLiteral(ONE_LITERAL,compressedStreamVector);
					oneCount = 0;
				}

			}else if(mainArray[i] == ONE_LITERAL){
				oneCount++;
				if (zeroCount > 1)
				{
					setZeroCount(zeroCount,compressedStreamVector);
					zeroCount = 0;
				}else if (zeroCount == 1)
				{
					setLiteral(0,compressedStreamVector);
					zeroCount = 0;
				}
			}else if (oneCount != 0)
			{
				if (oneCount == 1)
				{
					setLiteral(ONE_LITERAL,compressedStreamVector);
					oneCount =0;
				}else if(oneCount > 1){
					setOneCount(oneCount,compressedStreamVector);
					oneCount =0;
				}

				setLiteral(mainArray[i],compressedStreamVector);
			}else if (zeroCount != 0)
			{
				if (zeroCount == 1)
				{
					setLiteral(0,compressedStreamVector);
					zeroCount =0;	
				}else if (zeroCount > 1)
				{				
					setZeroCount(zeroCount,compressedStreamVector);
					zeroCount =0;
				}

				setLiteral(mainArray[i],compressedStreamVector);

			}else{
				setLiteral(mainArray[i],compressedStreamVector);
			}
			i++;
		}
		if (oneCount == 1)
		{
			setLiteral(ONE_LITERAL,compressedStreamVector);
			oneCount = 0;

		}else if (oneCount > 1)
		{
			setOneCount(oneCount,compressedStreamVector);
			oneCount = 0;
		}else if (zeroCount == 1)
		{
			setLiteral(0,compressedStreamVector);
			zeroCount =0;
		}
		else if (zeroCount > 1)
		{
			setZeroCount(zeroCount,compressedStreamVector);
			zeroCount =0;
		}		
		delete mainArray;
	}

	void WAHStructure::setOneCount(unsigned long int count,vector<unsigned long int> & compressed)
	{
		unsigned long int comressedValue = count;
		comressedValue = comressedValue | ONE_GAP_START_FLAG;
		compressed.push_back(comressedValue);	

	} 

	void WAHStructure::setZeroCount(unsigned long int count,vector<unsigned long int> & compressed)
	{
		unsigned long int compressedValue = count;
		compressedValue = compressedValue| ZERO_GAP_START_FLAG;
		compressed.push_back(compressedValue);
	} 

	void WAHStructure::setLiteral(unsigned long int literalVal,vector<unsigned long int> & compressed)
	{	
		compressed.push_back(literalVal);
	}

	void WAHStructure::printVector()
	{	
		for (int i = 0; i < compressedStreamVector.size(); i++)
		{
			cout << "Index  "<<i<<":"<<compressedStreamVector[i] << endl;
		}	
	}

	void WAHStructure::printCompressedStream()
	{
		if (&compressedStreamVector != NULL && compressedStreamVector.size() > 0)
		{
			dynamic_bitset<> temp(compressedStreamVector.size() * WORD_SIZE);
			for (int i  = 0 ; i < compressedStreamVector.size(); i++)
			{
				copyIntegerToBitMap(temp,i);
			}

			cout << temp << endl;
		}


		if (ActiveWordSize() > 0 )
		{
			unsigned long int act = (unsigned long int) activeWord;
			dynamic_bitset<> activeTemp(ActiveWordSize(),act);
			cout << "Active word : "<< activeTemp <<endl;
		}

	}

	void WAHStructure::copyIntegerToBitMap(dynamic_bitset<> &bitmap,int index)
	{
		int bitmapStartIndex = index*WORD_SIZE;
		unsigned long int val = (unsigned long int) compressedStreamVector[index];
		dynamic_bitset<> temp(32,val);
		for (int i = 0;  i < temp.size(); i++)
		{
			bitmap[bitmapStartIndex++] = temp[i];		
		}
	}

	void WAHStructure::flip()
	{
		if (activeWord > 0)
		{
			activeWord  = !activeWord;
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
	}



	WAHStructure * WAHStructure::operator &(WAHStructure & structure)
	{
		int compressedStreamLength = mainArraySize;
		vector<unsigned long int> rightOperand = structure.getCompressedVector();
		vector<unsigned long int> leftOperand = compressedStreamVector;
		bool rightOperandZeroRun = true;
		bool leftOperandZeroRun = true;	
		int leftOperandStartPosition = 0;
		int leftOperandEndPosition = 0;
		int rightOperandStartPosition = 0;
		int rightOperandEndPosition = 0;
		int compressedWordValue = 0;
		int leftOperandIndex = 0;
		int rightOperandIndex = 0;
		int compressedWordIndex = 0;
		bool gapExtension  = false;

		WAHStructure * st = new WAHStructure();
		vector<unsigned long int> compressedResult;

		while(leftOperandEndPosition < compressedStreamLength || rightOperandEndPosition < compressedStreamLength)
		{
			if (leftOperandEndPosition == rightOperandEndPosition)
			{
				int leftOperandStartBitValue = getStartBitValue(leftOperand[leftOperandIndex]);
				if (leftOperandStartBitValue == LITERAL_WORD)
				{
					leftOperandEndPosition++;
					leftOperandStartPosition = leftOperandEndPosition;
					int rightOperandStartBitValue = getStartBitValue(rightOperand[rightOperandIndex]);

					if (rightOperandStartBitValue == LITERAL_WORD)
					{
						rightOperandEndPosition++;
						rightOperandStartPosition = rightOperandEndPosition;
						setValueOnCompressedWord(leftOperand[leftOperandIndex++] & rightOperand[rightOperandIndex++],compressedResult);
					}else if (rightOperandStartBitValue == ONE_GAP_WORD)
					{
						rightOperandEndPosition += (rightOperand[rightOperandIndex] - ONE_GAP_START_FLAG);	
						rightOperandZeroRun = false;
					}
					else if (rightOperandStartBitValue == ZERO_GAP_WORD)
					{
						rightOperandEndPosition += (rightOperand[rightOperandIndex] - ZERO_GAP_START_FLAG);	
						rightOperandZeroRun = true;
					}

				}else if (leftOperandStartBitValue == ONE_GAP_WORD)
				{
					leftOperandEndPosition += (leftOperand[leftOperandIndex] - ONE_GAP_START_FLAG);
					leftOperandZeroRun = false;
					int rightOperandStartBitValue =  getStartBitValue(rightOperand[rightOperandIndex]);
					if (rightOperandStartBitValue == LITERAL_WORD)
					{					
						rightOperandEndPosition++;										
					}else if (rightOperandStartBitValue == ONE_GAP_WORD)
					{
						rightOperandEndPosition += (rightOperand[rightOperandIndex] - ONE_GAP_START_FLAG);	
						if (rightOperandEndPosition == leftOperandEndPosition)
						{
							setValueOnCompressedWord(leftOperand[leftOperandIndex],compressedResult);
							leftOperandIndex++;
							rightOperandIndex++;
						}
						rightOperandZeroRun = false;
					}
					else if (rightOperandStartBitValue == ZERO_GAP_WORD)
					{
						rightOperandEndPosition += (rightOperand[rightOperandIndex] - ZERO_GAP_START_FLAG);	
						if (rightOperandEndPosition == leftOperandEndPosition)
						{
							setValueOnCompressedWord(rightOperand[rightOperandIndex],compressedResult);
							leftOperandIndex++;
							rightOperandIndex++;
						}
						rightOperandZeroRun = true;
					}

				}else if (leftOperandStartBitValue == ZERO_GAP_WORD)
				{
					leftOperandEndPosition += (leftOperand[leftOperandIndex] - ZERO_GAP_START_FLAG);
					leftOperandZeroRun = true;
					int rightOperandStartBitValue = getStartBitValue(rightOperand[rightOperandIndex]);
					if (rightOperandStartBitValue == LITERAL_WORD)
					{					
						rightOperandEndPosition++;										
					}else if (rightOperandStartBitValue == ONE_GAP_WORD)
					{
						rightOperandEndPosition += (rightOperand[rightOperandIndex] - ONE_GAP_START_FLAG);	
						if (rightOperandEndPosition == leftOperandEndPosition)
						{
							setValueOnCompressedWord(leftOperand[leftOperandIndex],compressedResult);
							leftOperandIndex++;
							rightOperandIndex++;
						}
						rightOperandZeroRun = false;
					}
					else if (rightOperandStartBitValue == ZERO_GAP_WORD)
					{
						rightOperandEndPosition += (rightOperand[rightOperandIndex] - ZERO_GAP_START_FLAG);	
						if (rightOperandEndPosition == leftOperandEndPosition)
						{
							setValueOnCompressedWord(leftOperand[leftOperandIndex],compressedResult);
							leftOperandIndex++;
							rightOperandIndex++;
						}
						rightOperandZeroRun = true;
					}

				}

			}
			else if (leftOperandEndPosition > rightOperandEndPosition)
			{

				if (gapExtension)
				{
					int leftStartBit = getStartBitValue(leftOperand[leftOperandIndex]);
					if (leftStartBit == ZERO_GAP_WORD)
					{
						unsigned long int blockRun = leftOperandEndPosition - rightOperandEndPosition;
						unsigned long int gapRunBlocks = leftOperand[leftOperandIndex] - ZERO_GAP_START_FLAG;
						unsigned long int resultGap = leftOperand[leftOperandIndex] - gapRunBlocks + blockRun;
						setValueOnCompressedWord(resultGap,compressedResult);
						vector<int> result = alignRightWithLeft(leftOperandEndPosition,rightOperandIndex,rightOperandEndPosition,rightOperand,compressedResult,ZERO_GAP,AND,true);
						rightOperandIndex = result[0];
						rightOperandEndPosition = result[1];
						if (leftOperandEndPosition == rightOperandEndPosition)
						{
							leftOperandIndex++;
							rightOperandIndex++;
							gapExtension = false;
						}else if(leftOperandEndPosition < rightOperandEndPosition)
						{
							gapExtension = true;
						}
					}else if (leftStartBit == ONE_GAP_WORD)
					{
						vector<int> result = alignRightWithLeft(leftOperandEndPosition,rightOperandIndex,rightOperandEndPosition,rightOperand,compressedResult,ONE_GAP,AND,true);
						rightOperandIndex = result[0];
						rightOperandEndPosition = result[1];
						if (leftOperandEndPosition == rightOperandEndPosition)
						{
							leftOperandIndex++;
							rightOperandIndex++;
							gapExtension = false;
						}else if(leftOperandEndPosition < rightOperandEndPosition)
						{
							gapExtension = true;
						}
					}


				}else
				{if (leftOperandZeroRun)
				{
					setValueOnCompressedWord(leftOperand[leftOperandIndex],compressedResult);
					vector<int> result = alignRightWithLeft(leftOperandEndPosition,rightOperandIndex,rightOperandEndPosition,rightOperand,compressedResult,ZERO_GAP,AND,false);
					rightOperandIndex = result[0];
					rightOperandEndPosition = result[1];
					if (leftOperandEndPosition == rightOperandEndPosition)
					{
						leftOperandIndex++;
						rightOperandIndex++;
						gapExtension = false;
					}else if(leftOperandEndPosition < rightOperandEndPosition)
					{
						gapExtension = true;
					}
				} 
				else
				{
					vector<int> result = alignRightWithLeft(leftOperandEndPosition,rightOperandIndex,rightOperandEndPosition,rightOperand,compressedResult,ONE_GAP,AND,false);
					rightOperandIndex = result[0];
					rightOperandEndPosition = result[1];
					if (leftOperandEndPosition == rightOperandEndPosition)
					{
						leftOperandIndex++;
						rightOperandIndex++;
						gapExtension = false;
					}else if(leftOperandEndPosition < rightOperandEndPosition)
					{
						gapExtension = true;
					}

				}
				}
			}else if (leftOperandEndPosition < rightOperandEndPosition)
			{
				if (gapExtension)
				{
					int rightStartBit = getStartBitValue(rightOperand[rightOperandIndex]);
					if (rightStartBit == ZERO_GAP_WORD)
					{
						unsigned long int blockRun = rightOperandEndPosition - leftOperandEndPosition;
						unsigned long int gapRunBlocks = rightOperand[rightOperandIndex] - ZERO_GAP_START_FLAG;
						unsigned long int resultGap = ZERO_GAP_START_FLAG + blockRun;
						setValueOnCompressedWord(resultGap,compressedResult);
						vector<int> result = alignLeftWithRight(rightOperandEndPosition,leftOperandIndex,leftOperandEndPosition,leftOperand,compressedResult,ZERO_GAP,AND,gapExtension);
						leftOperandIndex = result[0];
						leftOperandEndPosition = result[1];
						if (leftOperandEndPosition == rightOperandEndPosition)
						{
							leftOperandIndex++;
							rightOperandIndex++;
							gapExtension = false;
						}else if(rightOperandEndPosition < leftOperandEndPosition)
						{
							gapExtension = true;
						}
					}else if (rightStartBit == ONE_GAP_WORD)
					{
						vector<int> result = alignLeftWithRight(rightOperandEndPosition,leftOperandIndex,leftOperandEndPosition,leftOperand,compressedResult,ONE_GAP,AND,gapExtension);
						leftOperandIndex = result[0];
						leftOperandEndPosition = result[1];
						if (leftOperandEndPosition == rightOperandEndPosition)
						{
							leftOperandIndex++;
							rightOperandIndex++;
							gapExtension = false;
						}else if(rightOperandEndPosition < leftOperandEndPosition)
						{
							gapExtension = true;
						}
					}

				}else
				{if (rightOperandZeroRun)
				{
					setValueOnCompressedWord(rightOperand[rightOperandIndex],compressedResult);
					vector<int> result = alignLeftWithRight(rightOperandEndPosition,leftOperandIndex,leftOperandEndPosition,leftOperand,compressedResult,ZERO_GAP,AND,gapExtension);
					leftOperandIndex = result[0];
					leftOperandEndPosition = result[1];
					if (leftOperandEndPosition == rightOperandEndPosition)
					{
						leftOperandIndex++;
						rightOperandIndex++;
						gapExtension = false;
					}else if(rightOperandEndPosition < leftOperandEndPosition)
					{
						gapExtension = true;
					}
				} 
				else
				{
					vector<int> result = alignLeftWithRight(rightOperandEndPosition,leftOperandIndex,leftOperandEndPosition,leftOperand,compressedResult,ONE_GAP,AND,gapExtension);
					leftOperandIndex = result[0];
					leftOperandEndPosition = result[1];
					if (leftOperandEndPosition == rightOperandEndPosition)
					{
						leftOperandIndex++;
						rightOperandIndex++;
						gapExtension = false;
					}else if(rightOperandEndPosition < leftOperandEndPosition)
					{
						gapExtension = true;
					}
				}
				}
			}
		}

		if (activeWordSize > 0)
		{
			st->ActiveWordSize(ActiveWordSize());
			st->ActiveWord(activeWord & (structure.ActiveWord()));
		}
		st->setCompressedStream(compressedResult);
		st->MainArraySize(MainArraySize());
		return st;
	}



	WAHStructure * WAHStructure::operator |(WAHStructure & structure)
	{
		int compressedStreamLength = mainArraySize;
		vector<unsigned long int> rightOperand = structure.getCompressedVector();
		vector<unsigned long int> leftOperand = compressedStreamVector;
		bool rightOperandZeroRun = true;
		bool leftOperandZeroRun = true;	
		int leftOperandStartPosition = 0;
		int leftOperandEndPosition = 0;
		int rightOperandStartPosition = 0;
		int rightOperandEndPosition = 0;
		int compressedWordValue = 0;
		int leftOperandIndex = 0;
		int rightOperandIndex = 0;
		int compressedWordIndex = 0;
		bool gapExtension  = false;

		WAHStructure * st = new WAHStructure();
		vector<unsigned long int> compressedResult;

		while(leftOperandEndPosition < compressedStreamLength || rightOperandEndPosition < compressedStreamLength)
		{
			if (leftOperandEndPosition == rightOperandEndPosition)
			{
				int leftOperandStartBitValue = getStartBitValue(leftOperand[leftOperandIndex]);
				if (leftOperandStartBitValue == LITERAL_WORD)
				{
					leftOperandEndPosition++;
					leftOperandStartPosition = leftOperandEndPosition;
					int rightOperandStartBitValue = getStartBitValue(rightOperand[rightOperandIndex]);

					if (rightOperandStartBitValue == LITERAL_WORD)
					{
						rightOperandEndPosition++;
						rightOperandStartPosition = rightOperandEndPosition;
						setValueOnCompressedWord(leftOperand[leftOperandIndex++] | rightOperand[rightOperandIndex++],compressedResult);
					}else if (rightOperandStartBitValue == ONE_GAP_WORD)
					{
						rightOperandEndPosition += (rightOperand[rightOperandIndex] - ONE_GAP_START_FLAG);	
						rightOperandZeroRun = false;
					}
					else if (rightOperandStartBitValue == ZERO_GAP_WORD)
					{
						rightOperandEndPosition += (rightOperand[rightOperandIndex] - ZERO_GAP_START_FLAG);	
						rightOperandZeroRun = true;
					}

				}else if (leftOperandStartBitValue == ZERO_GAP_WORD)
				{
					leftOperandEndPosition += (leftOperand[leftOperandIndex] - ZERO_GAP_START_FLAG);
					leftOperandZeroRun = true;
					int rightOperandStartBitValue =  getStartBitValue(rightOperand[rightOperandIndex]);
					if (rightOperandStartBitValue == LITERAL_WORD)
					{					
						rightOperandEndPosition++;										
					}else if (rightOperandStartBitValue == ONE_GAP_WORD)
					{
						rightOperandEndPosition += (rightOperand[rightOperandIndex] - ONE_GAP_START_FLAG);	
						if (rightOperandEndPosition == leftOperandEndPosition)
						{
							setValueOnCompressedWord(rightOperand[rightOperandIndex],compressedResult);
							leftOperandIndex++;
							rightOperandIndex++;
						}
						rightOperandZeroRun = false;
					}
					else if (rightOperandStartBitValue == ZERO_GAP_WORD)
					{
						rightOperandEndPosition += (rightOperand[rightOperandIndex] - ZERO_GAP_START_FLAG);	
						if (rightOperandEndPosition == leftOperandEndPosition)
						{
							setValueOnCompressedWord(rightOperand[rightOperandIndex],compressedResult);
							leftOperandIndex++;
							rightOperandIndex++;
						}
						rightOperandZeroRun = true;
					}

				}else if (leftOperandStartBitValue == ONE_GAP_WORD)
				{
					leftOperandEndPosition += (leftOperand[leftOperandIndex] - ONE_GAP_START_FLAG);
					leftOperandZeroRun = false;
					int rightOperandStartBitValue = getStartBitValue(rightOperand[rightOperandIndex]);
					if (rightOperandStartBitValue == LITERAL_WORD)
					{					
						rightOperandEndPosition++;										
					}else if (rightOperandStartBitValue == ONE_GAP_WORD)
					{
						rightOperandEndPosition += (rightOperand[rightOperandIndex] - ONE_GAP_START_FLAG);	
						if (rightOperandEndPosition == leftOperandEndPosition)
						{
							setValueOnCompressedWord(leftOperand[leftOperandIndex],compressedResult);
							leftOperandIndex++;
							rightOperandIndex++;
						}
						rightOperandZeroRun = false;
					}
					else if (rightOperandStartBitValue == ZERO_GAP_WORD)
					{
						rightOperandEndPosition += (rightOperand[rightOperandIndex] - ZERO_GAP_START_FLAG);	
						if (rightOperandEndPosition == leftOperandEndPosition)
						{
							setValueOnCompressedWord(leftOperand[leftOperandIndex],compressedResult);
							leftOperandIndex++;
							rightOperandIndex++;
						}
						rightOperandZeroRun = true;
					}

				}

			}
			else if (leftOperandEndPosition > rightOperandEndPosition)
			{

				if (gapExtension)
				{
					int leftStartBit = getStartBitValue(leftOperand[leftOperandIndex]);
					if (leftStartBit == ONE_GAP_WORD)
					{
						unsigned long int blockRun = leftOperandEndPosition - rightOperandEndPosition;
						unsigned long int gapRunBlocks = leftOperand[leftOperandIndex] - ONE_GAP_START_FLAG;
						unsigned long int resultGap = leftOperand[leftOperandIndex] - gapRunBlocks + blockRun;
						setValueOnCompressedWord(resultGap,compressedResult);
						vector<int> result = alignRightWithLeft(leftOperandEndPosition,rightOperandIndex,rightOperandEndPosition,rightOperand,compressedResult,ONE_GAP,OR,gapExtension);
						rightOperandIndex = result[0];
						rightOperandEndPosition = result[1];
						if (leftOperandEndPosition == rightOperandEndPosition)
						{
							leftOperandIndex++;
							rightOperandIndex++;
							gapExtension = false;
						}else if(leftOperandEndPosition < rightOperandEndPosition)
						{
							gapExtension = true;
						}
					}else if (leftStartBit == ZERO_GAP_WORD)
					{
						vector<int> result = alignRightWithLeft(leftOperandEndPosition,rightOperandIndex,rightOperandEndPosition,rightOperand,compressedResult,ZERO_GAP,OR,gapExtension);
						rightOperandIndex = result[0];
						rightOperandEndPosition = result[1];
						if (leftOperandEndPosition == rightOperandEndPosition)
						{
							leftOperandIndex++;
							rightOperandIndex++;
							gapExtension = false;
						}else if(leftOperandEndPosition < rightOperandEndPosition)
						{
							gapExtension = true;
						}
					}


				}else
				{if (!leftOperandZeroRun)
				{
					setValueOnCompressedWord(leftOperand[leftOperandIndex],compressedResult);
					vector<int> result = alignRightWithLeft(leftOperandEndPosition,rightOperandIndex,rightOperandEndPosition,rightOperand,compressedResult,ONE_GAP,OR,gapExtension);
					rightOperandIndex = result[0];
					rightOperandEndPosition = result[1];
					if (leftOperandEndPosition == rightOperandEndPosition)
					{
						leftOperandIndex++;
						rightOperandIndex++;
						gapExtension = false;
					}else if(leftOperandEndPosition < rightOperandEndPosition)
					{
						gapExtension = true;
					}
				} 
				else
				{
					vector<int> result = alignRightWithLeft(leftOperandEndPosition,rightOperandIndex,rightOperandEndPosition,rightOperand,compressedResult,ZERO_GAP,OR,gapExtension);
					rightOperandIndex = result[0];
					rightOperandEndPosition = result[1];
					if (leftOperandEndPosition == rightOperandEndPosition)
					{
						leftOperandIndex++;
						rightOperandIndex++;
						gapExtension = false;
					}else if(leftOperandEndPosition < rightOperandEndPosition)
					{
						gapExtension = true;
					}

				}
				}
			}else if (leftOperandEndPosition < rightOperandEndPosition)
			{
				if (gapExtension)
				{
					int rightStartBit = getStartBitValue(rightOperand[rightOperandIndex]);
					if (rightStartBit == ONE_GAP_WORD)
					{
						unsigned long int blockRun = rightOperandEndPosition - leftOperandEndPosition;
						//unsigned long int gapRunBlocks = rightOperand[rightOperandIndex] - ONE_GAP_START_FLAG;
						unsigned long int resultGap = ONE_GAP_START_FLAG + blockRun;
						setValueOnCompressedWord(resultGap,compressedResult);
						vector<int> result = alignLeftWithRight(rightOperandEndPosition,leftOperandIndex,leftOperandEndPosition,leftOperand,compressedResult,ONE_GAP,OR,gapExtension);
						leftOperandIndex = result[0];
						leftOperandEndPosition = result[1];
						if (leftOperandEndPosition == rightOperandEndPosition)
						{
							leftOperandIndex++;
							rightOperandIndex++;
							gapExtension = false;
						}else if(rightOperandEndPosition < leftOperandEndPosition)
						{
							gapExtension = true;
						}
					}else if (rightStartBit == ZERO_GAP_WORD)
					{
						vector<int> result = alignLeftWithRight(rightOperandEndPosition,leftOperandIndex,leftOperandEndPosition,leftOperand,compressedResult,ZERO_GAP,OR,gapExtension);
						leftOperandIndex = result[0];
						leftOperandEndPosition = result[1];
						if (leftOperandEndPosition == rightOperandEndPosition)
						{
							leftOperandIndex++;
							rightOperandIndex++;
							gapExtension = false;
						}else if(rightOperandEndPosition < leftOperandEndPosition)
						{
							gapExtension = true;
						}
					}

				}else
				{if (!rightOperandZeroRun)
				{
					setValueOnCompressedWord(rightOperand[rightOperandIndex],compressedResult);
					vector<int> result = alignLeftWithRight(rightOperandEndPosition,leftOperandIndex,leftOperandEndPosition,leftOperand,compressedResult,ONE_GAP,OR,gapExtension);
					leftOperandIndex = result[0];
					leftOperandEndPosition = result[1];
					if (leftOperandEndPosition == rightOperandEndPosition)
					{
						leftOperandIndex++;
						rightOperandIndex++;
						gapExtension = false;
					}else if(rightOperandEndPosition < leftOperandEndPosition)
					{
						gapExtension = true;
					}
				} 
				else
				{
					vector<int> result = alignLeftWithRight(rightOperandEndPosition,leftOperandIndex,leftOperandEndPosition,leftOperand,compressedResult,ZERO_GAP,OR,gapExtension);
					leftOperandIndex = result[0];
					leftOperandEndPosition = result[1];
					if (leftOperandEndPosition == rightOperandEndPosition)
					{
						leftOperandIndex++;
						rightOperandIndex++;
						gapExtension = false;
					}else if(rightOperandEndPosition < leftOperandEndPosition)
					{
						gapExtension = true;
					}
				}
				}
			}
		}

		if (activeWordSize > 0)
		{
			st->ActiveWordSize(ActiveWordSize());
			st->ActiveWord(activeWord | (structure.ActiveWord()));
		}
		st->setCompressedStream(compressedResult);
		st->MainArraySize(MainArraySize());
		return st;
	}



	vector<int> WAHStructure::alignRightWithLeft( int leftOpEndPos,int rightOpIndex,int rightOpEndPos,vector<unsigned long int> &rightOperand,vector<unsigned long int> &resultVector,longer_operand longer,operation_type type /*= AND*/,bool gapExtensionState )
	{
		vector<int> result;
		int indexVal=0;
		int opEndPos = 0;
		bool zeroGap = false;

		if (type == AND)
		{
			if (longer == ONE_GAP && !gapExtensionState)
			{
				/*int rightStartBit = getStartBitValue(rightOperand[rightOpIndex]);
				if (rightStartBit == LITERAL_WORD)
				{*/
				setValueOnCompressedWord(rightOperand[rightOpIndex],resultVector);
				//}
			}

			while(leftOpEndPos > rightOpEndPos){
				int startBit = getStartBitValue(rightOperand[++rightOpIndex]);
				if (startBit == LITERAL_WORD)
				{
					rightOpEndPos++;			
				}else if (startBit == ONE_GAP_WORD)
				{
					rightOpEndPos += (rightOperand[rightOpIndex] - ONE_GAP_START_FLAG);			
				}else if (startBit == ZERO_GAP_WORD)
				{
					rightOpEndPos += (rightOperand[rightOpIndex] - ZERO_GAP_START_FLAG);			
				}
				indexVal = rightOpIndex;
				opEndPos = rightOpEndPos;	

				if (longer == ONE_GAP)
				{
					if (leftOpEndPos >= rightOpEndPos)
					{
						setValueOnCompressedWord(rightOperand[rightOpIndex],resultVector);
					}else{
						unsigned long int extraBlocks = rightOpEndPos - leftOpEndPos;
						unsigned long int  rightGapValue = rightOperand[rightOpIndex];
						int startBitValue = getStartBitValue(rightGapValue);
						unsigned long int newGapWord = 0;
						unsigned long int totalBlocks =0;
						if (startBitValue == ONE_GAP_WORD)
						{
							totalBlocks = rightGapValue - ONE_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ONE_GAP_START_FLAG;

						}else if (startBitValue == ZERO_GAP_WORD)
						{
							totalBlocks = rightGapValue - ZERO_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ZERO_GAP_START_FLAG;
						}
						setValueOnCompressedWord(newGapWord,resultVector);

					}
				}
			}
		} 
		else if (type == OR)
		{
			// When performing OR operation the initial literal of the left oerand is not written to the compressed
			//result. This code block performs that. 
			if (longer == ZERO_GAP & !gapExtensionState)
			{
				// 				int rightStartBit = getStartBitValue(rightOperand[rightOpIndex]);
				// 				if (rightStartBit == LITERAL_WORD)
				// 				{
				setValueOnCompressedWord(rightOperand[rightOpIndex],resultVector);
				//				}


			}

			while(leftOpEndPos > rightOpEndPos){
				int startBit = getStartBitValue(rightOperand[++rightOpIndex]);
				if (startBit == LITERAL_WORD)
				{
					rightOpEndPos++;			
				}else if (startBit == ONE_GAP_WORD)
				{
					rightOpEndPos += (rightOperand[rightOpIndex] - ONE_GAP_START_FLAG);			
				}else if (startBit == ZERO_GAP_WORD)
				{
					rightOpEndPos += (rightOperand[rightOpIndex] - ZERO_GAP_START_FLAG);			
				}
				indexVal = rightOpIndex;
				opEndPos = rightOpEndPos;	

				if (longer == ZERO_GAP)
				{
					// In an OR operation when the GAP is zero, corresponding literals in  the other operand(right operand)
					//should be copied on to the compressed result.
					if (leftOpEndPos >= rightOpEndPos)
					{
						setValueOnCompressedWord(rightOperand[rightOpIndex],resultVector);
					}else{

						// If the right operand encounters a gap and extends beyond the left operand this code executes
						// Even though the right operand extends beyond, the overlapping segment should 
						//be copied to compressed result as it the performing operation is OR
						unsigned long int extraBlocks = rightOpEndPos - leftOpEndPos;
						unsigned long int  rightGapValue = rightOperand[rightOpIndex];
						int startBitValue = getStartBitValue(rightGapValue);
						unsigned long int newGapWord = 0;
						unsigned long int totalBlocks =0;
						if (startBitValue == ONE_GAP_WORD)
						{
							totalBlocks = rightGapValue - ONE_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ONE_GAP_START_FLAG;

						}else if (startBitValue == ZERO_GAP_WORD)
						{
							totalBlocks = rightGapValue - ZERO_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ZERO_GAP_START_FLAG;
						}
						setValueOnCompressedWord(newGapWord,resultVector);

					}
				}
			}
		}
		result.push_back(indexVal);
		result.push_back(opEndPos);

		return result;
	}

	vector<int> WAHStructure::alignLeftWithRight( int rightOpEndPos,int leftOpIndex,int leftOpEndPos,vector<unsigned long int> &leftoperand,vector<unsigned long int> &resultVector, longer_operand longer,operation_type type /*= AND*/,bool gapExtensionState )
	{
		vector<int> result;
		int indexVal=0;
		int opEndPos = 0;

		if (type == AND)
		{
			if (longer == ONE_GAP & !gapExtensionState)
			{
				// 				int leftStartBit = getStartBitValue(leftoperand[leftOpIndex]);
				// 				if (leftStartBit == LITERAL_WORD)
				// 				{
				setValueOnCompressedWord(leftoperand[leftOpIndex],resultVector);
				//}


			}

			while(leftOpEndPos < rightOpEndPos){
				int startBit = getStartBitValue(leftoperand[++leftOpIndex]);
				if (startBit == LITERAL_WORD)
				{
					leftOpEndPos++;

				}else if (startBit == ONE_GAP_WORD)
				{
					leftOpEndPos += (leftoperand[leftOpIndex] - ONE_GAP_START_FLAG);

				}else if (startBit == ZERO_GAP_WORD)
				{
					leftOpEndPos += (leftoperand[leftOpIndex] - ZERO_GAP_START_FLAG);

				}

				if (longer == ONE_GAP)
				{
					if (leftOpEndPos <= rightOpEndPos)
					{
						setValueOnCompressedWord(leftoperand[leftOpIndex],resultVector);
					}else{
						unsigned long int extraBlocks = leftOpEndPos-rightOpEndPos ;
						unsigned long int  leftGapValue = leftoperand[leftOpIndex];
						int startBitValue = getStartBitValue(leftGapValue);
						unsigned long int newGapWord = 0;
						unsigned long int totalBlocks =0;
						if (startBitValue == ONE_GAP_WORD)
						{
							totalBlocks = leftGapValue - ONE_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ONE_GAP_START_FLAG;

						}else if (startBitValue == ZERO_GAP_WORD)
						{
							totalBlocks = leftGapValue - ZERO_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ZERO_GAP_START_FLAG;
						}
						setValueOnCompressedWord(newGapWord,resultVector);

					}
				}
			}
			// No need of setting values here. leftOpIndex and leftOpEndPos can be directly
			// inserted to the vector. 
			indexVal = leftOpIndex;
			opEndPos = leftOpEndPos;
		} 
		else if (type == OR)
		{
			if (longer == ZERO_GAP & !gapExtensionState)
			{
// 				int leftStartBit = getStartBitValue(leftoperand[leftOpIndex]);
// 				if (leftStartBit == LITERAL_WORD)
// 				{
					setValueOnCompressedWord(leftoperand[leftOpIndex],resultVector);
				//}

			}

			while(leftOpEndPos < rightOpEndPos){
				int startBit = getStartBitValue(leftoperand[++leftOpIndex]);
				if (startBit == LITERAL_WORD)
				{
					leftOpEndPos++;

				}else if (startBit == ONE_GAP_WORD)
				{
					leftOpEndPos += (leftoperand[leftOpIndex] - ONE_GAP_START_FLAG);

				}else if (startBit == ZERO_GAP_WORD)
				{
					leftOpEndPos += (leftoperand[leftOpIndex] - ZERO_GAP_START_FLAG);

				}

				if (longer == ZERO_GAP)
				{
					if (leftOpEndPos <= rightOpEndPos)
					{
						setValueOnCompressedWord(leftoperand[leftOpIndex],resultVector);
					}else{
						unsigned long int extraBlocks = leftOpEndPos-rightOpEndPos ;
						unsigned long int  leftGapValue = leftoperand[leftOpIndex];
						int startBitValue = getStartBitValue(leftGapValue);
						unsigned long int newGapWord = 0;
						unsigned long int totalBlocks =0;
						if (startBitValue == ONE_GAP_WORD)
						{
							totalBlocks = leftGapValue - ONE_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ONE_GAP_START_FLAG;

						}else if (startBitValue == ZERO_GAP_WORD)
						{
							totalBlocks = leftGapValue - ZERO_GAP_START_FLAG;
							newGapWord = (totalBlocks - extraBlocks) + ZERO_GAP_START_FLAG;
						}
						setValueOnCompressedWord(newGapWord,resultVector);

					}
				}
			}
			// No need of setting values here. leftOpIndex and leftOpEndPos can be directly
			// inserted to the vector. 
			indexVal = leftOpIndex;
			opEndPos = leftOpEndPos;
		}


		result.push_back(indexVal);
		result.push_back(opEndPos);
		return result;
	}

	int WAHStructure::getStartBitValue(unsigned long int word)
	{
		int startBitValue= 0;
		unsigned long int result = word & ZERO_GAP_START_FLAG;
		if (result == 0)
		{
			startBitValue = LITERAL_WORD;
		}
		else 
		{
			result = word & ONE_GAP_START_FLAG;
			if (result == ZERO_GAP_START_FLAG)
			{
				startBitValue = ZERO_GAP_WORD;
			}else
			{
				startBitValue = ONE_GAP_WORD;
			}

		}
		return startBitValue;
	} 

	//This method checks the previous word and the current word if possible
	void WAHStructure::setValueOnCompressedWord(unsigned long int word, vector<unsigned long int> &resultVector)
	{
		if (resultVector.size() == 0)
		{

			resultVector.push_back(word);

		}
		else {
			unsigned long int lastWord = 0;

			lastWord = resultVector[resultVector.size()-1];

			// Maximum maximum number represented for the value has not been considered yet.
			// Remember to use it before final version.
			unsigned long int result = 0;
			if (word == ZERO_LITERAL && lastWord == ZERO_LITERAL )
			{
				resultVector[resultVector.size()-1] = ZERO_GAP_START_FLAG | 2;
			}
			else if (word == ZERO_LITERAL && getStartBitValue(lastWord) == ZERO_GAP_WORD)
			{
				resultVector[resultVector.size()-1] = lastWord + 1;
			}
			else if (word == ONE_LITERAL && lastWord == ONE_LITERAL)
			{
				resultVector[resultVector.size()-1] = ONE_GAP_START_FLAG + 2;
			}
			else if (word == ONE_LITERAL && getStartBitValue(lastWord) == ONE_GAP_WORD)
			{
				resultVector[resultVector.size()-1] = lastWord + 1;
			}
			else if (getStartBitValue(word) == ONE_GAP_WORD && getStartBitValue(lastWord) == ONE_GAP_WORD)
			{
				resultVector[resultVector.size()-1] = word + lastWord - ONE_GAP_START_FLAG;
			}
			else if(getStartBitValue(word) == ZERO_GAP_WORD && getStartBitValue(lastWord) == ZERO_GAP_WORD)
			{
				resultVector[resultVector.size()-1] = word + lastWord - ZERO_GAP_START_FLAG;
			}else
			{
				resultVector.push_back(word);
			}
		}
	}


	void WAHStructure::setCompressedStream(vector<unsigned long int> &x)
	{
		compressedStreamVector = x;
	}

	int WAHStructure::getValue(boost::dynamic_bitset<> & bitMap,int startIndex,int offset)
	{

		boost::dynamic_bitset<> temp(offset);
		for (int i =0; i < offset; i++)
		{
			temp[i] = bitMap[startIndex + i];
		}

		return (int)temp.to_ulong();
	}

	boost::dynamic_bitset<> WAHStructure::getCompressedWord()
	{
		return compressedStream;
	}

	vector<unsigned long int>& WAHStructure::getCompressedVector()
	{
		return compressedStreamVector;
	}

	int WAHStructure::getSpaceUtilisation()
	{
		int compressedSize  = 0;
		int active = 0;

		compressedSize = 24 + (int)(compressedStream.size()/8);	
		active= 4;

		return (active + compressedSize);
	}

	dynamic_bitset<> WAHStructure::convertVectorToBitmap(std::vector<unsigned long int> &decompressedVector)
	{
		int size = decompressedVector.size();
		dynamic_bitset<> result(size * (WORD_SIZE - 1));
		int bitmapIndex = 0;
		for (int i=0; i < size ; i++)
		{
			dynamic_bitset<> temp(WORD_SIZE-1,decompressedVector[i]);
			for (int j = 0; j < WORD_SIZE-1 ;)
			{
				result[bitmapIndex++] = temp[j++];
			}
		}
		return result;
	}


	vector<unsigned long int> WAHStructure::expandCompressedVector(vector<unsigned long int> & compressedVector)
	{

		vector<unsigned long int> result;
		int resultIndex = 0;
		for (int i = 0 ; i < compressedVector.size() ; i++)
		{
			unsigned long int vectorValue = compressedVector[i];
			int startBit = getStartBitValue(vectorValue);
			if (startBit == LITERAL_WORD)
			{
				result.push_back(vectorValue);
			}else if (startBit == ONE_GAP_WORD)
			{
				int blocks  = vectorValue - ONE_GAP_START_FLAG;
				for (int blockNo = 0; blockNo < blocks ; blockNo++)
				{
					result.push_back(ONE_LITERAL);
				}
			}else if (startBit == ZERO_GAP_WORD)
			{
				int blocks  = vectorValue - ZERO_GAP_START_FLAG;
				for (int blockNo = 0; blockNo < blocks ; blockNo++)
				{
					result.push_back(ZERO_LITERAL);
				}

			}
		}

		return result;
	}

	dynamic_bitset<> WAHStructure::decompress()
	{
		vector<unsigned long int> result = expandCompressedVector(compressedStreamVector);
		dynamic_bitset<> resultBitset = convertVectorToBitmap(result);
		if (activeWordSize > 0)
		{
			dynamic_bitset<> temp(activeWordSize,activeWord);
			for (int i = 0; i < temp.size() ; i++)
			{
				resultBitset.push_back(temp[i]);
			}
		}
		return resultBitset;
	}

	WAHStructure * WAHStructure::operator ~(){
		WAHStructure * result = new WAHStructure();
		vector<unsigned long int> newCompressedStream;
		vector<unsigned long int>::iterator vectorIterator  = compressedStreamVector.begin();
		while(vectorIterator != compressedStreamVector.end())
		{
			unsigned long int vectorValue = *(vectorIterator);
			int startBitValue = getStartBitValue(vectorValue);
			if (startBitValue == LITERAL_WORD)
			{
				newCompressedStream.push_back((~vectorValue) & ONE_LITERAL);
			}else if (startBitValue == ONE_GAP_WORD)
			{
				newCompressedStream.push_back((vectorValue - ONE_GAP_START_FLAG + ZERO_GAP_START_FLAG));
			}else if(startBitValue == ZERO_GAP_WORD)
			{
				newCompressedStream.push_back((vectorValue - ZERO_GAP_START_FLAG + ONE_GAP_START_FLAG));
			}
			vectorIterator++;
		}
		result->setCompressedStream(newCompressedStream);
		result->MainArraySize(mainArraySize);
		if (activeWordSize > 0)
		{
			result->ActiveWordSize(activeWordSize);
			result->ActiveWord(~activeWord);
		}
		return result;
	}}