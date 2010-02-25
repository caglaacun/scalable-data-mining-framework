#include <iostream>
#include <cstdlib>

#include "PTreeSet.h"
#include "Exception.h"
using namespace std;

/**
This is a sample file that can be used as a practical example to figure
out the usage of the PtreeAPI2

This example has almost all trypes of data

This program will create Ptrees if not exising 
Try to do some sample Ptree opertions to find frequent items

*/



int main(int argc, char **argv){

	try{    
		//Set up some initial variable
		PTreeSet ps;
		string ptreeSetID = "EX-1";
		string dataFile="example_meta.xml";

		PTreeInfo pi;

		////////////////////////////////////////
		//
		//           Generate Ptrees 
		//
		////////////////////////////////////////
		//check to see if Data set exisits        
		//If you need to stripe the data again delete the data set

		DIR *dir = opendir(ptreeSetID.c_str());
		if (!(dir)){
			MetaFileParser mParser(dataFile);
			RelationalDataFeeder *rFeeder=new RelationalDataFeeder(mParser);
			ps.feed(rFeeder);
			ps.store(ptreeSetID);
			//Remove your garbage after use
			delete rFeeder;
		}
		else
			ps.load(ptreeSetID);

		////////////////////////////////////////
		//
		//           Ptree Operations 
		//
		///////////////////////////////////////

		//Check to see if the data set is ready for operations
		pi = ps.getPTreeInfo();
		cout<<"cardinality:num of rows    = "<<pi.cardinality();cout<<endl;
		cout<<"degree     :num of columns = "<<pi.degree();cout<<endl;

		//Get the names of the attribute in the data set  
		for(int i=0;i<pi.degree();i++){
			cout<<"binary length:"<<pi[i].binaryLength()
				<<"  Attribute ["<<i<<"] "<<pi[i].name()
				<<endl;	
		}
		//Check some records using index
		cout<<ps.getTuple(1)<<endl;
		cout<<endl;
		cout<<"======All transactions============= "<<endl;
		for(int i=0;i<pi.cardinality();i++){
			cout<<ps.getTuple(i)<<"["<<i<<"]"<<endl; 
		}
		cout<<"=================== "<<endl;

		//Do some Sample Ptree operations
		//Create a BasicPt
		//check for basic counts
		//BasicPt that can be used to easily manipulate the ptrees 
		BasicPt p(pi);

		//c1 is a temporary pattern created from p(0,0) Attribute 0 Ptree(bit) 0
		//in this example all attribute have only 1 bit (binary)
		BasicPt c1=p(0,0);
		//.....
		BasicPt c2=p(1,0);
		BasicPt c3=p(2,0);

		//To obtain the count for each pt count(pt)
		//Following counts may not produce any usefull usefull conts 
		//Only for ilustration puposes

		cout<<"p 0,0 = "<<ps.count(c1)<<endl;  //Or use ps.count(p(0,0));
		cout<<"p 1,0 = "<<ps.count(c2)<<endl;
		cout<<"p 2,0 = "<<ps.count(c3)<<endl; 

		//To obtain the count for a combination of basic pts 
		cout<<" 0,0 AND 1,0 OR 2,0 ="<<ps.count(c1&c2|c3)<<endl;
		cout<<" 0,0 AND 1,0 OR 2,0 ="<<ps.count(p(0,0)&p(1,0)|p(2,0))<<endl;
		//Using a temporary pt
		BasicPt myPt=p(0,0)&p(1,0)|p(2,0);
		cout<<" 0,0 AND 1,0 OR 2,0 ="<<ps.count(myPt)<<endl;
		cout<<endl;
		cout<<"=================== "<<endl;


		//Say for eg. you need to find all tuple with G B valuse > 191   
		// 192 in binary 11000000  
		// ignore last 6 bits and look for pattern 11 
		// Need to find first two bits 0 and 1 note bits are numbered n..0 atribs 1..n                      
		// Exp =     p(atrib G,bit 7) & p(atrib G,bit 6) &
		//              p(atrib B,bit 6) & p(atrib B,bit 6) &
		//		     

		BasicPt Rgt191 = p(0,7) & p(0,6); 
		cout<<"  R > 191 count ="<<ps.count(Rgt191)<<endl;

		BasicPt Ggt191 = p(1,7) & p(1,6); 
		cout<<"  G > 191 count ="<<ps.count(Ggt191)<<endl;

		BasicPt Bgt191 = p(2,7) & p(2,6); 
		cout<<"  B > 191 count ="<<ps.count(Bgt191)<<endl;

		BasicPt GBgt191 = p(1,7) & p(1,6) & p(2,7) & p(2,6);
		cout<<"  G&B > 191 count ="<<ps.count(GBgt191)<<endl;

		//.......Sky is the limit with pTrees!

	}

	catch(const exception& e){
		cout<<e.what()<<endl;
	}
	catch(...){
		cout<<"Don't  know what went wrong!!"<<endl;
	}
	return (EXIT_SUCCESS);
}
