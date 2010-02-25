#include <cmath>
#include "AttributeInfo.h"
#include "Type.h"

//////////////////////////////////////////////////////////////////////
AttributeInfo::AttributeInfo(){
	setName("unknown");  
}
AttributeInfo::AttributeInfo(const string& name):name_(name)
{}
 
AttributeInfo::~AttributeInfo(){

}
void AttributeInfo::setName(const string& name){ 
    this->name_ = name;
}
int AttributeInfo::binaryLength()const{
    return binary_length_;
}

// name of the attrbute
string AttributeInfo::name()const{
return name_;
}

void AttributeInfo::setBinaryLength(int length){
    binary_length_ = length;
}
