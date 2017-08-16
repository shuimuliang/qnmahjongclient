LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := mvc_static

LOCAL_MODULE_FILENAME := libmvc

LOCAL_SRC_FILES := \
				  core/Controller.cpp\
				  core/Facade.cpp\
				  core/Modele.cpp\
				  core/Observer.cpp\
				  core/View.cpp\
				  interface/ICommand.cpp\
				  interface/IController.cpp\
				  interface/IMediator.cpp\
				  interface/INotification.cpp\
				  interface/IProxy.cpp\
				  interface/ISend.cpp\
				  pattern/obsever/Notification.cpp\
				  CCFacade.cpp\
				  CCIModule.cpp\
				  CCModuleManager.cpp\
				  CCNetFacade.cpp\

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../\
$(LOCAL_PATH)/../../../../extensions\
$(LOCAL_PATH)/../Network\
#$(LOCAL_PATH)/../UI\
#$(LOCAL_PATH)/../Common\
$(LOCAL_PATH)/../Mvc\
#$(LOCAL_PATH)/../lua\

#$(LOCAL_PATH)/../../Module

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static netWork_static

include $(BUILD_STATIC_LIBRARY)
#$(call import-add-path,$(LOCAL_PATH)/../../../../)
#$(call import-add-path,$(LOCAL_PATH)/../../../../cocos2dx/platform/third_party/android/prebuilt)

#$(call import-module,cocos2dx) \
#$(call import-module,extensions)\
#$(call import-module,CocosDenshion/android) 
