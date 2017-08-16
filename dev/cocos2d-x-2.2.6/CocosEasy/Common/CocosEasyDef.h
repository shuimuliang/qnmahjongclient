#ifndef __CE_COCOS_EASY_DEF_H__
#define __CE_COCOS_EASY_DEF_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "CommInclude.h"
#include "core_ext.h"
#include "Script/ScriptObject.h"

#ifdef __cplusplus
#define NS_CC_EASY_BEGIN                     namespace cocoseasy {
#define NS_CC_EASY_END                       }
#define USING_NS_CC_EASY					using namespace cocoseasy
#else
#define NS_CC_EASY_BEGIN 
#define NS_CC_EASY_END 
#define USING_NS_CC_EASY 
#endif  

#define CE_SWITCH_BEGIN(condition) switch (condition) \
	{
#define CE_CASE(val) case val:
#define CE_DEFAULT default:
#define CE_SWITCH_END }

#ifdef COCOS2D_DEBUG
#define astBack {assert(false); return ;}
#define astReturn(val) {assert(false); return (val);}
#define astBreak {assert(false); break;}
#else
#define astBack {return ;}
#define astReturn(val) {return (val);}
#define astBreak {break;}
#endif // COCOS2D_DEBUG



USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

#endif