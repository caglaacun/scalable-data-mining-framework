#pragma once
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "Math.h"
#include <vector>

using namespace std;
using namespace  boost;
namespace CompressedStructure{
	class WAHStructure
	{

	public:

		WAHStructure(void);	
		~WAHStructure(void);	
		void CompressWords(boost::dynamic_bitset<>& _bit_map);
		void PrintCompressedStream();
		void SetCompressedStream(vector<unsigned long int> &_compressed_vector);			
		void printCompressedStream();
		vector<unsigned long int>& GetCompressedVector();
		dynamic_bitset<> Decompress();				
		unsigned long int GetOriginalStreamLength();
		int getSpaceUtilisation();
		// Overloaded operators

		WAHStructure * operator~();
		WAHStructure * operator & (WAHStructure&);
		WAHStructure * operator |(WAHStructure & _structure);
		//WAHStructure * WAHStructure::operator | (WAHStructure&);		
		

		//Public Getters and setters
		const unsigned long int ActiveWordSize() const { return m_iActiveWordSize; }
		void ActiveWordSize(unsigned long int val) { m_iActiveWordSize = val; }
		unsigned long int ActiveWord() const { return m_ulActiveWord; }
		void ActiveWord(unsigned long int val) { m_ulActiveWord = val; }
		int OriginalStreamSize() const { return m_iOriginalStreamSize; }
		void OriginalStreamSize(unsigned long int val) { m_iOriginalStreamSize = val; }
		unsigned long long Count();
		

		static const unsigned long int ZERO_LITERAL = 0;
		static const unsigned long int ONE_LITERAL = 2147483647;
		static const unsigned long int ONE_GAP_START_FLAG = 3221225472;
		static const unsigned long int ZERO_GAP_START_FLAG = 2147483648;
		static const int LITERAL_WORD = 0;
		static const int ZERO_GAP_WORD = 1;
		static const int ONE_GAP_WORD = 2;
		enum operation_type {AND,OR};



	private:

		enum longer_operand{ONE_GAP,ZERO_GAP};
		void BuildArray(boost::dynamic_bitset<>& _bitmap);
		dynamic_bitset<> ConvertVectorToBitmap(vector<unsigned long int> &_decompressed_vector);
		vector<unsigned long int> ExpandCompressedVector(vector<unsigned long int>& _compressed_vector);
		vector<int> AlignRightWithLeft(int _left_op_end_pos,int _right_op_index,int _right_op_end_pos,vector<unsigned long int> &_right_operand,vector<unsigned long int> &_result_vector,longer_operand _longer,operation_type _type,bool _gap_state);	
		vector<int> AlignLeftWithRight(int _right_op_end_pos,int _left_op_index,int _left_op_end_pos,vector<unsigned long int> &_left_operand,vector<unsigned long int> &_result_vector, longer_operand _longer,operation_type _type ,bool _gap_extension_state);	
		void SetOneCount(unsigned long int _count,vector<unsigned long int> & _compressed);
		void SetZeroCount(unsigned long int _count,vector<unsigned long int> & _compressed);
		void SetLiteral(unsigned long int _literal_val,vector<unsigned long int> & _compressed);
		void CopyIntegerToBitMap(dynamic_bitset<> &_bitmap,size_t _index);
		int GetStartBitValue(size_t _word);	
		void SetValueOnCompressedWord(unsigned long int _word, vector<unsigned long int> &_compressed_result);			
		unsigned long int GetLiteralCount(unsigned long int &_bit_literal);
	

		vector<unsigned long int> m_compressed_stream;	
		int * m_iMainArray;
		unsigned long int m_iOriginalStreamSize;			
		unsigned long int m_ulActiveWord;	
		unsigned long int m_iActiveWordSize;	
		static const int WORD_SIZE = 32;		

	};
}