#pragma once
#include "BitStreamHolder.h"
#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include <vector>


namespace Algorithm{
class AlgoUtils
{
public:
	AlgoUtils(void);
	~AlgoUtils(void);
	BitStreamHolder * FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamHolder *> & _container);
	BitStreamInfo * FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamInfo *> & _container);
	dynamic_bitset<> FindPattern(dynamic_bitset<> & _pattern,vector<dynamic_bitset<>> & _container);

};
}