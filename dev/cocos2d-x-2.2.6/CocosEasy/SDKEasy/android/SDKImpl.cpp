//
//  SDKImpl.cpp
//  client
//
//  Created by liulihua on 14-4-8.
//
//

#include "../SDKManager.h"
#include "platform/android/jni/JniHelper.h"

void CSDKManager::initPlatform(int code)
{
    m_ChannelID = code;
    
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "initChannelSDK"
                                     , "()V")) {
            t.env->CallVoidMethod((jobject)obj, t.methodID);
        }
        
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "getChannelId"
                                     , "()I")) {
            jint _int = (jint)t.env->CallIntMethod((jobject)obj, t.methodID);
            
            t.env->DeleteLocalRef(t.classID);
            m_ChannelID = (int)_int;
        }
    }
}

LanguageType CSDKManager::getDesignLanguage(){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "getDesignLanguage"
                                     , "()I")) {
            jint _int = (jint)t.env->CallIntMethod((jobject)obj, t.methodID);
            
            t.env->DeleteLocalRef(t.classID);
            
            return (LanguageType)(int)_int;
        }
    }
    return kLanguageEnglish;
}

void CSDKManager::preLoadGoods(const char* goodsList){
}


void CSDKManager::login()
{
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);

        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "loginSDK"
                                     , "()V")) {
            t.env->CallVoidMethod((jobject)obj, t.methodID);
        }
    }

}
void CSDKManager::logout(bool iscallback)
{
    
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "logoutSDK"
                                     , "(Z)V")) {
            t.env->CallVoidMethod((jobject)obj, t.methodID, iscallback);
        }
    }
}
void CSDKManager::userCenter()
{
    
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "userCenterSDK"
                                     , "()V")) {
            t.env->CallVoidMethod((jobject)obj, t.methodID);
        }
    }
}
bool CSDKManager::isLogined()
{
    bool ret = false;
    
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "isLoginSDK"
                                     , "()Z")) {
            ret = t.env->CallBooleanMethod((jobject)obj, t.methodID);
        }
        
    }
    return ret;
}

bool CSDKManager::isInited()
{
    bool ret = false;

    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "isInitSDK"
                                     , "()Z")) {
            ret = (bool)t.env->CallBooleanMethod((jobject)obj, t.methodID);
        }
   
    }
    return ret;
}

const std::string CSDKManager::getAccountName(){
    
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "getAccountName"
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
const std::string CSDKManager::getAccountID(){

    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "getAccountID"
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
const std::string CSDKManager::getSessionID(){
    
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "getSessionID"
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
void CSDKManager::unInit(){
    
}
void CSDKManager::pay(const char* extraData)
{
    
    JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "paySDK"
                                     , "(Ljava/lang/String;)V")) {
            
            jstring jExtraData = t.env->NewStringUTF(extraData);
            
            t.env->CallVoidMethod((jobject)obj, t.methodID,jExtraData);
            t.env->DeleteLocalRef(jExtraData);
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

void CSDKManager::share(const char* extraData)
{
    
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "share"
                                     , "(Ljava/lang/String;)V")) {
            
            jstring jExtraData = t.env->NewStringUTF(extraData);
            
            t.env->CallVoidMethod((jobject)obj, t.methodID,jExtraData);
            t.env->DeleteLocalRef(jExtraData);
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

bool CSDKManager::isWXAppInstalled()
{
    
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "isWXAppInstalled"
                                     , "()Z")) {
            
            bool ret = t.env->CallBooleanMethod((jobject)obj, t.methodID);
            t.env->DeleteLocalRef(t.classID);
            
            if (ret) {
            }
            return ret;
        }
    }
}

void CSDKManager::showWebPayView(std::string url,
                                 std::string jsonData){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "showWebPayView"
                                     , "(Ljava/lang/String;Ljava/lang/String;)V")) {
            jstring jUrl = t.env->NewStringUTF(url.c_str());
            jstring jExtraData = t.env->NewStringUTF(jsonData.c_str());
            
            t.env->CallVoidMethod((jobject)obj, t.methodID,jUrl,jExtraData);
            t.env->DeleteLocalRef(jUrl);
            t.env->DeleteLocalRef(jExtraData);
            t.env->DeleteLocalRef(t.classID);
            
        }
    }
}

void CSDKManager::payWithPayPal(const char* parmas){
}

const std::string CSDKManager::getMissOrderList()
{
    if (m_ChannelID == SDK_CHANNEL_GOOGLE_PLAY) {
        
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t
                                           , "com/snowcat/casino/AppSDKManager"
                                           , "getInstance"
                                           , "()Lcom/snowcat/casino/AppSDKManager;")) {
            jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
            if (JniHelper::getMethodInfo(t
                                         , "com/snowcat/casino/AppSDKManager"
                                         , "getMissOrderList"
                                         , "()Ljava/lang/String;")) {
                jstring jstr = (jstring)t.env->CallObjectMethod((jobject)obj, t.methodID);
                
                CCString *ret = new CCString(JniHelper::jstring2string(jstr).c_str());
                t.env->DeleteLocalRef(jstr);
                t.env->DeleteLocalRef(t.classID);
                
                return ret->m_sString;
            }
        }
        
        return NULL;
    }
    return "";
}


const std::string CSDKManager::getOrderInfoAndRemove(const char* orderID)
{
    if (m_ChannelID == SDK_CHANNEL_GOOGLE_PLAY) {
        
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t
                                           , "com/snowcat/casino/AppSDKManager"
                                           , "getInstance"
                                           , "()Lcom/snowcat/casino/AppSDKManager;")) {
            jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
            if (JniHelper::getMethodInfo(t
                                         , "com/snowcat/casino/AppSDKManager"
                                         , "getOrderInfoAndRemove"
                                         , "(Ljava/lang/String;)Ljava/lang/String;")) {
                
                jstring jOrderID = t.env->NewStringUTF(orderID);
                jstring jstr = (jstring)t.env->CallObjectMethod((jobject)obj, t.methodID,jOrderID);
                
                t.env->DeleteLocalRef(t.classID);
                t.env->DeleteLocalRef(jOrderID);
                
                CCString *ret = new CCString(JniHelper::jstring2string(jstr).c_str());
                t.env->DeleteLocalRef(jstr);
                
                return ret->m_sString;
            }
        }
        
        return NULL;
    }
    return "";
}

void CSDKManager::createRoleAnayasis(unsigned int serverId){
//    if (m_ChannelID == SDK_CHANNEL_PPS) {
//        JniMethodInfo t;
//        
//        if (JniHelper::getStaticMethodInfo(t
//                                           , "com/snowcat/casino/AppSDKManager"
//                                           , "getInstance"
//                                           , "()Lcom/snowcat/casino/AppSDKManager;")) {
//            jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
//            if (JniHelper::getMethodInfo(t
//                                         , "com/snowcat/casino/AppSDKManager"
//                                         , "createRoleAnayasis"
//                                         , "(I)V")) {
//                t.env->CallVoidMethod((jobject)obj, t.methodID,(int)serverId);
//                t.env->DeleteLocalRef(t.classID);
//            }
//        }
//    }
}

void CSDKManager::enterMainSceneAnayasis(unsigned int serverId){
//    if (m_ChannelID == SDK_CHANNEL_PPS) {
//        
//        JniMethodInfo t;
//        if (JniHelper::getStaticMethodInfo(t
//                                           , "com/snowcat/casino/AppSDKManager"
//                                           , "getInstance"
//                                           , "()Lcom/snowcat/casino/AppSDKManager;")) {
//            jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
//            if (JniHelper::getMethodInfo(t
//                                         , "com/snowcat/casino/AppSDKManager"
//                                         , "enterMainSceneAnayasis"
//                                         , "(I)V")) {
//                t.env->CallVoidMethod((jobject)obj, t.methodID,(int)serverId);
//                t.env->DeleteLocalRef(t.classID);
//            }
//        }
//        
//    }
}

void CSDKManager::thirdLogin(int loginType){ // 第三方登陆
    
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "thirdLogin"
                                     , "(I)V")) {
            t.env->CallVoidMethod((jobject)obj, t.methodID,(int)loginType);
            t.env->DeleteLocalRef(t.classID);
        }
    }
}
AppThirdLoginType CSDKManager::getLoginType()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "getLoginType"
                                     , "()I")) {
            jint _int = (jint)t.env->CallIntMethod((jobject)obj, t.methodID);
            
            t.env->DeleteLocalRef(t.classID);
            
            return (AppThirdLoginType)(int)_int;
        }
    }
    return kAppThirdLoginTypeNone;
}

void CSDKManager::playVideo(const char* parmas){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       , "com/snowcat/casino/AppSDKManager"
                                       , "getInstance"
                                       , "()Lcom/snowcat/casino/AppSDKManager;")) {
        jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        if (JniHelper::getMethodInfo(t
                                     , "com/snowcat/casino/AppSDKManager"
                                     , "playVideo"
                                     , "(Ljava/lang/String;)V")) {
            jstring jParm = t.env->NewStringUTF(parmas);
            
            t.env->CallVoidMethod((jobject)obj, t.methodID,jParm);
            t.env->DeleteLocalRef(jParm);
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

const std::string CSDKManager::getSimCountryISO(){
    return "cn";
}


