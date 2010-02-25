#pragma once
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "Math.h"
#include <vector>

using namespace std;
using namespace  boost;
class WAHStructure
{
public:
	WAHStructure(void);
	~WAHStructure(void);	
	void compressWords(boost::dynamic_bitset<>& bitMap);
	void printVector();
	void setCompressedStream(vector<unsigned long int> &x);	
	void printArray();	
	void printCompressedStream();
	vector<unsigned long int>& getCompressedVector();
	dynamic_bitset<> decompress();
	void flip();
	WAHStructure * operator!();
	int getMainArrayLength();
	
	WAHStructure * WAHStructure::operator & (WAHStructure&);
	//WAHStructure * WAHStructure::operator | (WAHStructure&);
	boost::dynamic_bitset<> getCompressedWord();
	int getValue(boost::dynamic_bitset<> & bitMap,int startIndex,int offset);
	int getSpaceUtilisation();

	//Public Getters and setters
	const short int ActiveWordSize() { return activeWordSize; }
	void ActiveWordSize(short int val) { activeWordSize = val; }
	unsigned long int ActiveWord() const { return activeWord; }
	void ActiveWord(unsigned long int val) { activeWord = val; }
	int MainArraySize() const { return mainArraySize; }
	unsigned long long count();
	void MainArraySize(int val) { mainArraySize = val; }
	static const unsigned long int ZERO_LITERAL = 0;
	static const unsigned long int ONE_LITERAL = 2147483647;
	static const unsigned long int ONE_GAP_START_FLAG = 3221225472;
	static const unsigned long int ZERO_GAP_START_FLAG = 2147483648;
	static const int LITERAL_WORD = 0;
	static const int ZERO_GAP_WORD = 1;
	static const int ONE_GAP_WORD = 2;



private:
	void buildArray(boost::dynamic_bitset<>& bitMap);
	dynamic_bitset<> convertVectorToBitmap(vector<unsigned long int> &decompressedVector);
	vector<unsigned long int> expandCompressedVector(vector<unsigned long int>& compressedVector);
	vector<int> alignRightWithLeft(int leftOpEndPos,int rightOpIndex,int rightOpEndPos,vector<unsigned long int> &rightOperand,vector<unsigned long int> &resultVector,bool longerOperand);	
	vector<int> alignLeftWithRight(int rightOpEndPos,int leftOpIndex,int leftOpEndPos,vector<unsigned long int> &leftoperand,vector<unsigned long int> &resultVector,bool longerOperand);	
	void setOneCount(unsigned long int count,vector<unsigned long int> & compressed);
	void setZeroCount(unsigned long int count,vector<unsigned long int> & compressed);
	void setLiteral(unsigned long int literalVal,vector<unsigned long int> & compressed);
	void copyIntegerToBitMap(dynamic_bitset<> &bitmap,int index);
	int getStartBitValue(unsigned long int word);	
	void setValueOnCompressedWord(unsigned long int word, vector<unsigned long	int> &compressedResult);	
	unsigned long int incrementZeroWord(unsigned long int amount,unsigned long int word, vector<unsigned long int> &compressVector);
	vector<unsigned long int> compressedStreamVector;
	boost::dynamic_bitset<>  compressedStream;
	unsigned long int getLiteralCount(unsigned long int &bitLiteral);
	unsigned long int getGapCount(unsigned long int bitGap);

	int * mainArray;
	int mainArraySize;	
	//int count;
	unsigned long int activeWord;	
	short int activeWordSize;	
	static const int WORD_SIZE = 32;		
	
};
