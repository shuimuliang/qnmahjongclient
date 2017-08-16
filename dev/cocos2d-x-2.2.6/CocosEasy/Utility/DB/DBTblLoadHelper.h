//////////////////////////////////////////////////////////////////////////
// filename	:DBTblLoadHelper.h
// des		:db helper class
//////////////////////////////////////////////////////////////////////////

#ifndef __DBTBL_LOAD_HELPER__
#define __DBTBL_LOAD_HELPER__

#include "CppSQLite3.h"
#include "DBRSWrapper.h"
#include <string>

class CDBTlLoadHelper
{
public:
	CDBTlLoadHelper(){}
	~CDBTlLoadHelper(){}

public:
	template < class DataType, class DataMgr>
	bool loadTblData(const char* apszDBName, const char* apszTblName, DataMgr* apDataMgr)
	{
		bool lbRet = true;
		DataType* lpData = NULL;

		CppSQLite3DB loDB;
		CppSQLite3Query loQry;
		try
		{
			loDB.open(apszDBName, SQLITE_OPEN_READONLY);

			if (!loDB.isOpen())
			{
				return false;
			}

			char lszSql[256] = { 0 };
			sprintf_s(lszSql, "select * from %s", apszTblName);

			loQry = loDB.execQuery(lszSql);

			while (!loQry.eof())
			{
				lpData = new DataType();
				lpData->LoadFromDB(CDBRSWrapper(&loQry));
				apDataMgr->OnDataLoaded(lpData);
				loQry.nextRow();
			}
		}
		catch (CppSQLite3Exception* e)
		{
			lbRet = false;
			const char* lpErr = e->errorMessage();

		}
		return lbRet;
	}
protected:
private:
};

#endif __DBTBL_LOAD_HELPER__