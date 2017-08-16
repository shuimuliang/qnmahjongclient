
#include "platform/android/jni/JniHelper.h"

#include "../ImagePicker.h"
NS_CC_EASY_BEGIN

void ImagePicker::selectPic(int clipWidth){
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
		, "com/zerogame/pluginlibs/AppPlugin"
		, "selectImageNotification"
		, "(I)V")) {

			t.env->CallStaticVoidMethod(t.classID, t.methodID,clipWidth);
	}
}




NS_CC_EASY_END