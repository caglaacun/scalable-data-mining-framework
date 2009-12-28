#pragma once
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "Math.h"
class Structure2
{
public:
	Structure2(void);
	~Structure2(void);
	void buildArray(boost::dynamic_bitset<> bitMap);
	void compressWords();
	void setCompressedStream(boost::dynamic_bitset<> & x);
	void setActiveWord(boost::dynamic_bitset<> & x);
	void printArray();
	int setOneCount(int count,boost::dynamic_bitset<> & compressed,int startIndex);
	int setZeroCount(int count,boost::dynamic_bitset<> & compressed,int startIndex);
	int setLiteral(int literalVal,boost::dynamic_bitset<> & compressed,int startIndex);
	void printCompressedStream();
	void printCount();
	void flip();
	int getMainArrayLength();
	boost::dynamic_bitset<> getActiveWord();	
	Structure2 * Structure2::operator & (Structure2&);
	Structure2 * Structure2::operator | (Structure2&);
	boost::dynamic_bitset<> getCompressedWord();
	int getValue(boost::dynamic_bitset<> & bitMap,int startIndex,int offset);
	int getSpaceUtilisation();
	
private:
	int * mainArray;
	int mainArraySize;
	int count;
	static const int WORD_SIZE = 32;
	boost::dynamic_bitset<>  activeWord;
	boost::dynamic_bitset<>  compressedStream;
	static const int MAX_WORD_VALUE = 2147483647;
};
