#include "Structure.h"
#include <iostream>
using namespace std;
Structure::Structure(void)
{
}

Structure::~Structure(void)
{
}

void Structure::buildStructure(boost::dynamic_bitset<> & uncompStream)
{
	boost::dynamic_bitset<> compStream(uncompStream.size());
	bool prevPureOneBlock=false;
	bool prevPureZeroBlock=false;
	for (int i = 0;i < uncompStream.size(); i+=WORD_SIZE)
	{   
		int compStreamIndex= 0;
	boost::dynamic_bitset<> temp = getBlock((WORD_SIZE-1),i,uncompStream);	
	if(prevPureZeroBlock && temp.to_ulong() == 0)
	{
		prevPureZeroBlock = true;
		compStreamIndex = putLiteral(compStreamIndex,compStream,temp);
	}else if(prevPureOneBlock && temp.to_ulong()== (int)(pow((double)2,(double)WORD_SIZE)-1)){
		prevPureOneBlock = true;
		compStreamIndex = putLiteral(compStreamIndex,compStream,temp);
	}else{
		if (temp.to_ulong() == 0)
		{
			prevPureZeroBlock = true;
			compStreamIndex = putLiteral(compStreamIndex,compStream,temp);
		}else if (temp.to_ulong()== (int)(pow((double)2,(double)(WORD_SIZE-1))-1))
		{
			prevPureOneBlock = true;
			compStreamIndex = putLiteral(compStreamIndex,compStream,temp);
		}
	}
	}
	cout<<"Test Compression Stream: "<<compStream<<endl;
}
boost::dynamic_bitset<> Structure::getBlock(int offset,int start,boost::dynamic_bitset<> & input)
{
	boost::dynamic_bitset<> temp(offset);
	for (int i=0; i < offset; i++)
	{
		temp[i] = input[i+start];
	}
	return temp;
}

int Structure::putLiteral(int startPos,boost::dynamic_bitset<>& comprStream,boost::dynamic_bitset<>& lit)
{
	comprStream[startPos] = 0;
	int endPos = 0;
	for (int i=0; i < (WORD_SIZE-1);i++)
	{
		comprStream[i+startPos+1] = lit[i];
		endPos = i+startPos+1;
	}
	endPos++;
	return endPos;
}