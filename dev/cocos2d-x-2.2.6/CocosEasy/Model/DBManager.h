#ifndef __CE_DB_MANAGER_H__
#define __CE_DB_MANAGER_H__

#include "Common/CocosEasyDef.h"

struct sqlite3;

typedef  std::map<std::string, std::string> TSqlRow;
typedef  std::vector< TSqlRow >  TSqlTable;

class CDBManager : public CCObject
{
public:
	~CDBManager();
	static CDBManager* getInstance();
	void end();

public:
	bool openDB(const char* dbname,const char* mode = "rw");
	void closeDB(const char* dbname);
	bool exeSql(const char* dbname, const char* sql,TSqlTable* table = NULL);

protected:
	CDBManager();
	sqlite3* find(const char* dbname);
	
protected:
	std::map<std::string,sqlite3*> m_sqliteList;

private:
	static CDBManager* m_Instance;
};





#endif