//////////////////////////////////////////////////////////////////////////
// filename	:DBRSWrapper.h
// des		:db helper class
//////////////////////////////////////////////////////////////////////////

#ifndef __DBRS_WRAPPER_H__
#define __DBRS_WRAPPER_H__

#include "CppSQLite3.h"

class CDBRSWrapper
{
public:
	CDBRSWrapper(CppSQLite3Query* apDBRS);
	~CDBRSWrapper();

public:
	int GetIntField(const char* apszField);
	int GetIntField(int aiIdx);
	float GetFloatField(const char* apszField);
	const char* GetStringField(const char* apszField);

protected:
private:
	CppSQLite3Query* m_pDBRS;
	char m_dedstr[1];
};
#endif __DBRS_WRAPPER_H__