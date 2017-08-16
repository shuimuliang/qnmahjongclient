#include "Log.h"
#include "Utils.h"
#include <string>
#include <sstream>
#include <stdio.h>


NS_CC_EASY_BEGIN

CELog * CELog::ince = NULL;
const string	DIR = "celog";
const string	SDCARD_PATH = "/mnt/sdcard/";
const string	REG = ".log";
const int		MAX_LINE = 1000;
const string	LOG_TAG_STRS[] = {"DEBUG","INFO","WARN","ERROR","FATAL"};
#define MAX_LEN         (cocos2d::kMaxLogLen + 1)
const int		MAX_CACHE_CHARS = 0;
CELog * CELog::getInstance()
{
	if(!ince)
	{
		ince = new CELog();
	}
	return ince;
}

void CELog::end()
{
	CC_SAFE_RELEASE_NULL(ince);
}

void CELog::write(int eLog , const std::string &log)
{
	
	if(eLog < m_logLevel) return;
	string tempLog = "[" +  getCurTime() + " " + getLogType(eLog) + "] " + log;
	CCLOG("%s",tempLog.c_str());
	if(!m_curFile.is_open()){
		return;
	} 
	pthread_mutex_lock(&m_mutex_receive_queue);
	if(m_lineNum>MAX_LINE){
		m_curFile.close();
		m_curFile.open(m_fullPath.c_str(),ios::out);
		m_lineNum = 0;
	}
	m_cacheStr<<m_lineNum<<" "<<tempLog<<endl;
	++m_lineNum;
	if(m_cacheStr.str().size()>m_cacheMaxChars){
		m_curFile<<m_cacheStr.str();
		m_curFile.flush();
		m_cacheStr.clear();
		m_cacheStr.str("");
	}
	pthread_mutex_unlock(&m_mutex_receive_queue);
}

int CELog::getLineNum()
{
	return m_lineNum;
}

CELog::CELog()
:m_lineNum(0)
,m_logLevel(eLogError)
,m_curFile()
,m_cacheMaxChars(MAX_CACHE_CHARS)
{
	pthread_mutex_init(&m_mutex_receive_queue,NULL);
}

CELog::~CELog()
{
	pthread_mutex_destroy(&m_mutex_receive_queue);
	closeFile();
	m_cacheStr<<"hello "<<endl;
}

bool CELog::createFile(string tag)
{
	//find or create dir failed ,than quit
	if(!checkAndCreateDir())return false;
	//close current file
	closeFile();
	//create new file
	m_fullPath = factoryDownloadPath(DIR);
	m_fullPath += "/";
	m_fullPath += tag;
	m_fullPath += REG;
	m_curFile.open(m_fullPath.c_str(),ios::out);
	writeTitleInfo();
	m_cacheStr.clear();
	m_cacheStr.str("");
	m_lineNum = 0;
	return true;
}

void CELog::closeFile()
{
	m_curFile<<m_cacheStr.str();
	m_curFile.flush();
	m_curFile.close();
}

bool CELog::checkAndCreateDir()
{
	string finalDir = factoryDownloadPath(DIR);
	Engine::utils::mkdirs(finalDir.c_str());
	return true;
}

std::string CELog::factoryDownloadPath(std::string fileName)
{
	std::string path = SDCARD_PATH;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	if( !CCFileUtils::sharedFileUtils()->isFileExist( SDCARD_PATH ) )
	{
		path = CCFileUtils::sharedFileUtils()->getWritablePath();
	}

#else
	path = CCFileUtils::sharedFileUtils()->getWritablePath();
#endif
	std::string DIR_ROOT = "";
	path += DIR_ROOT + fileName;
	
	return path;
}

std::string CELog::getCurTime()
{
	struct tm *tm;  
	time_t timep;  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep);  
#else  
	struct cc_timeval now;   
	CCTime::gettimeofdayCocos2d(&now, NULL);   
	timep = now.tv_sec;  
#endif  

	tm = localtime(&timep);  
	int hour=tm->tm_hour;  
	int min=tm->tm_min;  
	int second=tm->tm_sec; 
	int year = tm->tm_year + 1900;  
	int month = tm->tm_mon + 1;  
	int day = tm->tm_mday;  
	
	stringstream format;
	format<<"d:"<<day<<"["<<hour<<":"<<min<<":"<<second<<"]";
	return format.str();
	
}

void CELog::writeTitleInfo()
{
	struct tm *tm;  
	time_t timep;  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	time(&timep);  
#else  
	struct cc_timeval now;   
	CCTime::gettimeofdayCocos2d(&now, NULL);   
	timep = now.tv_sec;  
#endif  

	tm = localtime(&timep);  
	int year = tm->tm_year + 1900;  
	int month = tm->tm_mon + 1;  
	int day = tm->tm_mday;  
	int hour=tm->tm_hour;  
	int min=tm->tm_min;  
	int second=tm->tm_sec;   
	m_curFile<<"Date:"<<year<<"y"<<month<<"m"<<day<<"d"<<endl;
	m_curFile<<"Time:"<<hour<<"h"<<min<<"m"<<second<<"s"<<endl;
	m_curFile<<endl;
	m_curFile.flush();
}

string CELog::getLogType(int logType)
{
	string result = "unkown";
	if(logType>=eLogDebug&&logType<eLogMax){
		result = LOG_TAG_STRS[logType];
	}
	return result;
}

void CELog::ceLog(int level , const char * pszFormat, ...)
{
	va_list ap;
	va_start(ap, pszFormat);
	char buf[MAX_LEN] = {0};
	vsnprintf(buf, MAX_LEN, pszFormat, ap);
	va_end(ap);
	write(level,buf);
}

void CELog::ceLog(const char* fileName , int lineNum ,const char* funcName , int level , const char *pszFormat,...)
{
	//error
	va_list ap;
	va_start(ap, pszFormat);
	char buf[MAX_LEN] = {0};
	vsnprintf(buf, MAX_LEN, pszFormat, ap);
	va_end(ap);

	//file name
	string strFileName = fileName;
	int index = strFileName.find_last_of('\\');
	if(index>0){
			strFileName = strFileName.substr(index+1);
	}

	//function name
	string strFuncName = funcName;
	int funcIndex = strFuncName.find_last_of(":");
	if(funcIndex>0){
		strFuncName = strFuncName.substr(funcIndex + 1);
	}
	
	//output [%s:%d %s]
	stringstream ss;
	ss<<"["<< strFileName << ":" <<lineNum <<" "<< strFuncName<<"]:" << buf;
	
	write(level,ss.str());
}

void CELog::setLogLevel(int level)
{
	E_LOG_T eLog = eLogDebug;
	switch (level)
	{
	case 1:
		eLog =  eLogInfo;
		break;
	case 2:
		eLog = eLogWarn;
		break;
	case 3:
		eLog = eLogError;
		break;
	case 4:
		eLog = eLogFatal;
		break;
	}
	setLogLevel(eLog);
}

NS_CC_EASY_END