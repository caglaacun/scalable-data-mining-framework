// 
// File:   CatAttributeInfo.cc
// Author: Masum H. serazi
//
// Created on October 16, 2003, 1:05 PM
//

#include "CatAttributeInfo.h"

CatAttributeInfo:: CatAttributeInfo(): AttributeInfo(){ 
 	//setDomain(vector<string>(0));
 	setBinaryLength(0);	
}
 
 CatAttributeInfo:: ~CatAttributeInfo(){ 
 	
 }
 
vector<string> CatAttributeInfo::getDomain(){
	return domain;
} 
