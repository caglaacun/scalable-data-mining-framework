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

	dynamic_bitset<> temp(this->NoOfVBitStreams());
	int val=0;

	for (int i=0 ; i < this->NoOfVBitStreams() ;i++)
	{
		temp[i] = this->vBitStreams()[i]->getProcessedBitStream()[tupleID - 1];
	}

	val = temp.to_ulong();
		
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
