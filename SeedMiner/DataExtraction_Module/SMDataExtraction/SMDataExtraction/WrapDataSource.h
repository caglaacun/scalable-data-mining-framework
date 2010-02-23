#pragma once
#include "DBQueryExecution.h"
#include "EncodedIntAttribute.h"
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
	void encodeIntAttributes(vector<PureIntAttInfo*> intAtts);
	


private:
	DBQueryExecution _queryDataInfo;
	vector<EncodedIntAttribute*> _codedIntAtts;
	int _noOfRows;
	int _noOfAttributes;

};
