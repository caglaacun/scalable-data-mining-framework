#ifndef _DATA_FEEDER_H_
#define _DATA_FEEDER_H_
#include "Tuple.h"
#include "PTreeInfo.h"
#include "PException.h"


/**
* DataFeeder is used to feed the orginal (non Ptree) data into the Ptree set for the 
* Purpose of createing ptees. Responsible to read the data apropriately using the availabl
* meta info. DatatFeeder is to be extended/implemented for each type of different data 
* feeders  eg. RelationalDataFeeder, TIFFFeeder, etc.
*/

class DataFeeder
{
public:
	/**
	* Used to tuple/line/record from the data read from the input file 
	* When creating Ptrees this will be called continuously to get a stream
	* input tuples
	* An Exception will be thrown if you provide when invalid request is made 
	* while calling this method 
	* use try{...} catch( e){...} blocks.
	*/
	virtual Tuple getTuple() throw (OutOfRangeException, InvalidLengthException)= 0;

	/**
	* Will return the PTreeInfo the current PtreeSet. This will incldue all the 
	* inforamation about the attributes. 
	*/
	virtual PTreeInfo getPTreeInfo() = 0;

	/**
	* Will return the cardinality/number of records of the input set of files. 
	* Can be used to figure out the number of times getTuple() needs to be called 
	*/
	virtual std::size_t getCardinality() const = 0;

	/**
	* Will return the cardinality/number of records of the input set of files. 
	* Can be used to figure out the number of times getTuple() needs to be called 
	*/
	virtual std::string getMetaFileName()const = 0;
};

#endif
