#include "../Analysis.h"
#include "platform/android/jni/JniHelper.h"
#include "../../SDKEasy/SDKManager.h"
//#include <stdio.h>
//using namespace std;

void CAnalysis::initAnalysis()
{

}
void CAnalysis::logEvent(const char *eventID,const char *eventTag){
    const char* playerID = CAnalysis::getInstance()->getPlayerID().c_str();
    if (playerID == NULL) {
        playerID = "";
    }
    
    int channelID = CSDKManager::getInstance()->getChannelID();
    
    
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppEventLogger"
                                       , "logEvent"
                                       , "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V")) {
        
        jstring jeventID = t.env->NewStringUTF(eventID);
        jstring jeventTag = t.env->NewStringUTF(eventTag);
        jstring jplayerID = t.env->NewStringUTF(playerID);
        
		t.env->CallStaticObjectMethod(t.classID, t.methodID, jeventID, jeventTag, jplayerID, channelID);
        
        t.env->DeleteLocalRef(jeventID);
        t.env->DeleteLocalRef(jeventTag);
        t.env->DeleteLocalRef(jplayerID);
        
	}
}

void CAnalysis::logPurchase(const char* parmas){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppEventLogger"
                                       , "logPurchase"
                                       , "(Ljava/lang/String;)V")) {
        
        jstring jparmas = t.env->NewStringUTF(parmas);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jparmas);
        
        t.env->DeleteLocalRef(jparmas);
    }
}

void CAnalysis::logRegister(const char* parmas){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppEventLogger"
                                       , "logRegister"
                                       , "(Ljava/lang/String;)V")) {
        
        jstring jparmas = t.env->NewStringUTF(parmas);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jparmas);
        
        t.env->DeleteLocalRef(jparmas);
    }

}

void CAnalysis::logLogin(const char* parmas){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppEventLogger"
                                       , "logLogin"
                                       , "(Ljava/lang/String;)V")) {
        
        jstring jparmas = t.env->NewStringUTF(parmas);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jparmas);
        
        t.env->DeleteLocalRef(jparmas);
    }
}


void CAnalysis::pageviewStart(const char *pageName){
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppEventLogger"
                                       , "pageviewStart"
                                       , "(Ljava/lang/String;)V")) {
        
        jstring jpageName = t.env->NewStringUTF(pageName);
        
		t.env->CallStaticVoidMethod(t.classID, t.methodID, jpageName);
        
        t.env->DeleteLocalRef(jpageName);
	}
}
void CAnalysis::pageviewEnd(const char *pageName){
    
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppEventLogger"
                                       , "pageviewEnd"
                                       , "(Ljava/lang/String;)V")) {
        
        jstring jpageName = t.env->NewStringUTF(pageName);
        
		t.env->CallStaticVoidMethod(t.classID, t.methodID, jpageName);
        
        t.env->DeleteLocalRef(jpageName);
        
	}
}

const char*  CAnalysis::getAdID(){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "getMacAddress"
                                       , "()Ljava/lang/String;")) {
        
         jstring jstr = (jstring)(t.env->CallStaticObjectMethod(t.classID, t.methodID));
        
         CCString *ret = new CCString(JniHelper::jstring2string(jstr).c_str());
        
         t.env->DeleteLocalRef(jstr);
        
        return ret->m_sString.c_str();
    }
    
    return "";
}