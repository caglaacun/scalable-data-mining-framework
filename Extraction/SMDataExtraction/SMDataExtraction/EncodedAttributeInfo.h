#pragma once

#include "VBitStream.h"
#include "AttributeType.h"

#include <vector>

using namespace std;

class EncodedAttributeInfo{
public:
	__declspec(dllexport) EncodedAttributeInfo();
	__declspec(dllexport) ~EncodedAttributeInfo();
	__declspec(dllexport) vector<VBitStream*> vBitStreams();
	__declspec(dllexport) string attributeName();
	__declspec(dllexport) int attributeID();
	__declspec(dllexport) int NoOfVBitStreams();
	__declspec(dllexport) ATT_TYPE attributeType();
	__declspec(dllexport) void setVBitStreams(vector<VBitStream*> vBitset);
	__declspec(dllexport) void setNoOfVBitStreams(int novBitsets,int rows);
	__declspec(dllexport) void setAttType(ATT_TYPE type);
	__declspec(dllexport) void setAttName(string name);
	__declspec(dllexport) void setAttID(int attID);
	__declspec(dllexport) void setVBitStreamSize(int newSize);
	__declspec(dllexport) VBitStream* bitStreamAt(int bitStreamID);
	__declspec(dllexport) VBitStream* operator()(const int bitStreamID);
	
	template<typename T>
	__declspec(dllexport) T decodeTheTuple(int tupleID) = 0;


private:
	vector<VBitStream*> _vBitStreams;
	ATT_TYPE _attType;
	int _attID;
	string _attName;
	int _noOfVBitStreams;
};