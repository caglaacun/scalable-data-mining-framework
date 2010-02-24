#include "StdAfx.h"
#include "VBitStream.h"

VBitStream::VBitStream(int bitCount)
{
	this->_bitCount = bitCount;
	this->_dataBitStream.resize(bitCount);
}

VBitStream::~VBitStream(void)
{
}

int VBitStream::oneCount(){
	int oneCount = 0;
	for (int i = 0 ; i < this->_bitCount ; i++)
	{
		if(this->_dataBitStream[i]) oneCount++;
	}

	return oneCount;
}

vector<bool> VBitStream::BitStream(){
	return this->_dataBitStream;
}

string VBitStream::bitStreamAggregation(){
	return this->_bitAggregation;
}

int VBitStream::bitStreamAllocAttID(){
	return this->_bitStreamAllocAttID;
}

void VBitStream::setBitValue(int pos, bool val){
	this->_dataBitStream[pos] = val;
}

void VBitStream::setBitStreamAggregation(string bitAgg){
	this->_bitAggregation = bitAgg;
}

void VBitStream::setBitStreamAllocAttID(int attID){
	this->_bitStreamAllocAttID = attID;
}
