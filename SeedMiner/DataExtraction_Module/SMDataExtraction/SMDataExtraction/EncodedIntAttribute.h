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
	__declspec(dllexport) vector<VBitStream*> vBitStreams();
	__declspec(dllexport) vector<bool> SignBitMap();
	__declspec(dllexport) string attributeName();
	__declspec(dllexport) int attributeID();
	__declspec(dllexport) int NoOfVBitStreams();
	__declspec(dllexport) ATT_TYPE attributeType();
	__declspec(dllexport) void setVBitStreams(vector<VBitStream*> vBitset);
	__declspec(dllexport) void setNoOfVBitStreams(int novBitsets,int rows);
	__declspec(dllexport) void setAttType(ATT_TYPE type);
	__declspec(dllexport) void setAttName(string name);
	__declspec(dllexport) void setAttID(int attID);
	__declspec(dllexport) VBitStream* bitStreamAt(int bitStreamID);
	__declspec(dllexport) int decodeTheTuple(int tupleID);
	__declspec(dllexport) int getTheSignOf(int tupleID);
	__declspec(dllexport) void setTheSignBitMap(long int *values,int valSet);
	__declspec(dllexport) void setVBitStreamSize(int size);

private:
	vector<VBitStream*> _vBitStreamSet;
	vector<bool> _signBitMap; //sign bit map 0 for + and 1 for -.
	ATT_TYPE _attType;
	string _attName;
	int _attID;
	int _noOfVBitStreams;
};
