// 
// File:   TiffDataFeeder.h
// Author: serazi
//
// Created on December 1, 2003, 6:50 PM
//

#ifndef _TiffDataFeeder_H
#define	_TiffDataFeeder_H

#include <fstream>
#include "DataFeeder.h"
#include "MetaFileParser.h"
#include "Item.h"
#include "PException.h"
#include "Exception.h"

class TiffDataFeeder : public DataFeeder{
public:
	public:
    /**
     * Constructer with the apropriate MetaFileParser instansiated with the set of files
     * required to be read. 
     */
	TiffDataFeeder(const MetaFileParser& parser);
                
	/**
	 * Constructer with the apropriate MetaFile indicating set of data files
	 * required to be read and the properties of attributes.  
	 */
	TiffDataFeeder(const string& metafile)throw(XMLException);
                
	~TiffDataFeeder();
                
    /**
     * Used to tuple/line/record from the data read from the input file 
     * An Exception will be thrown if you provide when invalid request is made 
     * while calling this method 
     * use try{...} catch( e){...} blocks.
    
     * @todo need to check reading file for some tiff formats that break the stream 
     * into multiple streams. Eg. unable to read tiff generated from imagemagic   
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
	
	private:
		void init(const MetaFileParser& parser)
			throw (IOException, FormatException);
		Tuple convert(const vector<unsigned>& items) 
			throw (FormatException, InvalidLengthException);
	    bool is_little_endian(fstream& f);
	    bool is_tiff(fstream& f);
		vector<AttributeInfo*> attInfo;
		fstream *instream_;
		vector<string> files;
		string meta_file;
		vector<int> att_counts;
		size_t tuple_counter;
		size_t cardinality_;
};
#endif	/* _TiffDataFeeder_H */

