// 
// File:   MultiCatAttributeInfo.h
// Author: serazi
//
// Created on October 13, 2003, 12:42 PM
//

#ifndef _MULTICATATTRIBUTE_INFO_H_
#define _MULTICATATTRIBUTE_INFO_H_
#include "CatAttributeInfo.h"
/**
 * Class for Multi valued categorical attributes.
 * Multiple values from the domain can be picked for an Item
 */
class MultiCatAttributeInfo : public CatAttributeInfo{
	public:
		MultiCatAttributeInfo();
		MultiCatAttributeInfo(const string& name, const vector<string>& doms);
		~MultiCatAttributeInfo();
		 inline virtual int type() const ; 
                 
        /**
        * Get the Binary encoded representation of the given Item (value set). 
        * This is the encoding used when creating the Ptrees 
        */ 
        virtual boost::dynamic_bitset<> encode(Item* val) 
            throw (OutOfDomainException, InvalidTypeException);
        
        /**
        * Get the original vlaue set represented by the given binary encoding. 
        * This is the encoding used when creating the Ptrees 
        */
        virtual Item* decode(const boost::dynamic_bitset<>& bits) 
            throw (InvalidLengthException);
        
        /**
        * Set the domain of categorical values 
        * This order will be used for the encoding when creating the Ptrees
        * Each value will be encoded as a Bitvector 
        */ 
        virtual void setDomain(const vector<string>& doms);
};
#endif
