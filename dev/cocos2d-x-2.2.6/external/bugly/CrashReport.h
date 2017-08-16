#ifndef CRASHREPORT_H
#define CRASHREPORT_H

#include "CrashReportFeature.h"

#ifdef CC_TARGET_PLATFORM
    #include "cocos2d.h"
#endif

#if (BUGLY_REPORT_LUA)
#include "CCLuaEngine.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#include <jni.h>
#endif


#if (BUGLY_REPORT_JS)

#endif

class CrashReport
{
public:
    
	static void initCrashReport(const char* appId, bool debug);
    
	static void reportException(int category, const char* type, const char* msg, const char* traceback);
	static void setUserSceneTag(int tag);
	static void putUserData(const char* key, const char* value);
	static void setUserId(const char* userId);
	static void removeUserData(const char* key);

    enum CRLogLevel{ Error=4,Warning=3,Info=2,Debug=1,Verbose=0 };
    
    static void log(CrashReport::CRLogLevel level, const char* tag, const char * fmt, ...);

    static void initCrashReport(const char* appId, bool debug, CrashReport::CRLogLevel level);
    
    static void setAppChannel(const char* channel);
    static void setAppVersion(const char* version);
    
private:
	static bool initialized;
	
	#if (BUGLY_REPORT_LUA)
	
	static lua_State* getLuaState();
	static int buglyReportLuaException(lua_State* luaState);
	static int buglyLuaPutUserData(lua_State* luaState);
	static int buglyLuaRemoveUserData(lua_State* luaState);
	static int buglyLuaSetUserSceneTag(lua_State* luaState);
	static int buglyLuaSetUserId(lua_State* luaState);
	static int buglyLuaLog(lua_State* luaState);
	
	#endif

	#if (BUGLY_REPORT_JS)

	#endif
};

#endif