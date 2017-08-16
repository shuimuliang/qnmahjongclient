#include "DBManager.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
CDBManager* CDBManager::m_Instance = NULL;
CDBManager* CDBManager::getInstance(){
	if(!m_Instance)
	{
		m_Instance = new CDBManager();
	}
	return m_Instance;
}

CDBManager::~CDBManager(void){}
CDBManager::CDBManager() {}
void CDBManager::end(){};

	bool CDBManager::openDB(const char* dbname,const char* mode ){return true;}
	void CDBManager::closeDB(const char* dbname){}

	bool CDBManager::exeSql( const char* dbname, const char* sql , TSqlTable* table)
	{
		return false;
	}
#else
#include "Utility/DB/sqlite3.h"
#include "Utility/DB/CppSQLite3.h"

#define DB_PASSWORD "x,%c**$y(&k#@j-"

CDBManager* CDBManager::m_Instance = NULL;

CDBManager* CDBManager::getInstance()
{
	if(!m_Instance)
	{
		m_Instance = new CDBManager();
	}
	return m_Instance;
}

CDBManager::CDBManager() 
{
	sqlite3* pdb = NULL;
	m_sqliteList["aaa"] = pdb;
}

CDBManager::~CDBManager()
{

}

/************************************************/
//for android
//ref:http://blog.csdn.net/xujiayin/article/details/9221851
bool isFileExist(const char* pFileName)
{
    if(!pFileName) return false;
    
    std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath();
    filePath+=pFileName;
    FILE *pFp = fopen(filePath.c_str(),"r");
    CCLog(" android db path:  %s  "  ,filePath.c_str());
    if(pFp)
    {
        fclose(pFp);
        return true;
    }
    return false;
}
void copyData(const char* pFileName)
{
    std::string strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pFileName);
    unsigned long len=0;
    unsigned char* data =NULL;
    data = CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(),"r",&len);
    
    
    std::string destPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    destPath+= pFileName;
    
    
    FILE *pFp=fopen(destPath.c_str(),"w+");
    fwrite(data,sizeof(char),len,pFp);
    fclose(pFp);
    delete []data;
    data=NULL;
}
/************************************************/


void CDBManager::end()
{
	std::map<std::string,sqlite3*>::iterator it = m_sqliteList.begin();
	for (it; it!=m_sqliteList.end(); ++it)
	{
		sqlite3* pdb = it->second; 
		sqlite3_close(pdb);
	}
	m_sqliteList.clear();
	CC_SAFE_DELETE(m_Instance);
	m_Instance = NULL;
}

bool CDBManager::openDB( const char* dbname , const char* mode)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //if(isFileExist(dbname)==false)	// for temp
    {
        copyData(dbname);
    }
#endif
	if(!dbname) return false;
	sqlite3* pdb = find(dbname);
	if(pdb) return true; 
	
	int flag = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
	std::string smode = mode;
	size_t rpos = smode.find("r");
	size_t wpos = smode.find("w");
	if (rpos != std::string::npos) flag = SQLITE_OPEN_READONLY;
	if (wpos != std::string::npos) flag = SQLITE_OPEN_READWRITE;

	std::string name = dbname;
	std::string absname = dbname;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // android need to copy db to writeable path
    absname = CCFileUtils::sharedFileUtils()->getWritablePath();
    absname += dbname;
#else
    absname = CCFileUtils::sharedFileUtils()->fullPathForFilename(name.c_str());
#endif

	if(name == absname && flag == SQLITE_OPEN_READONLY)
	{
		CCLog("db file %s can not find",dbname);
		return true;
	}

	//int err = sqlite3_open_v2(absname.c_str(),&pdb,flag,NULL);
	int err = sqlite3_open(absname.c_str(), &pdb);
	if(err != SQLITE_OK)
	{
        CCLog("sqlite3_open_v2 error code : %d",err);
		return false;
	}
#if COCOS2D_DEBUG > 0
    //do nothing
#else
	//sqlite3_key(pdb, DB_PASSWORD, strlen(DB_PASSWORD));
#endif // RELEASE

	m_sqliteList[dbname] = pdb;
	return true;
}



void CDBManager::closeDB( const char* dbname )
{
	if(!dbname) return;
	std::map<std::string,sqlite3*>::iterator it = m_sqliteList.find(dbname);
	if(it == m_sqliteList.end()) return;
	sqlite3* pdb = it->second; 
	sqlite3_close(pdb);
	m_sqliteList.erase(it);
}

int loadRecord( void * para, int n_column, char ** column_value, char ** column_name )  
{ 
	TSqlTable* vect = (TSqlTable*)para; 
	if(!vect) return 0;
	std::map<std::string,std::string> hmap ;  
	for (int i =0;i<n_column;i++)  
	{  
		hmap[column_name[i]] = column_value[i] ? column_value[i] : "";
	}  
	vect->push_back(hmap);  
	return 0;   
}  

bool CDBManager::exeSql( const char* dbname, const char* sql , TSqlTable* table)
{
	if(!openDB(dbname)) return false;
	sqlite3* pdb = find(dbname); assert(pdb);

	char * errMsg;
	int err = sqlite3_exec( pdb, sql ,loadRecord, table, &errMsg );    
	if(err != SQLITE_OK) 
	{
		CCLog("[CDBManager::exeSql] error code : %d , sql : %s",err,sql);
		return false;
	}
	else
		return true;

	/*
	if(!dbname || !sql ) astReturn(false);
	
	std::string absname = dbname;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // android need to copy db to writeable path
	absname = CCFileUtils::sharedFileUtils()->getWritablePath();
	absname += (std::string)dbname;
#else
	absname = CCFileUtils::sharedFileUtils()->fullPathForFilename(dbname);
#endif

	CppSQLite3DB loDB;
	CppSQLite3Query loQry;
	std::vector<std::string> keyCache;
	try
	{
		loDB.open(absname.c_str());
		if (!loDB.isOpen()) return false;
		loQry = loDB.execQuery(sql);
		if(!table) return true;
		if(loQry.eof()) return true;
		
		// save key cache
		unsigned int nCol = loQry.numFields();
		keyCache.reserve(nCol);
		for(unsigned int i=0; i<nCol; ++i) {keyCache.push_back(loQry.fieldName(i));}

		// save data
		unsigned int row = 0;
		while (!loQry.eof())
		{
			TSqlRow tRow;
			for(unsigned int i=0; i<nCol; ++i)
			{
				const char* val = loQry.fieldValue(i);
				tRow[keyCache[i]] = val ? val : "";
			}
			table->push_back(tRow);
			loQry.nextRow();
			++row;
		}
	}
	catch (CppSQLite3Exception* e)
	{
		const char* lpErr = e->errorMessage();
		printf(lpErr);
		return false;
	}*/
}

sqlite3* CDBManager::find( const char* dbname )
{
	if(!dbname) return NULL;
	std::map<std::string,sqlite3*>::iterator it = m_sqliteList.find(dbname);
	if(it == m_sqliteList.end()) return NULL;
	return it->second;
}


#endif