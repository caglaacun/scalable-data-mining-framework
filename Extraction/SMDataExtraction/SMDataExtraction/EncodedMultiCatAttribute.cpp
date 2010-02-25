#include "StdAfx.h"
#include "EncodedMultiCatAttribute.h"
#include "MultiCatDataInfo.h"
#include "../Include/boost/dynamic_bitset.hpp"
#include "VBitStream.h"

EncodedMultiCatAttribute::EncodedMultiCatAttribute(void)
{
}

EncodedMultiCatAttribute::~EncodedMultiCatAttribute(void)
{
}

int* EncodedMultiCatAttribute::getMappedIntVals(){
	return this->_mappedIntVals;
}

int EncodedMultiCatAttribute::noOfUniqueValues(){
	return this->_noOfUniqueVals;
}

void EncodedMultiCatAttribute::mapStringDataToCategories(string* _valueList,vector<string> _uniqueValList,int noOfRows){
	this->_uniqueValList = _uniqueValList;
	int maxUniqueIndex = _uniqueValList.size();
	this->_noOfVBitStreams = (int)(ceil(log10((double)maxUniqueIndex)/log10(2.0)));
	this->_vBitStreamSet.resize(this->_noOfVBitStreams);
	this->_noOfUniqueVals = _uniqueValList.size();

	MultiCatDataInfo *df = new MultiCatDataInfo(_uniqueValList);
	this->_mappedIntVals = df->getAssignedEncodedNumberList();
	
	for (int j = 0 ; j < this->_noOfVBitStreams ; j++)
	{
		this->_vBitStreamSet[j] = new VBitStream(noOfRows);
	}

	for (int i = 0 ; i < noOfRows ; i++)
	{
		int pos = std::find(_uniqueValList.begin(),_uniqueValList.end(),_valueList[i]) - _uniqueValList.begin();
		dynamic_bitset<> bitSet(this->_noOfVBitStreams,(unsigned long)pos);
		this->_mappedValList.push_back(bitSet);
	}

}

vector<dynamic_bitset<>> EncodedMultiCatAttribute::mappedValList(){
	return this->_mappedValList;
}

int EncodedMultiCatAttribute::NoOfVBitStreams(){
	return this->_noOfVBitStreams;
}

vector<VBitStream*> EncodedMultiCatAttribute::vBitStreams(){
	return this->_vBitStreamSet;
}

string EncodedMultiCatAttribute::attributeName(){
	return this->_attName;
}

int EncodedMultiCatAttribute::attributeID(){
	return this->_attID;
}

ATT_TYPE EncodedMultiCatAttribute::attributeType(){
	return this->_attType;
}

vector<string> EncodedMultiCatAttribute::uniqueValList(){
	return this->_uniqueValList;
}

void EncodedMultiCatAttribute::setAttID(int attID){
	this->_attID = attID;
}

void EncodedMultiCatAttribute::setAttName(std::string name){
	this->_attName = name;
}

void EncodedMultiCatAttribute::setAttType(ATT_TYPE type){
	this->_attType = type;
}

void EncodedMultiCatAttribute::setNoOfVBitStreams(int novBitsets, int rows){
	this->_noOfVBitStreams = novBitsets;
	this->_vBitStreamSet.resize(this->_noOfVBitStreams);

	for (int i = 0 ; i <novBitsets ; i++)
	{
		this->_vBitStreamSet[i] = new VBitStream(rows);
	}
}

VBitStream* EncodedMultiCatAttribute::bitStreamAt(int bitStreamID){
	return this->_vBitStreamSet[bitStreamID];
}

string EncodedMultiCatAttribute::DecodeTheTuple(int tupleID){
	int i = 0;
	bool* bitSet = new bool[this->_noOfVBitStreams];
	int val=0;
	for (; i<this->_noOfVBitStreams ; i++)
	{
		bitSet[i] = this->_vBitStreamSet[i]->BitStream()[tupleID - 1];
		if(bitSet[i]) {val += pow(2.0,(double)(this->_noOfVBitStreams - i - 1));}
	}
	
	return this->_uniqueValList[val];
}

VBitStream* EncodedMultiCatAttribute::operator()(const int bitStreamID){
	return this->bitStreamAt(bitStreamID);
}
