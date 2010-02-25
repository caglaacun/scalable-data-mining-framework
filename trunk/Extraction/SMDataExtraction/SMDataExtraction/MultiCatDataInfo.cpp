#include "StdAfx.h"
#include "MultiCatDataInfo.h"
#include <math.h>

MultiCatDataInfo::MultiCatDataInfo(vector<string> uniqueDataList)
{
	this->_uniqueStrings = uniqueDataList;
}

int* MultiCatDataInfo::getAssignedEncodedNumberList(){
	int uniqueRows = this->_uniqueStrings.size();
	this->_noOfBitsNeeded = (int) ceil(log10((double)uniqueRows / 2.0));
	int noOfAssignedInts = (int) pow(2.0 , (double)this->_noOfBitsNeeded);
	this->_assignedEncodedNumbers = new int[noOfAssignedInts];

	int temp = 0;
	for (; temp<noOfAssignedInts ; temp++)
	{
		this->_assignedEncodedNumbers[0] = temp;
	}
	return this->_assignedEncodedNumbers;
}

MultiCatDataInfo::~MultiCatDataInfo(void)
{
}
