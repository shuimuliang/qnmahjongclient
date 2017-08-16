#include <string.h>

#ifdef CC_TARGET_PLATFORM
#include "cocos2d.h"
#endif

#include "CrashReport.h"

#if (BUGLY_REPORT_LUA)
#include "CCLuaEngine.h"
#endif

#define CATEGORY_LUA_EXCEPTION 6
#define CATEGORY_JS_EXCEPTION 7

#define LOG_TAG "CrashReport"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	#include <android/log.h>
	#include <jni.h>
	#include "platform/android/jni/JniHelper.h"

	#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
	#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
	#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

	#define CRASHREPORT_CLASS "com/tencent/bugly/cocos/Cocos2dxAgent"
	#define METHOD_INIT "initCrashReport"
	#define METHOD_INIT_PARAMETER "(Landroid/content/Context;Ljava/lang/String;Z)V"
	#define METHOD_POST_EXCEPTION "postException"
	#define METHOD_POST_EXCEPTION_PARAMETER "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V"
	#define METHOD_SET_USER_ID "setUserId"
	#define METHOD_SET_USER_ID_PARAMETER "(Ljava/lang/String;)V"
	#define METHOD_SET_LOG "setLog"
	#define METHOD_SET_LOG_PARAMETER "(ILjava/lang/String;Ljava/lang/String;)V"
	#define METHOD_SET_USER_SCENE_TAG "setUserSceneTag"
	#define METHOD_SET_USER_SCENE_TAG_PARAMETER "(Landroid/content/Context;I)V"
	#define METHOD_PUT_USER_DATA "putUserData"
	#define METHOD_PUT_USER_DATA_PARAMETER "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;)V"
	#define METHOD_REMOVE_USER_DATA "removeUserData"
	#define METHOD_REMOVE_USER_DATA_PARAMETER "(Landroid/content/Context;Ljava/lang/String;)V"
	#define METHOD_SET_CHANNEL "setSDKPackagePrefixName"
	#define METHOD_SET_CHANNEL_PARAMETER "(Ljava/lang/String;)V"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #import <Foundation/Foundation.h>

    #define NSStringMake(const_char_pointer) (const_char_pointer == NULL ? nil : @(const_char_pointer))
    #define NSStringNonnullMake(const_char_pointer) (const_char_pointer == NULL ? @"" : @(const_char_pointer))

    #define LOGI(fmt, args...) printf("[Info] %s: ", LOG_TAG);\
                               printf(fmt, ##args);\
                               printf("\n");

    #define LOGD(fmt, args...) printf("[Debug] %s: ", LOG_TAG);\
                               printf(fmt, ##args);\
                               printf("\n");

    #define LOGE(fmt, args...) printf("[Error] %s: ", LOG_TAG);\
                               printf(fmt, ##args);\
                               printf("\n");

    #define BUGLY_AGENT_CLASS @"BuglyAgent"
    #define BUGLY_AGENT_METHOD_INIT @"initWithAppId:debugMode:"
    #define BUGLY_AGENT_METHOD_INIT_LOG @"initWithAppId:debugMode:logger:"
    #define BUGLY_AGENT_METHOD_USER @"setUserIdentifier:"
    #define BUGLY_AGENT_METHOD_CHANNEL @"setAppChannel:"
    #define BUGLY_AGENT_METHOD_VERSION @"setAppVersion:"
    #define BUGLY_AGENT_METHOD_EXCEPTION @"reportException:name:message:stackTrace:userInfo:terminateApp:"
    #define BUGLY_AGENT_METHOD_SCENE @"setSceneTag:"
    #define BUGLY_AGENT_METHOD_SCENE_VALUE @"setSceneValue:forKey:"
	#define BUGLY_AGENT_METHOD_SCENE_CLEAN @"removeSceneValueForKey:"
    #define BUGLY_AGENT_METHOD_LOG @"level:tag:log:"
#endif


bool CrashReport::initialized = false;

void CrashReport::initCrashReport(const char* appId, bool isDebug) {
    CrashReport::initCrashReport(appId, isDebug, Warning);
}

void CrashReport::initCrashReport(const char* appId, bool isDebug, CrashReport::CRLogLevel level) {
	if (!initialized) {
	
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			LOGI("[cocos2d-x] start init.");
			initialized = true;
			JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
			if (jvm == NULL) {
				LOGE("[cocos2d-x] JavaVM is null.");
				return;
			}
			JNIEnv* env = NULL;
			jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
			if (env == NULL) {
				LOGE("[cocos2d-x] JNIEnv is null.");
				return;
			}

			jvm->AttachCurrentThread(&env, 0);

			//get activity
			LOGI("[cocos2d-x] try get org.cocos2dx.lib.Cocos2dxActivity");
			jclass activityClass = env->FindClass("org/cocos2dx/lib/Cocos2dxActivity");
			if (activityClass == NULL) {
				LOGE("[cocos2d-x] Cocos2dxActivity is Null");
				return;
			}
			jmethodID methodActivity = env->GetStaticMethodID(activityClass, "getContext", "()Landroid/content/Context;");
			jobject activity = (jobject) env->CallStaticObjectMethod(activityClass, methodActivity);
			if (activity == NULL) {
				LOGE("[cocos2d-x] activity is Null");
				return;
			}
			//call channel set package name
			#if(SDK_CHANNEL_INDEX != 0)
			LOGI("[cocos2d-x] set channel: %d", SDK_CHANNEL_INDEX);
			char* packageName = "";
			switch(SDK_CHANNEL_INDEX) {
				case 1:
				packageName = "com.tencent.bugly.msdk";
				break;
			}
			jmethodID setChannelMethod = env->GetStaticMethodID(env->FindClass(CRASHREPORT_CLASS), METHOD_SET_CHANNEL, METHOD_SET_CHANNEL_PARAMETER);
			LOGI("set packagename: %s", packageName);
			env->CallStaticVoidMethod(env->FindClass(CRASHREPORT_CLASS), setChannelMethod, env->NewStringUTF(packageName));	
			#endif

			//call init
			LOGI("[cocos2d-x] init by bugly.jar");
			jmethodID initMethod = env->GetStaticMethodID(env->FindClass(CRASHREPORT_CLASS), METHOD_INIT, METHOD_INIT_PARAMETER);
			env->CallStaticVoidMethod(env->FindClass(CRASHREPORT_CLASS), initMethod, activity, env->NewStringUTF(appId), isDebug);

        //  iOS
		#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        	NSString* pAppId = NSStringMake(appId);
            BOOL pDebug = isDebug ? YES : NO;
        
			Class clazz = NSClassFromString(BUGLY_AGENT_CLASS);
            if (clazz) {
                BOOL initLog = true;
                SEL selector = NSSelectorFromString(BUGLY_AGENT_METHOD_INIT_LOG);
                
                NSMethodSignature* signature = [clazz methodSignatureForSelector:selector];
                if (signature == nil) {
                    selector = NSSelectorFromString(BUGLY_AGENT_METHOD_INIT);
                    signature = [clazz methodSignatureForSelector:selector];
                    
                    initLog = false;
                }
                if (signature) {
                    
                    LOGI("Init the sdk with App ID: %s", appId);
                    
                    NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
                    [invocation setTarget:clazz];
                    [invocation setSelector:selector];
                    
                    [invocation setArgument:&pAppId atIndex:2];
                    [invocation setArgument:&pDebug atIndex:3];
                    
                    if (initLog) {
                        NSInteger pLevel = (level >= 4 ? 1 : (level == 3 ? 2 : (level == 2 ? 4 : (level == 1 ? 8 : 16))));
                        [invocation setArgument:&pLevel atIndex:4];
                    }
                    
                    [invocation invoke];
                }
            } else {
                NSLog(@"Warning: Fail to get class by NSSelectorFromString(%@)", BUGLY_AGENT_CLASS);
            }
        #endif
        
        // register lua handler
#if (BUGLY_REPORT_LUA)
        lua_register(getLuaState(), "buglyReportLuaException", buglyReportLuaException);
        lua_register(getLuaState(), "buglyPutUserData", buglyLuaPutUserData);
        lua_register(getLuaState(), "buglyRemoveUserData", buglyLuaRemoveUserData);
        lua_register(getLuaState(), "buglySetUserSceneTag", buglyLuaSetUserSceneTag);
        lua_register(getLuaState(), "buglySetUserId", buglyLuaSetUserId);
        lua_register(getLuaState(), "buglyLog", buglyLuaLog);

        LOGI("[cocos2d-x] registered bugly lua functions, init finished");
#endif

#if (BUGLY_REPORT_JS)

#endif
			initialized = true;
		}
}

void CrashReport::setUserSceneTag(int tag) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
	if (jvm == NULL) {
		LOGE("[cocos2d-x] JavaVM is null.");
		return;
	}
	JNIEnv* env = NULL;
	jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
	if (env == NULL) {
		LOGE("[cocos2d-x] JNIEnv is null.");
		return;
	}
	LOGI("[cocos2d-x] set user scene tag: %d", tag);
	//get activity
	jclass activityClass = env->FindClass("org/cocos2dx/lib/Cocos2dxActivity");
	if (activityClass == NULL) {
		LOGE("[cocos2d-x] Cocos2dxActivity is Null");
		return;
	}
	jmethodID methodActivity = env->GetStaticMethodID(activityClass, "getContext", "()Landroid/content/Context;");
	jobject activity = (jobject) env->CallStaticObjectMethod(activityClass, methodActivity);
	if (activity == NULL) {
		LOGE("[cocos2d-x] activity is Null");
		return;
	}

	jclass reportClass = env->FindClass(CRASHREPORT_CLASS);
	jmethodID method = env->GetStaticMethodID(reportClass, METHOD_SET_USER_SCENE_TAG, METHOD_SET_USER_SCENE_TAG_PARAMETER);
	env->CallStaticVoidMethod(reportClass, method, activity, tag);

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (tag < 0) {
        return;
    }
	NSUInteger pTag = tag;
    
	Class clazz = NSClassFromString(BUGLY_AGENT_CLASS);
    if (clazz) {
        SEL selector = NSSelectorFromString(BUGLY_AGENT_METHOD_SCENE);
        NSMethodSignature* signature = [clazz methodSignatureForSelector:selector];
        if (signature) {
            
            LOGI("Set user scene tag id: %d", tag);
            
            NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
            [invocation setTarget:clazz];
            [invocation setSelector:selector];
            
            [invocation setArgument:&pTag atIndex:2];
            
            [invocation invoke];
        }
    } else {
        NSLog(@"Warning: Fail to get class by NSSelectorFromString(%@)", BUGLY_AGENT_CLASS);
    }
#endif
}

void CrashReport::putUserData(const char* key, const char* value) {
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
	if (jvm == NULL) {
		LOGE("[cocos2d-x] JavaVM is null.");
		return;
	}
	JNIEnv* env = NULL;
	jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
	if (env == NULL) {
		LOGE("[cocos2d-x] JNIEnv is null.");
		return;
	}
		
	LOGI("[cocos2d-x] put user data: %s:%s", key, value);
	//get activity
	jclass activityClass = env->FindClass("org/cocos2dx/lib/Cocos2dxActivity");
	if (activityClass == NULL) {
		LOGE("[cocos2d-x] Cocos2dxActivity is Null");
		return;
	}
	jmethodID methodActivity = env->GetStaticMethodID(activityClass, "getContext", "()Landroid/content/Context;");
	jobject activity = (jobject) env->CallStaticObjectMethod(activityClass, methodActivity);
	if (activity == NULL) {
		LOGE("[cocos2d-x] activity is Null");
		return;
	}

	jclass reportClass = env->FindClass(CRASHREPORT_CLASS);
	jmethodID method = env->GetStaticMethodID(reportClass, METHOD_PUT_USER_DATA, METHOD_PUT_USER_DATA_PARAMETER);
	env->CallStaticVoidMethod(reportClass, method, activity, env->NewStringUTF(key), env->NewStringUTF(value));
	return;

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	NSString * pKey = NSStringNonnullMake(key);
	NSString * pValue = NSStringNonnullMake(value);
    
	Class clazz = NSClassFromString(BUGLY_AGENT_CLASS);
    if (clazz) {
        SEL selector = NSSelectorFromString(BUGLY_AGENT_METHOD_SCENE_VALUE);
        
        NSMethodSignature* signature = [clazz methodSignatureForSelector:selector];
        
        if (signature) {
            
            LOGI("Set user Key-Value: [%s, %s]", key, value);
            
            NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
            [invocation setTarget:clazz];
            [invocation setSelector:selector];
            
            [invocation setArgument:&pValue atIndex:2];
            [invocation setArgument:&pKey atIndex:3];
            
            [invocation invoke];
        }
    } else {
        NSLog(@"Warning: Fail to get class by NSSelectorFromString(%@)", BUGLY_AGENT_CLASS);
    }
#endif
}

void CrashReport::removeUserData(const char* key) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
	if (jvm == NULL) {
		LOGE("[cocos2d-x] JavaVM is null.");
		return;
	}
	JNIEnv* env = NULL;
	jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
	if (env == NULL) {
		LOGE("[cocos2d-x] JNIEnv is null.");
		return;
	}
	LOGI("[cocos2d-x] remove user data: %s", key);
	//get activity
	jclass activityClass = env->FindClass("org/cocos2dx/lib/Cocos2dxActivity");
	if (activityClass == NULL) {
		LOGE("[cocos2d-x] Cocos2dxActivity is Null");
		return;
	}
	jmethodID methodActivity = env->GetStaticMethodID(activityClass, "getContext", "()Landroid/content/Context;");
	jobject activity = (jobject) env->CallStaticObjectMethod(activityClass, methodActivity);
	if (activity == NULL) {
		LOGE("[cocos2d-x] activity is Null");
		return;
	}

	jclass reportClass = env->FindClass(CRASHREPORT_CLASS);
	jmethodID method = env->GetStaticMethodID(reportClass, METHOD_REMOVE_USER_DATA, METHOD_REMOVE_USER_DATA_PARAMETER);
	env->CallStaticVoidMethod(reportClass, method, activity, env->NewStringUTF(key));
	return;

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	NSString * pKey = NSStringNonnullMake(key);
	
    Class clazz = NSClassFromString(BUGLY_AGENT_CLASS);
    
    if (clazz) {
        SEL selector = NSSelectorFromString(BUGLY_AGENT_METHOD_SCENE_CLEAN);
        
        NSMethodSignature* signature = [clazz methodSignatureForSelector:selector];
        
        if (signature) {
            NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
            [invocation setTarget:clazz];
            [invocation setSelector:selector];
            
            [invocation setArgument:&pKey atIndex:2];
            
            [invocation invoke];
        }
    } else {
        NSLog(@"Warning: Fail to get class by NSSelectorFromString(%@)", BUGLY_AGENT_CLASS);
    }
    
#endif
}

void CrashReport::setUserId(const char* userId) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
	if (jvm == NULL) {
		LOGE("[cocos2d-x] JavaVM is null.");
		return;
	}
	JNIEnv* env = NULL;
	jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
	if (env == NULL) {
		LOGE("[cocos2d-x] JNIEnv is null.");
		return;
	}
	LOGI("[cocos2d-x] set user id: %s", userId);
	jclass reportClass = env->FindClass(CRASHREPORT_CLASS);
	jmethodID method = env->GetStaticMethodID(reportClass, METHOD_SET_USER_ID, METHOD_SET_USER_ID_PARAMETER);
	env->CallStaticVoidMethod(reportClass, method, env->NewStringUTF(userId));
	return;

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	NSString * pUserId = NSStringMake(userId);
	
    Class clazz = NSClassFromString(BUGLY_AGENT_CLASS);
    
    if (clazz) {
        SEL selector = NSSelectorFromString(BUGLY_AGENT_METHOD_USER);
        NSMethodSignature* signature = [clazz methodSignatureForSelector:selector];
        if (signature) {
            
            LOGI("Set user id: %s", userId);
            
            NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
            [invocation setTarget:clazz];
            [invocation setSelector:selector];
            
            [invocation setArgument:&pUserId atIndex:2];
            
            [invocation invoke];
        }
    } else {
        NSLog(@"Warning: Fail to get class by NSSelectorFromString(%@)", BUGLY_AGENT_CLASS);
    }
#endif
}

void CrashReport::reportException(int category, const char* type, const char* msg, const char* traceback) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
	if (jvm == NULL) {
		LOGE("[cocos2d-x] JavaVM is null.");
		return;
	}
	JNIEnv* env = NULL;
	jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
	if (env == NULL) {
		LOGE("[cocos2d-x] JNIEnv is null.");
		return;
	}

	jclass reportClass = env->FindClass(CRASHREPORT_CLASS);
	jmethodID reportMethod = env->GetStaticMethodID(reportClass, METHOD_POST_EXCEPTION, METHOD_POST_EXCEPTION_PARAMETER);
	jstring typeStr = env->NewStringUTF(type);
	jstring msgStr = env->NewStringUTF(msg);
	jstring traceStr = env->NewStringUTF(traceback);
	env->CallStaticVoidMethod(reportClass, reportMethod, category, typeStr, msgStr, traceStr, false);
	env->DeleteLocalRef(typeStr);
	env->DeleteLocalRef(msgStr);
	env->DeleteLocalRef(traceStr);

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	NSUInteger pCategory = category;
	NSString * pType = NSStringMake(type);
	NSString * pMsg = NSStringMake(msg);
	NSString * pTraceStack = NSStringMake(traceback);
    NSDictionary * nullObject = nil;
	BOOL terminateApp = NO;

	Class clazz = NSClassFromString(BUGLY_AGENT_CLASS);
    
    if (clazz) {
        SEL selector = NSSelectorFromString(BUGLY_AGENT_METHOD_EXCEPTION);
        NSMethodSignature* signature = [clazz methodSignatureForSelector:selector];
        
        if (signature) {
            LOGE("Report exception: %s, %s\n%s", type, msg, traceback);
            
            NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
            [invocation setTarget:clazz];
            [invocation setSelector:selector];
            
            [invocation setArgument:&pCategory atIndex:2];
            [invocation setArgument:&pType atIndex:3];
            [invocation setArgument:&pMsg atIndex:4];
            [invocation setArgument:&pTraceStack atIndex:5];
            [invocation setArgument:&nullObject atIndex:6];
            [invocation setArgument:&terminateApp atIndex:7];
            
            [invocation invoke];
        }
    } else {
        NSLog(@"Warning: Fail to get class by NSSelectorFromString(%@)", BUGLY_AGENT_CLASS);
    }
#endif
}

void CrashReport::setAppChannel(const char * channel){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    LOGI("No impl");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString * pUserId = NSStringMake(channel);
    
    Class clazz = NSClassFromString(BUGLY_AGENT_CLASS);
    
    if (clazz) {
        SEL selector = NSSelectorFromString(BUGLY_AGENT_METHOD_CHANNEL);
        NSMethodSignature* signature = [clazz methodSignatureForSelector:selector];
        if (signature) {
            
            LOGI("Set channel: %s", channel);
            
            NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
            [invocation setTarget:clazz];
            [invocation setSelector:selector];
            
            [invocation setArgument:&pUserId atIndex:2];
            
            [invocation invoke];
        }
    }
#endif
}

void CrashReport::setAppVersion(const char * version){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
     LOGI("No impl");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString * pUserId = NSStringMake(version);
    
    Class clazz = NSClassFromString(BUGLY_AGENT_CLASS);
    
    if (clazz) {
        SEL selector = NSSelectorFromString(BUGLY_AGENT_METHOD_VERSION);
        NSMethodSignature* signature = [clazz methodSignatureForSelector:selector];
        if (signature) {
            
            LOGI("Set version: %s", version);
            
            NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
            [invocation setTarget:clazz];
            [invocation setSelector:selector];
            
            [invocation setArgument:&pUserId atIndex:2];
            
            [invocation invoke];
        }
    }
#endif
}

void CrashReport::log(CrashReport::CRLogLevel level, const char * tag, const char * fmts, ...) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
	if (jvm == NULL) {
		LOGE("[cocos2d-x] JavaVM is null.");
		return;
	}
	JNIEnv* env = NULL;
	jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
	if (env == NULL) {
		LOGE("[cocos2d-x] JNIEnv is null.");
		return;
	}
	LOGI("[cocos2d-x] set log: %s - %s", tag, fmts);
	jclass reportClass = env->FindClass(CRASHREPORT_CLASS);
	jmethodID method = env->GetStaticMethodID(reportClass, METHOD_SET_LOG, METHOD_SET_LOG_PARAMETER);
	env->CallStaticVoidMethod(reportClass, method, (int)level, env->NewStringUTF(tag), env->NewStringUTF(fmts));
	return;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (NULL == fmts) {
        return;
    }
    
    //Error=4,Warn=3,Info=2,Debug=1,Verbose=0
    //LogError=1<<0,Warn=1<<1,Info=1<<2,Debug=1<<3,Verbose=1<<4
    NSInteger pLevel = (level >= 4 ? 1 : (level == 3 ? 2 : (level == 2 ? 4 : (level == 1 ? 8 : 16))));
    NSString * pTag = NSStringMake(tag);
    
    char msg[256] = {0};
    va_list args;
    va_start(args, fmts);
    vsprintf(msg, fmts, args);
    va_end(args);

    NSString * pMsg = NSStringMake(msg);
    
    Class clazz = NSClassFromString(BUGLY_AGENT_CLASS);
    
    if (clazz) {
        SEL selector = NSSelectorFromString(BUGLY_AGENT_METHOD_LOG);
        NSMethodSignature* signature = [clazz methodSignatureForSelector:selector];
        
        if (signature) {
            NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:signature];
            [invocation setTarget:clazz];
            [invocation setSelector:selector];
            
            [invocation setArgument:&pLevel atIndex:2];
            [invocation setArgument:&pTag atIndex:3];
            [invocation setArgument:&pMsg atIndex:4];
            
            [invocation invoke];
        }
    }
#endif
    
}

//special for lua
#if (BUGLY_REPORT_LUA)

int CrashReport::buglyReportLuaException(lua_State* luaState) {
	const char* type = "";
	const char* msg = lua_tostring(luaState, 1);
	const char* traceback = lua_tostring(luaState, 2);

	CrashReport::reportException(CATEGORY_LUA_EXCEPTION, type, msg, traceback);
    
	return 0;
}

int CrashReport::buglyLuaPutUserData(lua_State* luaState) {
	const char* key = lua_tostring(luaState, 1);
	const char* value = lua_tostring(luaState, 2);
    
	CrashReport::putUserData(key, value);
    
    return 0;
}

int CrashReport::buglyLuaRemoveUserData(lua_State* luaState) {
	const char* key = lua_tostring(luaState, 1);
	CrashReport::removeUserData(key);
    
    return 0;
}

int CrashReport::buglyLuaSetUserSceneTag(lua_State* luaState) {
	int tag = lua_tonumber(luaState, 1);
	CrashReport::setUserSceneTag(tag);
    return 0;
}

int CrashReport::buglyLuaSetUserId(lua_State* luaState) {
	const char* userId = lua_tostring(luaState, 1);
	CrashReport::setUserId(userId);
    return 0;
}

int CrashReport::buglyLuaLog(lua_State* luaState) {
	int level = lua_tonumber(luaState, 1);
	const char* tag = lua_tostring(luaState, 2);
	const char* logStr = lua_tostring(luaState, 3);
	CRLogLevel crLevel = Verbose;
	switch (level) {
		case 0:
			crLevel = Verbose;
			break;
		case 1:
			crLevel = Debug;
			break;
		case 2:
			crLevel = Info;
			break;
		case 3:
			crLevel = Warning;
			break;
		case 4:
			crLevel = Error;
			break;
	}
	CrashReport::log(crLevel, tag, logStr);
    return 0;
}

lua_State* CrashReport::getLuaState() {
	#if COCOS2D_VERSION >= 0x00030000
		return cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState(); 
	#else
		return cocos2d::CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState(); 
	#endif
}

#endif

// special for js
#if (BUGLY_REPORT_JS)

#endif