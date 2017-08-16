#include "DownloadManager.h"
using namespace std;
CDownloadManager* CDownloadManager::m_Instance = NULL;

CDownloadManager* CDownloadManager::getInstance()
{
	if(!m_Instance)
	{
		m_Instance = new CDownloadManager();
	}
	return m_Instance;
}

CDownloadManager::CDownloadManager()
: m_isRunning(false)
, _curl(NULL)
, _tid(NULL)
, m_timeout(0)
{
	
}

CDownloadManager::~CDownloadManager()
{

}

void CDownloadManager::end()
{

}


bool CDownloadManager::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
	mode_t processMask = umask(0);
	int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
	umask(processMask);
	if (ret != 0 && (errno != EEXIST))
	{
		return false;
	}

	return true;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	wstringstream wss;
	wss<<path;
	const wchar_t* wdir;
	wdir = wss.str().c_str();
	BOOL ret = CreateDirectoryW(wdir, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}

	return true;
#else
	BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}

	return true;
#endif
}