// 
// File:   TiffDataFeeder.cc
// Author: serazi
//
// Created on December 1, 2003, 6:50 PM
//

#include "TiffDataFeeder.h"

#include <cstring>
#include <cstdlib>  

//
// Constructor
//
TiffDataFeeder::TiffDataFeeder(const MetaFileParser& parser)                        
	:tuple_counter(0)  
{
	try{
		init(parser);
	}
	catch(const exception& e){
		throw;
	}	
}

TiffDataFeeder::TiffDataFeeder(const string& metafile)
	throw(XMLException){
	try {
		MetaFileParser parser(metafile);
		init(parser);
	}
	catch(const XMLException& e){
		throw;
	}
}
//
// Destructor
//
TiffDataFeeder::~TiffDataFeeder()  
{
	for(int i=0; i<files.size(); i++){
		instream_[i].close();  
	}
	delete []instream_;
}

/**
 * construct a tuple and return it, sequentially
 */  
Tuple TiffDataFeeder::getTuple() 
	throw (OutOfRangeException, InvalidLengthException)
{	
	//cout<<tuple_counter<<" "<<cardinality_<<endl;  
	if(tuple_counter >= cardinality_) 
		throw OutOfRangeException("Asking for a tuple beyond the end at: " + tuple_counter );
	vector<unsigned> items;
	unsigned char abyte;
	for(size_t i=0; i<files.size(); i++){
		for(size_t j=0; j<att_counts[i]; j++){
			instream_[i].read(reinterpret_cast<char*>(&abyte), sizeof(unsigned char));
			items.push_back(static_cast<unsigned>(abyte));	
		}	
	}
	tuple_counter++;
	try{  
		return convert(items);
	}
	catch(InvalidLengthException e){
		throw;
	}			
}

/**
 * create the ptree info set return it
 */ 
PTreeInfo TiffDataFeeder::getPTreeInfo(){
	PTreeInfo pi;
	vector<AttributeInfo*>::iterator it = attInfo.begin();
	for(;it!=attInfo.end();++it){
		if((*it)->type() != Type::SKIP) pi.addAttributeInfo(*it);
	}
	pi.setCardinality(cardinality_);  
	return pi;
}

/**
 * Return total number of tuple/record in the dataset
 */  
std::size_t TiffDataFeeder::getCardinality() const{        
	return cardinality_;
}

/**
 * convert a vector of unsigned int into a tuple as describe 
 * in the info
 */  
Tuple TiffDataFeeder::convert(const vector<unsigned>& items)
	 throw (FormatException, InvalidLengthException){	
	if(items.size() != attInfo.size())
		throw  InvalidLengthException("Number of item mismatch...,");           	
	vector<AttributeInfo*>::iterator it = attInfo.begin(); 
	size_t i = 0;
	Tuple tp;         
	for(; it!=attInfo.end();++it, i++){
		AttributeInfo* att = *it;
		int t =  att->type();
		switch(t){
			case Type::UNSIGNED_INT:
				tp.add(new UsignIntItem(items[i]));
				break;
			case Type::SKIP:
				break;
		}
	}
	return tp;	 
}

/**
 * Metafile name used by the feeder
 */ 
std::string TiffDataFeeder::getMetaFileName()const{
	return meta_file;
}

/**
 * Initialize the feeder
 */  
void TiffDataFeeder::init(const MetaFileParser& parser)
	throw (IOException, FormatException)
{
	attInfo = parser.getAttributeInfo();
	cardinality_ = parser.getCardinality();
	meta_file = parser.getMetaFileName();
	vector<AttributeInfo*>::iterator it = attInfo.begin();                      
	string file_name = "";
	int atrib_cnt = 0;
	
	for(;it!=attInfo.end();++it){
		string current_file_name =parser.getAttributeFile((*it)->name());
		if (current_file_name!=file_name)
		{
			files.push_back(current_file_name);
			//cout<<current_file_name<<endl;
			file_name=current_file_name;
			if ( it!=attInfo.begin() ) att_counts.push_back(atrib_cnt); 
			atrib_cnt=0;
		}
		atrib_cnt++;	
	}
	att_counts.push_back(atrib_cnt);
	instream_ = new fstream[files.size()];
	for(int i=0; i<files.size(); i++){
		//cout<<"Opening file: "<<files[i]<<endl;
		instream_[i].open(files[i].c_str(), ios::in);  
		if(!instream_[i])
			throw IOException("Error: "+ files[i]+" does not exist"); 
	}
	//cout<<"Opening files done"<<endl;
	//read the header for each files
	for(size_t i=0; i<files.size(); i++){
		//cout<<"Now looping for seek"<<endl;
		if(!is_little_endian(instream_[i]))
			throw FormatException("Error: "+files[i]+" is not in little endian byte order");
		if(!is_tiff(instream_[i]))
			throw FormatException("Error: "+files[i]+" is not a TIFF file");
		//read 4 bytes get IFD offset
		int ifd_offset;
		instream_[i].read(reinterpret_cast<char*>(&ifd_offset), 4);
		//cout<<"ifd_offset: "<<ifd_offset<<endl;
		//move the file pointer by ifd_iffset from begining
		instream_[i].seekg(ifd_offset, ios::beg);
		//read 2 bytes for number of entries in IFD
		short int num_entries;
		instream_[i].read(reinterpret_cast<char*>(&num_entries), 2);
		//cout<<"num_entries: "<<num_entries<<endl;
		short int tag, type, word; 
		int count, offset, strip_offset, width, length,
			strip_byte_count_offset, num_strip;
		vector<short int> bits_per_sample;
		int num_attributes = 0;
		for(size_t j=0; j<num_entries; j++){
			instream_[i].read(reinterpret_cast<char*>(&tag), 2);
			instream_[i].read(reinterpret_cast<char*>(&type), 2);
			instream_[i].read(reinterpret_cast<char*>(&count), 4);
			instream_[i].read(reinterpret_cast<char*>(&offset), 4);
			
			if(tag==256){
				width = offset;
			}
			else if(tag==257){
				length = offset;
			}
			else if(tag==258){
        		if(count<=1){
            		bits_per_sample.push_back(offset);
            	}
            	else{
            		int location = instream_[i].tellg();
                	instream_[i].seekg(offset, ios::beg);                                                                                                                                             
                  	for(int k=0; k<count; k++){
                    	instream_[i].read(reinterpret_cast<char *>(&word), 2);
                    	bits_per_sample.push_back(word);
                  	}
                  	instream_[i].seekg(location, ios::beg);
               	}
        	}
        	else if(tag==273){
        		strip_offset = offset;
            	num_strip = count;
        	}
                                                                                                                                                             
        	else if(tag==277){
              num_attributes += offset;
              //bands.push_back( numberOfBands);
        	}
                                                                                                                                                             
        	if(tag==279) strip_byte_count_offset = offset;
		}
        if(cardinality_ > width*length)
			throw FormatException(files[i]+" does not have");	
		instream_[i].seekg(strip_offset, ios::beg);	
	}	
}

/**
 * test for little endian
 */  
bool TiffDataFeeder::is_little_endian(fstream& f){
	short int aword;
	f.read(reinterpret_cast<char*>(&aword), 2);
	return (aword==0x4949)?true:false;
}

/**
 * test for tiff file 
 */
bool TiffDataFeeder::is_tiff(fstream& f){
	short int aword;
	f.read(reinterpret_cast<char*>(&aword), 2);
	// 42 if its a tiff file
	return (aword==0x2A)?true:false;
}
