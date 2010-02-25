// 
// File:   SkipAttributeInfo.h
// Author: serazi
//
// Created on October 22, 2003, 2:18 PM
//

#ifndef _SkipAttributeInfo_H
#define	_SkipAttributeInfo_H
#include "AttributeInfo.h"
#include "Type.h"

class SkipAttributeInfo : public AttributeInfo{
public:
	SkipAttributeInfo(); 
	~SkipAttributeInfo();
	virtual int type()const;
	virtual boost::dynamic_bitset<> encode(Item* val) 
            throw (OutOfDomainException, InvalidTypeException);
    virtual Item* decode(const boost::dynamic_bitset<>& bits) 
            throw (InvalidLengthException);
protected:

private:

};

#endif	/* _SkipAttributeInfo_H */

