#include "StdAfx.h"
#include "TestAlgoUtil.h"



TestAlgoUtil::TestAlgoUtil(void)
{
}

TestAlgoUtil::~TestAlgoUtil(void)
{
}

void TestAlgoUtil::TestPatternFindBitInfoMethod()
{
	DataSourceGenerator dg;
	BitsetGenerator bg;
	dynamic_bitset<> bit_set_1 = bg.getAlternatingBitset(100);
	dynamic_bitset<> bit_set_2 = bg.getPureBitset(100,false);
	dynamic_bitset<> bit_set_3 = bg.getPureBitset(100,true);
	// 	bit_set_1[10] = 1;
	// 	bit_set_1[20] = 1;
	// 	bit_set_1[30] = 1;
	vector<dynamic_bitset<>> _container;
	_container.push_back(bit_set_2);
	_container.push_back(bit_set_1);
_container.push_back(bit_set_3);
	dynamic_bitset<> pattern(3);
	vector<vector<dynamic_bitset<>>> _cont;
	_cont.push_back(_container);
	//pattern[2] = 1;
	WrapDataSource * wrpped = dg.CreateDataSource(_cont,BitStreamInfo::WAH_COMPRESSION);
	vector<EncodedAttributeInfo *> atts = wrpped->codedAttributes();
	vector<BitStreamInfo *> bit_streams = atts.at(0)->vBitStreams();
	AlgoUtils utils;
	PrintBitSets(bit_streams);
	BitStreamInfo * result = utils.FindPattern(pattern,bit_streams);
	cout << "Result : " << endl;
	result->Print();
	cout << "Count : " << result->Count() << endl;
}

void TestAlgoUtil::TestPatternFindBitsetMethods()
{
	BitsetGenerator bg;
	dynamic_bitset<> bit_set_1 = bg.getPureBitset(100,false);
	dynamic_bitset<> bit_set_2 = bit_set_1;
// 	bit_set_1[10] = 1;
// 	bit_set_1[20] = 1;
// 	bit_set_1[30] = 1;
	bit_set_1 = bg.getAlternatingBitset(100);
	vector<dynamic_bitset<>> _container;
	_container.push_back(bit_set_2);
	_container.push_back(bit_set_1);
	dynamic_bitset<> pattern(2);
	PrintBitSets(_container);
	pattern[pattern.size()-1] = 1;	
	pattern[0] = 1;	
	AlgoUtils utils;
	dynamic_bitset<> result = utils.FindPattern(pattern,_container);
	cout << "Result : " << result<<endl;

}

void TestAlgoUtil::PrintBitSets(vector<BitStreamInfo *> & _bit_sets)
{
for (size_t i = 0; i < _bit_sets.size() ; i++)
{
	cout <<"Index " << i << " : " <<endl;
	BitStreamInfo * inf = _bit_sets.at(i);
	dynamic_bitset<> decompressed = inf->Decompress(); 
	if (inf->Type() == BitStreamInfo::WAH_COMPRESSION)
	{
		WAHStructure * compressed = dynamic_cast<WAHStructure *>(inf);
		cout <<"Compressed Stream : " << endl;
		compressed->printCompressedStream();
	}
	cout << decompressed << endl;	
}
}

void TestAlgoUtil::PrintBitSets(vector<dynamic_bitset<>> & _bit_sets)
{
	for (size_t i = 0; i < _bit_sets.size(); i++)
	{
		cout << "Index " << i << " : " << _bit_sets.at(i)<<endl;
	}
}
