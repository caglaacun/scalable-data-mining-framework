// 
// File:   RelationalDataFeeder.h
// Author: serazi
//
// Created on October 22, 2003, 11:31 AM
//

#ifndef _RelationalDataFeeder_H
#define _RelationalDataFeeder_H

#include <fstream>
#include "DataFeeder.h"
#include "MetaFileParser.h"
#include "Item.h"
#include "PException.h"
#include "string_token_iterator.h"


using namespace std;
/**
 * RelationaDataFeeder is used to feed the orginal flat file data into the Ptree set.  
 * This implements the required methods in DatFeeder. Needs an XML to descirbe the format of
 * Data to be read.  
 * Can be used to feed delimited (comma,space,tab, etc) flat tables
 * eg. 
 * a,p,x,12,14 
 * b,r,y,15,11
 * ..........
 */
class RelationalDataFeeder : public DataFeeder{    
	public:
	    /**
	     * Constructer with the apropriate MetaFileParser instansiated with the set of files
	     * required to be read. 
	     */
		RelationalDataFeeder(const MetaFileParser& parser);
                
        /**
         * Constructer with the apropriate MetaFile indicating set of data files
         * required to be read and the properties of attributes.  
         */
		RelationalDataFeeder(const string& metafile)throw(XMLException);
                
		~RelationalDataFeeder();
                
        /**
         * Used to tuple/line/record from the data read from the input file 
         * An Exception will be thrown if you provide when invalid request is made 
         * while calling this method 
         * use try{...} catch( e){...} blocks.
         */
		virtual Tuple getTuple() throw (OutOfRangeException, InvalidLengthException);
                
        /**
         * Will return the PTreeInfo the current PtreeSet. This will incldue all the 
         * inforamation about the attributes.
         */
		virtual PTreeInfo getPTreeInfo();
                
        /**
         * Will return the cardinality/number of records of the input set of files. 
         * Can be used to figure out the number of times getTuple() needs to be called 
         */
		virtual std::size_t getCardinality() const;
                
        /**
         * Will return the name of the meta file used by the Feeder 
         * 
         */         
		virtual std::string getMetaFileName()const;
	protected: 

	private:
		Tuple convert(const vector<string>& str_items) 
			throw (FormatException, InvalidLengthException);
		vector<AttributeInfo*> attInfo;
		fstream *instream_;
		vector<string> files;
		string meta_file;
		vector<int> att_counts;
		size_t tuple_counter;
		size_t cardinality_;
		const char* delimiter_;
	    const char* cat_delimiter_;
		//MetaFileParser parser_;
};

#endif	/* _RelationalDataFeeder_H */

