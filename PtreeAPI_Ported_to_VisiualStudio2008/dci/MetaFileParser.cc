// 
// File:   MetaFileParser.cc
// Author: serazi
//
// Created on October 21, 2003, 5:25 PM
//

#include "MetaFileParser.h"
#include "Xpdms.h"
#include "XMLException.h"
#include "boost\tokenizer.hpp"

//
// Constructor
///
MetaFileParser::MetaFileParser(const string& file) throw(XMLException) 
    :data_root_(""),is_data_root_set(false){
  doc.setFileName(file.c_str());
  //cout<<"parsing XPdms..."<<endl;
  doc.parse();
  //cout<<"parsing XPdms done"<<endl;
  attributes_=doc.getAttributeInfo();
  cardinality_=doc.getCardinality();
  delimiter_=doc.getDelimiter();
  cat_delimiter_=doc.getCatDelimiter();
  meta_file = file;
  description_=doc.getDescription();
  //get all file name
  vector<AttributeInfo*>::iterator it = attributes_.begin();                      
  string file_name = "";
    for(int i=0;it!=attributes_.end();++it,i++){
            string current_file_name =getAttributeFile((*it)->name());
            //cout<<i<<"th attributes file name: "<< current_file_name<<endl;
            if (current_file_name!=file_name)
            {
                    files_.push_back(data_root_ + current_file_name);
                    file_name=current_file_name;
            }	
    }  
}
//
// Destructor
//
MetaFileParser::~MetaFileParser()
{
	//cout<<"~MetaFileParser()"<<endl;
}

/**
 * set the data root directory iff data files are not in the current dir and in meta
 * file only file names are given not file path.
 */
void MetaFileParser::setDataRoot(const string& data_root){
    is_data_root_set = true;
    data_root_ = data_root;   
}

size_t MetaFileParser::getCardinality() const {  
  return cardinality_;
}

const char* MetaFileParser::getDelimiter() const {
  return delimiter_;
}



const char* MetaFileParser::getCatDelimiter() const {
  return cat_delimiter_;
}

string MetaFileParser::getDescription() const {
  return description_;
}

vector<AttributeInfo*> MetaFileParser::getAttributeInfo() const {
  return attributes_;
}

PTreeInfo MetaFileParser::getPTreeInfo(){
	//cout<<"getPTreeInfo() has been called.."<<endl;
	PTreeInfo pi;
	vector<AttributeInfo*>::iterator it = attributes_.begin();
	//vector<AttributeInfo*> attributes_
	for(; it!=attributes_.end(); ++it){
		//cout<<"Type in getPTreeInfo() "<<(*it)->type()<<endl;
		if((*it)->type() != Type::SKIP) pi.addAttributeInfo(*it);
	}
	pi.setCardinality(cardinality_);  
	return pi;
}

string MetaFileParser::getAttributeFile(const std::string& att_name) const {
  return data_root_ + doc.getAttributeFile(att_name);
}

string MetaFileParser::getMetaFileName()const{
	return /*data_root_ + */meta_file;
}

string MetaFileParser::getFileExtension()const{
	string ext = "unknown";
        if(files_.size() > 0){
            boost::tokenizer<> tok(files_[0]);
            for(boost::tokenizer<>::iterator i = tok.begin(); i!=tok.end(); ++i){
		ext = *i;
            }
        }
	return ext;
}
