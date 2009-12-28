#pragma once
#include "boost\dynamic_bitset\dynamic_bitset.hpp"

class Structure
{
public:
	Structure(void);
	~Structure(void);
	void buildStructure(boost::dynamic_bitset<>&);
	boost::dynamic_bitset<> getBlock(int offset,int start,boost::dynamic_bitset<> &);
	int putLiteral(int startPos,boost::dynamic_bitset<>&,boost::dynamic_bitset<>& lit);	
	static const int WORD_SIZE = 32;
private:
	int * mainArray;
	
};
