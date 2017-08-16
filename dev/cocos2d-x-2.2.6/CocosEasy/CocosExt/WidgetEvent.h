#ifndef __CE_WIDGET_EVENT_H__
#define __CE_WIDGET_EVENT_H__

#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN

#define EVT_PUSHDOWN(name) name##_PushDown
#define EVT_MOVE(name) name##_Move
#define EVT_RELEASE(name) name##_Release
#define EVT_CANCEL(name) name##_Cancel

#define DEF_WIDGET_EVT(name) \
	void EVT_PUSHDOWN(name)(CCObject* pSender); \
	void EVT_MOVE(name)(CCObject* pSender); \
	void EVT_RELEASE(name)(CCObject* pSender); \
	void EVT_CANCEL(name)(CCObject* pSender); \

#define DCL_WIDGET_EVT_PUSHDOWN(name) void EVT_PUSHDOWN(name)(CCObject* pSender)
#define DCL_WIDGET_EVT_MOVE(name) void EVT_MOVE(name)(CCObject* pSender)
#define DCL_WIDGET_EVT_RELEASE(name) void EVT_RELEASE(name)(CCObject* pSender)
#define DCL_WIDGET_EVT_CANCEL(name) void EVT_CANCEL(name)(CCObject* pSender)


#define CE_GET_SCENE_NODE(type,node,tag,component) CE_GET_COMPONENT(type,node->getChildByTag(tag),component)
#define CE_GET_COMPONENT(type,node,component) (type*)(static_cast<cocos2d::extension::CCComRender*>(node->getComponent(component)))->getNode();

NS_CC_EASY_END


#endif