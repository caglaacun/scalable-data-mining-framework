// 
// File:   MultiCatAttributeInfo.cc
// Author: Masum H. Serazi
//
// Created on October 16, 2003, 3:45 PM
//

#include "MultiCatAttributeInfo.h" 

namespace {
   /**
	 * REQUIREMENT: dom.size() == bits.size()
	 * point to note: index of vector and index of bitset are not in same order
	 * vector: 0 1 2 3 4 5 6 
	 * bitset: 6 5 4 3 2 1 0
	 * 
	 * in Same order now
	 * -amal 01/04
	 * i th bit set will be the i th domain value in vector  
	 * 
	 */
	vector<string> decode_imple(const vector<string>& dom,        
			const boost::dynamic_bitset<>& bits)
	{
		//cout<<"in multy cat decode_imple(): "<<endl;
		//copy(dom.begin(), dom.end(), ostream_iterator<string>(cout, ","));
		//cout<<endl; 
		//cout<<"bits: "<<bits<<endl;
		//typedef vector<string>::iterator iterator;
		vector<string> ov;
		int i =0;	
		for(size_t i=0; i<dom.size(); i++){
		         /*
			  * if(bits[bits.size() - 1 - i] == true) {
			  *
		          * Changing above line of code to take off 
		          * the effect of reversing in domain 
		          * -amal 01/04
			  */
			if(bits[i] == true) {
				ov.push_back( dom[i]);
				//++it;
			}	
		}

		//copy(ov.begin(), ov.end(), ostream_iterator<string>(cout, ","));
		//cout<<endl; 
		return ov;
	}
	/**
	 * REQUIREMENT: dom.size() == value.size()
	 */
	boost::dynamic_bitset<> encode_imple(const vector<string>& dom, 
			const vector<string>& value)throw(OutOfDomainException){
		//cout<<"in multy cat encode_imple():"<<endl;
		//copy(dom.begin(), dom.end(), ostream_iterator<string>(cout, ","));
		//cout<<endl; 
		//copy(value.begin(), value.end(), ostream_iterator<string>(cout, ","));
		//cout<<endl;		
	        boost::dynamic_bitset<> bset(dom.size());			
		for(int i=0; i<value.size(); i++){
			for(int j=0; j<dom.size(); j++){
				if(value[i] == dom[j]){			        
				  /*
				   *  bset[bset.size() - j -1] = true;
				   *
				   * Changing above line of code to take off 
				   * the effect of reversing in domain 
				   * -amal 01/04
				   */
          			        bset[j] = true;
					break;
				}
			}
		}

		//cout<<"in multy cat encode_imple(): "<<endl;
		//cout<<"bset: "<<bset<<endl;
		if(value.size() >0 && bset.count()==0)
			throw OutOfDomainException("ERROR: Given value is not in the domain");
		return bset;		
	}
}

MultiCatAttributeInfo::MultiCatAttributeInfo():CatAttributeInfo(){
	setDomain(vector<string>(0));
}

MultiCatAttributeInfo::MultiCatAttributeInfo(const string& name, 
	const vector<string>& doms){ 
	setName(name);
	setDomain(doms);														
}

MultiCatAttributeInfo::~MultiCatAttributeInfo(){ 

}
int MultiCatAttributeInfo::type() const {
	return Type::MULT_CAT;
}

boost::dynamic_bitset<> MultiCatAttributeInfo::encode(Item* itm) 
            throw (OutOfDomainException, InvalidTypeException){
    //cout<<"MultiCatAttributeInfo::encode(Item* itm)"<<endl;
	int t = itm->type();
	if(type() != t) 
		throw InvalidTypeException("Given item type does not match with att. type");
	try{	
    	vector<string> value = (dynamic_cast<MultiCatItem*>(itm))->value();
    	return encode_imple(domain, value);
	}
    catch(const OutOfDomainException& e){
    	throw;
    }
}

Item* MultiCatAttributeInfo::decode(const boost::dynamic_bitset<>& bits) 
            throw (InvalidLengthException){
	if(bits.size() != binaryLength())
		throw  InvalidLengthException("Binary length exception...");
	
	return new MultiCatItem(decode_imple(domain, bits));	           
}
void MultiCatAttributeInfo::setDomain(const vector<string>& doms){              
	domain=doms;
        setBinaryLength(domain.size());
}
