#pragma once

#include "stdhead.h"
#include "PureIntAttInfo.h"
#include "PureDoubleAttInfo.h"
#include "PureStringAttInfo.h"
#include <vector>

/************************************************************************/
/*    Class  :DBQueryExecution.h
/*	  Started:19.02.2010 21:28:12
/*    Updated:19.02.2010 21:38:12
/*    Author :SEEDMiner
/*    Subj   :DataBase Query Execution class.Query is to be executed and 
			  bind it to a appropriate Object and return
/*    Version: 
/************************************************************************/

namespace DBQueryExecutionInfo{
	class DBQueryExecution{
		public:
			__declspec(dllexport) DBQueryExecution(const char* statement);
			__declspec(dllexport) DBQueryExecution();
			__declspec(dllexport) ~DBQueryExecution();
			__declspec(dllexport) bool ExecuteQueryAndBindData(CGOdbcConnect cCon);
			__declspec(dllexport) CGOdbcStmt* DBStatementPtr();
			__declspec(dllexport) std::vector<PureIntAttInfo*> RetievedIntData();
			__declspec(dllexport) std::vector<PureDoubleAttInfo*> RetrievedDoubleData();
			__declspec(dllexport) std::vector<PureStringAttInfo*> RetrievedStringData();
			__declspec(dllexport) int RowCount();


		private:
			const char *_query_stmt;
			CGOdbcStmt *_stmtPtr;
			std::vector<PureIntAttInfo*> _intData;
			std::vector<PureDoubleAttInfo*> _doubleData;
			std::vector<PureStringAttInfo*> _stringData;
			int _rowCount;
	};
}