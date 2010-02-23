#include "StdAfx.h"
#include "WrapDataSource.h"
#include "PureIntAttInfo.h"
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include <iostream>

using namespace std;

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
	
}

int WrapDataSource::noOfAttributes(){
	return this->_noOfAttributes;
}

int WrapDataSource::noOfRows(){
	return this->_noOfRows;
}

void WrapDataSource::encodeIntAttributes(vector<PureIntAttInfo*> intAtts){
		
}