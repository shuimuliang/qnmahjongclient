#include "DPIJni.h"
#include "jni/JniHelper.h"

USING_NS_CC;

extern "C" {

int getDPIJNI()
{
    JniMethodInfo t;
    jint ret = -1;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "getDPI", "()I")) {
        ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}
    
void updateAlbum(const char* imgPath){
    JniMethodInfo t;
    jint ret = -1;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "updateAlbum", "(Ljava/lang/String;)V")) {
        jstring jparmas = t.env->NewStringUTF(imgPath);
            
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jparmas);
            
        t.env->DeleteLocalRef(t.classID);
    }
}
    
} // extern "C"
