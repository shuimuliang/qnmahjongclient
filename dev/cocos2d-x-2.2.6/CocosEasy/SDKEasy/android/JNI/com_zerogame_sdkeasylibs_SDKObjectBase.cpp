#include "com_zerogame_sdkeasylibs_SDKObjectBase.h"
#include <jni.h>
#include <android/log.h>
#include "CocosEasy.h"
#include "../../SDKConstants.h"

 void  Java_com_zerogame_sdkeasylibs_SDKObjectBase_loginResult
(JNIEnv *env, jobject obj, jboolean result){
	  CSDKManager::getInstance()->loginCallBack(result);
}

void  Java_com_zerogame_sdkeasylibs_SDKObjectBase_logoutResult
(JNIEnv *env, jobject obj){
    CSDKManager::getInstance()->logoutCallBack();
}

void  Java_com_zerogame_sdkeasylibs_SDKObjectBase_thirdLoginResult
(JNIEnv *env, jobject obj, jint code, jstring info){
    const char* str = env->GetStringUTFChars(info, false);
    CSDKManager::getInstance()->thirdLoginCallBack(code,str);
    env->ReleaseStringUTFChars(info, str);
}

 void  Java_com_zerogame_sdkeasylibs_SDKObjectBase_payResult
(JNIEnv *env, jobject obj, jboolean result, jstring order){
    const char* str = env->GetStringUTFChars(order, false);
    CSDKManager::getInstance()->buyCallBack(result,str);
    
    env->ReleaseStringUTFChars(order, str);
}

void JNICALL Java_com_zerogame_sdkeasylibs_SDKObjectBase_videoCallBack
(JNIEnv *env, jobject obj, jint status){
    CSDKManager::getInstance()->videoCallBack(status);
}


void  Java_com_zerogame_sdkeasylibs_SDKObjectBase_shareResult
(JNIEnv *env, jobject obj, jboolean result){
    CSDKManager::getInstance()->shareCallBack(result);
}

 void  Java_com_zerogame_sdkeasylibs_SDKObjectBase_initSdk
(JNIEnv *env, jobject obj,jint code){
	CSDKManager::getInstance()->initPlatform(code);
}

jstring JNICALL Java_com_snowcat_casino_AppSDKManager_getGooglePublicKey
(JNIEnv *env, jobject obj){
    int channelID = CSDKManager::getInstance()->getChannelID();
    if (channelID == SDK_CHANNEL_GOOGLE_PLAY) {
        return env->NewStringUTF(googlePublicKey_USA);
    }else if(channelID == SDK_CHANNEL_MALAI){
        return env->NewStringUTF(googlePublicKey_MaLai);
    }else if(channelID == SDK_CHANNEL_ARABIC){
        return env->NewStringUTF(googlePublicKey_Arabic);
    }
    return env->NewStringUTF(googlePublicKey_USA);
}

jstring JNICALL Java_com_zerogame_sdkeasylibs_SDKObjectBase_getSDKPrivateKey
(JNIEnv *env, jobject obj){
    int channelID = CSDKManager::getInstance()->getChannelID();
    const char* key = "";
    
    if (channelID == SDK_CHANNEL_MALAI) {
        key = paymentWallPrivate;
    }
    
    return env->NewStringUTF(key);
}

jstring JNICALL Java_com_zerogame_sdkeasylibs_SDKObjectBase_getSDKPublicKey
(JNIEnv *env, jobject obj){
    int channelID = CSDKManager::getInstance()->getChannelID();
    const char* key = "";
    
    if (channelID == SDK_CHANNEL_MALAI) {
        key = paymentWallPublic;
    }
    
    return env->NewStringUTF(key);
}

void  Java_com_zerogame_sdkeasylibs_SDKObjectBase_setDeviceToken
(JNIEnv *env, jobject obj, jstring token){
    const char* str = env->GetStringUTFChars(token, false);
    CSDKManager::getInstance()->setDeviceToken(str);
    env->ReleaseStringUTFChars(token, str);
}


