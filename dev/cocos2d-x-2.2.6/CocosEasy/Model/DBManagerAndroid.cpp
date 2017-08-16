#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "DBManager.h"
#include "jni.h"
#include "jni/JniHelper.h"

#define DATA_MANAGER_PACKAGE = "org/cocos2dx/lib/Cocos2dxLocalStorage", 

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

}

CDBManager::~CDBManager()
{

}

void CDBManager::end()
{
	CC_SAFE_DELETE(m_Instance);
	m_Instance = NULL;
}

void splitFilename (std::string& str)
{
	size_t found = 0;
	found=str.find_last_of("/\\");
	if (found != std::string::npos)
	{
		str = str.substr(found+1);
	}
}

bool CDBManager::openDB( const char* dbname , const char* mode)
{
	if (!dbname) return false;
	JniMethodInfo t;
	if (! JniHelper::getStaticMethodInfo(t, 
		DATA_MANAGER_PACKAGE, 
		"openDB", 
		"()V")) 
	{
		printf("[CDBManager::openDB] getStaticMethodInfo failed");
		return false;
	}
	std::string strDBFilename = fullpath;
	std::string name = dbname;
	splitFilename(name);
	jstring jdbName = t.env->NewStringUTF(name.c_str());
	jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, jdbName);
	t.env->DeleteLocalRef(jdbName);
	t.env->DeleteLocalRef(t.classID);
	return true;
}



void CDBManager::closeDB( const char* dbname )
{
	if (!dbname) return;
	JniMethodInfo t;
	if (!JniHelper::getStaticMethodInfo(t, 
		DATA_MANAGER_PACKAGE, 
		"closeDB", 
		"()V"))
	{
		printf("[CDBManager::closeDB] getStaticMethodInfo failed");
		return;
	}
	std::string name = dbname;
	splitFilename(name);
	jstring jdbName = t.env->NewStringUTF(name.c_str());
	t.env->CallStaticVoidMethod(t.classID, t.methodID, jdbName);
	t.env->DeleteLocalRef(t.classID); 
	t.env->DeleteLocalRef(jdbName); 
}



bool CDBManager::exeSql( const char* dbname, const char* sql , TSqlTable* table)
{
	char * errMsg;
	JniMethodInfo t;
	CCString* pStr = NULL;
	if (! JniHelper::getStaticMethodInfo(t, 
		DATA_MANAGER_PACKAGE, 
		"exeSql", 
		"(Ljava/lang/String;)Ljava/lang/String;"))
	{
		printf("[CDBManager::exeSql] getStaticMethodInfo failed");
		return;
	}
	return true;
}

sqlite3* CDBManager::find( const char* dbname )
{
	return NULL;
}




#endif