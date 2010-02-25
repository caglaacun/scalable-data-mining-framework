// 
// File:   CatAttributeInfo.h
// Author: Masum H. Serazi
//
// Created on October 13, 2003, 12:42 PM
//

#ifndef _CatAttributeInfo_H
#define	_CatAttributeInfo_H
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <string>
#include "AttributeInfo.h"
#include "PException.h"
#include "Item.h"

/**
 * Class for categorical attributes. 
 */
class CatAttributeInfo : public AttributeInfo{
	public:
    	CatAttributeInfo();
        virtual ~CatAttributeInfo();
        
        //CatAttributeInfo(const vector<string>& doms);
        /**
        * Get type of attribute
        */ 
        inline virtual int type() const = 0;
        
        /**
        * Get the Binary encoded representation of the given Item. 
        * This is the encoding used when creating the Ptrees 
        */ 
        virtual boost::dynamic_bitset<> encode(Item* val) 
            throw (OutOfDomainException, InvalidTypeException) = 0;
        
        /**
        * Get the original vlaue set represented by the given binary encoding. 
        * This is the encoding used when creating the Ptrees 
        */
        virtual Item* decode(const boost::dynamic_bitset<>& bits) 
            throw (InvalidLengthException) = 0;
        
        /**
        * Set the domain of categorical values 
        * This order will be used for the encoding when creating the Ptrees
        * The values will be encoded as a Bitvector 
        */
        virtual void setDomain(const vector<string>& doms) = 0;
        
        /**
        * Get the domain of categorical values 
        * This order was used for the encoding when creating the Ptrees
        * Each domain value was encoded as a Bitvector 
        */
        vector<string> getDomain();
	protected:
        vector<string> domain;
};

#endif	/* _CatAttributeInfo_H */

