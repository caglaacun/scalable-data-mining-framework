#include "StdAfx.h"
#include "CompressionHandler.h"

CompressionHandler::CompressionHandler(void)
{
}

CompressionHandler::~CompressionHandler(void)
{
}
void CompressionHandler::ConvertTo( WrapDataSource * _pdata_source, vertical_bit_type _end_type )
{
	vector<EncodedAttributeInfo * > attributes = _pdata_source->codedAttributes();
	vector<EncodedAttributeInfo * >::iterator iter = attributes.begin();
	while(iter != attributes.end())
	{

		ConvertAttributeTo(*(iter),_end_type);
	}
}
void CompressionHandler::ConvertAttributeTo( EncodedAttributeInfo * _attribute,vertical_bit_type _end_type )
{
	vector<BitStreamInfo *> bit_streams = _attribute->vBitStreams();
	vector<BitStreamInfo *>::iterator iter = bit_streams.begin();
	while(iter != bit_streams.end())
	{
		ConvertBitStreamTo(*(iter),_end_type);
			iter++;
	}
}
void CompressionHandler::ConvertBitStreamTo( BitStreamInfo * _bit_stream,vertical_bit_type _end_type )
{
	dynamic_bitset<> bit_stream = _bit_stream->Decompress();	
	switch(_end_type)
	{
	case WAH_COMPRESSION:
		{
			_bit_stream = new WAHStructure();
			break;
		}
	case VERTICAL_STREAM_FORMAT:
		{
			_bit_stream = new VBitStream();
			break;
		}

	default:{
		break;
			}
	}
	_bit_stream->CompressWords(bit_stream);
}
