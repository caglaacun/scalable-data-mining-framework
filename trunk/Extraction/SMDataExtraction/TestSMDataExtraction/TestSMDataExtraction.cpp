// TestSMDataExtraction.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DBConnection.h"
#include "DBQueryExecution.h"
#include "WrapDataSource.h"
#include "DecodedTuple.h"
#include <iostream>

using namespace std;
using namespace DBConnectionInfo;
using namespace DBQueryExecutionInfo;

int _tmain(int argc, _TCHAR* argv[])
{
	//Testing the Library for initializing and connecting to the database.
	DBConnection cCon("AdventureWorks","","");
	if (cCon.initiateConnectionToDB())
	{
		cout<<"successfully connected"<<endl;
	}
	
	//Testing the Library for executing query statements and binding data.
	const char* statement="select TOP(20) BusinessType,GeographyKey,ProductLine from DimReseller";
	DBQueryExecution cExec(statement);
	if ((cExec.ExecuteQueryAndBindData(cCon.DBConnectionPtr())))
	{
		cout<<endl<<"Query executed & Fetched successfully"<<endl;
	}
	
	//Testing the retrieved Integer data.
	vector<PureIntAttInfo*> retrievedIntData = cExec.RetievedIntData();
	int i;
	int j;
	for (i=0 ; i<retrievedIntData.size() ; i++)
	{
		for (j=0 ; j<cExec.RowCount() ; j++)
		{
			cout<<retrievedIntData[i]->ValueList()[j]<<endl;
		}	
		cout<<"Max Value of " << retrievedIntData[i]->attName<< " : " << retrievedIntData[i]->Upper()<<endl; 
		cout<<"Min Value of " << retrievedIntData[i]->attName<< " : " << retrievedIntData[i]->Lower()<<endl;

	}
	
	//Testing the retrieved Double data.
	vector<PureDoubleAttInfo*>  retrievedDoubleData = cExec.RetrievedDoubleData();

	for (i=0 ; i<retrievedDoubleData.size() ; i++)
	{
		for (j=0 ; j<cExec.RowCount() ; j++)
		{
			cout<<retrievedDoubleData[i]->ValueList()[j]<<endl;
		}
		cout<<"Max Value of " << retrievedDoubleData[i]->attName <<" : " << retrievedDoubleData[i]->Upper()<<endl;
		cout<<"Min Value of " << retrievedDoubleData[i]->attName <<" : " << retrievedDoubleData[i]->Lower()<<endl;
	}	

	//Testing the retrieved String data.
	vector<PureStringAttInfo*> retrievedStringData = cExec.RetrievedStringData();

	for (i=0 ; i<retrievedStringData.size() ; i++)
	{
		for (j=0 ; j<cExec.RowCount() ; j++)
		{
			cout<<retrievedStringData[i]->ValueList()[j]<<endl;
		}

		cout<<endl<<"Testing Unique Data Received"<<endl;

		//Represents the unique string data.
		vector<string> uniqueData = retrievedStringData[i]->uniqueValueList();
		for (j=0 ; j<uniqueData.size() ; j++)
		{
			cout<<uniqueData[j]<<endl;
		}
	}

	WrapDataSource *ds = new WrapDataSource(cExec);
	ds->encodeAtrributes();
	
	for (int i = 0 ; i < ds->codedIntAtts().size() ; i++)
	{
		EncodedIntAttribute intAtt = *ds->codedIntAtts()[i];
		cout<<intAtt.attributeName()<<endl;
		cout<<intAtt.attributeID()<<endl;
		for (int j = 0; j < intAtt.NoOfVBitStreams() ; j++)
		{
			cout<<"Attribute ID: "<<intAtt.vBitStreams()[j]->bitStreamAllocAttID()<<" ";
			for (int k = 0 ; k < ds->noOfRows() ; k++)
			{
				cout<<intAtt.vBitStreams()[j]->BitStream()[k];
			}
			cout<<endl;
		}

		//Testing the method for retrieving a bit stream on given index.
		//VBitStream* vBitStream = intAtt->bitStreamAt(2);
		
		VBitStream* vBitStream = intAtt(1);
		for (int j=0 ; j<ds->noOfRows() ; j++)
		{
			cout<<vBitStream->BitStream()[j];
		}

		//Decode a particular tuple upon given a tuple ID.
		cout<<endl<<intAtt.decodeTheTuple(20)<<endl;;
	}

	for (int j = 0 ; j < ds->codedStringAtts().size() ; j++)
	{
		EncodedMultiCatAttribute multiCatAtt = *ds->codedStringAtts()[j];
		cout<<endl<<endl<<multiCatAtt.attributeName()<<endl;
		cout<<multiCatAtt.attributeID()<<endl;
		cout<<"No Of Unique Data Values : " <<multiCatAtt.noOfUniqueValues()<<endl;
		
		for (int i = 0 ; i < multiCatAtt.noOfUniqueValues() ; i++)
		{
			cout<< i << " - " << cExec.RetrievedStringData()[j]->uniqueValueList()[i]<<endl;
		}

		for (int k = 0 ; k < multiCatAtt.NoOfVBitStreams() ; k++)
		{
			cout<<"Attribute ID: "<<multiCatAtt.vBitStreams()[k]->bitStreamAllocAttID()<<" ";
			for (int l=0 ; l < ds->noOfRows() ; l++)
			{
				cout<<multiCatAtt.vBitStreams()[k]->BitStream()[l];
			}
			cout<<endl;
		}

		//Testing the method for retrieving a bit stream on given a bit stream index.
		//For convenience the same duty can be achieved by using parenthesis operator
		// eg: multicat(2)
		//VBitStream* vBitStream = multiCatAtt.bitStreamAt(1);
		VBitStream* vBitStream = multiCatAtt(1);
		for (int j=0 ; j<ds->noOfRows() ; j++)
		{
			cout<<vBitStream->BitStream()[j];
		}

		//Decode a particular tuple upon given a tuple ID.
		cout<<endl<<multiCatAtt.DecodeTheTuple(5)<<endl;;
	}

	//Testing the decoding an entire tuple ability.
	int tupID = 3;
	Tuple* tuples = ds->DecodeTheTuple(tupID);
	cout<<"\n Decoding the Tuple ID: " << tupID<<endl;
	for (int i=0 ; i<tuples->decodedInts().size() ; i++)
	{
		cout << "AttributeID : " << tuples->decodedInts()[i]->attID << " - "<<tuples->decodedInts()[i]->ValueList()[0]<<endl;
	}

	for (int j=0 ; j<tuples->decodedStringAtts().size() ; j++)
	{
		cout << "AttributeID : " << tuples->decodedStringAtts()[j]->attID << " - "<<tuples->decodedStringAtts()[j]->ValueList()[0]<<endl;
	}
	
	//Testing the closing connection method.
	if (cCon.closeConnectionWithDB())
	{
		cout<<endl<<"Connection to the database is terminated successfully."<<endl;
	}
	return 0;

}

