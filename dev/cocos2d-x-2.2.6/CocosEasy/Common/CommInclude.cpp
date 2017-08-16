#include "CommInclude.h"
#include "cocos2d.h"

#ifdef __GNUC__
#include <unistd.h>
#include <sys/time.h>
#include <netinet/in.h>
#endif
#ifdef WIN32
#include <windows.h>
#endif // _WIN32

#if(CC_TARGET_PLATFORM ==  CC_PLATFORM_WP8)
#include "CCPThreadWinRT.h"
#endif

static unsigned long long s_startTime = 0;

unsigned int getTickCount()
{
	struct cocos2d::cc_timeval now;
	if (cocos2d::CCTime::gettimeofdayCocos2d(&now, NULL) != 0)
	{
		CCLOG("error in gettimeofday");
		float ftick = (float)clock() / (float)CLOCKS_PER_SEC * 1000.0f;
		return (unsigned int)(int)ftick;
	}

	unsigned long long tick = (unsigned long long)(now.tv_sec * 1000 + now.tv_usec / 1000);
	if (s_startTime == 0) s_startTime = tick;
	unsigned int ret = (unsigned int)(tick - s_startTime);
	return ret;
	

// 	unsigned long long t = GetTickCount64();
// 	float ftick = (float)clock() / (float)CLOCKS_PER_SEC * 1000.0f;
// 	return (unsigned int)(int)ftick;
/*
	struct cocos2d::cc_timeval now;
	if (cocos2d::CCTime::gettimeofdayCocos2d(&now, NULL) != 0)
	{
		CCLOG("error in gettimeofday");
		float ftick = (float)clock() / (float)CLOCKS_PER_SEC * 1000.0f;
		return (unsigned int)(int)ftick;
	}

	unsigned int tick = (unsigned int)(now.tv_sec * 1000.0f + now.tv_usec / 1000.0f);
	return tick;
	

	float ftick = (float)clock() / (float)CLOCKS_PER_SEC * 1000.0f;
	return (unsigned int)(int)ftick;
	
	unsigned int currentTime;
#ifdef WIN32
	currentTime = (unsigned int)GetTickCount64();
#endif
#ifdef __GNUC__
	struct timeval current;
	gettimeofday(&current, NULL);
	currentTime = current.tv_sec * 1000 + current.tv_usec/1000;
#endif
	return currentTime;
	*/
}

unsigned int hashString( const char* str )
{
	unsigned int len = strlen(str);
	const char *end = str + len;
	unsigned int hash;

	for (hash = 0; str < end; str++)
	{
		hash *= 16777619;
		hash ^= (unsigned int) (unsigned char) toupper(*str);
	}
	return (hash);
}

void wait( unsigned int ms )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Sleep(ms);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	Concurrency::wait(ms);	
#endif

#ifdef __GNUC__
	timeval timeout;
	timeout.tv_sec	= (int)(ms / 1000);
	timeout.tv_usec	= ms - timeout.tv_sec * 1000;
	select(0, NULL, NULL, NULL, &timeout);
#endif
}
