#include "StdAfx.h"
#include "WrapDataSource.h"
#include "PureIntAttInfo.h"
#include "EncodedIntAttribute.h"
#include "../Include/boost/dynamic_bitset.hpp"
#include <iostream>
#include <vector>
#include <math.h>
#include "AttributeType.h"

using namespace std;
using namespace boost;

WrapDataSource::WrapDataSource(DBQueryExecution cExec)
{
	this->_queryDataInfo = cExec;
	this->_noOfAttributes = cExec.RetievedIntData().size() + cExec.RetrievedDoubleData().size() + cExec.RetrievedStringData().size();
	this->_noOfRows = cExec.RowCount();
}

WrapDataSource::~WrapDataSource(void)
{
}



void WrapDataSource::encodeAtrributes(){
	encodeIntAttributes(this->_queryDataInfo.RetievedIntData());
	encodeStringAttributes(this->_queryDataInfo.RetrievedStringData());
}

int WrapDataSource::noOfAttributes(){
	return this->_noOfAttributes;
}

int WrapDataSource::noOfRows(){
	return this->_noOfRows;
}

void WrapDataSource::encodeIntAttributes(vector<PureIntAttInfo*> intAtts){
	int i = 0;
	for (; i< intAtts.size() ; i++)
	{
		PureIntAttInfo* pureIntAtt = intAtts[i];
		EncodedIntAttribute *encodedIntAtt = new EncodedIntAttribute();
		encodedIntAtt->setAttName(pureIntAtt->attName);
		encodedIntAtt->setAttType(ATT_TYPE::SIGNEDINT_VAL);
		encodedIntAtt->setAttID(pureIntAtt->attID);
		
		int upperNum = pureIntAtt->Upper();
		encodedIntAtt->setNoOfVBitStreams((int)ceil(log10((double)upperNum)/log10(2.0)),this->_noOfRows);
		long int *values = pureIntAtt->ValueList();
		encodedIntAtt->setTheSignBitMap(values,this->_noOfRows);
		int j = 0;
		vector<dynamic_bitset<>> convertedBitSet;
		for (; j < this->_noOfRows ; j++)
		{
			dynamic_bitset<> bitSet(encodedIntAtt->NoOfVBitStreams(),(unsigned long)values[j]);
			convertedBitSet.push_back(bitSet);
		}
		encodedIntAtt->setVBitStreamSize(encodedIntAtt->NoOfVBitStreams());
		for(int l = 0 ; l < convertedBitSet.size() ; l++)
		{
			for (int k = (encodedIntAtt->NoOfVBitStreams() - 1) ; k >= 0  ; k--)
			{
				bool val = convertedBitSet[l][k];
				encodedIntAtt->vBitStreams()[encodedIntAtt->NoOfVBitStreams() - k - 1]->setBitStreamAllocAttID(pureIntAtt->attID);
				encodedIntAtt->vBitStreams()[encodedIntAtt->NoOfVBitStreams() - k - 1]->setBitStreamAggregation(encodedIntAtt->attributeName());
				encodedIntAtt->vBitStreams()[encodedIntAtt->NoOfVBitStreams() - k - 1]->setBitValue(l,val);
			}
		}
		this->_codedIntAtts.push_back(encodedIntAtt);
		this->_codedAtts.insert(this->_codedAtts.begin(),encodedIntAtt->attributeID(),encodedIntAtt);
	}
}

void WrapDataSource::encodeStringAttributes(vector<PureStringAttInfo*> stringAtts){
	for (int i = 0 ; i < stringAtts.size() ; i++)
	{
		PureStringAttInfo* stringAtt = stringAtts[i];
		EncodedMultiCatAttribute* multiCatAtt = new EncodedMultiCatAttribute();
		multiCatAtt->setAttID(stringAtt->attID);
		multiCatAtt->setAttName(stringAtt->attName);
		multiCatAtt->setAttType(ATT_TYPE::MULTICAT_VAL);

		multiCatAtt->mapStringDataToCategories(stringAtt->ValueList(),stringAtt->uniqueValueList(),this->_noOfRows);

		for (int j = 0 ; j < multiCatAtt->mappedValList().size() ; j++)
		{
			for (int k = (multiCatAtt->NoOfVBitStreams() - 1) ; k >= 0  ; k--)
			{
				bool val = multiCatAtt->mappedValList()[j][k];
				multiCatAtt->vBitStreams()[multiCatAtt->NoOfVBitStreams() - k - 1]->setBitStreamAllocAttID(stringAtt->attID);
				multiCatAtt->vBitStreams()[multiCatAtt->NoOfVBitStreams() - k - 1]->setBitStreamAggregation(stringAtt->attName);
				multiCatAtt->vBitStreams()[multiCatAtt->NoOfVBitStreams() - k - 1]->setBitValue(j,val);
			}
		}
		this->_codedStringAtts.push_back(multiCatAtt);
		this->_codedAtts.insert(this->_codedAtts.begin(),multiCatAtt->attributeID(),multiCatAtt);
	}
}


vector<EncodedIntAttribute*> WrapDataSource::codedIntAtts(){
	return this->_codedIntAtts;
}

vector<EncodedMultiCatAttribute*> WrapDataSource::codedStringAtts(){
	return this->_codedStringAtts;
}

Tuple* WrapDataSource::DecodeTheTuple(int tupleID){
	Tuple* decodedTuple = new Tuple();

	vector<PureIntAttInfo*> intTuples;
	vector<PureStringAttInfo*> stringTuples;

	for (int i=0 ; i < this->_codedIntAtts.size() ; i++)
	{
		PureIntAttInfo* intAtt = new PureIntAttInfo();
		int val = this->_codedIntAtts[i]->decodeTheTuple(tupleID);
		long int *vals = new long int[1];
		vals[0] = val;
		intAtt->setValueList(vals);
		intAtt->setLower(val);
		intAtt->setUpper(val);
		intAtt->attID = this->_codedIntAtts[i]->attributeID();
		intAtt->attName = this->_codedIntAtts[i]->attributeName();
		intAtt->type =  this->_codedIntAtts[i]->attributeType();

	}

	for (int j=0 ; j < this->_codedStringAtts.size() ; j++)
	{
		PureStringAttInfo* strAtt=new PureStringAttInfo();
		string val = this->_codedStringAtts[j]->decodeTheTuple(tupleID);
		string* vals = new string[1];
		vals[0] = val;
		strAtt->setValueList(vals,1);
		strAtt->attID =  this->_codedStringAtts[j]->attributeID();
		strAtt->attName =  this->_codedStringAtts[j]->attributeName();
		strAtt->type =  this->_codedStringAtts[j]->attributeType();

		stringTuples.push_back(strAtt);
	}

	decodedTuple->setDecodedInts(intTuples);
	decodedTuple->setDecodedStrings(stringTuples);

	return decodedTuple;
}

vector<EncodedAttributeInfo*> WrapDataSource::codedAttributes(){
	return this->_codedAtts;
}

EncodedAttributeInfo* WrapDataSource::operator ()(const int attID){
	return this->_codedAtts[attID];
}

VBitStream* WrapDataSource::operator ()(const int attID, const int BitStreamID){
	return (*(this->_codedAtts[attID]))(BitStreamID);
}