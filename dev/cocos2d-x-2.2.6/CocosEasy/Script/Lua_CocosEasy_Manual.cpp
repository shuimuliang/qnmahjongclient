#include "Lua_CocosEasy_Manual.h"
#include "LuaCocosEasy.h"

extern "C" {
#include "cocos2dx_support/tolua_fix.h"
}


// ====== CCNode
tolua_CocosEasy_class_function_def(CCNode,setRotationY,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCNode",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCNode* self = (CCNode*)  tolua_tousertype(tolua_S,1,0);
		float newRotation = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'", NULL);
#endif
		{
			self->setRotationY(newRotation);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setRotationY'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCNode,setRotationX,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCNode",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCNode* self = (CCNode*)  tolua_tousertype(tolua_S,1,0);
		float newRotation = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'", NULL);
#endif
		{
			self->setRotationX(newRotation);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setRotationX'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CCNode,getRotationX,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCNode* self = (CCNode*)  tolua_tousertype(tolua_S,1,0);
		float tolua_ret = self->getRotationX();
		tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getRotationX'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCNode,getRotationY,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCNode* self = (CCNode*)  tolua_tousertype(tolua_S,1,0);
		float tolua_ret = self->getRotationY();
		tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getRotationY'.",&tolua_err);
	return 0;
#endif
}

// ======= CCNodeRGBA
tolua_CocosEasy_class_function_def(CCNodeRGBA,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCNodeRGBA",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		{
			CCNodeRGBA* tolua_ret = CCNodeRGBA::create();
			int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
			int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
			toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCNodeRGBA");
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCNodeRGBA,setHSL,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCNodeRGBA",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"ccHSL3F",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCNodeRGBA* node = (CCNodeRGBA*)  tolua_tousertype(tolua_S,1,0);
		ccHSL3F hsl = *((ccHSL3F*)  tolua_tousertype(tolua_S,2,0));
		node->setHSL(hsl);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setHSL'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCNodeRGBA,getHSL,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCNodeRGBA",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCNodeRGBA* node = (CCNodeRGBA*)  tolua_tousertype(tolua_S,1,0);
		const ccHSL3F& tolua_ret = node->getHSL();
#ifdef __cplusplus
		void* tolua_obj = Mtolua_new((ccHSL3F)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"ccHSL3F");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
		void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(ccHSL3F));
		tolua_pushusertype(tolua_S,tolua_obj,"ccHSL3F");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getHSL'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CCNodeRGBA,enableHSL,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCNodeRGBA",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCNodeRGBA* self = (CCNodeRGBA*)  tolua_tousertype(tolua_S,1,0);
		self->enableHSL();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'enableHSL'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CCNodeRGBA,enableGrey,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCNodeRGBA",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCNodeRGBA* self = (CCNodeRGBA*)  tolua_tousertype(tolua_S,1,0);
		self->enableGrey();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'enableGrey'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CCNodeRGBA,defaultShader,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCNodeRGBA",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCNodeRGBA* self = (CCNodeRGBA*)  tolua_tousertype(tolua_S,1,0);
		self->defaultShader();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'defaultShader'.",&tolua_err);
	return 0;
#endif
}


// ======= CCLayerRGBA
tolua_CocosEasy_class_function_def(CCLayerRGBA,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCLayerRGBA",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		{
			CCLayerRGBA* tolua_ret = CCLayerRGBA::create();
			int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
			int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
			toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCLayerRGBA");
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}


// ======= CCSprite
tolua_CocosEasy_class_function_def(CCSprite,initWithTexture,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCSprite",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCTexture2D",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCSprite* self = (CCSprite*)  tolua_tousertype(tolua_S,1,0);
		CCTexture2D* texture = (CCTexture2D*)  tolua_tousertype(tolua_S,2,0);
		const CCRect* rect = (const CCRect*)  tolua_tousertype(tolua_S,3,0);
		if (!rect)
			self->initWithTexture(texture);
		else
			self->initWithTexture(texture,*rect);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'initWithTexture'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CCSprite,enableOutline,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCSprite",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCSprite* self = (CCSprite*)  tolua_tousertype(tolua_S,1,0);
		self->enableOutline();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'enableOutline'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CCSprite,setOutline,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCSprite",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"ccColor3B",0,&tolua_err)) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCSprite* node = (CCSprite*)  tolua_tousertype(tolua_S,1,0);
		ccColor3B color = *((ccColor3B*)  tolua_tousertype(tolua_S,2,0));
		node->setOutline(color);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setOutline'.",&tolua_err);
	return 0;
#endif
}


//CCFileUtils
tolua_CocosEasy_class_function_def(CCFileUtils,getFileData,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCFileUtils",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isstring(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCFileUtils* self = (CCFileUtils*)  tolua_tousertype(tolua_S,1,0);
		const char* pszFileName = ((const char*)  tolua_tostring(tolua_S,2,0));
		const char* pszMode = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFileData'", NULL);
#endif
		{
			unsigned long size = 0;
			unsigned char* buffer = self->getFileData(pszFileName,pszMode,&size);
			lua_pushlstring(tolua_S, (const char*)buffer, size);
			tolua_pushnumber(tolua_S,size);
		}
	}
	return 2;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getFileData'.",&tolua_err);
	return 0;
#endif
}


// action manager
tolua_CocosEasy_class_function_def(ActionManager,playActionByName,01)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"ActionManager",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isstring(tolua_S,3,0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		ActionManager* self = (ActionManager*)  tolua_tousertype(tolua_S,1,0);
		const char* jsonName = ((const char*)  tolua_tostring(tolua_S,2,0));
		const char* actionName = ((const char*)  tolua_tostring(tolua_S,3,0));
		CCCallFunc* callFunc = (CCCallFunc*)  tolua_tousertype(tolua_S,4,0);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'playActionByName'", NULL);
#endif
		{
			ActionObject* tolua_ret = (ActionObject*)  self->playActionByName(jsonName,actionName,callFunc);
			int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
			int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
			toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"ActionObject");
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'playActionByName'.",&tolua_err);
	return 0;
#endif
}


// scheduler
tolua_CocosEasy_class_function_def(CCScheduler,pauseAllTargets,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCScheduler",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCScheduler* self = (CCScheduler*)  tolua_tousertype(tolua_S,1,0);
		CCSet* tolua_ret = self->pauseAllTargets();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCSet");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'pauseAllTargets'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCScheduler,pauseAllTargetsWithMinPriority,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCScheduler",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCScheduler* self = (CCScheduler*)  tolua_tousertype(tolua_S,1,0);
		int priority = (int) tolua_tonumber(tolua_S, 2, 0);
		CCSet* tolua_ret = self->pauseAllTargetsWithMinPriority(priority);
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCSet");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'pauseAllTargetsWithMinPriority'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CCScheduler,resumeTargets,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCScheduler",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCSet",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCScheduler* self = (CCScheduler*)  tolua_tousertype(tolua_S,1,0);
		CCSet* cset = (CCSet*)  tolua_tousertype(tolua_S,2,0);
		self->resumeTargets(cset);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'resumeTargets'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCScheduler,resumeTarget,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCScheduler",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCObject",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCScheduler* self = (CCScheduler*)  tolua_tousertype(tolua_S,1,0);
		CCObject* pObj = (CCObject*)  tolua_tousertype(tolua_S,2,0);
		self->resumeTarget(pObj);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'resumeTarget'.",&tolua_err);
	return 0;
#endif
}


// TouchGroup
tolua_CocosEasy_class_function_def(TouchGroup,create,01)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"TouchGroup",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		{
			TouchGroup* tolua_ret = (TouchGroup*)  TouchGroup::create();
			int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
			int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
			toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"TouchGroup");
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}


// CCTouch
tolua_CocosEasy_class_function_def(CCTouch,new_local,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCTouch",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCTouch* tolua_ret = (CCTouch*)  Mtolua_new((CCTouch)());
		tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCTouch");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'new_local' ",&tolua_err);
	return 0;
#endif
}

// CCEvent
tolua_CocosEasy_class_function_def(CCEvent,new_local,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCEvent",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCEvent* tolua_ret = (CCEvent*)  Mtolua_new((CCEvent)());
		tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCEvent");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'new_local' ",&tolua_err);
	return 0;
#endif
}

// ScrollView
tolua_CocosEasy_class_function_def(ScrollView,addChildWithSize,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"ScrollView",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"const CCSize",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		ScrollView* self = (ScrollView*)  tolua_tousertype(tolua_S,1,0);
		CCNode* pNode = (CCNode*)  tolua_tousertype(tolua_S,2,0);
		CCSize size = *((CCSize*)  tolua_tousertype(tolua_S,3,0));
		self->addChildWithSize(pNode,size);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'addChildWithSize' ",&tolua_err);
	return 0;
#endif
}

// UILabel
tolua_CocosEasy_class_function_def(Label,setDefaultFontName,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"Label",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		std::string name = tolua_tostring(tolua_S,2,"");
		Label::setDefaultFontName(name);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setDefaultFontName' ",&tolua_err);
	return 0;
#endif
}

// CCLabelTTF
tolua_CocosEasy_class_function_def(CCLabelTTF,setDefaultFontName,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CCLabelTTF",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        std::string name = tolua_tostring(tolua_S,2,"");
        CCLabelTTF::setDefaultFontName(name);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setDefaultFontName' ",&tolua_err);
    return 0;
#endif
}


//Widget
class LuaWidgetTouchRepeatedEventListener:public CCObject
{
public:
	LuaWidgetTouchRepeatedEventListener(){};
	virtual ~LuaWidgetTouchRepeatedEventListener(){};

	static LuaWidgetTouchRepeatedEventListener* create();

	void eventCallbackFunc(CCObject* sender);

	void setHandler(int handler){ m_handler = handler; }
	int  getHandler() { return m_handler; }
private:
	int m_handler;
};

LuaWidgetTouchRepeatedEventListener* LuaWidgetTouchRepeatedEventListener::create()
{
	LuaWidgetTouchRepeatedEventListener* listener = new LuaWidgetTouchRepeatedEventListener();
	if (NULL == listener)
		return NULL;

	listener->autorelease();

	return listener;
}

void LuaWidgetTouchRepeatedEventListener::eventCallbackFunc(CCObject* sender)
{
	if (0 != m_handler)
	{
		CCArray* pArrayArgs = CCArray::createWithCapacity(0);
		CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_handler,pArrayArgs);
	}
}

tolua_CocosEasy_class_function_def(Widget,getTouchStartPos,01)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"Widget",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		Widget* self = (Widget*)  tolua_tousertype(tolua_S,1,0);
		const CCPoint& pos = self->getTouchStartPos();
		tolua_pushnumber(tolua_S,(lua_Number)pos.x);
		tolua_pushnumber(tolua_S,(lua_Number)pos.y);
	}
	return 2;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getTouchStartPos' ",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(Widget,getTouchMovePos,01)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"Widget",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		Widget* self = (Widget*)  tolua_tousertype(tolua_S,1,0);
		const CCPoint& pos = self->getTouchMovePos();
		tolua_pushnumber(tolua_S,(lua_Number)pos.x);
		tolua_pushnumber(tolua_S,(lua_Number)pos.y);
	}
	return 2;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getTouchMovePos' ",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(Widget,getTouchEndPos,01)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"Widget",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		Widget* self = (Widget*)  tolua_tousertype(tolua_S,1,0);
		const CCPoint& pos = self->getTouchEndPos();
		tolua_pushnumber(tolua_S,(lua_Number)pos.x);
		tolua_pushnumber(tolua_S,(lua_Number)pos.y);
	}
	return 2;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getTouchEndPos' ",&tolua_err);
	return 0;
#endif
}


// CCLayer
tolua_CocosEasy_class_function_def(CCLayer,ccTouchBegan,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCLayer",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCTouch",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCEvent",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCLayer* self = (CCLayer*)  tolua_tousertype(tolua_S,1,0);
		CCTouch* touch = ((CCTouch*)  tolua_tousertype(tolua_S,2,0));
		CCEvent* unusedEvent = ((CCEvent*)  tolua_tousertype(tolua_S,3,0));
		bool tolua_ret = (bool)  self->ccTouchBegan(touch,unusedEvent);
		tolua_pushboolean(tolua_S,(bool)tolua_ret);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'ccTouchBegan' ",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCLayer,ccTouchMoved,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCLayer",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCTouch",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCEvent",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCLayer* self = (CCLayer*)  tolua_tousertype(tolua_S,1,0);
		CCTouch* touch = ((CCTouch*)  tolua_tousertype(tolua_S,2,0));
		CCEvent* unusedEvent = ((CCEvent*)  tolua_tousertype(tolua_S,3,0));
		self->ccTouchMoved(touch,unusedEvent);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'ccTouchMoved' ",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCLayer,ccTouchEnded,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCLayer",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCTouch",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCEvent",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCLayer* self = (CCLayer*)  tolua_tousertype(tolua_S,1,0);
		CCTouch* touch = ((CCTouch*)  tolua_tousertype(tolua_S,2,0));
		CCEvent* unusedEvent = ((CCEvent*)  tolua_tousertype(tolua_S,3,0));
		self->ccTouchEnded(touch,unusedEvent);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'ccTouchEnded' ",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CCLayer,ccTouchCancelled,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCLayer",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCTouch",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"CCEvent",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCLayer* self = (CCLayer*)  tolua_tousertype(tolua_S,1,0);
		CCTouch* touch = ((CCTouch*)  tolua_tousertype(tolua_S,2,0));
		CCEvent* unusedEvent = ((CCEvent*)  tolua_tousertype(tolua_S,3,0));
		self->ccTouchCancelled(touch,unusedEvent);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'ccTouchCancelled' ",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CCDirector,addRenderExtra,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCDirector",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCDirector* self = (CCDirector*)  tolua_tousertype(tolua_S,1,0);
		CCNode* node = ((CCNode*)  tolua_tousertype(tolua_S,2,0));
		self->addRenderExtra(node);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'addRenderExtra' ",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCDirector,reStart,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCDirector",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCDirector* self = (CCDirector*)  tolua_tousertype(tolua_S,1,0);
		self->reStart();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'reStart' ",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(GUIReader,readJsonFile,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"GUIReader",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		GUIReader* self = (GUIReader*)  tolua_tousertype(tolua_S,1,0);
		self->readJsonFile(tolua_tostring(tolua_S,2,""));
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'readJsonFile' ",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(GUIReader,widgetFromDataStru,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"GUIReader",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		GUIReader* self = (GUIReader*)  tolua_tousertype(tolua_S,1,0);
		const char* fileName = ((const char*)  tolua_tostring(tolua_S,2,0));
		Widget* tolua_ret = (Widget*)  self->widgetFromDataStru(fileName);
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"Widget");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'widgetFromDataStru' ",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCRenderTexture,doBlur,01)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CCRenderTexture",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
        !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,4,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CCRenderTexture* self = (CCRenderTexture*)  tolua_tousertype(tolua_S,1,0);
        self->doBlur(tolua_tonumber(tolua_S,2,0), tolua_tonumber(tolua_S,3,0));
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'doBlur' ",&tolua_err);
    return 0;
#endif
}

//tolua_CocosEasy_class_function_def(CCRenderTexture,doFastBlur,01)
//{
//#ifndef TOLUA_RELEASE
//    tolua_Error tolua_err;
//    if (
//        !tolua_isusertype(tolua_S,1,"CCRenderTexture",0,&tolua_err) ||
//        !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
//        !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
//        !tolua_isnoobj(tolua_S,4,&tolua_err)
//        )
//        goto tolua_lerror;
//    else
//#endif
//    {
//        CCRenderTexture* self = (CCRenderTexture*)  tolua_tousertype(tolua_S,1,0);
//        self->doFastBlur(tolua_tonumber(tolua_S,2,0),tolua_tonumber(tolua_S, 3, 0));
//    }
//    return 1;
//#ifndef TOLUA_RELEASE
//tolua_lerror:
//    tolua_error(tolua_S,"#ferror in function 'doFastBlur' ",&tolua_err);
//    return 0;
//#endif
//}

tolua_CocosEasy_class_function_def(CCRenderTexture,doFastBlur,01)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CCRenderTexture",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
        !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,4,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CCRenderTexture* self = (CCRenderTexture*)  tolua_tousertype(tolua_S,1,0);
        CCSprite* tolua_ret = self->doFastBlur(tolua_tonumber(tolua_S,2,0),tolua_tonumber(tolua_S, 3, 0));
        int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCSprite");
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'doFastBlur' ",&tolua_err);
    return 0;
#endif
}


// ============================= extend ============================= //

static void extendCCNode(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "CCNode");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "setRotationX", tolua_CocosEasy_CCNode_setRotationX_00);
		tolua_function(tolua_S, "setRotationY", tolua_CocosEasy_CCNode_setRotationY_00);
		tolua_function(tolua_S, "getRotationX", tolua_CocosEasy_CCNode_getRotationX_00);
		tolua_function(tolua_S, "getRotationY", tolua_CocosEasy_CCNode_getRotationY_00);		
	}
	lua_pop(tolua_S, 1);
}

static void extendCCNodeRGBA(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "CCNodeRGBA");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "create", tolua_CocosEasy_CCNodeRGBA_create_00);
		tolua_function(tolua_S, "getHSL", tolua_CocosEasy_CCNodeRGBA_getHSL_00);
		tolua_function(tolua_S, "setHSL", tolua_CocosEasy_CCNodeRGBA_setHSL_00);
		tolua_function(tolua_S, "enableHSL", tolua_CocosEasy_CCNodeRGBA_enableHSL_00);
		tolua_function(tolua_S, "enableGrey", tolua_CocosEasy_CCNodeRGBA_enableGrey_00);
		tolua_function(tolua_S, "defaultShader", tolua_CocosEasy_CCNodeRGBA_defaultShader_00);
	}
	lua_pop(tolua_S, 1);
}

static void extendCCLayerRGBA(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "CCLayerRGBA");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "create", tolua_CocosEasy_CCLayerRGBA_create_00);
	}
	lua_pop(tolua_S, 1);
}

static void extendCCSprite(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "CCSprite");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "initWithTexture", tolua_CocosEasy_CCSprite_initWithTexture_00);
		tolua_function(tolua_S, "enableOutline", tolua_CocosEasy_CCSprite_enableOutline_00);
		tolua_function(tolua_S, "setOutline", tolua_CocosEasy_CCSprite_setOutline_00);
	}
	lua_pop(tolua_S, 1);
}

static void extendCCFileUtils(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "CCFileUtils");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "getFileData", tolua_CocosEasy_CCFileUtils_getFileData_00);
	}
	lua_pop(tolua_S, 1);
}

static void extendActionManager(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "ActionManager");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "playActionByName", tolua_CocosEasy_ActionManager_playActionByName_01);
	}
	lua_pop(tolua_S, 1);
}

static void extendCCScheduler(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "CCScheduler");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "pauseAllTargets", tolua_CocosEasy_CCScheduler_pauseAllTargets_00);
		tolua_function(tolua_S, "pauseAllTargetsWithMinPriority", tolua_CocosEasy_CCScheduler_pauseAllTargetsWithMinPriority_00);
		tolua_function(tolua_S, "resumeTarget", tolua_CocosEasy_CCScheduler_resumeTarget_00);
		tolua_function(tolua_S, "resumeTargets", tolua_CocosEasy_CCScheduler_resumeTargets_00);
	}
	lua_pop(tolua_S, 1);
}

static void extendTouchGroup(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "TouchGroup");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "create", tolua_CocosEasy_TouchGroup_create_01);
	}
	lua_pop(tolua_S, 1);
}

static void extendCCTouch(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "CCTouch");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "new_local", tolua_CocosEasy_CCTouch_new_local_00);
	}
	lua_pop(tolua_S, 1);
}

static void extendCCEvent(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "CCEvent");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "new_local", tolua_CocosEasy_CCEvent_new_local_00);
	}
	lua_pop(tolua_S, 1);
}

static void extendScrollView(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "ScrollView");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "addChildWithSize", tolua_CocosEasy_ScrollView_addChildWithSize_00);
	}
	lua_pop(tolua_S, 1);
}

static void extendLabel(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "Label");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "setDefaultFontName", tolua_CocosEasy_Label_setDefaultFontName_00);
	}
	lua_pop(tolua_S, 1);
}

static void extendWidget(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "Widget");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "getTouchStartPos", tolua_CocosEasy_Widget_getTouchStartPos_01);	
		tolua_function(tolua_S, "getTouchMovePos", tolua_CocosEasy_Widget_getTouchMovePos_01);	
		tolua_function(tolua_S, "getTouchEndPos", tolua_CocosEasy_Widget_getTouchEndPos_01);	
	}
	lua_pop(tolua_S, 1);
}


static void extendCCLayer(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "CCLayer");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "ccTouchBegan", tolua_CocosEasy_CCLayer_ccTouchBegan_00);
		tolua_function(tolua_S, "ccTouchMoved", tolua_CocosEasy_CCLayer_ccTouchMoved_00);
		tolua_function(tolua_S, "ccTouchEnded", tolua_CocosEasy_CCLayer_ccTouchEnded_00);	
		tolua_function(tolua_S, "ccTouchCancelled", tolua_CocosEasy_CCLayer_ccTouchCancelled_00);	
	}
	lua_pop(tolua_S, 1);
}

static void extendCCDirector(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "CCDirector");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "addRenderExtra", tolua_CocosEasy_CCDirector_addRenderExtra_00);
		tolua_function(tolua_S, "reStart", tolua_CocosEasy_CCDirector_reStart_00);
	}
	lua_pop(tolua_S, 1);
}

static void extendGUIReader(lua_State* tolua_S)
{
	lua_pushstring(tolua_S, "GUIReader");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S,-1))
	{
		tolua_function(tolua_S, "readJsonFile", tolua_CocosEasy_GUIReader_readJsonFile_00);
		tolua_function(tolua_S, "widgetFromDataStru", tolua_CocosEasy_GUIReader_widgetFromDataStru_00);
	}
	lua_pop(tolua_S, 1);
}

static void extendRenderTexture(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "CCRenderTexture");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "doBlur", tolua_CocosEasy_CCRenderTexture_doBlur_01);
        tolua_function(tolua_S, "doFastBlur", tolua_CocosEasy_CCRenderTexture_doFastBlur_01);
    }
    lua_pop(tolua_S, 1);
}
static void extendCCLabelTTF(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "CCLabelTTF");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "setDefaultFontName", tolua_CocosEasy_CCLabelTTF_setDefaultFontName_00);
    }
    lua_pop(tolua_S, 1);
}

LUA_API int register_all_CocosEasy_manual( lua_State* tolua_S )
{
	extendCCNode(tolua_S);
	extendCCNodeRGBA(tolua_S);
	extendCCLayerRGBA(tolua_S);
	extendCCSprite(tolua_S);
	extendCCFileUtils(tolua_S);
	extendActionManager(tolua_S);
	extendCCScheduler(tolua_S);
	extendCCTouch(tolua_S);
	extendCCEvent(tolua_S);
	extendScrollView(tolua_S);
	extendLabel(tolua_S);
    extendTouchGroup(tolua_S);
	extendWidget(tolua_S);
	extendCCLayer(tolua_S);
	extendCCDirector(tolua_S);
	extendGUIReader(tolua_S);
    extendRenderTexture(tolua_S);
    extendCCLabelTTF(tolua_S);
	return 0;
}


