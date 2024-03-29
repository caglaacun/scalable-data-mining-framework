#include "StdAfx.h"
#include "EWAH.h"
#include <iostream>
using namespace std;

EWAH::EWAH(void)
{
	Type(BitStreamInfo::EWAH_COMPRESSION);
}

EWAH::~EWAH(void)
{
//	m_bitvector.clear();
//	cout << "Inside EWAH destructor " << endl;
	m_bitvector.~WrappedBitVector();
}

EWAH::EWAH(WrappedBitVector & _vect)
{	
//	this(void);
	m_bitvector.copy(_vect);
	m_bitvector.compress();
}

BitStreamInfo * EWAH::operator ~()
{
	/*
	WrappedBitVector * wrapped_vect = ~(m_bitvector);
		EWAH * new_vect = new EWAH(*(wrapped_vect));
		return new_vect;
		*/
	EWAH * new_vect = new EWAH(m_bitvector);
	m_bitvector.Flip();
	return new_vect;
	
}

BitStreamInfo * EWAH::Clone()
{
	EWAH * bit_stream = new EWAH(m_bitvector);
	bit_stream->Type(this->Type());
	BitStreamInfo::Clone(bit_stream);
	return bit_stream;
}

BitStreamInfo * EWAH::operator &(BitStreamInfo & _right_op)
{	
	/*
	EWAH * right_op = dynamic_cast<EWAH *>(&_right_op);
		WrappedBitVector * result_vect = m_bitvector & right_op->m_bitvector;	
		right_op = new EWAH(*(result_vect));
		delete result_vect;
		//result_vect->~WrappedBitVector();
		*/
	EWAH * right_op = dynamic_cast<EWAH *>(&_right_op);
	EWAH * result_vect = new EWAH(m_bitvector);
	result_vect->m_bitvector &= right_op->m_bitvector;		
	return result_vect;
}

BitStreamInfo * EWAH::operator |(BitStreamInfo & _right_op)
{	
	EWAH * right_op = dynamic_cast<EWAH *>(&_right_op);
	EWAH * result_vect = new EWAH(m_bitvector);
	result_vect->m_bitvector |= right_op->m_bitvector;		
	return result_vect;
	
}

unsigned long long EWAH::Count()
{
	return m_bitvector.count();
}

void EWAH::CompressWords(boost::dynamic_bitset<> &bitMap)
{
	WrappedBitVector vect(bitMap);
	m_bitvector.copy(vect);	
	vect.~WrappedBitVector();
	
}

dynamic_bitset<> EWAH::Decompress()
{
	dynamic_bitset<> temp_bitset((int)m_bitvector.size(),0ul);
	m_bitvector.decompress(temp_bitset);
	return temp_bitset;
}

size_t EWAH::SpaceUtilisation()
{
	return sizeof(EWAH) + m_bitvector.SpaceUtilsation();	
}

void EWAH::Print()
{
	m_bitvector.print();
}