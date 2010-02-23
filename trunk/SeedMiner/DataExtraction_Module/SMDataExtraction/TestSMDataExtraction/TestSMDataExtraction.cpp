// TestSMDataExtraction.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DBConnection.h"
#include "DBQueryExecution.h"
#include "WrapDataSource.h"
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
	const char* statement="select TOP(50) StartDate from DimProduct";
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

		cout<<"Testing Unique Data Received"<<endl;

		//Represents the unique string data.
		vector<string> uniqueData = retrievedStringData[i]->uniqueValueList();
		for (j=0 ; j<uniqueData.size() ; j++)
		{
			cout<<uniqueData[j]<<endl;
		}
	}

	WrapDataSource ds(cExec);
	
	return 0;

}

