#pragma once
#include <vector>
#include "BitStreamInfo.h"
#include <iostream>
using namespace std;
namespace Algorithm{
	class BitStreamHolder
	{
	public:
		BitStreamHolder(void);
		~BitStreamHolder(void);
	private:
		vector<int> m_attribute_no;
		vector<int> m_bit_stream_no;
		BitStreamInfo * m_bit_stream;
	};


}