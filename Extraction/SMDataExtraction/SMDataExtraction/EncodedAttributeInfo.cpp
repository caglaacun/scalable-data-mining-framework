#include "stdafx.h"
#include "EncodedAttributeInfo.h"
#include "BitStreamInfo.h"

EncodedAttributeInfo::EncodedAttributeInfo(){

}

EncodedAttributeInfo::~EncodedAttributeInfo(){

}

vector<BitStreamInfo*> EncodedAttributeInfo::vBitStreams(){
	return this->_vBitStreams;
}

int EncodedAttributeInfo::attributeID(){
	return this->_attID;
}

string EncodedAttributeInfo::attributeName(){
	return this->_attName;
}

ATT_TYPE EncodedAttributeInfo::attributeType(){
	return this->_attType;
}

int EncodedAttributeInfo::NoOfVBitStreams(){
	return this->_noOfVBitStreams;
}

BitStreamInfo* EncodedAttributeInfo::bitStreamAt(int bitStreamID){
	return this->_vBitStreams[bitStreamID];
}

BitStreamInfo* EncodedAttributeInfo::operator ()(const int bitStreamID){
	return this->bitStreamAt(bitStreamID);
}

void EncodedAttributeInfo::setVBitStreamSize(int newSize){
	//this->_vBitStreams.clear();
	//this->_vBitStreams.resize(newSize);
}

void EncodedAttributeInfo::setAttID(int attID){
	this->_attID = attID;
}

void EncodedAttributeInfo::setAttName(std::string name){
	this->_attName = name;
}

void EncodedAttributeInfo::setAttType(ATT_TYPE type){
	this->_attType = type;
}

void EncodedAttributeInfo::setNoOfVBitStreams(int novBitsets, int rows){
	this->_noOfVBitStreams = novBitsets;
	this->_vBitStreams.resize(this->_noOfVBitStreams);

	for (int i = 0 ; i <novBitsets ; i++)
	{
		this->_vBitStreams[i] = new VBitStream(rows);
	}
}

void EncodedAttributeInfo::setVBitStreams(std::vector<BitStreamInfo*> vBitset){
	this->_vBitStreams = vBitset;
}