#include "WrapDataSource.h"
#include "DataSources.h"
#include "EncodedAttributeInfo.h"
#include "BitStreamInfo.h"
#include "WAHStructure.h"
#include <vector>
//#include "EncodedAttributeInfo.h"
//#include "BitStreamInfo.h"
#pragma once
using namespace CompressedStructure;

class CompressionHandler
{
public:
	enum vertical_bit_type{WAH_COMPRESSION, VERTICAL_STREAM_FORMAT};
	void ConvertTo(WrapDataSource * _pdata_source, vertical_bit_type _end_type);
	CompressionHandler(void);
	~CompressionHandler(void);

private :	
	void ConvertAttributeTo(EncodedAttributeInfo * _attribute,vertical_bit_type _end_type);
	void ConvertBitStreamTo(BitStreamInfo * _bit_stream,vertical_bit_type _end_type);
};
