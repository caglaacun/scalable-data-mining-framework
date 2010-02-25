#include "StdAfx.h"
#include "EncodedIntAttribute.h"

#include <bitset>
#include <math.h>

EncodedIntAttribute::EncodedIntAttribute(void)
{
}

EncodedIntAttribute::~EncodedIntAttribute(void)
{
}


vector<bool> EncodedIntAttribute::SignBitMap(){
	return this->_signBitMap;
}


int EncodedIntAttribute::getTheSignOf(int tupleID){
	return (int)this->_signBitMap[tupleID - 1];
}

int EncodedIntAttribute::decodeTheTuple(int tupleID){
	int i = 0;
	bool* bitSet = new bool[this->NoOfVBitStreams()];
	int val=0;
	for (; i<this->NoOfVBitStreams() ; i++)
	{
		bitSet[i] = this->vBitStreams()[i]->BitStream()[tupleID - 1];
		if(bitSet[i]) {val += pow(2.0,(double)(this->NoOfVBitStreams() - i - 1));}
	}

	if (this->_signBitMap[tupleID - 1])
	{
		val *= -1;
	}
	
	return val;
}


void EncodedIntAttribute::setTheSignBitMap(long int *values,int valSet){
	this->_signBitMap.resize(valSet);
	for (int i = 0 ; i < valSet ; i++)
	{
		if(values[i] == abs(values[i])) this->_signBitMap[i] = false;
		else this->_signBitMap[i] = true;
	}
}
