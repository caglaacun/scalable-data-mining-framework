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

vector<VBitStream> EncodedIntAttribute::vBitStreams(){
	return this->_vBitStreamSet;
}

vector<bool> EncodedIntAttribute::SignBitMap(){
	return this->_signBitMap;
}

char* EncodedIntAttribute::attributeName(){
	return this->_attName;
}

int EncodedIntAttribute::NoOfVBitStreams(){
	return this->_noOfVBitStreams;
}

ATT_TYPE EncodedIntAttribute::attributeType(){
	return this->_attType;
}

VBitStream EncodedIntAttribute::bitStreamAt(int bitStreamID){
	int temp = 0;
	for (; temp<this->_vBitStreamSet.size() ; temp++)
	{
		if (this->_vBitStreamSet[temp].bitStreamPosition() == bitStreamID)
		{
			return this->_vBitStreamSet[temp];
		}
	}
	return NULL;
}

int EncodedIntAttribute::getTheSignOf(int tupleID){
	return (int)this->_signBitMap[tupleID - 1];
}

int EncodedIntAttribute::decodeTheTuple(int tupleID){
	int i = 0;
	bool* bitSet = new bool[this->_noOfVBitStreams];
	int val=0;
	for (; i<this->_noOfVBitStreams ; i++)
	{
		bitSet[i] = this->_vBitStreamSet[i].BitStream()[tupleID - 1];
		if(bitSet[i]) {val += pow(2.0,(double)(this->_noOfVBitStreams - i - 1));}
	}

	if (this->_signBitMap[tupleID - 1])
	{
		val *= -1;
	}
	
	return val;
}

void EncodedIntAttribute::setAttName(char* name){
	this->_attName = name;
}

void EncodedIntAttribute::setAttType(ATT_TYPE type){
	this->_attType = type;
}

void EncodedIntAttribute::setNoOfVBitStreams(int novBitsets){
	this->_noOfVBitStreams = novBitsets;
}

void EncodedIntAttribute::setVBitStreams(vector<VBitStream> vBitset){
	this->_vBitStreamSet = vBitset;
}

void EncodedIntAttribute::setSignBitMap(vector<bool> sBitMap){
	this->_signBitMap = sBitMap;
}