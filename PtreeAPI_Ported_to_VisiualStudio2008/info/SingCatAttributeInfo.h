// 
// File:   SingCatAttributeInfo.h
// Author: Masum H. Serazi
//
// Created on October 13, 2003, 12:42 PM
//

#ifndef _SINGCATATTRIBUTE_INFO_H_
#define _SINGCATATTRIBUTE_INFO_H_
#include "CatAttributeInfo.h"
/**
 * A single value from the domain can be picked for an Item
 */
class SingCatAttributeInfo : public CatAttributeInfo{
	public:
		SingCatAttributeInfo();
		SingCatAttributeInfo(const string& name, const vector<string>& doms);
		~SingCatAttributeInfo();
		 inline virtual int type() const ; 
                 
        /**
        * Get the Binary encoded representation of the given Item. 
        * This is the encoding used when creating the Ptrees 
        */                         
        virtual boost::dynamic_bitset<> encode(Item* val) 
            throw (OutOfDomainException, InvalidTypeException);
        
         /**
        * Get the original vlaue represented by the given binary encoding. 
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
