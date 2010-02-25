#include <iostream>
#include <cstdlib>

#include "PTreeSet.h"
#include "Exception.h"
#include "MultiCatAttributeInfo.h"
using namespace std;

/**
Attempt to create Ptree DB for KDD02 data
*/

int main(int argc, char **argv){

	try{    
		//Set up some initial variable
		PTreeSet ps;   
		string ptreeSetID = "YEAST";  
		string dataFile="yeast_data/yeast_genes_meta.xml";
		string dataRoot="yeast_data/";

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
			mParser.setDataRoot(dataRoot);
			cout<<"metafile:"<<endl;

			RelationalDataFeeder *rFeeder=new RelationalDataFeeder(mParser);
			cout<<"FEEDER :"<<endl;

			ps.feed(rFeeder);

			cout<<"FEED:"<<endl;

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
		/*
		cout<<ps.getTuple(1)<<endl;
		cout<<endl;

		cout<<"======100 Sample transactions============= "<<endl;
		for(int i=0;i<100;i++){
		cout<<ps.getTuple(i)<<"["<<i<<"]"<<endl; 
		}
		cout<<"=================== "<<endl;
		*/

		//Do some Sample Ptree operations
		//Create a BasicPt
		//check for basic counts
		//BasicPt that can be used to easily manipulate the ptrees 
		BasicPt p(pi);

		//c1 is a temporary pattern created from p(0,0) Attribute 0 Ptree(bit) 0
		BasicPt c1=p(0,3);
		//.....
		BasicPt c2=p(1,2);
		BasicPt c3=p(1,79);

		//To obtain the count for each pt count(pt)
		//Following counts may not produce any usefull usefull conts 
		//Only for ilustration puposes

		cout<<"p 0,3 = "<<ps.count(c1)<<endl;  //Or use ps.count(p(0,0));
		cout<<"p 1,2 = "<<ps.count(c2)<<endl;
		cout<<"p 2,79 = "<<ps.count(c3)<<endl;   

		/*
		AttributeInfo *ai = &pi[0];
		MultiCatAttributeInfo *mci= dynamic_cast<MultiCatAttributeInfo*>(ai); 
		vector<string> dm=mci->getDomain();
		*/


		for(int a=0;a<pi.degree();a++) 
		{
			vector<string> dm= (dynamic_cast<CatAttributeInfo*>(&pi[a]))->getDomain();

			//for(int i=0;i<dm.size();i++)
			//cout<<"idx :"<<i<<":"<<dm[i]<<endl; 
			cout<<"=================================="<<endl;
			cout<<"Attribute name :"<<pi[a].name()<<"\t Sample Root Counts of 10 Ptrees"<<endl; 
			for(int i=0;((i<pi[a].binaryLength())&&(i<10));i++)
			{
				cout<<"P"<<i<<":rc:"<<ps.count(p(a,i))<<"\t:name:"<<dm[i]<<endl;
			}
		}
	}

	catch(const exception& e){
		cout<<e.what()<<endl;
	}
	catch(...){
		cout<<"Don't  know what went wrong!!"<<endl;
	}
	return (EXIT_SUCCESS);
}
