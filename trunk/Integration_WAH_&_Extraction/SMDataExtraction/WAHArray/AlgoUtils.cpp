#include "StdAfx.h"
#include "AlgoUtils.h"
namespace Algorithm{
	AlgoUtils::AlgoUtils(void)
	{
	}

	AlgoUtils::~AlgoUtils(void)
	{
	}

	BitStreamInfo * AlgoUtils::FindPattern(dynamic_bitset<> & _pattern,vector<BitStreamInfo *> & _container)
	{
		// Replace This with an exception
		size_t pattern_size = _pattern.size();
		assert(pattern_size > 0 && pattern_size == _container.size());
		BitStreamInfo * left_op = NULL;
		BitStreamInfo * right_op = NULL;	
		size_t index = 0;
		if (_pattern[index] == 0)
		{
			left_op = ~(*(_container.at(index)));
		}else{
			left_op = _container.at(index);
		}

		for (index = 1; index < pattern_size;index++)
		{
			if (_pattern[index] == 0)
			{
				right_op = ~(*(_container.at(index)));
			}else{
				right_op = _container.at(index);
			}

			left_op = (*(left_op) & *(right_op));
			right_op = NULL;
		}
		return left_op;
	}

	dynamic_bitset<> AlgoUtils::FindPattern(dynamic_bitset<> & _pattern,vector<dynamic_bitset<>> & _container)
	{
		size_t pattern_size = _pattern.size();
		assert(pattern_size > 0 && pattern_size == _container.size());
		dynamic_bitset<> left_op;
		dynamic_bitset<> right_op;
		size_t index = 0;
		left_op = _container.at(index);
		if (_pattern[index] == 0)
		{
			left_op.flip();
		}
		for (index =1; index < pattern_size ; index++)
		{
			right_op = _container[index];
			if (_pattern[index] == 0)
			{
				right_op.flip();
			}
			left_op = (left_op & right_op);
		}

		return left_op;
	}
}