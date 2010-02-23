#pragma once
#include <vector>
#include "VBitStream.h"
#include "AttributeType.h"

using namespace std;

class EncodedIntAttribute
{
public:
	__declspec(dllexport) EncodedIntAttribute(void);
	__declspec(dllexport) ~EncodedIntAttribute(void);
	__declspec(dllexport) vector<VBitStream> vBitStreams();
	__declspec(dllexport) vector<bool> SignBitMap();
	__declspec(dllexport) char* attributeName();
	__declspec(dllexport) int NoOfVBitStreams();
	__declspec(dllexport) ATT_TYPE attributeType();
	__declspec(dllexport) void setVBitStreams(vector<VBitStream> vBitset);
	__declspec(dllexport) void setSignBitMap(vector<bool> sBitMap);
	__declspec(dllexport) void setNoOfVBitStreams(int novBitsets);
	__declspec(dllexport) void setAttType(ATT_TYPE type);
	__declspec(dllexport) void setAttName(char* name);
	__declspec(dllexport) VBitStream bitStreamAt(int bitStreamID);
	__declspec(dllexport) int decodeTheTuple(int tupleID);
	__declspec(dllexport) int getTheSignOf(int tupleID);

private:
	vector<VBitStream> _vBitStreamSet;
	vector<bool> _signBitMap; //sign bit map 0 for + and 1 for -.
	ATT_TYPE _attType;
	char* _attName;
	int _noOfVBitStreams;
};
