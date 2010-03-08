#include "stdafx.h"
#include "EncodedAttributeInfo.h"
#include "BitStreamInfo.h"
#include <iostream>

using namespace std;

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
	return this->_vBitStreams.at(bitStreamID);
}

BitStreamInfo* EncodedAttributeInfo::operator ()(const int bitStreamID){
	try{
	return this->bitStreamAt(bitStreamID);
}
	catch(std::exception &e){
		std::cerr<<"Error in retrieving bit stream : "<<e.what()<<endl;
		exit(0);
	}
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
	try{
	this->_noOfVBitStreams = novBitsets;
	this->_vBitStreams.resize(this->_noOfVBitStreams);

	for (int i = 0 ; i <novBitsets ; i++)
	{
			this->_vBitStreams.at(i) = new VBitStream(rows);
		}
	}
	catch(std::exception &e){
		cerr<<"Error in creating vertical bit streams for a given # of bit stream"<<endl;
		exit(5);
	}
}

void EncodedAttributeInfo::setVBitStreams(std::vector<BitStreamInfo*> vBitset){
	this->_vBitStreams = vBitset;
}