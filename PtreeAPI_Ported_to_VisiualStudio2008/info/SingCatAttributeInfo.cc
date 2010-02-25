// 
// File:   SingCatAttributeInfo.cc
// Author: Masum H. Serazi
//
// Created on October 13, 2003, 12:42 PM
//

#include "SingCatAttributeInfo.h"
#include "BinaryMath.h"

SingCatAttributeInfo::SingCatAttributeInfo():CatAttributeInfo(){
	setDomain(vector<string>(0));
}

SingCatAttributeInfo::SingCatAttributeInfo(const string& name, 													const vector<string>& doms){ 
	setName(name);
	setDomain(doms);														
}

SingCatAttributeInfo::~SingCatAttributeInfo(){

}
int SingCatAttributeInfo::type() const {
	return Type::SING_CAT;
}

boost::dynamic_bitset<> SingCatAttributeInfo::encode(Item* itm) 
            throw (OutOfDomainException, InvalidTypeException){
    // cout<<"SingCatAttributeInfo::encode(Item* itm)"<<endl;       	
	int t = itm->type();
	if(type() != t) 
		throw InvalidTypeException("Given item type does not match with att. type");
    string value = (dynamic_cast<SingCatItem*>(itm))->value();
    int index = 0, i=0;
    //cout<<"domain size:"<<domain.size()<<endl;
    for(; i<domain.size(); i++){
        //cout<<"i:"<<domain[i]<<endl;
    	if(domain[i]==value){
    		index = i;
    		break;
    	}
    } 
    //cout<<"Inside SingCatAttributeInfo index: "<<index<<" value:"<<value<<endl;	
    if(i >= domain.size() )
    	throw OutOfDomainException("Given item "+value+" is not in current domain !");
    return boost::dynamic_bitset<>(binaryLength(), static_cast<unsigned long>(index));
}

Item* SingCatAttributeInfo::decode(const boost::dynamic_bitset<>& bits) 
            throw (InvalidLengthException){
	if(bits.size() != binaryLength())
		throw  InvalidLengthException("Binary length exception...");
	int index = bits.to_ulong();
	return new SingCatItem(domain[index]);	           
}
void SingCatAttributeInfo::setDomain(const vector<string>& doms){
	domain=doms;
	setBinaryLength(numbits(domain.size()));
}
