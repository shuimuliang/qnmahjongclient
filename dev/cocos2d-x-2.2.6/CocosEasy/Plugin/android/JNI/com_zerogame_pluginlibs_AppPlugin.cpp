#include "com_zerogame_pluginlibs_AppPlugin.h"
#include <jni.h>
#include <android/log.h>
#include "CocosEasy.h"
#include "../../ImagePicker.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

/*
 * Class:     com_zerogame_client_AppPlugin
 * Method:    callMsgHandler
 * Signature: (I)V
 */
 void  Java_com_zerogame_pluginlibs_AppPlugin_callMsgHandler
  (JNIEnv *env, jobject obj, jint handler,jint flag){

	   CCArray* pArrayArgs = CCArray::createWithCapacity(1);
	                pArrayArgs->addObject(CCInteger::create(flag));
	                CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(handler,pArrayArgs);


}

/*
 * Class:     com_zerogame_client_AppPlugin
 * Method:    callMsgHandler
 * Signature: (I)V
 */
void  Java_com_zerogame_pluginlibs_AppPlugin_callMsgHandler2
(JNIEnv *env, jobject obj,jint index){
    CCEPlugin::messageBox2Callback(index);
}

 void  Java_com_zerogame_pluginlibs_AppPlugin_onNotifySelectImage
	 (JNIEnv *env, jobject obj, jstring filePath){
		 std:string path = JniHelper::jstring2string((jstring)filePath);
		 ImagePicker::getInstance()->selectPicFinished(path);
 }


