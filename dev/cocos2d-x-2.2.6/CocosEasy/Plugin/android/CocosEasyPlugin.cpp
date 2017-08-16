#include "CocosEasyPlugin.h"

#include "platform/android/jni/JniHelper.h"
#include <string>
#include <map>

using namespace std;
using namespace cocos2d;

static int androidDeviceID = -1;

extern "C"
{
	static jobject createJavaMapObject(JNIEnv *env, std::map<std::string, std::string>* map) {
		jclass class_Hashtable = env->FindClass("java/util/HashMap");
		jmethodID construct_method = env->GetMethodID( class_Hashtable, "<init>","()V");
		jobject obj_Map = env->NewObject( class_Hashtable, construct_method, "");
		if (map != NULL) {
			jmethodID add_method= env->GetMethodID( class_Hashtable,"put","(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
			for (std::map<std::string, std::string>::const_iterator it = map->begin(); it != map->end(); ++it) {
				env->CallObjectMethod(obj_Map, add_method, env->NewStringUTF(it->first.c_str()), env->NewStringUTF(it->second.c_str()));
			}
		}
		env->DeleteLocalRef(class_Hashtable);
		return obj_Map;
	}
}

unsigned int CCEPlugin::isIOSSimulator()
{
    return 0;
}

const std::string CCEPlugin::getDeviceModel(){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "getInstance"
                                       , "()Lcom/zerogame/pluginlibs/AppPlugin;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/zerogame/pluginlibs/AppPlugin"
                                     , "getDeviceModel"
                                     , "()Ljava/lang/String;")) {
            jstring jstr = (jstring)t.env->CallObjectMethod((jobject)obj, t.methodID);
            
            t.env->DeleteLocalRef(t.classID);
            
            CCString *ret = new CCString(JniHelper::jstring2string(jstr).c_str());
            t.env->DeleteLocalRef(jstr);
            
            return ret->m_sString;
        }
    }
    return NULL;
}

const int CCEPlugin::getOSVersion(){
    return 1;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "getInstance"
                                       , "()Lcom/zerogame/pluginlibs/AppPlugin;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/zerogame/pluginlibs/AppPlugin"
                                     , "getOSVersion"
                                     , "()I")) {
            jint _int = (jint)t.env->CallIntMethod((jobject)obj, t.methodID);
        
            t.env->DeleteLocalRef(t.classID);
        
            return (int)_int;
        }
    }
    return NULL;
}

const int CCEPlugin::isJailBreak(){ //是否root 或 越狱  【0:未越狱，1：越狱】
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "getInstance"
                                       , "()Lcom/zerogame/pluginlibs/AppPlugin;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/zerogame/pluginlibs/AppPlugin"
                                     , "getIsRoot"
                                     , "()I")) {
            jint _int = (jint)t.env->CallIntMethod((jobject)obj, t.methodID);
            
            t.env->DeleteLocalRef(t.classID);
            
            return (int)_int;
        }
    }
    
    return 0;
}

const std::string CCEPlugin::getDeviceID() {
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "getInstance"
                                       , "()Lcom/zerogame/pluginlibs/AppPlugin;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/zerogame/pluginlibs/AppPlugin"
                                     , "getDeviceUniqueID"
                                     , "()Ljava/lang/String;")) {
            jstring jstr = (jstring)t.env->CallObjectMethod((jobject)obj, t.methodID);
            
            t.env->DeleteLocalRef(t.classID);
            
            CCString *ret = new CCString(JniHelper::jstring2string(jstr).c_str());
            t.env->DeleteLocalRef(jstr);
            
            return ret->m_sString;
        }
    }
    return NULL;
}

const std::string CCEPlugin::getAndroidSDCardPath() {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "getInstance"
                                       , "()Lcom/zerogame/pluginlibs/AppPlugin;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/zerogame/pluginlibs/AppPlugin"
                                     , "getSDCardPath"
                                     , "()Ljava/lang/String;")) {
            jstring jstr = (jstring)t.env->CallObjectMethod((jobject)obj, t.methodID);
            
            t.env->DeleteLocalRef(t.classID);
            
            CCString *ret = new CCString(JniHelper::jstring2string(jstr).c_str());
            t.env->DeleteLocalRef(jstr);
            
            return ret->m_sString;
        }
    }
    return NULL;
}

const int CCEPlugin::getDeviceLV(){
    if (androidDeviceID == -1) {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t
                                           , "com/zerogame/pluginlibs/AppPlugin"
                                           , "getInstance"
                                           , "()Lcom/zerogame/pluginlibs/AppPlugin;")) {
            jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
            if (JniHelper::getMethodInfo(t
                                         , "com/zerogame/pluginlibs/AppPlugin"
                                         , "getDeviceLV"
                                         , "()I")) {
                jint _int = (jint)t.env->CallIntMethod((jobject)obj, t.methodID);
                
                t.env->DeleteLocalRef(t.classID);
                
                androidDeviceID = (int)_int;
            }
        }
    }
    
    return androidDeviceID;
}


void CCEPlugin::openURL(std::string url) {
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "openURL"
                                       , "(Ljava/lang/String;)V")) {
        
        jstring jUrl = t.env->NewStringUTF(url.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID,jUrl);
        
        t.env->DeleteLocalRef(jUrl);
        t.env->DeleteLocalRef(t.classID);
    }
}

void CCEPlugin::messageBox2(const char* title,
                            const char* msg,
                            const char* button1Title,
                            const char* button2Title,
                            CCObject *pTarget,
                            SEL_CallFuncO callback){
    pMsgBoxTarget = pTarget;
    msgBoxCallback = callback;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "messageBox2"
                                       , "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        
        jstring jTitle = t.env->NewStringUTF(title);
        jstring jMsg = t.env->NewStringUTF(msg);
        jstring jButton1Title = t.env->NewStringUTF(button1Title);
        jstring jButton2Title = t.env->NewStringUTF(button2Title);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID,jTitle,jMsg,jButton1Title,jButton2Title);
        
        t.env->DeleteLocalRef(jTitle);
        t.env->DeleteLocalRef(jMsg);
        t.env->DeleteLocalRef(jButton1Title);
        t.env->DeleteLocalRef(jButton2Title);
        t.env->DeleteLocalRef(jButton2Title);
        t.env->DeleteLocalRef(t.classID);
    }

}

void CCEPlugin::messageBox2Callback(int index){
    if (pMsgBoxTarget && msgBoxCallback){
        CCLog("index index11111111  %d",index);
        (pMsgBoxTarget->*msgBoxCallback)(CCInteger::create(index));
    }
//    pMsgBoxTarget = NULL;
//    msgBoxCallback = NULL;
}


void CCEPlugin::messageBox(const char* title,
                           const char* msg,
                           const char* button1Title,
                           const char* button2Title,
                           int handler){
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "messageBox"
                                       , "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V")) {
        
        jstring jTitle = t.env->NewStringUTF(title);
        jstring jMsg = t.env->NewStringUTF(msg);
        jstring jButton1Title = t.env->NewStringUTF(button1Title);
        jstring jButton2Title = t.env->NewStringUTF(button2Title);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID,jTitle,jMsg,jButton1Title,jButton2Title,handler);
        
        t.env->DeleteLocalRef(jTitle);
        t.env->DeleteLocalRef(jMsg);
        t.env->DeleteLocalRef(jButton1Title);
        t.env->DeleteLocalRef(jButton2Title);
        t.env->DeleteLocalRef(jButton2Title);
        t.env->DeleteLocalRef(t.classID);
    }
}

//system clipboard text
void CCEPlugin::setClipboardTxt(const char* pTtxt){
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "setClipboardTxt"
                                       , "(Ljava/lang/String;)V")) {
        
        jstring jPTtxt = t.env->NewStringUTF(pTtxt);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID,jPTtxt);
        
        t.env->DeleteLocalRef(jPTtxt);
        t.env->DeleteLocalRef(t.classID);
    }   
}


void CCEPlugin::startNotification(E_Noti_T notiType,int repeatDays ,int timeByMin , const char *title , const char *msg ,int tagId)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
		, "com/zerogame/pluginlibs/AppPlugin"
		, "setAlarmNotification"
		, "(IIILjava/lang/String;Ljava/lang/String;I)V")) {
            jstring jTitle = t.env->NewStringUTF(title);
			jstring jMsg = t.env->NewStringUTF(msg);
			t.env->CallStaticVoidMethod(t.classID, t.methodID,notiType,repeatDays,timeByMin,jTitle,jMsg,tagId);

			t.env->DeleteLocalRef(jTitle);
			t.env->DeleteLocalRef(jMsg);
			t.env->DeleteLocalRef(t.classID);
	}
}

void CCEPlugin::closeAlarmNotification(int id)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
		, "com/zerogame/pluginlibs/AppPlugin"
		, "closeAlarmNotification"
		, "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID,id);
		t.env->DeleteLocalRef(t.classID);
	}
}
void CCEPlugin::setIdleTimerDisabled(bool disabled){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "setIdleTimerDisabled"
                                       , "(Z)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID,disabled);
        t.env->DeleteLocalRef(t.classID);
    }
}
void CCEPlugin::playVibration(){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "playVibration"
                                       , "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}



double CCEPlugin::getLatitude(){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "getInstance"
                                       , "()Lcom/zerogame/pluginlibs/AppPlugin;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/zerogame/pluginlibs/AppPlugin"
                                     , "getLatitude"
                                     , "()D")) {
            jdouble _double = (jdouble)t.env->CallDoubleMethod((jobject)obj, t.methodID);
            
            t.env->DeleteLocalRef(t.classID);
            
            return (double)_double;
        }
    }
    
    return 0;
}

double CCEPlugin::getLongitude(){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "getInstance"
                                       , "()Lcom/zerogame/pluginlibs/AppPlugin;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/zerogame/pluginlibs/AppPlugin"
                                     , "getLongitude"
                                     , "()D")) {
            jdouble _double = (jdouble)t.env->CallDoubleMethod((jobject)obj, t.methodID);
            
            t.env->DeleteLocalRef(t.classID);
            
            return (double)_double;
        }
    }
    
    return 0;
}

bool CCEPlugin::locationServicesEnabled(){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "getInstance"
                                       , "()Lcom/zerogame/pluginlibs/AppPlugin;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/zerogame/pluginlibs/AppPlugin"
                                     , "locationServicesEnabled"
                                     , "()Z")) {
            jboolean _boolean = (jdouble)t.env->CallBooleanMethod((jobject)obj, t.methodID);
            
            t.env->DeleteLocalRef(t.classID);
            return (bool)_boolean;
        }
    }
    
    return false;
}
void CCEPlugin::requestLocationService(){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/zerogame/pluginlibs/AppPlugin"
                                       , "requestLocationService"
                                       , "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}
