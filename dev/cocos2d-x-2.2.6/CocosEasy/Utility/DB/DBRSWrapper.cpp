//////////////////////////////////////////////////////////////////////////
// filename	:DBRSWrapper.cpp
// des		:db helper class
//////////////////////////////////////////////////////////////////////////
#include "DBRSWrapper.h"

CDBRSWrapper::CDBRSWrapper(CppSQLite3Query* apDBRS) : m_pDBRS(apDBRS)
{
	m_dedstr[0] = 0;
}

CDBRSWrapper::~CDBRSWrapper()
{

}

int CDBRSWrapper::GetIntField(const char* apszField)
{
	try
	{
		return m_pDBRS->getIntField(apszField, 0);
	}
	catch (CppSQLite3Exception* e)
	{
		const char* lpErr = e->errorMessage();
		return 0;
	}
}

int CDBRSWrapper::GetIntField(int aiIdx)
{
	try
	{
		return m_pDBRS->getIntField(aiIdx, 0);
	}
	catch (CppSQLite3Exception* e)
	{
		const char* lpErr = e->errorMessage();
		return 0;
	}
}

float CDBRSWrapper::GetFloatField(const char* apszField)
{
	try
	{
		return static_cast<float>(m_pDBRS->getFloatField(apszField));
	}
	catch (CppSQLite3Exception* e)
	{
		const char* lpErr = e->errorMessage();
		return 0.00f;
	}
}

const char* CDBRSWrapper::GetStringField(const char* apszField)
{
	try
	{
		return m_pDBRS->getStringField(apszField, "");
	}
	catch (CppSQLite3Exception* e)
	{
		const char* lpErr = e->errorMessage();
		return m_dedstr;
	}
}
