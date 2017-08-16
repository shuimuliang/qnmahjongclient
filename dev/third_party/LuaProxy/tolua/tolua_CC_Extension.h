#pragma once
#ifndef __TOLUA_CC_EXTENSION_H__
#define __TOLUA_CC_EXTENSION_H__

extern "C" {
#include "tolua++.h"
#include "tolua_fix.h"
}
#include "tolua_CCBAnimationManager.h"
//#include "tolua_CCControl.h"
//#include "tolua_CCEditBox.h"
#include "tolua_CCScrollView.h"
#include "tolua_CCTableView.h"
//#include "tolua_CCScale9Sprite.h"
#include "tolua_CCHttpClient.h"
#include "tolua_CCGLProgram.h"

//######################################## CCPhysicsWorld ##########################
//######################################## CCPhysicsSprite ##########################

//CCClippingNode
static int tolua_CCClippingNode_create(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertable(l, 1, "CCClippingNode", 0, &err)
		|| !(tolua_isnoobj(l, 2, &err) || tolua_isusertype(l, 2, "CCNode", 0, &err))
		|| !tolua_isnoobj(l, 3, &err)){
		tolua_error(l,"#ferror in function 'CCClippingNode.create'.",&err);
		return 0;
	}
#endif
	CCNode *s = (CCNode *)tolua_tousertype(l, 2, NULL);
	CCClippingNode *o = s ? CCClippingNode::create() : CCClippingNode::create(s);
	tolua_pushusertype(l, o, "CCClippingNode");
	return 1;
}

static int tolua_CCClippingNode_getStencil(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCClippingNode", 0, &err) || !tolua_isnoobj(l, 2, &err)){
		tolua_error(l,"#ferror in function 'CCClippingNode.getStencil'.",&err);
		return 0;
	}
#endif
	CCClippingNode *o = (CCClippingNode *)tolua_tousertype(l, 1, NULL);
	tolua_pushusertype(l, o->getStencil(), "CCNode");
	return 1;
}

static int tolua_CCClippingNode_setStencil(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCClippingNode", 0, &err) || !tolua_isusertype(l, 2, "CCNode", 0, &err)){
		tolua_error(l,"#ferror in function 'CCClippingNode.setStencil'.",&err);
		return 0;
	}
#endif
	CCClippingNode *o = (CCClippingNode *)tolua_tousertype(l, 1, NULL);
	CCNode *s = (CCNode *)tolua_tousertype(l, 2, NULL);
	o->setStencil(s);
	return 1;
}

static int tolua_CCClippingNode_getAlphaThreshold(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCClippingNode", 0, &err) || !tolua_isnoobj(l, 2, &err)){
		tolua_error(l,"#ferror in function 'CCClippingNode.getAlphaThreshold'.",&err);
		return 0;
	}
#endif
	CCClippingNode *o = (CCClippingNode *)tolua_tousertype(l, 1, NULL);
	tolua_pushnumber(l, o->getAlphaThreshold());
	return 1;
}

static int tolua_CCClippingNode_setAlphaThreshold(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCClippingNode", 0, &err) || !tolua_isnumber(l, 2, 0, &err)){
		tolua_error(l,"#ferror in function 'CCClippingNode.setAlphaThreshold'.",&err);
		return 0;
	}
#endif
	CCClippingNode *o = (CCClippingNode *)tolua_tousertype(l, 1, NULL);
	o->setAlphaThreshold( tolua_tonumber(l, 2, 0) );
	return 1;
}

static int tolua_CCClippingNode_isInverted(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCClippingNode", 0, &err) || !tolua_isnoobj(l, 2, &err)){
		tolua_error(l,"#ferror in function 'CCClippingNode.isInverted'.",&err);
		return 0;
	}
#endif
	CCClippingNode *o = (CCClippingNode *)tolua_tousertype(l, 1, NULL);
	tolua_pushboolean(l, o->isInverted());
	return 1;
}

static int tolua_CCClippingNode_setInverted(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCClippingNode", 0, &err) || !tolua_isboolean(l, 2, 0, &err)){
		tolua_error(l,"#ferror in function 'CCClippingNode.setInverted'.",&err);
		return 0;
	}
#endif
	CCClippingNode *o = (CCClippingNode *)tolua_tousertype(l, 1, NULL);
	o->setInverted( tolua_toboolean(l, 2, 0) == 1 );
	return 1;
}

//CCDrawNode
static int tolua_CCDrawNode_create(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertable(l, 1, "CCDrawNode", 0, &err) || !tolua_isnoobj(l, 2, &err)){
		tolua_error(l,"#ferror in function 'CCDrawNode.create'.",&err);
		return 0;
	}
#endif
	CCDrawNode *o = CCDrawNode::create();
	tolua_pushusertype(l, o, "CCDrawNode");
	return 1;
}

static int tolua_CCDrawNode_drawDot(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCDrawNode", 0, &err) || !tolua_isusertype(l, 2, "CCPoint", 0, &err)
		|| !tolua_isnumber(l, 3, 0, &err) || !tolua_isusertype(l, 4, "ccColor4F", 0, &err)){
		tolua_error(l,"#ferror in function 'CCDrawNode.drawDot'.",&err);
		return 0;
	}
#endif
	CCDrawNode *o = (CCDrawNode *)tolua_tousertype(l, 1, NULL);
	CCPoint p = *((CCPoint *)tolua_tousertype(l, 2, NULL));
	ccColor4F c = *((ccColor4F *)tolua_tousertype(l, 4, NULL));
	o->drawDot( p, (float)tolua_tonumber(l, 3, 0), c );
	return 1;
}

static int tolua_CCDrawNode_drawSegment(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCDrawNode", 0, &err) || !tolua_isusertype(l, 2, "CCPoint", 0, &err)
		|| !tolua_isusertype(l, 3, "CCPoint", 0, &err) || !tolua_isnumber(l, 4, 0, &err) || !tolua_isusertype(l, 5, "ccColor4F", 0, &err)){
		tolua_error(l,"#ferror in function 'CCDrawNode.drawSegment'.",&err);
		return 0;
	}
#endif
	CCDrawNode *o = (CCDrawNode *)tolua_tousertype(l, 1, NULL);
	CCPoint fp = *((CCPoint *)tolua_tousertype(l, 2, NULL));
	CCPoint tp = *((CCPoint *)tolua_tousertype(l, 3, NULL));
	ccColor4F c = *((ccColor4F *)tolua_tousertype(l, 5, NULL));
	o->drawSegment(fp, tp, (float)tolua_tonumber(l, 4, 0), c);
	return 1;
}

static int tolua_CCDrawNode_drawPolygon(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCDrawNode", 0, &err) || !tolua_isusertype(l, 2, "CCPointArray", 0, &err)
		|| !tolua_isnumber(l, 3, 0, &err) || !tolua_isusertype(l, 4, "ccColor4F", 0, &err) || !tolua_isnumber(l, 5, 0, &err)
		|| !tolua_isusertype(l, 6, "ccColor4F", 0, &err)){
		tolua_error(l,"#ferror in function 'CCDrawNode.drawPolygon'.",&err);
		return 0;
	}
#endif
	CCDrawNode *o = (CCDrawNode *)tolua_tousertype(l, 1, NULL);
	CCPointArray *vertsArr = (CCPointArray *)tolua_tousertype(l, 2, NULL);
	int cnt = vertsArr->count();
	CCPoint *verts = new CCPoint[cnt];
	for (int i=0; i<cnt; i++) verts[i] = vertsArr->getControlPointAtIndex(i);	

	ccColor4F fc = *((ccColor4F *)tolua_tousertype(l, 4, NULL));
	ccColor4F bc = *((ccColor4F *)tolua_tousertype(l, 6, NULL));
	o->drawPolygon(verts, (int)tolua_tonumber(l, 3, 0), fc, (float)tolua_tonumber(l, 5, 0), bc);

	delete[] verts;
	verts = NULL;
	return 1;
}

static int tolua_CCDrawNode_clear(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCDrawNode", 0, &err) || !tolua_isnoobj(l, 2, &err)){
		tolua_error(l,"#ferror in function 'CCDrawNode.clear'.",&err);
		return 0;
	}
#endif
	CCDrawNode *o = (CCDrawNode *)tolua_tousertype(l, 1, NULL);
	o->clear();
	return 1;
}

static int tolua_CCDrawNode_getBlendFunc(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCDrawNode", 0, &err) || !tolua_isnoobj(l, 2, &err)){
		tolua_error(l,"#ferror in function 'CCDrawNode.getBlendFunc'.",&err);
		return 0;
	}
#endif
 {
  CCDrawNode *o = (CCDrawNode *)tolua_tousertype(l, 1, NULL);
#ifndef TOLUA_RELEASE
  if (!o) tolua_error(l,"invalid 'self' in function 'getBlendFunc'", NULL);
#endif
  {
   ccBlendFunc tolua_ret = (ccBlendFunc)  o->getBlendFunc();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((ccBlendFunc)(tolua_ret));
     tolua_pushusertype(l,tolua_obj,"ccBlendFunc");
    tolua_register_gc(l,lua_gettop(l));
#else
    void* tolua_obj = tolua_copy(l,(void*)&tolua_ret,sizeof(ccBlendFunc));
     tolua_pushusertype(l,tolua_obj,"ccBlendFunc");
    tolua_register_gc(l,lua_gettop(l));
#endif
   }
  }
 }
 return 1;
}

static int tolua_CCDrawNode_setBlendFunc(lua_State *l){
#ifndef TOLUA_RELEASE
 tolua_Error err;
 if (!tolua_isusertype(l, 1, "CCDrawNode", 0, &err) || (tolua_isvaluenil(l, 2, &err) || !tolua_isusertype(l, 2, "ccBlendFunc", 0, &err))
	 || !tolua_isnoobj(l, 3, &err)){
		tolua_error(l,"#ferror in function 'CCDrawNode.setBlendFunc'.",&err);
		return 0;
	}
#endif
	CCDrawNode *o = (CCDrawNode *)tolua_tousertype(l, 1, NULL);
	o->setBlendFunc( *((ccBlendFunc *)tolua_tousertype(l, 2, NULL)) );
	return 1;
}



TOLUA_API int tolua_CC_Extension_open(lua_State* l){
	tolua_open(l);
	tolua_usertype(l, "CCBAnimationManager");
	tolua_usertype(l, "CCBFile");
	tolua_usertype(l, "CCBReader");
	tolua_usertype(l, "CCBSequence");
	tolua_usertype(l, "CCControl");
	tolua_usertype(l, "CCControlButton");
	tolua_usertype(l, "CCHttpClient");
	tolua_usertype(l, "CCHttpRequest");
	tolua_usertype(l, "CCHttpResponse");
	tolua_usertype(l, "CCScale9Sprite");
	tolua_usertype(l, "CCScrollView");
	tolua_usertype(l, "CCTableView");
	tolua_usertype(l, "CCTableViewCell");
	tolua_usertype(l, "CCTableViewDataSource");
	tolua_usertype(l, "CCTableViewDelegate");
	tolua_usertype(l, "CCClippingNode");
	tolua_usertype(l, "CCDrawNode");
	tolua_module(l, NULL, 0);
	tolua_beginmodule(l, NULL);
		tolua_cclass(l,"CCBAnimationManager","CCBAnimationManager","CCObject",NULL);
		tolua_beginmodule(l,"CCBAnimationManager");
			tolua_function(l, "pickFromNode", tolua_CCBAnimationManager_pickFromNode);
			tolua_function(l, "getSequences", tolua_CCBAnimationManager_getSequences);
			tolua_function(l, "getAutoPlaySequenceId", tolua_CCBAnimationManager_getAutoPlaySequenceId);
			tolua_function(l, "setAutoPlaySequenceId", tolua_CCBAnimationManager_setAutoPlaySequenceId);
			tolua_function(l, "getRootNode", tolua_CCBAnimationManager_getRootNode);
			tolua_function(l, "setRootNode", tolua_CCBAnimationManager_setRootNode);
			tolua_function(l, "getRootContainerSize", tolua_CCBAnimationManager_getRootContainerSize);
			tolua_function(l, "setRootContainerSize", tolua_CCBAnimationManager_setRootContainerSize);
			tolua_function(l, "getRunningSequenceName", tolua_CCBAnimationManager_getRunningSequenceName);
			tolua_function(l, "getContainerSize", tolua_CCBAnimationManager_getContainerSize);
			tolua_function(l, "addNode", tolua_CCBAnimationManager_addNode);
			tolua_function(l, "setBaseValue", tolua_CCBAnimationManager_setBaseValue);
			tolua_function(l, "runAnimations", tolua_CCBAnimationManager_runAnimations);
			tolua_function(l, "runAnimationsForSequenceIdTweenDuration", tolua_CCBAnimationManager_runAnimationsForSequenceIdTweenDuration);
		tolua_endmodule(l);
		tolua_cclass(l,"CCBFile","CCBFile","CCNode",NULL);
		tolua_beginmodule(l,"CCBFile");
			tolua_function(l, "getCCBFileNode", tolua_CCBFile_getCCBFileNode);
			tolua_function(l, "setCCBFileNode", tolua_CCBFile_setCCBFileNode);
		tolua_endmodule(l);
		tolua_cclass(l,"CCBReader","CCBReader","CCObject",NULL);
		tolua_beginmodule(l,"CCBReader");
//			tolua_function(l,"load",tolua_CCBReader_load);
		tolua_endmodule(l);
		tolua_cclass(l,"CCBSequence","CCBSequence","CCObject",NULL);
		tolua_beginmodule(l,"CCBSequence");
			tolua_function(l,"getDuration",tolua_CCBSequence_getDuration);
			tolua_function(l,"setDuration",tolua_CCBSequence_setDuration);
			tolua_function(l,"getName",tolua_CCBSequence_getName);
			tolua_function(l,"setName",tolua_CCBSequence_setName);
			tolua_function(l,"getSequenceId",tolua_CCBSequence_getSequenceId);
			tolua_function(l,"setSequenceId",tolua_CCBSequence_setSequenceId);
			tolua_function(l,"getChainedSequenceId",tolua_CCBSequence_getChainedSequenceId);
			tolua_function(l,"setChainedSequenceId",tolua_CCBSequence_setChainedSequenceId);
		tolua_endmodule(l);
#ifdef __TOLUA_CCCONTROL__
		tolua_cclass(l, "CCControl", "CCControl", "CCLayer", NULL);
		tolua_beginmodule(l, "CCControl");
			tolua_function(l, "isEnabled", tolua_CCControl_isEnabled);
			tolua_function(l, "isHighlighted", tolua_CCControl_isHighlighted);
			tolua_function(l, "isSelected", tolua_CCControl_isSelected);
			tolua_function(l, "setSelected", tolua_CCControl_setSelected);
			tolua_function(l, "getState", tolua_CCControl_getState);
			tolua_function(l, "isTouchInside", tolua_CCControl_isTouchInside);
			tolua_function(l, "getTouchLocation", tolua_CCControl_getTouchLocation);
			tolua_function(l, "hasVisibleParents", tolua_CCControl_hasVisibleParents);
			tolua_function(l, "sendActionsForControlEvents", tolua_CCControl_sendActionsForControlEvents);
		tolua_endmodule(l);
		tolua_constant(l, "CCControlEventTouchDown", CCControlEventTouchDown);
		tolua_constant(l, "CCControlEventTouchDragInside", CCControlEventTouchDragInside);
		tolua_constant(l, "CCControlEventTouchDragOutside", CCControlEventTouchDragOutside);
		tolua_constant(l, "CCControlEventTouchDragEnter", CCControlEventTouchDragEnter);
		tolua_constant(l, "CCControlEventTouchDragExit", CCControlEventTouchDragExit);
		tolua_constant(l, "CCControlEventTouchUpInside", CCControlEventTouchUpInside);
		tolua_constant(l, "CCControlEventTouchUpOutside", CCControlEventTouchUpOutside);
		tolua_constant(l, "CCControlEventTouchCancel", CCControlEventTouchCancel);
		tolua_constant(l, "CCControlEventValueChanged", CCControlEventValueChanged);
		tolua_cclass(l, "CCControlButton", "CCControlButton", "CCControl", NULL);
		tolua_beginmodule(l, "CCControlButton");
			tolua_function(l, "create", tolua_CCControlButton_create);
			tolua_function(l, "getBackgroundSprite", tolua_CCControlButton_getBackgroundSprite);
			tolua_function(l, "setBackgroundSprite", tolua_CCControlButton_setBackgroundSprite);
			tolua_function(l, "getBackgroundSpriteForState", tolua_CCControlButton_getBackgroundSpriteForState);
			tolua_function(l, "setBackgroundSpriteForState", tolua_CCControlButton_setBackgroundSpriteForState);
			tolua_function(l, "setBackgroundSpriteFrameForState", tolua_CCControlButton_setBackgroundSpriteFrameForState);
			tolua_function(l, "getCurrentTitle", tolua_CCControlButton_getCurrentTitle);
			tolua_function(l, "setEnabled", tolua_CCControlButton_setEnabled);
			tolua_function(l, "getLabelAnchorPoint", tolua_CCControlButton_getLabelAnchorPoint);
			tolua_function(l, "setLabelAnchorPoint", tolua_CCControlButton_setLabelAnchorPoint);
			tolua_function(l, "setMargins", tolua_CCControlButton_setMargins);
			tolua_function(l, "getOpacity", tolua_CCControlButton_getOpacity);
			tolua_function(l, "setOpacity", tolua_CCControlButton_setOpacity);
			tolua_function(l, "getPreferredSize", tolua_CCControlButton_getPreferredSize);
			tolua_function(l, "setPreferredSize", tolua_CCControlButton_setPreferredSize);
			tolua_function(l, "isPushed", tolua_CCControlButton_isPushed);
			tolua_function(l, "getTitleLabel", tolua_CCControlButton_getTitleLabel);
			tolua_function(l, "setTitleLabel", tolua_CCControlButton_setTitleLabel);
			tolua_function(l, "getTitleBMFontForState", tolua_CCControlButton_getTitleBMFontForState);
			tolua_function(l, "setTitleBMFontForState", tolua_CCControlButton_setTitleBMFontForState);
			tolua_function(l, "getTitleColorForState", tolua_CCControlButton_getTitleColorForState);
			tolua_function(l, "setTitleColorForState", tolua_CCControlButton_setTitleColorForState);
			tolua_function(l, "getTitleTTFForState", tolua_CCControlButton_getTitleTTFForState);
			tolua_function(l, "setTitleTTFForState", tolua_CCControlButton_setTitleTTFForState);
			tolua_function(l, "getTitleForState", tolua_CCControlButton_getTitleForState);
			tolua_function(l, "setTitleForState", tolua_CCControlButton_setTitleForState);
		tolua_endmodule(l);
		tolua_constant(l, "CCControlStateNormal", CCControlStateNormal);
		tolua_constant(l, "CCControlStateHighlighted", CCControlStateHighlighted);
		tolua_constant(l, "CCControlStateDisabled", CCControlStateDisabled);
		tolua_constant(l, "CCControlStateSelected", CCControlStateSelected);
#endif
#ifdef LUAPROXY_CCEDITBOX_ENABLED
		tolua_cclass(l, "CCEditBox", "CCEditBox", "CCControlButton", NULL);
		tolua_beginmodule(l, "CCEditBox");
			tolua_function(l, "create", tolua_CCEditBox_create);
			tolua_function(l, "setContentSize", tolua_CCEditBox_setContentSize);
			tolua_function(l, "setFontColor", tolua_CCEditBox_setFontColor);
			tolua_function(l, "setInputFlag", tolua_CCEditBox_setInputFlag);
			tolua_function(l, "setInputMode", tolua_CCEditBox_setInputMode);
			tolua_function(l, "setMaxLength", tolua_CCEditBox_setMaxLength);
			tolua_function(l, "setPlaceHolder", tolua_CCEditBox_setPlaceHolder);
			tolua_function(l, "setPlaceHolderFontColor", tolua_CCEditBox_setPlaceHolderFontColor);
			tolua_function(l, "setReturnType", tolua_CCEditBox_setReturnType);
			tolua_function(l, "getText", tolua_CCEditBox_getText);
			tolua_function(l, "setText", tolua_CCEditBox_setText);
		tolua_endmodule(l);
		tolua_constant(l, "kEditBoxInputFlagPassword", kEditBoxInputFlagPassword);
		tolua_constant(l, "kEditBoxInputFlagSensitive", kEditBoxInputFlagSensitive);
		tolua_constant(l, "kEditBoxInputFlagInitialCapsWord", kEditBoxInputFlagInitialCapsWord);
		tolua_constant(l, "kEditBoxInputFlagInitialCapsSentence", kEditBoxInputFlagInitialCapsSentence);
		tolua_constant(l, "kEditBoxInputFlagInitialCapsAllCharacters", kEditBoxInputFlagInitialCapsAllCharacters);
		tolua_constant(l, "kEditBoxInputModeAny", kEditBoxInputModeAny);
		tolua_constant(l, "kEditBoxInputModeEmailAddr", kEditBoxInputModeEmailAddr);
		tolua_constant(l, "kEditBoxInputModeNumeric", kEditBoxInputModeNumeric);
		tolua_constant(l, "kEditBoxInputModePhoneNumber", kEditBoxInputModePhoneNumber);
		tolua_constant(l, "kEditBoxInputModeUrl", kEditBoxInputModeUrl);
		tolua_constant(l, "kEditBoxInputModeDecimal", kEditBoxInputModeDecimal);
		tolua_constant(l, "kEditBoxInputModeSingleLine", kEditBoxInputModeSingleLine);
		tolua_constant(l, "kKeyboardReturnTypeDefault", kKeyboardReturnTypeDefault);
		tolua_constant(l, "kKeyboardReturnTypeDone", kKeyboardReturnTypeDone);
		tolua_constant(l, "kKeyboardReturnTypeSend", kKeyboardReturnTypeSend);
		tolua_constant(l, "kKeyboardReturnTypeSearch", kKeyboardReturnTypeSearch);
		tolua_constant(l, "kKeyboardReturnTypeGo", kKeyboardReturnTypeGo);
#endif
		tolua_cclass(l, "CCHttpClient", "CCHttpClient", "CCObject", NULL);
		tolua_beginmodule(l, "CCHttpClient");
			tolua_function(l, "getInstance", tolua_CCHttpClient_getInstance);
			tolua_function(l, "getTimeoutForConnect", tolua_CCHttpClient_getTimeoutForConnect);
			tolua_function(l, "setTimeoutForConnect", tolua_CCHttpClient_setTimeoutForConnect);
			tolua_function(l, "getTimeoutForRead", tolua_CCHttpClient_getTimeoutForRead);
			tolua_function(l, "setTimeoutForRead", tolua_CCHttpClient_setTimeoutForRead);
			tolua_function(l, "send", tolua_CCHttpClient_send);
		tolua_endmodule(l);
		tolua_constant(l, "kHttpGet", CCHttpRequest::kHttpGet);
		tolua_constant(l, "kHttpPost", CCHttpRequest::kHttpPost);
		tolua_constant(l, "kHttpUnknow", CCHttpRequest::kHttpUnkown);
		tolua_cclass(l, "CCHttpRequest", "CCHttpRequest", "CCObject", NULL);
		tolua_beginmodule(l, "CCHttpRequest");
			tolua_function(l, "open", tolua_CCHttpRequest_open);
			tolua_function(l, "sendWithHandler", tolua_CCHttpRequest_sendWithHandler);
			tolua_function(l, "getRequestType", tolua_CCHttpRequest_getRequestType);
			tolua_function(l, "setRequestType", tolua_CCHttpRequest_setRequestType);
			tolua_function(l, "getUrl", tolua_CCHttpRequest_getUrl);
			tolua_function(l, "setUrl", tolua_CCHttpRequest_setUrl);
			tolua_function(l, "getRequestData", tolua_CCHttpRequest_getRequestData);
			tolua_function(l, "setRequestData", tolua_CCHttpRequest_setRequestData);
			tolua_function(l, "getRequestDataSize", tolua_CCHttpRequest_getRequestDataSize);
			tolua_function(l, "getTag", tolua_CCHttpRequest_getTag);
			tolua_function(l, "setTag", tolua_CCHttpRequest_setTag);
			tolua_function(l, "getUserData", tolua_CCHttpRequest_getUserData);
			tolua_function(l, "setUserData", tolua_CCHttpRequest_setUserData);
			tolua_function(l, "getTarget", tolua_CCHttpRequest_getTarget);
			tolua_function(l, "getHeaders", tolua_CCHttpRequest_getHeaders);
			tolua_function(l, "setHeaders", tolua_CCHttpRequest_setHeaders);
		tolua_endmodule(l);
		tolua_cclass(l, "CCHttpResponse", "CCHttpResponse", "CCObject", NULL);
		tolua_beginmodule(l, "CCHttpResponse");
			tolua_function(l, "getHttpRequest", tolua_CCHttpResponse_getHttpRequest);
			tolua_function(l, "isSucceed", tolua_CCHttpResponse_isSucceed);
			tolua_function(l, "setSucceed", tolua_CCHttpResponse_setSucceed);
			tolua_function(l, "getResponseData", tolua_CCHttpResponse_getResponseData);
			tolua_function(l, "setResponseData", tolua_CCHttpResponse_setResponseData);
			tolua_function(l, "getResponseCode", tolua_CCHttpResponse_getResponseCode);
			tolua_function(l, "setResponseCode", tolua_CCHttpResponse_setResponseCode);
			tolua_function(l, "getErrorBuffer", tolua_CCHttpResponse_getErrorBuffer);
			tolua_function(l, "setErrorBuffer", tolua_CCHttpResponse_setErrorBuffer);
		tolua_endmodule(l);
#ifdef __TOLUA_CCSCALE9SPRITE__
		tolua_cclass(l, "CCScale9Sprite", "CCScale9Sprite", "CCNode", NULL);
		tolua_beginmodule(l, "CCScale9Sprite");
			tolua_function(l, "create", tolua_CCScale9Sprite_create);
			tolua_function(l, "createWithSpriteFrame", tolua_CCScale9Sprite_createWithSpriteFrame);
			tolua_function(l, "setContentSize", tolua_CCScale9Sprite_setContentSize);
			tolua_function(l, "getCapInsets", tolua_CCScale9Sprite_getCapInsets);
			tolua_function(l, "setCapInsets", tolua_CCScale9Sprite_setCapInsets);
			tolua_function(l, "getColor", tolua_CCScale9Sprite_getColor);
			tolua_function(l, "setColor", tolua_CCScale9Sprite_setColor);
			tolua_function(l, "getOpacity", tolua_CCScale9Sprite_getOpacity);
			tolua_function(l, "setOpacity", tolua_CCScale9Sprite_setOpacity);
			tolua_function(l, "isOpacityModifyRGB", tolua_CCScale9Sprite_isOpacityModifyRGB);
			tolua_function(l, "setOpacityModifyRGB", tolua_CCScale9Sprite_setOpacityModifyRGB);
			tolua_function(l, "getPreferredSize", tolua_CCScale9Sprite_getPreferredSize);
			tolua_function(l, "setPreferredSize", tolua_CCScale9Sprite_setPreferredSize);
			tolua_function(l, "copyPreferredSize", tolua_CCScale9Sprite_copyPreferredSize);
			tolua_function(l, "setSpriteFrame", tolua_CCScale9Sprite_setSpriteFrame);
			tolua_function(l, "resizableSpriteWithCapInsets", tolua_CCScale9Sprite_resizableSpriteWithCapInsets);
			tolua_function(l, "updateWithBatchNode", tolua_CCScale9Sprite_updateWithBatchNode);
		tolua_endmodule(l);
#endif
		tolua_constant(l, "kCCScrollViewDirectionNone", kCCScrollViewDirectionNone);
		tolua_constant(l, "kCCScrollViewDirectionHorizontal", kCCScrollViewDirectionHorizontal);
		tolua_constant(l, "kCCScrollViewDirectionVertical", kCCScrollViewDirectionVertical);
		tolua_constant(l, "kCCScrollViewDirectionBoth", kCCScrollViewDirectionBoth);
		tolua_cclass(l, "CCScrollView", "CCScrollView", "CCLayer", NULL);
		tolua_beginmodule(l, "CCScrollView");
			tolua_function(l, "create", tolua_CCScrollView_create);
			tolua_function(l, "isBounceable", tolua_CCScrollView_isBounceable);
			tolua_function(l, "setBounceable", tolua_CCScrollView_setBounceable);
			tolua_function(l, "addChild", tolua_CCScrollView_addChild);
			tolua_function(l, "isClippingToBounds", tolua_CCScrollView_isClippingToBounds);
			tolua_function(l, "setClippingToBounds", tolua_CCScrollView_setClippingToBounds);
			tolua_function(l, "getContainer", tolua_CCScrollView_getContainer);
			tolua_function(l, "setContainer", tolua_CCScrollView_setContainer);
			tolua_function(l, "getContentOffset", tolua_CCScrollView_getContentOffset);
			tolua_function(l, "setContentOffset", tolua_CCScrollView_setContentOffset);
			tolua_function(l, "setContentOffsetInDuration", tolua_CCScrollView_setContentOffsetInDuration);
			tolua_function(l, "getContentSize", tolua_CCScrollView_getContentSize);
			tolua_function(l, "setContentSize", tolua_CCScrollView_setContentSize);
			tolua_function(l, "getDirection", tolua_CCScrollView_getDirection);
			tolua_function(l, "setDirection", tolua_CCScrollView_setDirection);
			tolua_function(l, "isDragging", tolua_CCScrollView_isDragging);
			tolua_function(l, "isNodeVisible", tolua_CCScrollView_isNodeVisible);
			tolua_function(l, "isTouchMoved", tolua_CCScrollView_isTouchMoved);
			tolua_function(l, "getViewSize", tolua_CCScrollView_getViewSize);
			tolua_function(l, "setViewSize", tolua_CCScrollView_setViewSize);
			tolua_function(l, "getZoomScale", tolua_CCScrollView_getZoomScale);
			tolua_function(l, "setZoomScale", tolua_CCScrollView_setZoomScale);
			tolua_function(l, "setZoomScaleInDuration", tolua_CCScrollView_setZoomScaleInDuration);
		tolua_endmodule(l);
		tolua_constant(l, "kCCTableViewFillTopDown", kCCTableViewFillTopDown);
		tolua_constant(l, "kCCTableViewFillBottomUp", kCCTableViewFillBottomUp);
		tolua_cclass(l, "CCTableViewDataSource", "CCTableViewDataSource", "", NULL);
		tolua_cclass(l, "CCTableViewDelegate", "CCTableViewDelegate", "", NULL);
		tolua_cclass(l, "CCTableView", "CCTableView", "CCScrollView", NULL);
		tolua_beginmodule(l, "CCTableView");
			tolua_function(l, "create", tolua_CCTableView_create);
			tolua_function(l, "getDataSource", tolua_CCTableView_getDataSource);
			tolua_function(l, "setDataSource", tolua_CCTableView_setDataSource);
			tolua_function(l, "getDelegate", tolua_CCTableView_getDelegate);
			tolua_function(l, "setDelegate", tolua_CCTableView_setDelegate);
			tolua_function(l, "getVerticalFillOrder", tolua_CCTableView_getVerticalFillOrder);
			tolua_function(l, "setVerticalFillOrder", tolua_CCTableView_setVerticalFillOrder);
			tolua_function(l, "updateCellAtIndex", tolua_CCTableView_updateCellAtIndex);
			tolua_function(l, "insertCellAtIndex", tolua_CCTableView_insertCellAtIndex);
			tolua_function(l, "removeCellAtIndex", tolua_CCTableView_removeCellAtIndex);
			tolua_function(l, "reloadData", tolua_CCTableView_reloadData);
			tolua_function(l, "dequeueCell", tolua_CCTableView_dequeueCell);
			tolua_function(l, "cellAtIndex", tolua_CCTableView_cellAtIndex);
		tolua_endmodule(l);
		tolua_cclass(l, "CCTableViewCell", "CCTableViewCell", "CCNode", NULL);
		tolua_beginmodule(l, "CCTableViewCell");
			tolua_function(l, "create", tolua_CCTableViewCell_create);
			tolua_function(l, "getIdx", tolua_CCTableViewCell_getIdx);
			tolua_function(l, "setIdx", tolua_CCTableViewCell_setIdx);
			tolua_function(l, "reset", tolua_CCTableViewCell_reset);
			tolua_function(l, "getObjectID", tolua_CCTableViewCell_getObjectID);
			tolua_function(l, "setObjectID", tolua_CCTableViewCell_setObjectID);
		tolua_endmodule(l);

		tolua_cclass(l, "CCGLProgram", "CCGLProgram", "CCObject", NULL);
		tolua_beginmodule(l, "CCGLProgram");
			tolua_function(l, "addAttribute", tolua_CCGLProgram_addAttribute);
			tolua_function(l, "getUniformLocationForName", tolua_CCGLProgram_getUniformLocationForName);
			tolua_function(l, "setUniformLocationWith", tolua_CCGLProgram_setUniformLocationWith);
		tolua_endmodule(l);

		tolua_cclass(l, "CCClippingNode", "CCClippingNode", "CCNode", NULL);
		tolua_beginmodule(l, "CCClippingNode");
			tolua_function(l, "create", tolua_CCClippingNode_create);
			tolua_function(l, "getStencil", tolua_CCClippingNode_getStencil);
			tolua_function(l, "setStencil", tolua_CCClippingNode_setStencil);
			tolua_function(l, "getAlphaThreshold", tolua_CCClippingNode_getAlphaThreshold);
			tolua_function(l, "setAlphaThreshold", tolua_CCClippingNode_setAlphaThreshold);
			tolua_function(l, "isInverted", tolua_CCClippingNode_isInverted);
			tolua_function(l, "setInverted", tolua_CCClippingNode_setInverted);
		tolua_endmodule(l);

		tolua_cclass(l, "CCDrawNode", "CCDrawNode", "CCNode", NULL);
		tolua_beginmodule(l, "CCDrawNode");
			tolua_function(l, "create", tolua_CCDrawNode_create);
			tolua_function(l, "drawDot", tolua_CCDrawNode_drawDot);
			tolua_function(l, "drawSegment", tolua_CCDrawNode_drawSegment);
			tolua_function(l, "drawPolygon", tolua_CCDrawNode_drawPolygon);
			tolua_function(l, "clear", tolua_CCDrawNode_clear);
			tolua_function(l, "getBlendFunc", tolua_CCDrawNode_getBlendFunc);
			tolua_function(l, "setBlendFunc", tolua_CCDrawNode_setBlendFunc);
		tolua_endmodule(l);
	tolua_endmodule(l);
	return 1;
}
#endif // __TOLUA_CC_EXTENSION_H__
