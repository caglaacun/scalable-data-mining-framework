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

vector<VBitStream*> EncodedIntAttribute::vBitStreams(){
	return this->_vBitStreamSet;
}

vector<bool> EncodedIntAttribute::SignBitMap(){
	return this->_signBitMap;
}

string EncodedIntAttribute::attributeName(){
	return this->_attName;
}

int EncodedIntAttribute::NoOfVBitStreams(){
	return this->_noOfVBitStreams;
}

ATT_TYPE EncodedIntAttribute::attributeType(){
	return this->_attType;
}

void EncodedIntAttribute::setVBitStreamSize(int size){
	this->_vBitStreamSet.resize(size);
}

VBitStream* EncodedIntAttribute::bitStreamAt(int bitStreamID){
	/*int temp = 0;
	for (; temp<this->_vBitStreamSet.size() ; temp++)
	{
		if (this->_vBitStreamSet[temp]->bitStreamPosition() == bitStreamID)
		{
			return this->_vBitStreamSet[temp];
		}
	}
	return NULL;*/
	return this->_vBitStreamSet[bitStreamID];
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
		bitSet[i] = this->_vBitStreamSet[i]->BitStream()[tupleID - 1];
		if(bitSet[i]) {val += pow(2.0,(double)(this->_noOfVBitStreams - i - 1));}
	}

	if (this->_signBitMap[tupleID - 1])
	{
		val *= -1;
	}
	
	return val;
}

void EncodedIntAttribute::setAttName(string name){
	this->_attName = name;
}

void EncodedIntAttribute::setAttType(ATT_TYPE type){
	this->_attType = type;
}

void EncodedIntAttribute::setNoOfVBitStreams(int novBitsets,int noRows){
	this->_noOfVBitStreams = novBitsets;
	this->_vBitStreamSet.resize(this->_noOfVBitStreams);

	for (int i = 0 ; i <novBitsets ; i++)
	{
		this->_vBitStreamSet[i] = new VBitStream(noRows);
	}
}

void EncodedIntAttribute::setVBitStreams(vector<VBitStream*> vBitset){
	this->_vBitStreamSet = vBitset;
}

void EncodedIntAttribute::setTheSignBitMap(long int *values,int valSet){
	this->_signBitMap.resize(valSet);
	for (int i = 0 ; i < valSet ; i++)
	{
		if(values[i] == abs(values[i])) this->_signBitMap[i] = false;
		else this->_signBitMap[i] = true;
	}
}