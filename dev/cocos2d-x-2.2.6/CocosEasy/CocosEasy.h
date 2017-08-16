#ifndef __CE_COCOS_EASY_H__
#define __CE_COCOS_EASY_H__

// cocos2d-x
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

// common
#include "Common/CocosEasyDef.h"
#include "Common/CommInclude.h"

// CocosExt
#include "CocosExt/ArmatureExt.h"
#include "CocosExt/DictionaryExt.h"
#include "CocosExt/TmxExt.h"
#include "CocosExt/WidgetEvent.h"
#include "CocosExt/UIPageViewExt.h"
#include "CocosExt/CCLString.h"
#include "CocosExt/ActionInternalExt.h"
#include "CocosExt/SpriteExt.h"
#include "CocosExt/UITabViewExt.h"
#include "CocosExt/CCScollPanelViewExt.h"
#include "CocosExt/CCScrollViewExt.h"
#include "CocosExt/UICircleViewExt.h"
#include "CocosExt/HttpRequestExt.h"
#include "CocosExt/RichTextExt.h"
#include "CocosExt/WStrExt.h"
#include "CocosExt/UIChoosePersonViewExt.h"
#include "CocosExt/WheelRotateExt.h"
#include "CocosExt/WordTable.h"
#include "CocosExt/ImeChatBox.h"
#include "CocosExt/CCWebView.h"

// model
#include "Model/ResourceLoader.h"
#include "Model/CocosEasyManger.h"
#include "Model/SceneTransit.h"
#include "Model/DBManager.h"
#include "Model/DownloadManager.h"
#include "Model/TimerManager.h"
#include "Model/AsyncSocket.h"

// Graph
#include "Graph/Graph.h"
#include "Graph/Geometry2D.h"
#include "Graph/DebugDraw.h"

// Utility
#include "Utility/EventManager.h"
#include "Utility/GContainer.h"
#include "Utility/DataParser.h"
#include "Utility/IFL.h"
#include "Utility/DynamicDownload.h"
#include "Utility/EventDef.h"
#include "Utility/Log.h"
#include "Utility/UploadManager.h"
#include "Utility/oi_tea.h"
#include "Utility/base64Easy.h"

// script
#include "Script/LuaCocosEasy.h"
#include "Script/Lua_CocosEasy_Manual.h"
#include "Script/CELuaEngine.h"

//CocosPlugin
#include "Plugin/CocosEasyPlugin.h"
#include "Plugin/ImagePicker.h"


//SDK
#include "SDKEasy/SDKManager.h"

//IM
#if CASINO_VOICE
#include "SDKEasy/IMManager.h"
#endif

//TokenData
#include "Analysis/Analysis.h"

// shader
#include "Shader/CEGLProgram.h"



USING_NS_CC_EASY;

#endif // !__CE_COCOS_EASY_H__
