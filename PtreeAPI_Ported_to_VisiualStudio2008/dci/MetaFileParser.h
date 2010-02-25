// 
// File:   MetaFileParser.h
// Author: serazi, perera, and vasily
//
// Created on October 21, 2003, 5:25 PM
//

#ifndef _MetaFileParser_H_
#define _MetaFileParser_H_  
 
#include <string>
#include "PTreeInfo.h"
#include "AttributeInfo.h"
#include "Exception.h"
#include "Xpdms.h"


/*
 * Used to parse the meta file for relational data 
 * Will be able to extract the required information from the Feeder
 * Will be responsible to provide the PTreeInfo and the list of 
 * input files from the XML meta file
 */ 

class MetaFileParser {
public:

    /*
     * Constructor with the name of the meta file to be parsed for data feeder
     */        
    MetaFileParser(const std::string& file) throw(XMLException);
       /**
        * Set the delimiter (how the data items are seperated), e.g., ',', '\t', etc.
        * @param d valid character as delimiter
        */
	void setDelimiter(const char* d);
        /**
        * Set the cardinality (number of rows) of the dataset
        * @param c a positive integer
        */
	void setCardinality(size_t c);
        /**
        * Set the description of the dataset
        * @param des string as a description
        */
	void setDescription(const std::string& des);

        /**
         * Will allow you to set the root directory where the data is stored 
         * This is required since the xml file will only specify the file names.
         * And if you want to have the data stored in any other directory this required
         * specify the directory.
         * @param data_root a valid directory name
         */   
        void setDataRoot(const string& data_root);
        /**
        * Get the delimiter of the dataset
        * @return the delimiter
        */
	const char* getDelimiter()const;
        /**
        * Get the delimiter if there is any multi-valued attribute
        * @return the delimiter
        */
	const char* getCatDelimiter()const;
        /**
        * Get number of rows of the dataset
        * @return number of rows
        */
	size_t getCardinality()const;
        /**
        * Get dexcription of the dataset
        * @return the description
        */
	string getDescription()const;
        
        /**
        * Get the extension of the dataset files (.tiff, .tif. .data, etc)
        * @return extension
        */
	string getFileExtension()const;
        
        /**
        * Get attributes info of the dataset
        * @return a vector of AttributeInfo
        * @see AttributeInfo
        */
	vector<AttributeInfo*> getAttributeInfo()const;
        /**
        * Get PTreeInfo of the dataset
        * @return PTreeInfo
        * @see PTreeInfo
        */
	PTreeInfo getPTreeInfo();
       
       /**
        * Given a attribute name this will give the file name of the dataset
        * where corresponding data is given
        * @param att_name a string as an attribute name
        * @return a string as a file name
        */
	string getAttributeFile(const std::string& att_name)const;
        
       /**
        * Get the name of the file (xml file) wher meta info has been given
        * @return a string as meta-file name
        */
	string getMetaFileName()const;
	~MetaFileParser();
protected:

private:
	vector<AttributeInfo*> attributes_;
	vector<std::string> files_;
	std::string meta_file;
	const char* delimiter_;
	const char* cat_delimiter_;
	size_t cardinality_;
	string description_;
	Xpdms doc;
        bool is_data_root_set;
        string data_root_;
};

#endif	/* _MetaFileParser_H */

