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
	return this->_oneCount;
}

vector<bool> VBitStream::BitStream(){
	return this->_dataBitStream;
}

int VBitStream::bitStreamPosition(){
	return this->_bitStreamPosition;
}
