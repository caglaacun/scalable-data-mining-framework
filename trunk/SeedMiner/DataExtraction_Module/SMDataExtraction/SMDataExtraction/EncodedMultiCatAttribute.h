#pragma once
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include "VBitStream.h"
#include "AttributeType.h"
#include "../Include/boost/dynamic_bitset.hpp";

using namespace boost;
using namespace std;

class EncodedMultiCatAttribute
{
public:
	EncodedMultiCatAttribute(void);
	~EncodedMultiCatAttribute(void);

	__declspec(dllexport) vector<VBitStream*> vBitStreams();
	__declspec(dllexport) vector<dynamic_bitset<>> mappedValList();
	__declspec(dllexport) string attributeName();
	__declspec(dllexport) int attributeID();
	__declspec(dllexport) int NoOfVBitStreams();
	__declspec(dllexport) ATT_TYPE attributeType();
	__declspec(dllexport) int* getMappedIntVals();
	__declspec(dllexport) int noOfUniqueValues();
	__declspec(dllexport) void setNoOfVBitStreams(int novBitsets,int rows);
	__declspec(dllexport) void setAttType(ATT_TYPE type);
	__declspec(dllexport) void setAttName(string name);
	__declspec(dllexport) void setAttID(int attID);
	__declspec(dllexport) VBitStream* bitStreamAt(int bitStreamID);
	void mapStringDataToCategories(string* _valueList,vector<string> _uniqueValList,int noOfRows);

private:
	int* _mappedIntVals;
	vector<dynamic_bitset<>> _mappedValList;
	vector<VBitStream*> _vBitStreamSet;
	ATT_TYPE _attType;
	string _attName;
	int _attID;
	int _noOfVBitStreams;
	int _noOfUniqueVals;
};
