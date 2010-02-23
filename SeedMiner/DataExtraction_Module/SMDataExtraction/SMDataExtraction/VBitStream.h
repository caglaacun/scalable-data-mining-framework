#pragma once

#include <bitset>
#include <vector>

using namespace std;

class VBitStream
{
public:
	__declspec(dllexport) VBitStream(int bitCount);
	__declspec(dllexport) ~VBitStream(void);
	__declspec(dllexport) vector<bool> BitStream();
	__declspec(dllexport) int oneCount();
	__declspec(dllexport) int bitStreamPosition();

	
private:
	vector<bool> _dataBitStream;
	int _oneCount;
	int _bitCount;
	int _bitStreamPosition;

};
