#include "BasicOperations.h"

BasicOperations::BasicOperations(void)
{
}

BasicOperations::~BasicOperations(void)
{
}

dynamic_bitset<> BasicOperations::doAndOperation(dynamic_bitset<> firstBitString, dynamic_bitset<> secondBitString)
{
	//dynamic_bitset<> x(5);
	//return x;
	return firstBitString & secondBitString;
}

dynamic_bitset<> BasicOperations::doOrOperation(dynamic_bitset<> firstBitString, dynamic_bitset<> secondBitString)
{
	//dynamic_bitset<> x(5);
	//return x;
	return firstBitString | secondBitString;
}

dynamic_bitset<> BasicOperations::doNotOperation(dynamic_bitset<> BitString)
{
	//dynamic_bitset<> x(5);
	//return x;
	return ~BitString;
}