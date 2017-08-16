LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libGCloudVoice
LOCAL_SRC_FILES:= ../../../../../CocosEasy/GCloudVoice/libs/Android/armeabi-v7a/libGCloudVoice.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dlua_shared

LOCAL_MODULE_FILENAME := libcocos2dlua

APP_CPPFLAGS := -Wno-error=format-security

LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog


#通过查找获取所有工程文件列表  
CPP_FILE_LIST :=$(wildcard $(LOCAL_PATH)/../../../../../CocosEasy/CocosExt/*.cpp) \
                $(wildcard $(LOCAL_PATH)/../../../../../CocosEasy/Common/*.cpp) \
                $(wildcard $(LOCAL_PATH)/../../../../../CocosEasy/Graph/*.cpp) \
                $(wildcard $(LOCAL_PATH)/../../../../../CocosEasy/Physics/*.cpp) \
                $(wildcard $(LOCAL_PATH)/../../../../../CocosEasy/Script/*.cpp) \
                $(wildcard $(LOCAL_PATH)/../../../../../CocosEasy/Utility/*.cpp)\
                $(wildcard $(LOCAL_PATH)/../../../../../CocosEasy/Utility/HTTP/*.cpp)\
                $(wildcard $(LOCAL_PATH)/../../../../../CocosEasy/Utility/ThreadPool/*.cpp)\
                $(wildcard $(LOCAL_PATH)/../../../../../CocosEasy/Utility/Socket/*.cpp)\
                $(wildcard $(LOCAL_PATH)/../../../../../extensions/cells/*.cpp)\
                $(wildcard $(LOCAL_PATH)/../../../../../extensions/dfont/*.cpp)\
                $(wildcard $(LOCAL_PATH)/../../../../../extensions/RichControls/*.cpp)\

C_FILE_LIST :=  $(wildcard $(LOCAL_PATH)/../../../../../CocosEasy/jansson/*.c)\
			    $(wildcard $(LOCAL_PATH)/../../../Classes/lua-cjson/*.c)\
                $(wildcard $(LOCAL_PATH)/../../../../../CocosEasy/Utility/*.c)\

#加入工程文件，之所以不直接加是需要进行一个LOCAL_PATH的替换 
LOCAL_SRC_FILES := $(CPP_FILE_LIST:$(LOCAL_PATH)/%=%)\
hellolua/main.cpp\
../../../Classes/AppDelegate.cpp\
../../../Classes/Config.cpp\
../../../Classes/pbc/pbc-lua.c\
../../../Classes/lpack/lpack.c\
../../../Classes/network/Proto.cpp\
../../../Classes/scene/CheckUpdateScene.cpp\
../../../Classes/LuaProxy/LuaEventHandler.cpp\
../../../Classes/LuaProxy/LuaEventHandler.hpp\
../../../Classes/LuaProxy/LuaProxy.hpp\
../../../Classes/LuaProxy/LuaTableView.hpp\
../../../Classes/LuaProxy/tolua/luaopen_LuaProxy.cpp\
../../../Classes/LuaProxy/ui/CursorTextField.cpp\
../../../Classes/LuaProxy/ui/UIUtil.cpp\
../../../Classes/LuaBox2D/LuaBox2D.cpp\
../../../Classes/LuaTakingGame/TalkingDataGA-Lua.cpp\
../../../../../CocosEasy/SDKEasy/android/JNI/com_zerogame_sdkeasylibs_SDKObjectBase.cpp\
../../../../../CocosEasy/Plugin/android/JNI/com_zerogame_pluginlibs_AppPlugin.cpp\
../../../../../CocosEasy/Plugin/android/CocosEasyPlugin.cpp\
../../../../../CocosEasy/Plugin/android/AndroidImagePicker.cpp\
../../../../../CocosEasy/Plugin/ImagePicker.cpp\
../../../../../CocosEasy/SDKEasy/SDKConstants.cpp\
../../../../../CocosEasy/SDKEasy/IMManager.cpp\
../../../../../CocosEasy/SDKEasy/SDKManager.cpp\
../../../../../CocosEasy/SDKEasy/android/SDKImpl.cpp\
../../../../../CocosEasy/Analysis/Analysis.cpp\
../../../../../CocosEasy/Analysis/android/AnalysisImpl.cpp\
../../../../../CocosEasy/Model/AsyncSocket.cpp\
../../../../../CocosEasy/Model/CocosEasyManger.cpp\
../../../../../CocosEasy/Model/DBManager.cpp\
../../../../../CocosEasy/Model/ResourceLoader.cpp\
../../../../../CocosEasy/Model/SceneTransit.cpp\
../../../../../CocosEasy/Model/TimerManager.cpp\
../../../../../CocosEasy/Shader/CEGLProgram.cpp\
../../../../../CocosEasy/CocosExt/android/CCWebView.cpp\

LOCAL_SRC_FILES += $(C_FILE_LIST:$(LOCAL_PATH)/%=%)  

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes\
				$(LOCAL_PATH)/../../../Classes/pbc\
				$(LOCAL_PATH)/../../../Classes/lpack\
				$(LOCAL_PATH)/../../../Classes/network\
				$(LOCAL_PATH)/../../../Classes/scene\
				$(LOCAL_PATH)/../../../../../scripting/lua\
				$(LOCAL_PATH)/../../../../../CocosEasy\
				$(LOCAL_PATH)/../../../../../CocosEasy/Utility\
				$(LOCAL_PATH)/../../../../../CocosEasy/Plugin\
				$(LOCAL_PATH)/../../../../../CocosEasy/jansson\
				$(LOCAL_PATH)/../../../../../CocosEasy/CocosExt\
				$(LOCAL_PATH)/../../../Classes/LuaProxy\
				$(LOCAL_PATH)/../../../Classes/LuaProxy/tolua\
				$(LOCAL_PATH)/../../../Classes/LuaProxy/ui\
				$(LOCAL_PATH)/../../../Classes/lua-cjson\
				$(LOCAL_PATH)/../../../Classes/LuaTakingGame\
				$(LOCAL_PATH)/../../../../../CocosEasy/GCloudVoice/include\

LOCAL_STATIC_LIBRARIES := curl_static_prebuilt
LOCAL_STATIC_LIBRARIES += bugly_crashreport_cocos_static


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += pbc
LOCAL_WHOLE_STATIC_LIBRARIES += wxsqlite3_static
LOCAL_WHOLE_STATIC_LIBRARIES += luasocket_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx-talkingdata

LOCAL_SHARED_LIBRARIES += libGCloudVoice

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,scripting/lua/proj.android)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
$(call import-module,../third_party/pbc-master)
$(call import-module,external/wxsqlite3)
$(call import-module,../third_party/luasocket-2.0.2)
$(call import-module,cocos2dx/platform/third_party/android/source/TalkingDataGameAnalytics/android)
$(call import-module,external/bugly)

LOCAL_SHARED_LIBRARIES := libcocklogic

LOCAL_SRC_FILES := prebuilt/libcocklogic.so

include $(CLEAR_VARS)

LOCAL_SHARED_LIBRARIES := libtnet-2.0.17-agoo

LOCAL_SRC_FILES := prebuilt/libtnet-2.0.17-agoo.so

include $(CLEAR_VARS)
