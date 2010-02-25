#include <iostream>
#include <cstdlib>

#include "PTreeSet.h"
#include "Exception.h"
using namespace std;

/**
This is a sample file that can be used as a practical example to figure
out the usage of the PtreeAPI2
This program will create Ptrees if not exising 
Try to do some sample Ptree opertions to find frequent items

*/



int main(int argc, char **argv){

	try{    
		//Set up some initial variable
		PTreeSet ps;
		string ptreeSetID = "OPT";
		string dataFile="opTest_meta.xml";

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
		cout<<"attrib 0 = "<<ps.count(c1)<<endl;  //Or use ps.count(p(0,0));
		cout<<"attrib 1 = "<<ps.count(c2)<<endl;
		cout<<"attrib 2 = "<<ps.count(c3)<<endl;

		cout<<"cheack for different types of expressions answer should be 8"<<endl;
		//To obtain the count for a combination of basic pts 
		cout<<"attrib 0 AND 1 OR !2 ="<<ps.count(!c1&!c2|c3)<<endl;
		cout<<"attrib 0 AND 1 OR !2 ="<<ps.count(p(0,0)&p(1,0)|!p(2,0))<<endl;
		//Using a temporary pt
		BasicPt myPt=p(0,0) & p(1,0) | (!p(2,0));
		//myPt.print();

		cout<<"attrib 0 AND 1 OR !2 ="<<ps.count(myPt)<<endl;
		cout<<endl;
		cout<<"=================== "<<endl;


		cout<<"cheack for different large stack answer should be same"<<endl;
		BasicPt myPtL=(p(0,0) & p(1,0) | !p(2,0)) |( p(0,0) & p(1,0)|!p(2,0)); 
		cout<<"attrib (0 AND 1 OR !2) | (0 AND 1 OR !2)   ="<<ps.count(myPtL)<<endl;

		//How to test expression ?
		//try aserting diferent formats of expressions
		//demorgans rule for multiple variables

		BasicPt w=!p(0,0);
		BasicPt x=!p(1,0);
		BasicPt y=!p(2,0);
		BasicPt z=!p(3,0);
		cout<<"=============[1] "<<endl;
		BasicPt LHS=!w & !x & !y & !z;
		BasicPt RHS=!(w|x|y|z);
		assert(ps.count(LHS)==ps.count(RHS));
		cout<<ps.count(LHS)<<" = "<<ps.count(RHS)<<endl;

		cout<<"=============[2]"<<endl;  
		LHS=!w | !x | !y | !z;
		RHS=!(w&x&y&z);
		assert(ps.count(LHS)==ps.count(RHS));
		cout<<ps.count(LHS)<<" = "<<ps.count(RHS)<<endl;

		cout<<"=============[3]"<<endl;  
		LHS= w & (x|y|z);
		RHS= w&x|w&y|w&z;
		assert(ps.count(LHS)==ps.count(RHS));
		cout<<ps.count(LHS)<<" = "<<ps.count(RHS)<<endl;

		cout<<"=============[4]"<<endl;  
		LHS= (w | x) & (y | z);
		RHS= w&y | w&z | x&y | x&z;
		assert(ps.count(LHS)==ps.count(RHS));
		cout<<ps.count(LHS)<<" = "<<ps.count(RHS)<<endl;

		cout<<"=============[5]"<<endl;  
		LHS= (x | y )       |(w & z)     ;
		RHS= (x | (!x & y)) |(w & (!w | z));
		assert(ps.count(LHS)==ps.count(RHS));
		cout<<ps.count(LHS)<<" = "<<ps.count(RHS)<<endl;

		cout<<"=============[6]"<<endl;  
		LHS= (x & ( y | (w & z) ) );
		RHS= x & y | x & w & z ;
		assert(ps.count(LHS)==ps.count(RHS));
		cout<<ps.count(LHS)<<" = "<<ps.count(RHS)<<endl;

		cout<<"=============[fail]"<<endl;  
		LHS= (x);
		RHS= x & y | x & w & z ;
		assert(ps.count(LHS)==ps.count(RHS));
		cout<<ps.count(LHS)<<" = "<<ps.count(RHS)<<endl;

		//.......Sky is the limit for testing possibilities!

	}

	catch(const exception& e){
		cout<<e.what()<<endl;
	}
	catch(...){
		cout<<"Don't  know what went wrong!!"<<endl;
	}
	return (EXIT_SUCCESS);
}
