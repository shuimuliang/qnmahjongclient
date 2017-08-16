#ifndef __CE_LOG_H__
#define __CE_LOG_H__

#include "Common/CocosEasyDef.h"
#include <fstream>
#include <queue>

using namespace std;
NS_CC_EASY_BEGIN

enum E_LOG_T{
	eLogDebug = 0,
	eLogInfo = 1,
	eLogWarn = 2,
	eLogError = 3,
	eLogFatal = 4,
	eLogMax = 5
};

class CELog:public CCObject
{
public:
	//get the only one CELog, returning null means that creating logFile error.
	static CELog *getInstance();
	//free resource
	void end();
	//createFile
	bool createFile(string tag);
	//write log to file
	void write(int eLog , const std::string &log);
	//get info
	int getLineNum();
	void ceLog(int level , const char * pszFormat, ...);
	void ceLog(const char* fileName , int lineNum ,const char* funcName , int level , const char *pszFormat,...);
	void setLogLevel(E_LOG_T level){m_logLevel = level;}
	void setLogLevel(int level);
	void setCacheMaxChars(int cacheMax){m_cacheMaxChars = cacheMax;}
	~CELog();
private:
	int					m_lineNum;
	string				m_fullPath;
	std::ofstream		m_curFile;
	pthread_mutex_t		m_mutex_receive_queue;
	stringstream        m_cacheStr;
	E_LOG_T				m_logLevel;
	unsigned int		m_cacheMaxChars;
	static CELog		*ince;
	CELog();
	//init
	bool checkAndCreateDir();
	void closeFile();
	string factoryDownloadPath(string fileName );
	//transfer
	std::string getCurTime();
	void writeTitleInfo();
	string getLogType(int tagType);
};

NS_CC_EASY_END

#define CELOG(level, format, ...)	cocoseasy::CELog::getInstance()->ceLog(__FILE__  , __LINE__ , __FUNCTION__ , level , format, ##__VA_ARGS__)
#define CEDebug(format, ...)		cocoseasy::CELog::getInstance()->ceLog(__FILE__  , __LINE__ , __FUNCTION__ ,cocoseasy::eLogDebug , format, ##__VA_ARGS__)
#define CEInfo(format, ...)			cocoseasy::CELog::getInstance()->ceLog(__FILE__  , __LINE__ , __FUNCTION__ ,cocoseasy::eLogInfo , format, ##__VA_ARGS__)
#define CEWarn(format, ...)			cocoseasy::CELog::getInstance()->ceLog(__FILE__  , __LINE__ , __FUNCTION__ , cocoseasy::eLogWarn , format, ##__VA_ARGS__)
#define CEFatal(format, ...)		cocoseasy::CELog::getInstance()->ceLog(__FILE__  , __LINE__ , __FUNCTION__ , cocoseasy::eLogFatal , format, ##__VA_ARGS__)
#define CEError(format, ...)		cocoseasy::CELog::getInstance()->ceLog(__FILE__  , __LINE__ , __FUNCTION__ , cocoseasy::eLogError , format, ##__VA_ARGS__)


#endif //__CE_LOG_H__