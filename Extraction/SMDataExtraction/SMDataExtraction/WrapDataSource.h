#pragma once
#include "DBQueryExecution.h"
#include "EncodedIntAttribute.h"
#include "EncodedMultiCatAttribute.h"
#include "DecodedTuple.h"
#include <vector>

using namespace DBQueryExecutionInfo;

class WrapDataSource
{
public:
	__declspec(dllexport) WrapDataSource(DBQueryExecution cExec);
	__declspec(dllexport) ~WrapDataSource(void);
	__declspec(dllexport) int noOfRows();
	__declspec(dllexport) int noOfAttributes();
	__declspec(dllexport) void encodeAtrributes();
	__declspec(dllexport) vector<EncodedIntAttribute*> codedIntAtts();
	__declspec(dllexport) vector<EncodedMultiCatAttribute*> codedStringAtts();
	__declspec(dllexport) Tuple* DecodeTheTuple(int tupleID);
	void encodeIntAttributes(vector<PureIntAttInfo*> intAtts);
	void encodeStringAttributes(vector<PureStringAttInfo*> stringAtts);



private:
	DBQueryExecution _queryDataInfo;
	vector<EncodedIntAttribute*> _codedIntAtts;
	vector<EncodedMultiCatAttribute*> _codedStringAtts;
	int _noOfRows;
	int _noOfAttributes;

};
