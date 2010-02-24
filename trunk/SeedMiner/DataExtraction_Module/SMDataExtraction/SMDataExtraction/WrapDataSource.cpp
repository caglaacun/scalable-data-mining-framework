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
				encodedIntAtt->vBitStreams()[encodedIntAtt->NoOfVBitStreams() - k - 1]->setBitStreamAggregation(encodedIntAtt->attributeName());
				encodedIntAtt->vBitStreams()[encodedIntAtt->NoOfVBitStreams() - k - 1]->setBitValue(l,val);// = convertedBitSet[l][k];
			}
		}
		this->_codedIntAtts.push_back(encodedIntAtt);
	}
}

vector<EncodedIntAttribute*> WrapDataSource::codedIntAtts(){
	return this->_codedIntAtts;
}