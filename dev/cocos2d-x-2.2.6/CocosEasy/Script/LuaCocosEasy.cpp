#include "LuaCocosEasy.h"
#include "CocosEasy.h"
#include "cocos2dx_support/CCLuaValue.h"
extern "C" {
#include "cocos2dx_support/tolua_fix.h"
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "TalkingDataGA-Lua.h"
#endif


#ifdef __cplusplus

// script object
tolua_collect_script_object_def(CScriptObject);

// CococsExt
tolua_collect_def(CArmatureExt);
tolua_collect_def(CDictionaryExt);
tolua_collect_def(CTmxMapping);
tolua_collect_def(CUIPageViewExt);
tolua_collect_def(CEShake);
tolua_collect_def(CCScrollViewExt);
tolua_collect_def(CUITabViewExt);
tolua_collect_def(HttpRequestExt);
tolua_collect_def(RichTextExt);
tolua_collect_def(UICircleViewBaseExt);
tolua_collect_def(UIChoosePersonViewExt);
tolua_collect_def(UICircleViewExt);
tolua_collect_def(CEWheelRotateExt);
tolua_collect_def(CEImeChatBox);
tolua_collect_def(CCWebView);

// Graph
tolua_collect_def(CDebugDraw);
tolua_collect_def(st_DrawInfo);
tolua_collect_def(CGraph);
tolua_collect_def(CPOINT);
tolua_collect_def(CLine);
tolua_collect_def(CCircle);
tolua_collect_def(CRectangle);
tolua_collect_def(CPoly);
tolua_collect_def(CRaycast);

// Model
tolua_collect_def(CCocosEasyManager);
tolua_collect_def(CResourceLoader);
tolua_collect_def(CSceneTransitMgr);
tolua_collect_def(CDBManager);

tolua_collect_def(CResLoaderList);
tolua_collect_def(CLoadingTransitLayer);

tolua_collect_script_object_def(CTimerData);
tolua_collect_script_object_def(CSystemTimerData);
tolua_collect_script_object_def(CAsyncSocket);

// Utility
tolua_collect_def(CDataParser);
tolua_collect_def(CEventManager);
tolua_collect_def(CIFL);
tolua_collect_def(CELog);

// plug in
tolua_collect_def(CCEPlugin);
tolua_collect_def(ImagePicker);

// cocos2d-x 
tolua_collect_def(CCClippingNode);

// SDK
tolua_collect_def(CSDKManager);

#if CASINO_VOICE
tolua_collect_def(CIMManager);
#endif
// talking data
tolua_collect_def(CAnalysis);

// cocosEmbed
tolua_collect_def(ccHSL3F);

#endif



/* =================================== function to register type  =================================== */
static void tolua_reg_types (lua_State* tolua_S)
{
    // script object
    tolua_usertype(tolua_S,"CScriptObject");
	// CococsExt
	tolua_usertype(tolua_S,"CArmatureExt");
	tolua_usertype(tolua_S,"CDictionaryExt");
	tolua_usertype(tolua_S,"CTmxMapping");
	tolua_usertype(tolua_S,"CUIPageViewExt");
	tolua_usertype(tolua_S,"CEShake");
	tolua_usertype(tolua_S,"CCScrollViewExt");
	tolua_usertype(tolua_S,"CUITabViewExt");
	tolua_usertype(tolua_S,"UICircleViewExt");
	tolua_usertype(tolua_S,"HttpRequestExt");
	tolua_usertype(tolua_S,"RichTextExt");
	tolua_usertype(tolua_S,"UICircleViewBaseExt");
	tolua_usertype(tolua_S,"UIChoosePersonViewExt");
	tolua_usertype(tolua_S,"CEWheelRotateExt");
	tolua_usertype(tolua_S,"CEImeChatBox");
    tolua_usertype(tolua_S,"CCWebView");
	// Graph
	tolua_usertype(tolua_S,"CDebugDraw");
	tolua_usertype(tolua_S,"st_DrawInfo");
	tolua_usertype(tolua_S,"CGraph");
	tolua_usertype(tolua_S,"CPOINT");
	tolua_usertype(tolua_S,"CLine");
	tolua_usertype(tolua_S,"CCircle");
	tolua_usertype(tolua_S,"CRectangle");
	tolua_usertype(tolua_S,"CPoly");
	tolua_usertype(tolua_S,"CRaycast");

	// Model
	tolua_usertype(tolua_S,"CCocosEasyManager");
	tolua_usertype(tolua_S,"CResourceLoader");
	tolua_usertype(tolua_S,"CSceneTransitMgr");
	tolua_usertype(tolua_S,"CSocketManager");
	tolua_usertype(tolua_S,"CDBManager");
	tolua_usertype(tolua_S,"CTimerManager");

	tolua_usertype(tolua_S,"CResLoaderList");
	tolua_usertype(tolua_S,"CLoadingTransitLayer");
    tolua_usertype(tolua_S,"CTimerData");
    tolua_usertype(tolua_S,"CSystemTimerData");
    tolua_usertype(tolua_S,"CAsyncSocket");
    
	// Utility
	tolua_usertype(tolua_S,"CDataParser");
	tolua_usertype(tolua_S,"CEventManager");
	tolua_usertype(tolua_S,"CIFL");
	tolua_usertype(tolua_S,"CELog");
	tolua_usertype(tolua_S,"DynamicDownloadExt");

	// plug in
	tolua_usertype(tolua_S,"CCEPlugin");
    tolua_usertype(tolua_S,"ImagePicker");

	// cocos2d-x
	tolua_usertype(tolua_S,"CCClippingNode");

	// SDK
	tolua_usertype(tolua_S,"CSDKManager");
    
    //IM
#if CASINO_VOICE
    tolua_usertype(tolua_S,"CIMManager");
#endif
    
	// talking data
	tolua_usertype(tolua_S,"CAnalysis");

	// cocosEmbed
	tolua_usertype(tolua_S,"ccHSL3F");

    //talkinggame
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    tolua_reg_tdga_types(tolua_S);
#endif

}

/* =================================== function for global functions  =================================== */

static int tolua_CocosEasy_getTickCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE

#endif
	{
		unsigned int tick = getTickCount();
		tolua_pushnumber(tolua_S,tick);
	}
	return 1;
}

static int tolua_CocosEasy_base64Encode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isstring(tolua_S,1,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		unsigned char* data = (unsigned char*)tolua_tostring(tolua_S,1,0);
		unsigned int datalen = (unsigned int)tolua_tonumber(tolua_S,2,0);
		char* out;
		int length = cocoseasy::base64EncodeEasy(data, datalen, &out);
		lua_pushlstring(tolua_S, (const char*)out, length);
		free(out);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'base64Encode'.",&tolua_err);
	return 0;
#endif
}

static int tolua_CocosEasy_base64Decode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isstring(tolua_S,1,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		unsigned char* data = (unsigned char*)tolua_tostring(tolua_S,1,0);
		unsigned int datalen = (unsigned int)tolua_tonumber(tolua_S,2,0);
		unsigned char* out;
		int length = cocoseasy::base64DecodeEasy(data, datalen, &out);
		lua_pushlstring(tolua_S, (const char*)out, length);
		free(out);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'base64Decode'.",&tolua_err);
	return 0;
#endif
}



static int tolua_CocosEasy_oi_symmetry_encrypt200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isstring(tolua_S,1,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isstring(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		unsigned char* data = (unsigned char*)tolua_tostring(tolua_S,1,0);
		unsigned int datalen = (unsigned int)tolua_tonumber(tolua_S,2,0);
		unsigned char* key = (unsigned char*)tolua_tostring(tolua_S,3,0);
		int length = datalen+17;
		unsigned char* enData = new unsigned char[length];
		oi_symmetry_encrypt2(data, datalen, key, enData, &length);
		lua_pushlstring(tolua_S, (const char*)enData, length);
		delete[] enData;
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'oi_symmetry_encrypt2'.",&tolua_err);
	return 0;
#endif
}

static int tolua_CocosEasy_oi_symmetry_decrypt200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isstring(tolua_S,1,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isstring(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		unsigned char* data = (unsigned char*)tolua_tostring(tolua_S,1,0);
		unsigned int datalen = (unsigned int)tolua_tonumber(tolua_S,2,0);
		unsigned char* key = (unsigned char*)tolua_tostring(tolua_S,3,0);
		int length = datalen+17;
		unsigned char* enData = new unsigned char[length];
		oi_symmetry_decrypt2(data, datalen, key, enData, &length);
		lua_pushlstring(tolua_S, (const char*)enData, length);
		delete[] enData;
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'oi_symmetry_decrypt2'.",&tolua_err);
	return 0;
#endif
}


static int tolua_CocosEasy_hashString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isstring(tolua_S,1,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* str = tolua_tostring(tolua_S,1,0);
		unsigned int hash = hashString(str);
		tolua_pushnumber(tolua_S,hash);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'hashString'.",&tolua_err);
	return 0;
#endif
}


static int tolua_CocosEasy_getOSPlatform00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE

#endif
	{
		tolua_pushnumber(tolua_S,CC_TARGET_PLATFORM);
	}
	return 1;
}


static int tolua_CocosEasy_cchsl300(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(tolua_S,1,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		float h = ((float)  tolua_tonumber(tolua_S,1,0));
		float s = ((float)  tolua_tonumber(tolua_S,2,0));
		float l = ((float)  tolua_tonumber(tolua_S,3,0));
		ccHSL3F tolua_ret = (ccHSL3F) cchsl3(h,s,l);

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
	tolua_error(tolua_S,"#ferror in function 'ccHSL3F'.",&tolua_err);
	return 0;
#endif
}


// Geometry2D
static int tolua_CocosEasy_g2Vdist00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		(tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"CCPoint",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCPoint* v1 = (CCPoint*)  tolua_tousertype(tolua_S,1,0);
		CCPoint* v2 = (CCPoint*)  tolua_tousertype(tolua_S,2,0);
		float dis = g2Vdist(*v1,*v2);
		tolua_pushnumber(tolua_S,dis);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'g2Vdist'.",&tolua_err);
	return 0;
#endif
}

static int tolua_CocosEasy_g2Vray00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		(tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"CCPoint",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err)) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCPoint* v1 = (CCPoint*)  tolua_tousertype(tolua_S,1,0);
		CCPoint* v2 = (CCPoint*)  tolua_tousertype(tolua_S,2,0);
		float dis = tolua_tonumber(tolua_S,3,0);
		CCPoint des = g2Vray(*v1,*v2,dis);
		void* tolua_obj = Mtolua_new((CCPoint)(des));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'g2Vray'.",&tolua_err);
	return 0;
#endif
}


static int tolua_CocosEasy_g2Vequal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		(tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"CCPoint",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err))
		)
		goto tolua_lerror;
	else
#endif
	{
		CCPoint* v1 = (CCPoint*)  tolua_tousertype(tolua_S,1,0);
		CCPoint* v2 = (CCPoint*)  tolua_tousertype(tolua_S,2,0);
		float threshold = (float)tolua_tonumber(tolua_S,3,0);
		bool equal = g2Vequal(*v1,*v2,threshold);
		ce_tolua_pushboolean(tolua_S,equal);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'g2Vequal'.",&tolua_err);
	return 0;
#endif
}


static int tolua_CocosEasy_g2Vadd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		(tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"CCPoint",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err))
		)
		goto tolua_lerror;
	else
#endif
	{
		CCPoint* v1 = (CCPoint*)  tolua_tousertype(tolua_S,1,0);
		CCPoint* v2 = (CCPoint*)  tolua_tousertype(tolua_S,2,0);
		CCPoint tolua_ret = g2Vadd(*v1,*v2);
		void* tolua_obj = Mtolua_new((CCPoint)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'g2Vadd'.",&tolua_err);
	return 0;
#endif
}


static int tolua_CocosEasy_g2Vsub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		(tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"CCPoint",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err))
		)
		goto tolua_lerror;
	else
#endif
	{
		CCPoint* v1 = (CCPoint*)  tolua_tousertype(tolua_S,1,0);
		CCPoint* v2 = (CCPoint*)  tolua_tousertype(tolua_S,2,0);
		CCPoint tolua_ret = g2Vsub(*v1,*v2);
		void* tolua_obj = Mtolua_new((CCPoint)(tolua_ret));
		tolua_pushusertype(tolua_S,tolua_obj,"CCPoint");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'g2Vsub'.",&tolua_err);
	return 0;
#endif
}


static int tolua_CocosEasy_angle2Rotate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(tolua_S,1,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		float angle = tolua_tonumber(tolua_S,1,0);
		float rotate = angle2Rotate(angle);
		tolua_pushnumber(tolua_S,rotate);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'angle2Rotate'.",&tolua_err);
	return 0;
#endif
}

static int tolua_CocosEasy_rotate2Angle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(tolua_S,1,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		float rotate = tolua_tonumber(tolua_S,1,0);
		float angle = rotate2Angle(rotate);
		tolua_pushnumber(tolua_S,angle);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'rotate2Angle'.",&tolua_err);
	return 0;
#endif
}


static int tolua_CocosEasy_g2PointInRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		(tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"CCPoint",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCRect",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCPoint* point = (CCPoint*)tolua_tousertype(tolua_S,1,0);
		CCRect* rect = (CCRect*)tolua_tousertype(tolua_S,2,0);
		bool ret = g2PointInRect(*point,*rect);
		ce_tolua_pushboolean(tolua_S,ret);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'g2PointInRect'.",&tolua_err);
	return 0;
#endif
}



/* =================================== function for all class  =================================== */

/* ======================= Graph  ======================= */
// st_DrawInfo
tolua_CocosEasy_get_set_class_reference(st_DrawInfo, rgba4b, _ccColor4B);
tolua_CocosEasy_get_set_class_uint(st_DrawInfo, size);
tolua_CocosEasy_get_set_class_bool(st_DrawInfo, isSolid);

tolua_CocosEasy_class_function_def(st_DrawInfo,new_local,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"st_DrawInfo",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		st_DrawInfo* tolua_ret = (st_DrawInfo*)  Mtolua_new((st_DrawInfo));
		tolua_pushusertype(tolua_S,(void*)tolua_ret,"st_DrawInfo");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'new_local'.",&tolua_err);
	return 0;
#endif
}


// CDebugDraw
tolua_CocosEasy_class_function_def(CDebugDraw,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CDebugDraw",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CDebugDraw* tolua_ret = CDebugDraw::create();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CDebugDraw");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CDebugDraw,drawPoint,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CDebugDraw",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"st_DrawInfo",0,&tolua_err))
		)
		goto tolua_lerror;
	else
#endif
	{
		CDebugDraw* self = (CDebugDraw*)  tolua_tousertype(tolua_S,1,0);
		CCPoint* point = (CCPoint*)  tolua_tousertype(tolua_S,2,0);
		st_DrawInfo* pInfo = (st_DrawInfo*)  tolua_tousertype(tolua_S,3,0);
		bool isStable = tolua_toboolean(tolua_S,4,0) != 0;
		self->drawPoint(*point,pInfo,isStable);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'drawPoint'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CDebugDraw,drawLine,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CDebugDraw",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCPoint",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"CCPoint",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"st_DrawInfo",0,&tolua_err))
		)
		goto tolua_lerror;
	else
#endif
	{
		CDebugDraw* self = (CDebugDraw*)  tolua_tousertype(tolua_S,1,0);
		CCPoint* src = (CCPoint*)  tolua_tousertype(tolua_S,2,0);
		CCPoint* des = (CCPoint*)  tolua_tousertype(tolua_S,3,0);
		st_DrawInfo* pInfo = (st_DrawInfo*)  tolua_tousertype(tolua_S,4,0);
		bool isStable = tolua_toboolean(tolua_S,5,0) != 0;
		self->drawLine(*src,*des,pInfo,isStable);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'drawLine'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CDebugDraw,drawRectangle,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CDebugDraw",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"CCRect",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"st_DrawInfo",0,&tolua_err))
		)
		goto tolua_lerror;
	else
#endif
	{
		CDebugDraw* self = (CDebugDraw*)  tolua_tousertype(tolua_S,1,0);
		CCRect* rect = (CCRect*)  tolua_tousertype(tolua_S,2,0);
		st_DrawInfo* pInfo = (st_DrawInfo*)  tolua_tousertype(tolua_S,3,0);
		bool isStable = tolua_toboolean(tolua_S,4,0) != 0;
		self->drawRectangle(*rect,pInfo,isStable);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'drawRectangle'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CDebugDraw,drawPoly,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CDebugDraw",0,&tolua_err) ||
        !tolua_isusertype(tolua_S, 2, "CCPointArray", 0, &tolua_err) ||
        (tolua_isvaluenil(tolua_S,3,&tolua_err) ||!tolua_isusertype(tolua_S,3,"st_DrawInfo",0,&tolua_err))
        )
        goto tolua_lerror;
    else
#endif
    {
        CDebugDraw* self = (CDebugDraw*)  tolua_tousertype(tolua_S,1,0);
        
        CCPointArray *vertsArr = (CCPointArray *)tolua_tousertype(tolua_S, 2, NULL);
        int cnt = vertsArr->count();
        std::vector<CCPoint> pointList;
        for (int i=0; i<cnt; i++) pointList.push_back(vertsArr->getControlPointAtIndex(i));
        
        st_DrawInfo* pInfo = (st_DrawInfo*)  tolua_tousertype(tolua_S,3,0);
        bool isStable = tolua_toboolean(tolua_S,4,0) != 0;
        self->drawPoly(pointList,pInfo,isStable);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'drawPoly'.",&tolua_err);
    return 0;
#endif
}




/* ======================= CocosExt  ======================= */
static int tolua_CocosEasy_createMaskedSprite00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		(tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"CCSprite",0,&tolua_err)) ||
		tolua_isstring(tolua_S,2,0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCSprite* sprite = (CCSprite*)  tolua_tousertype(tolua_S,1,0);
		const char* maskName = (const char*)  tolua_tousertype(tolua_S,2,0);
		CCSprite* tolua_ret = createMaskedSprite(sprite, maskName);
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCSprite");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'createMaskedSprite'.",&tolua_err);
	return 0;
#endif
}


// CArmatureExt
tolua_CocosEasy_class_function_def(CArmatureExt,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CArmatureExt",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* json = tolua_tostring(tolua_S,2,NULL);
		const char* name = tolua_tostring(tolua_S,3,NULL);
		CArmatureExt* tolua_ret = CArmatureExt::create(json,name);
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CArmatureExt");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CArmatureExt,play,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CArmatureExt",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CArmatureExt* self = (CArmatureExt*)  tolua_tousertype(tolua_S,1,0);
		const char* name = tolua_tostring(tolua_S,2,NULL);
		unsigned int frameIdex = tolua_tonumber(tolua_S,3,-1);
		int durationTo = tolua_tonumber(tolua_S,4,-1);
		int durationTween = tolua_tonumber(tolua_S,5,-1);
		int loop = tolua_tonumber(tolua_S,6,-1);
		self->play(name,frameIdex,durationTo,durationTween,loop);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CArmatureExt,stop,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CArmatureExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CArmatureExt* self = (CArmatureExt*)  tolua_tousertype(tolua_S,1,0);
		self->stop();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CArmatureExt,getCurAnimation,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CArmatureExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CArmatureExt* self = (CArmatureExt*)  tolua_tousertype(tolua_S,1,0);
		const char* name = self->getCurAnimation();
		if (!name) return 0;
		tolua_pushstring(tolua_S,name);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getCurAnimation'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CArmatureExt,getAnimationTime,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CArmatureExt",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CArmatureExt* self = (CArmatureExt*)  tolua_tousertype(tolua_S,1,0);
		const char* name = tolua_tostring(tolua_S,2,NULL);
		float time = self->getAnimationTime(name);
		tolua_pushnumber(tolua_S, (lua_Number)time);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getAnimationTime'.",&tolua_err);
	return 0;
#endif
}



tolua_CocosEasy_class_function_def(CArmatureExt,resume,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CArmatureExt",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CArmatureExt* self = (CArmatureExt*)  tolua_tousertype(tolua_S,1,0);
		int index = lua_isnumber(tolua_S,2) ? tolua_tonumber(tolua_S,2,-1) : -1;
		self->resume(index);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'resume'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CArmatureExt,pause,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CArmatureExt",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CArmatureExt* self = (CArmatureExt*)  tolua_tousertype(tolua_S,1,0);
		int index = lua_isnumber(tolua_S,2) ? tolua_tonumber(tolua_S,2,-1) : -1;
		self->pause(index);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'pause'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CArmatureExt,setScriptCurAnimationComplete,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CArmatureExt",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CArmatureExt* self = (CArmatureExt*)  tolua_tousertype(tolua_S,1,0);
		LUA_FUNCTION funcID = (  toluafix_ref_function(tolua_S,2,0));
		self->setScriptCurAnimationComplete(funcID);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setScriptCurAnimationComplete'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CArmatureExt,setScriptAnimationEventOnce,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CArmatureExt",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,4,&tolua_err) || !toluafix_isfunction(tolua_S,4,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CArmatureExt* self = (CArmatureExt*)  tolua_tousertype(tolua_S,1,0);
		const char* name = tolua_tostring(tolua_S,2,0);
		int tag = (int)tolua_tonumber(tolua_S,3,0);
		LUA_FUNCTION funcID = (  toluafix_ref_function(tolua_S,4,0));
		un_AnimationEventTag evtTag; evtTag.tag = tag;
		self->setScriptAnimationEventOnce(name,evtTag,funcID);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setScriptAnimationEventOnce'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CArmatureExt,getAnimationEventOnceData,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CArmatureExt",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CArmatureExt* self = (CArmatureExt*)  tolua_tousertype(tolua_S,1,0);
		const char* name = tolua_tostring(tolua_S,2,0);
		const CArmatureExt::st_AnimEvtItem* item = self->getAnimationEventOnceData(name);
		if(item) 
			tolua_pushnumber(tolua_S,item->tagEvent.tag);
		else
			lua_pushnil(tolua_S);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getAnimationEventOnceData'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CArmatureExt,isLoop,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CArmatureExt",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CArmatureExt* self = (CArmatureExt*)  tolua_tousertype(tolua_S,1,0);
		const char* name = tolua_tostring(tolua_S,2,0);
		bool isloop = self->isLoop(name);
		ce_tolua_pushboolean(tolua_S,isloop);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'isLoop'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CArmatureExt,getBoneTexture,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CArmatureExt",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CArmatureExt* self = (CArmatureExt*)  tolua_tousertype(tolua_S,1,0);
		const char* boneName = tolua_tostring(tolua_S,2,NULL);
		CCTexture2D* tolua_ret = self->getBoneTexture(boneName);
		if (!tolua_ret)
			lua_pushnil(tolua_S);
		else
		{
			int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
			int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
			toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCTexture2D");
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CArmatureExt,getBoneTextureRect,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CArmatureExt",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CArmatureExt* self = (CArmatureExt*)  tolua_tousertype(tolua_S,1,0);
		const char* boneName = tolua_tostring(tolua_S,2,NULL);
		const CCRect* pRect = self->getBoneTextureRect(boneName);
		if (!pRect)
			lua_pushnil(tolua_S);
		else
		{
			void* tolua_obj = Mtolua_new((CCRect)(*pRect));
			tolua_pushusertype(tolua_S,tolua_obj,"CCRect");
			tolua_register_gc(tolua_S,lua_gettop(tolua_S));
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getBoneTextureRect'.",&tolua_err);
	return 0;
#endif
}



// CUIPageViewExt
tolua_CocosEasy_class_function_def(CUIPageViewExt,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CUIPageViewExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CUIPageViewExt* tolua_ret = CUIPageViewExt::create();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CUIPageViewExt");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CUIPageViewExt,changePage,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CUIPageViewExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CUIPageViewExt* pUIPageView = ((CUIPageViewExt*)  tolua_tousertype(tolua_S,1,0));
		unsigned int idx = (unsigned int)((int)  tolua_tonumber(tolua_S,2,0));
		pUIPageView->changePage(idx);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'changePage'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CUIPageViewExt,setScrollCheckFactor,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CUIPageViewExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CUIPageViewExt* pUIPageView = ((CUIPageViewExt*)  tolua_tousertype(tolua_S,1,0));
		pUIPageView->setScrollCheckFactor(tolua_tonumber(tolua_S,2,0));
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setScrollCheckFactor'.",&tolua_err);
	return 0;
#endif
}
// CEShake
tolua_CocosEasy_class_function_def(CEShake,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CEShake",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		float d = ((float)  tolua_tonumber(tolua_S,2,0));
		float x = ((float)  tolua_tonumber(tolua_S,3,0));
		float y = ((float)  tolua_tonumber(tolua_S,4,0));
		CEShake* tolua_ret = CEShake::create(d,x,y);
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CEShake");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}

//CCScrollViewExt
tolua_CocosEasy_class_function_def(CCScrollViewExt,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCScrollViewExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{

		CCSize _size = CCSizeMake(tolua_tonumber(tolua_S,2,0),tolua_tonumber(tolua_S,3,0));
		CCScrollViewExt* tolua_ret = CCScrollViewExt::create(tolua_tonumber(tolua_S,2,0),tolua_tonumber(tolua_S,3,0));
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCScrollViewExt");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCScrollViewExt,addScrollPanel,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCScrollViewExt",0,&tolua_err)||
		!tolua_isstring(tolua_S,2,0,&tolua_err)||
		!tolua_isstring(tolua_S,3,0,&tolua_err)||
		!tolua_isnumber(tolua_S,4,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCScrollViewExt* pScroll = ((CCScrollViewExt*)  tolua_tousertype(tolua_S,1,0));
		const char *srcArrow = tolua_tostring(tolua_S,2,0);
		const char *srcPanel = tolua_tostring(tolua_S,3,0);
		int length = tolua_tonumber(tolua_S,4,0);
		pScroll->addScrollPanel(srcArrow,srcPanel,length);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'addScrollPanel'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCScrollViewExt,resetContainerPos,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCScrollViewExt",0,&tolua_err)||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCScrollViewExt* pScroll = ((CCScrollViewExt*)  tolua_tousertype(tolua_S,1,0));
		pScroll->resetContainerPos();
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'resetContainerPos'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCScrollViewExt,registerScriptScrollViewEvent,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCScrollViewExt",0,&tolua_err)||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCScrollViewExt* pScroll = ((CCScrollViewExt*)  tolua_tousertype(tolua_S,1,0));
		LUA_FUNCTION funcID = toluafix_ref_function(tolua_S,2,0);
		pScroll->registerScriptScrollViewEvent(funcID);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'registerScriptScrollViewEvent'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCScrollViewExt,setPageChangable,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCScrollViewExt",0,&tolua_err)||
		!tolua_isboolean(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCScrollViewExt* pScroll = ((CCScrollViewExt*)  tolua_tousertype(tolua_S,1,0));
		bool isPageChanable = ce_tolua_toboolean(tolua_S,2,0);
		pScroll->setPageChangable(isPageChanable);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setPageChangable'.",&tolua_err);
	return 0;
#endif
}	

tolua_CocosEasy_class_function_def(CCScrollViewExt,resetScrollPanel,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCScrollViewExt",0,&tolua_err)||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCScrollViewExt* pScroll = ((CCScrollViewExt*)  tolua_tousertype(tolua_S,1,0));
		pScroll->resetScrollPanel();
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'resetScrollPanel'.",&tolua_err);
	return 0;
#endif
}	

tolua_CocosEasy_class_function_def(CCScrollViewExt,setScrollPanelPos,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCScrollViewExt",0,&tolua_err)||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isnumber(tolua_S,3,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCScrollViewExt* pScroll = ((CCScrollViewExt*)  tolua_tousertype(tolua_S,1,0));
		float x = tolua_tonumber(tolua_S,2,0);
		float y = tolua_tonumber(tolua_S,3,0);
		pScroll->setScrollPanelPos(x,y);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setScrollPanelPos'.",&tolua_err);
	return 0;
#endif
}	

//CUITabViewExt
tolua_CocosEasy_class_function_def(CUITabViewExt,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CUITabViewExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CUITabViewExt* tolua_ret = CUITabViewExt::create();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CUITabViewExt");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}

//void addView(CCMenuItemImage* btn, CCNode* view);
tolua_CocosEasy_class_function_def(CUITabViewExt,addView,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CUITabViewExt",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"Button",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,3,"Widget",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CUITabViewExt* tolua_ret = (CUITabViewExt*)tolua_tousertype(tolua_S,1,0);
		Button* menuItemImage = (Button*)tolua_tousertype(tolua_S,2,0);
		Widget *pNode = (Widget*)tolua_tousertype(tolua_S,3,0);
		tolua_ret->addView(menuItemImage,pNode);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'addView'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CUITabViewExt,choose,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CUITabViewExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CUITabViewExt* tolua_ret = (CUITabViewExt*)tolua_tousertype(tolua_S,1,0);
		int id = tolua_tonumber(tolua_S,2,0);
		tolua_ret->choose(id,false);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'choose'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CUITabViewExt,resetToFirstView,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CUITabViewExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CUITabViewExt* tolua_ret = (CUITabViewExt*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->resetToFirstView();
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'resetToFirstView'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CUITabViewExt,getButton,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CUITabViewExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CUITabViewExt* tolua_ret = (CUITabViewExt*)tolua_tousertype(tolua_S,1,0);
		int id = tolua_tonumber(tolua_S,2,0);
		Button *result = tolua_ret->getButton(id);
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)result,"Button");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getButton'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CUITabViewExt,getView,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CUITabViewExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CUITabViewExt* tolua_ret = (CUITabViewExt*)tolua_tousertype(tolua_S,1,0);
		int id = tolua_tonumber(tolua_S,2,0);
		CCNode *result = tolua_ret->getView(id);
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)result,"Widget");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getView'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CUITabViewExt,registScriptClickEvent,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CUITabViewExt",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CUITabViewExt* tolua_ret = (CUITabViewExt*)tolua_tousertype(tolua_S,1,0);
		LUA_FUNCTION funcID = toluafix_ref_function(tolua_S,2,0);
		tolua_ret->registScriptClickEvent(funcID);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'registScriptClickEvent'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CUITabViewExt,getCurTabId,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CUITabViewExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CUITabViewExt* tolua_ret = (CUITabViewExt*)tolua_tousertype(tolua_S,1,0);
		tolua_pushnumber(tolua_S,tolua_ret->getCurTabId());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getCurTabId'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CUITabViewExt,getCurButton,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CUITabViewExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CUITabViewExt* tolua_ret = (CUITabViewExt*)tolua_tousertype(tolua_S,1,0);
		Button *result = tolua_ret->getCurButton();

		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)result,"Button");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getCurButton'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CUITabViewExt,getCurView,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CUITabViewExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CUITabViewExt* tolua_ret = (CUITabViewExt*)tolua_tousertype(tolua_S,1,0);
		Widget *result = tolua_ret->getCurView();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)result,"Widget");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getCurView'.",&tolua_err);
	return 0;
#endif
}


//CCScrollViewExt
tolua_CocosEasy_class_function_def(UICircleViewExt,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"UICircleViewExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UICircleViewExt* tolua_ret = UICircleViewExt::create(tolua_tonumber(tolua_S,2,0),tolua_tonumber(tolua_S,3,0));
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"UICircleViewExt");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(UICircleViewExt,addCell,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"UICircleViewExt",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"Widget",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UICircleViewExt* tolua_ret = (UICircleViewExt*)tolua_tousertype(tolua_S,1,0);
		Widget* pWidget = (Widget*)tolua_tousertype(tolua_S,2,0);
		tolua_ret->addCell(pWidget);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'addCell'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(UICircleViewExt,resetCellPosition,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"UICircleViewExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UICircleViewExt* tolua_ret = (UICircleViewExt*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->resetCellPosition();
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'resetCellPosition'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(UICircleViewExt,setTargetCellScale,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"UICircleViewExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UICircleViewExt* tolua_ret = (UICircleViewExt*)tolua_tousertype(tolua_S,1,0);
		float scale = tolua_tonumber(tolua_S,2,0);
		tolua_ret->setTargetCellScale(scale);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setTargetCellScale'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(UICircleViewExt,setTargetOffY,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"UICircleViewExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UICircleViewExt* tolua_ret = (UICircleViewExt*)tolua_tousertype(tolua_S,1,0);
		float offY = tolua_tonumber(tolua_S,2,0);
		tolua_ret->setTargetOffY(offY);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setTargetOffY'.",&tolua_err);
	return 0;
#endif
}

//HttpRequestExt
tolua_CocosEasy_class_function_def(HttpRequestExt,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"HttpRequestExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		HttpRequestExt* tolua_ret = HttpRequestExt::create();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"HttpRequestExt");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(HttpRequestExt,sendGet,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"HttpRequestExt",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err)||
		(tolua_isvaluenil(tolua_S,3,&tolua_err) || 
		!toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		HttpRequestExt* tolua_ret = (HttpRequestExt*)tolua_tousertype(tolua_S,1,0);
		string url = tolua_tostring(tolua_S,2,0);
		LUA_FUNCTION funcID = toluafix_ref_function(tolua_S,3,0);
		tolua_ret->sendGet(url,funcID);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'sendGet'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(HttpRequestExt,setTag,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"HttpRequestExt",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		HttpRequestExt* tolua_ret = (HttpRequestExt*)tolua_tousertype(tolua_S,1,0);
		string tag = tolua_tostring(tolua_S,2,0);
		tolua_ret->setTag(tag);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setTag'.",&tolua_err);
	return 0;
#endif
}


//RichTextExt
tolua_CocosEasy_class_function_def(RichTextExt,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"RichTextExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isstring(tolua_S,3,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		RichTextExt* tolua_ret = RichTextExt::create(tolua_tonumber(tolua_S,2,0),tolua_tostring(tolua_S,3,0));
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"RichTextExt");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(RichTextExt,setText,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"RichTextExt",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		RichTextExt* tolua_ret = (RichTextExt*)tolua_tousertype(tolua_S,1,0);
		string text = tolua_tostring(tolua_S,2,0);
		tolua_ret->setText(text);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setText'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(RichTextExt,setTextHorizontalAlignment,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"RichTextExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		RichTextExt* tolua_ret = (RichTextExt*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->setTextHorizontalAlignment(tolua_ret->transFerToAlignment(tolua_tonumber(tolua_S,2,0)));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setTextHorizontalAlignment'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(RichTextExt,setLineSpacing,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"RichTextExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		RichTextExt* tolua_ret = (RichTextExt*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->setLineSpacing(tolua_tonumber(tolua_S,2,0));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setLineSpacing'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(RichTextExt,setDefaultFontSize,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"RichTextExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		RichTextExt* tolua_ret = (RichTextExt*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->setDefaultFontSize(tolua_tonumber(tolua_S,2,0));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setDefaultFontSize'.",&tolua_err);
	return 0;
#endif
}

//UICircleViewBaseExt
tolua_CocosEasy_class_function_def(UICircleViewBaseExt,turnRight,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"UICircleViewBaseExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UICircleViewBaseExt* tolua_ret = (UICircleViewBaseExt*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->turnRight();
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'turnRight'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(UICircleViewBaseExt,turnLeft,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"UICircleViewBaseExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UICircleViewBaseExt* tolua_ret = (UICircleViewBaseExt*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->turnLeft();
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'turnLeft'.",&tolua_err);
	return 0;
#endif
}
//UIChoosePersonViewExt
tolua_CocosEasy_class_function_def(UIChoosePersonViewExt,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"UIChoosePersonViewExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isnumber(tolua_S,3,0,&tolua_err)||
		!tolua_isstring(tolua_S,4,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UIChoosePersonViewExt* tolua_ret = UIChoosePersonViewExt::create(tolua_tonumber(tolua_S,2,0),tolua_tonumber(tolua_S,3,0),tolua_tostring(tolua_S,4,0));
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"UIChoosePersonViewExt");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(UIChoosePersonViewExt,addCell,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"UIChoosePersonViewExt",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"ImageView",0,&tolua_err)||
		!tolua_isusertype(tolua_S,3,"ImageView",0,&tolua_err)||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UIChoosePersonViewExt* tolua_ret = (UIChoosePersonViewExt*)tolua_tousertype(tolua_S,1,0);
		ImageView* pPortrait = (ImageView*)tolua_tousertype(tolua_S,2,0);
		ImageView* pHalfPortrait = (ImageView*)tolua_tousertype(tolua_S,3,0);
		tolua_ret->addCell(pPortrait,pHalfPortrait);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'addCell'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(UIChoosePersonViewExt,resetAfterAddCell,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"UIChoosePersonViewExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UIChoosePersonViewExt* tolua_ret = (UIChoosePersonViewExt*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->resetAfterAddCell();
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'resetAfterAddCell'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(UIChoosePersonViewExt,registScriptEvent,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"UIChoosePersonViewExt",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UIChoosePersonViewExt* tolua_ret = (UIChoosePersonViewExt*)tolua_tousertype(tolua_S,1,0);
		LUA_FUNCTION funcID = toluafix_ref_function(tolua_S,2,0);
		tolua_ret->registScriptEvent(funcID);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'registScriptEvent'.",&tolua_err);
	return 0;
#endif
}

//CEWheelRotateExt
tolua_CocosEasy_class_function_def(CEWheelRotateExt,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CEWheelRotateExt",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"ImageView",0,&tolua_err)||
		!tolua_isnumber(tolua_S,3,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEWheelRotateExt* tolua_ret = CEWheelRotateExt::create((ImageView*)tolua_tousertype(tolua_S,2,0),tolua_tonumber(tolua_S,3,0));
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CEWheelRotateExt");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CEWheelRotateExt,startWithSpeed,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CEWheelRotateExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEWheelRotateExt* tolua_ret = (CEWheelRotateExt*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->startWithSpeed(tolua_tonumber(tolua_S,2,0));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'startWithSpeed'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CEWheelRotateExt,startToAngel,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CEWheelRotateExt",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isnumber(tolua_S,3,0,&tolua_err)||
		!tolua_isnumber(tolua_S,4,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEWheelRotateExt* tolua_ret = (CEWheelRotateExt*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->startToAngel(tolua_tonumber(tolua_S,2,0),tolua_tonumber(tolua_S,3,0),tolua_tonumber(tolua_S,4,0));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'startToAngel'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CEWheelRotateExt,stop,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CEWheelRotateExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEWheelRotateExt* tolua_ret = (CEWheelRotateExt*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->stop();
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'stop'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CEWheelRotateExt,registerStopScriptHandler,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CEWheelRotateExt",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,3,&tolua_err) || !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEWheelRotateExt* tolua_ret = (CEWheelRotateExt*)tolua_tousertype(tolua_S,1,0);
		LUA_FUNCTION funcID1 = toluafix_ref_function(tolua_S,2,0);
		LUA_FUNCTION funcID2 = toluafix_ref_function(tolua_S,3,0);
		tolua_ret->registerScriptHandler(funcID1,funcID2);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'registerStopScriptHandler'.",&tolua_err);
	return 0;
#endif
}

// CEImeChatBox
tolua_CocosEasy_class_function_def(CEImeChatBox,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CEImeChatBox",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isstring(tolua_S,5,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,6,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCScale9Sprite *pSpr = CCScale9Sprite::create(tolua_tostring(tolua_S,5,""));
		CEImeChatBox* tolua_ret = CEImeChatBox::create(CCSizeMake(tolua_tonumber(tolua_S,2,0),tolua_tonumber(tolua_S,3,0)),tolua_tonumber(tolua_S,4,0),pSpr);
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CEImeChatBox");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CEImeChatBox,show,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CEImeChatBox",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEImeChatBox* tolua_ret = (CEImeChatBox*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->show();
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'show'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CEImeChatBox,hide,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CEImeChatBox",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEImeChatBox* tolua_ret = (CEImeChatBox*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->hide();
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'hide'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CEImeChatBox,registInputScript,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CEImeChatBox",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,3,&tolua_err) || !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,4,&tolua_err) || !toluafix_isfunction(tolua_S,4,"LUA_FUNCTION",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,5,&tolua_err) || !toluafix_isfunction(tolua_S,5,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,6,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEImeChatBox* tolua_ret = (CEImeChatBox*)tolua_tousertype(tolua_S,1,0);
		LUA_FUNCTION funcID1 = toluafix_ref_function(tolua_S,2,0);
		LUA_FUNCTION funcID2 = toluafix_ref_function(tolua_S,3,0);
		LUA_FUNCTION funcID3 = toluafix_ref_function(tolua_S,4,0);
		LUA_FUNCTION funcID4 = toluafix_ref_function(tolua_S,5,0);

		tolua_ret->registInputScript(funcID1,funcID2,funcID3,funcID4);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'registInputScript'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CEImeChatBox,setFontColor,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CEImeChatBox",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isnumber(tolua_S,3,0,&tolua_err)||
		!tolua_isnumber(tolua_S,4,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEImeChatBox* tolua_ret = (CEImeChatBox*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->setFontColor(ccc3(tolua_tonumber(tolua_S,2,0),tolua_tonumber(tolua_S,3,0),tolua_tonumber(tolua_S,4,0)));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setFontColor'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CEImeChatBox,setFontSize,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CEImeChatBox",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEImeChatBox* tolua_ret = (CEImeChatBox*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->setFontSize(tolua_tonumber(tolua_S,2,0));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setFontSize'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CEImeChatBox,setFontName,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CEImeChatBox",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEImeChatBox* tolua_ret = (CEImeChatBox*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->setFontName(tolua_tostring(tolua_S,2,NULL));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setFontName'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CEImeChatBox,getStrInput,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CEImeChatBox",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEImeChatBox* tolua_ret = (CEImeChatBox*)tolua_tousertype(tolua_S,1,0);
		tolua_pushstring(tolua_S , tolua_ret->getStrInput().c_str());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getStrInput'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CEImeChatBox,reset,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CEImeChatBox",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEImeChatBox* tolua_ret = (CEImeChatBox*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->reset();
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'reset'.",&tolua_err);
	return 0;
#endif
}

/* method: create of class  CCWebView */
tolua_CocosEasy_class_function_def(CCWebView,create,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CCWebView",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CCWebView* tolua_ret = (CCWebView*)  CCWebView::create();
        tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCWebView");
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}

/* method: create of class  CCWebView */
tolua_CocosEasy_class_function_def(CCWebView,create,01)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CCWebView",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        const char* activityName = ((const char*)  tolua_tostring(tolua_S,2,0));
        {
            CCWebView* tolua_ret = (CCWebView*)  CCWebView::create(activityName);
            tolua_pushusertype(tolua_S,(void*)tolua_ret,"CCWebView");
        }
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    return tolua_CocosEasy_CCWebView_create_00(tolua_S);
#endif
}

/* method: init of class  CCWebView */
tolua_CocosEasy_class_function_def(CCWebView,init,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CCWebView",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CCWebView* tolua_ret = (CCWebView*)tolua_tousertype(tolua_S,1,0);
        bool result = tolua_ret->init();
        tolua_pushboolean(tolua_S,(bool)result);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'init'.",&tolua_err);
    return 0;
#endif
}

/* method: showWebView of class  CCWebView */
tolua_CocosEasy_class_function_def(CCWebView,showWebView,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CCWebView",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err)||
        !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
        !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
        !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
        !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,7,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CCWebView* tolua_ret = (CCWebView*)tolua_tousertype(tolua_S,1,0);
        const char* url = tolua_tostring(tolua_S,2,NULL);
        float x = (float) tolua_tonumber(tolua_S,3,0);
        float y = (float) tolua_tonumber(tolua_S,4,0);
        float width = (float) tolua_tonumber(tolua_S,5,0);
        float height = (float) tolua_tonumber(tolua_S,6,0);
        tolua_ret->showWebView(url,x,y,width,height);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'showWebView'.",&tolua_err);
    return 0;
#endif
}

/* method: updateURL of class  CCWebView */
tolua_CocosEasy_class_function_def(CCWebView,updateURL,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CCWebView",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err)||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CCWebView* tolua_ret = (CCWebView*)tolua_tousertype(tolua_S,1,0);
        const char* url = tolua_tostring(tolua_S,2,NULL);
        tolua_ret->updateURL(url);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'updateURL'.",&tolua_err);
    return 0;
#endif
}

/* method: removeWebView of class  CCWebView */
tolua_CocosEasy_class_function_def(CCWebView,removeWebView,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CCWebView",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CCWebView* tolua_ret = (CCWebView*)tolua_tousertype(tolua_S,1,0);
        tolua_ret->removeWebView();
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'removeWebView'.",&tolua_err);
    return 0;
#endif
}

/* method: getActivityName of class  CCWebView */
tolua_CocosEasy_class_function_def(CCWebView,getActivityName,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CCWebView",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CCWebView* webView = (CCWebView*)tolua_tousertype(tolua_S,1,0);
        const char* tolua_ret = (const char*)  webView->getActivityName();
        tolua_pushstring(tolua_S,(const char*)tolua_ret);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getActivityName'.",&tolua_err);
    return 0;
#endif
}


/* ======================= Models  ======================= */

//CSceneTransitMgr
tolua_CocosEasy_class_function_def(CSceneTransitMgr,getInstance,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CSceneTransitMgr",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CSceneTransitMgr* tolua_ret = CSceneTransitMgr::getInstance();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CSceneTransitMgr");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSceneTransitMgr,setLoadingLayer,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSceneTransitMgr",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CLoadingTransitLayer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CLoadingTransitLayer* pScene = ((CLoadingTransitLayer*)  tolua_tousertype(tolua_S,2,0));
		CSceneTransitMgr::getInstance()->setLoadingLayer(pScene);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setLoadingLayer'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSceneTransitMgr,runWithScene,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSceneTransitMgr",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCScene",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCScene* pScene = ((CCScene*)  tolua_tousertype(tolua_S,2,0));
		CSceneTransitMgr::getInstance()->runWithScene(pScene);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'runWithScene'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSceneTransitMgr,replaceScene,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSceneTransitMgr",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCScene",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCScene* pScene = ((CCScene*)  tolua_tousertype(tolua_S,2,0));
		const char* resName = tolua_tostring(tolua_S,3,NULL);
		CSceneTransitMgr::getInstance()->replaceScene(pScene,resName);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'replaceScene'.",&tolua_err);
	return 0;
#endif
}

// CEventManager
tolua_CocosEasy_class_function_def(CEventManager,getInstance,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CEventManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEventManager* tolua_ret = CEventManager::getInstance();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CEventManager");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CEventManager,registerScriptEvent,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CEventManager",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		(tolua_isvaluenil(tolua_S,3,&tolua_err) || !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CEventManager* tolua_ret = (CEventManager*)tolua_tousertype(tolua_S,1,0);
		unsigned int eventID = tolua_tonumber(tolua_S,2,0);
		LUA_FUNCTION funcID = toluafix_ref_function(tolua_S,3,0);
		tolua_ret->registerScriptEvent(eventID,funcID);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'registerScriptEvent'.",&tolua_err);
	return 0;
#endif
}

// CELog
tolua_CocosEasy_class_function_def(CELog,getInstance,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CELog",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CELog* tolua_ret = CELog::getInstance();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CELog");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CELog,write,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CELog",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isstring(tolua_S,3,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CELog* tolua_ret = (CELog*)tolua_tousertype(tolua_S,1,0);
		unsigned int eLog = tolua_tonumber(tolua_S,2,0);
		string log  = tolua_tostring(tolua_S,3,0);
		tolua_ret->write(eLog,log);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'write'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CELog,createFile,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CELog",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CELog* tolua_ret = (CELog*)tolua_tousertype(tolua_S,1,0);
		string fileName  = tolua_tostring(tolua_S,2,0);
		tolua_ret->createFile(fileName);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'createFile'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CELog,setLogLevel,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CELog",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CELog* tolua_ret = (CELog*)tolua_tousertype(tolua_S,1,0);
		int level  = tolua_tonumber(tolua_S,2,0);
		tolua_ret->setLogLevel(level);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setLogLevel'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CELog,setCacheMaxChars,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CELog",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CELog* tolua_ret = (CELog*)tolua_tousertype(tolua_S,1,0);
		int level  = tolua_tonumber(tolua_S,2,0);
		tolua_ret->setCacheMaxChars(level);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setCacheMaxChars'.",&tolua_err);
	return 0;
#endif
}

// DynamicDownloadExt
tolua_CocosEasy_class_function_def(DynamicDownloadExt,getInstance,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"DynamicDownloadExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		DynamicDownloadExt* tolua_ret = DynamicDownloadExt::getInstance();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"DynamicDownloadExt");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(DynamicDownloadExt,startDownload,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"DynamicDownloadExt",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err)||
		!tolua_isboolean(tolua_S,3,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		DynamicDownloadExt* tolua_ret = (DynamicDownloadExt*)tolua_tousertype(tolua_S,1,0);
		string url = tolua_tostring(tolua_S,2,0);
		bool isZip = ce_tolua_toboolean(tolua_S,3,0);
		tolua_ret->startDownload(url,isZip);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'startDownload'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(DynamicDownloadExt,setScriptEvent,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"DynamicDownloadExt",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,3,&tolua_err) || !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,4,&tolua_err) || !toluafix_isfunction(tolua_S,4,"LUA_FUNCTION",0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,5,&tolua_err) || !toluafix_isfunction(tolua_S,5,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,6,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		DynamicDownloadExt* tolua_ret = (DynamicDownloadExt*)tolua_tousertype(tolua_S,1,0);
		//get functionID 
		LUA_FUNCTION funcIDOnStart = toluafix_ref_function(tolua_S,2,0);
		LUA_FUNCTION funcIDOnProcess = toluafix_ref_function(tolua_S,3,0);
		LUA_FUNCTION funcIDOnComplete = toluafix_ref_function(tolua_S,4,0);
		LUA_FUNCTION funcIDOnError = toluafix_ref_function(tolua_S,5,0);
		tolua_ret->setScriptEvent(funcIDOnStart,funcIDOnProcess,funcIDOnComplete,funcIDOnStart);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setScriptEvent'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(DynamicDownloadExt,isHasZipInDownloadDir,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"DynamicDownloadExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		DynamicDownloadExt* tolua_ret = (DynamicDownloadExt*)tolua_tousertype(tolua_S,1,0);
		bool result = (bool)  tolua_ret->isHasZipInDownloadDir();
		tolua_pushboolean(tolua_S,(bool)result);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'isHasZipInDownloadDir'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(DynamicDownloadExt,uncompressDownloadzip,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"DynamicDownloadExt",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		DynamicDownloadExt* tolua_ret = (DynamicDownloadExt*)tolua_tousertype(tolua_S,1,0);
		tolua_ret->uncompressDownloadzip();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'uncompressDownloadzip'.",&tolua_err);
	return 0;
#endif
}
// CLoadingTransitLayer
tolua_CocosEasy_class_function_def(CLoadingTransitLayer,enterNextScene,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CLoadingTransitLayer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CLoadingTransitLayer* pScene = ((CLoadingTransitLayer*)  tolua_tousertype(tolua_S,1,0));
		pScene->enterNextScene();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'enterNextScene'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CLoadingTransitLayer,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CLoadingTransitLayer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CLoadingTransitLayer* tolua_ret = CLoadingTransitLayer::create();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CLoadingTransitLayer");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CLoadingTransitLayer,registerLoadStepScriptHandler,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CLoadingTransitLayer",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CLoadingTransitLayer* pLayer = (CLoadingTransitLayer*)tolua_tousertype(tolua_S,1,NULL);
		LUA_FUNCTION funcID = (  toluafix_ref_function(tolua_S,2,0));
		pLayer->registerLoadStepScriptHandler(funcID);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'registerLoadStepScriptHandler'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CLoadingTransitLayer,getNextScene,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CLoadingTransitLayer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err))
		goto tolua_lerror;
	else
#endif
	{
		CLoadingTransitLayer* pScene = ((CLoadingTransitLayer*)  tolua_tousertype(tolua_S,1,0));
		CCScene* tolua_ret = pScene->getNextScene();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCScene");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getNextScene'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CLoadingTransitLayer,startResourceLoad,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CLoadingTransitLayer",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CLoadingTransitLayer* pLayer = (CLoadingTransitLayer*)tolua_tousertype(tolua_S,1,NULL);
		pLayer->startResourceLoad();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'startResourceLoad'.",&tolua_err);
	return 0;
#endif
}


// CResourceLoader
tolua_CocosEasy_class_function_def(CResourceLoader,getInstance,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CResourceLoader* tolua_ret = CResourceLoader::getInstance();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CResourceLoader");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CResourceLoader,start,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_istable(tolua_S, 2, 0, &tolua_err)             ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* path = NULL;
		std::vector<std::string> list;
		lua_pushnil(tolua_S); 
		while (lua_next(tolua_S,2) != 0)
		{
			if(!tolua_isstring(tolua_S,-1,1,&tolua_err)) goto tolua_lerror;
			path = tolua_tostring(tolua_S,-1,0);
			lua_pop(tolua_S, 1);
			list.push_back(path);
		}
		CResourceLoader::getInstance()->start(list);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'start'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CResourceLoader,setSearchPath,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_istable(tolua_S, 2, 0, &tolua_err)             ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* path = NULL;
		std::vector<std::string> list;
		lua_pushnil(tolua_S);
		while (lua_next(tolua_S,2) != 0)
		{
			if(!tolua_isstring(tolua_S,-1,1,&tolua_err)) return 0;
			path = tolua_tostring(tolua_S,-1,0);
			lua_pop(tolua_S,1);
			list.push_back(path);
		}
		CResourceLoader::getInstance()->setSearchPath(list);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setSearchPath'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CResourceLoader,addResourceLoaderList,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err)  ||
		(tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"CResLoaderList", 0, &tolua_err))  ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* key = tolua_tostring(tolua_S,2,NULL);
		CResLoaderList* resloader = (CResLoaderList*)  tolua_tousertype(tolua_S,3,0);
		CResourceLoader::getInstance()->addResourceLoaderList(key,*resloader);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'addResourceLoaderList'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CResourceLoader,run,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CResLoaderList", 0, &tolua_err)  ||
		(!tolua_isvaluenil(tolua_S,3,&tolua_err) && !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isboolean(tolua_S,4,0,&tolua_err)  ||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CResLoaderList* resloader = (CResLoaderList*)  tolua_tousertype(tolua_S,2,0);
		LUA_FUNCTION nHandler = toluafix_ref_function(tolua_S,3,0);
		bool noticeStep = tolua_toboolean(tolua_S,4,0) != 0;
		CResourceLoader::getInstance()->run(*resloader,nHandler,noticeStep);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'run'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CResourceLoader,run,01)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0, &tolua_err)  ||
		(!tolua_isvaluenil(tolua_S,3,&tolua_err) && !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isboolean(tolua_S,4,0,&tolua_err)  ||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* key = tolua_tostring(tolua_S,2,NULL);
		LUA_FUNCTION nHandler = toluafix_ref_function(tolua_S,3,0);
		bool noticeStep = tolua_toboolean(tolua_S,4,0) != 0;
		int ret = (int)CResourceLoader::getInstance()->run(key,nHandler,noticeStep);
		tolua_pushboolean(tolua_S,ret);
	}
	return 1;

tolua_lerror:
	return tolua_CocosEasy_CResourceLoader_run_00(tolua_S);
}

tolua_CocosEasy_class_function_def(CResourceLoader,loadArmature,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0, &tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* file = tolua_tostring(tolua_S,2,NULL);
		CResourceLoader::getInstance()->loadArmature(file);
	}
	return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'loadArmature'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CResourceLoader,unloadArmature,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0, &tolua_err)  ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* file = tolua_tostring(tolua_S,2,NULL);
		CResourceLoader::getInstance()->unloadArmature(file);
	}
	return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'unloadArmature'.",&tolua_err);
	return 0;
#endif
}



tolua_CocosEasy_class_function_def(CResourceLoader,unloadEffect,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0, &tolua_err)  ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* file = tolua_tostring(tolua_S,2,NULL);
		CResourceLoader::getInstance()->unloadEffect(file);
	}
	return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'unloadEffect'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CResourceLoader,loadEffect,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0, &tolua_err)  ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* file = tolua_tostring(tolua_S,2,NULL);
		CResourceLoader::getInstance()->loadEffect(file);
	}
	return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'loadEffect'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CResourceLoader,playEffect,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0, &tolua_err)  ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* file = tolua_tostring(tolua_S,2,NULL);
		unsigned int nSoundId = CResourceLoader::getInstance()->playEffect(file);
		tolua_pushnumber(tolua_S,nSoundId);
	}
	return 1;

#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'playEffect'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CResourceLoader,isRunning,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		bool isRun = CResourceLoader::getInstance()->isRunning();
		ce_tolua_pushboolean(tolua_S,isRun);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'isRunning'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CResourceLoader,getPercent,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		float percent = CResourceLoader::getInstance()->getPercent();
		tolua_pushnumber(tolua_S,(lua_Number)percent);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getPercent'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CResourceLoader,setMaxLoadStepTime,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		float time = (float)tolua_tonumber(tolua_S,2,0.016f);
		CResourceLoader::getInstance()->setMaxLoadStepTime(time);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setMaxLoadStepTime'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CResourceLoader,unloadAllResource,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResourceLoader",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CResourceLoader::getInstance()->unloadAllResource();
	}
	return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'unloadAllResource'.",&tolua_err);
	return 0;
#endif
}

// CDBManager
tolua_CocosEasy_class_function_def(CDBManager,getInstance,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CDBManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CDBManager* tolua_ret = CDBManager::getInstance();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CDBManager");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CDBManager,openDB,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CDBManager",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isstring(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* dbname = tolua_tostring(tolua_S,2,NULL);
		const char* mode = tolua_tostring(tolua_S,3,NULL);
		bool ret = CDBManager::getInstance()->openDB(dbname,mode);
		tolua_pushboolean(tolua_S,(int)ret);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'openDB'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CDBManager,closeDB,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CDBManager",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* dbname = tolua_tostring(tolua_S,2,NULL);
		CDBManager::getInstance()->closeDB(dbname);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'closeDB'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CDBManager,exeSql,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CDBManager",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isstring(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* dbname = tolua_tostring(tolua_S,2,NULL);
		const char* sql = tolua_tostring(tolua_S,3,NULL);
		TSqlTable table;
		CDBManager::getInstance()->exeSql(dbname,sql,&table);
		if(table.size() == 0) return 0;
		lua_createtable(tolua_S,table.size(),0);
		for (unsigned int i=0; i<table.size(); ++i)
		{
			TSqlRow& row = table[i];
			tolua_pushnumber(tolua_S,i+1);
			lua_createtable(tolua_S,0,row.size());
			TSqlRow::iterator it = row.begin();
			for (it; it!=row.end(); ++it)
			{
				tolua_pushstring(tolua_S,it->first.c_str());
				tolua_pushstring(tolua_S,it->second.c_str());
				lua_settable(tolua_S,-3);
			}
			lua_settable(tolua_S,-3);
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'exeSql'.",&tolua_err);
	return 0;
#endif
}

// CTimerData
tolua_CocosEasy_class_function_def(CTimerData,new_local,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CTimerData",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CTimerData* tolua_ret = (CTimerData*)  Mtolua_new((CTimerData));
        tolua_pushusertype(tolua_S, (void*)tolua_ret,"CTimerData");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'new_local'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CTimerData,addTimer,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CTimerData",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
        !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
        (tolua_isvaluenil(tolua_S,4,&tolua_err) || !toluafix_isfunction(tolua_S,4,"LUA_FUNCTION",0,&tolua_err)) ||
        !tolua_isnoobj(tolua_S,5,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CTimerData* self = (CTimerData*)  tolua_tousertype(tolua_S,1,0);
        float time = (float) tolua_tonumber(tolua_S,2,0);
        bool repeated = ce_tolua_toboolean(tolua_S,3,0);
        LUA_FUNCTION funcID = toluafix_ref_function(tolua_S,4,0);
        unsigned int timerID = self->addTimer(time, repeated, funcID);
        tolua_pushnumber(tolua_S,timerID);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'addTimer'.",&tolua_err);
    return 0;
#endif
}


tolua_CocosEasy_class_function_def(CTimerData,delTimer,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CTimerData",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CTimerData* self = (CTimerData*)  tolua_tousertype(tolua_S,1,0);
        float timerID = (float) tolua_tonumber(tolua_S,2,0);
        self->delTimer(timerID);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'addTimer'.",&tolua_err);
    return 0;
#endif
}


tolua_CocosEasy_class_function_def(CTimerData,clearTimer,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CTimerData",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CTimerData* self = (CTimerData*)  tolua_tousertype(tolua_S,1,0);
        self->clearTimer();
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'clearTimer'.",&tolua_err);
    return 0;
#endif
}

// CSystemTimerData
tolua_CocosEasy_class_function_def(CSystemTimerData,new_local,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CSystemTimerData",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CSystemTimerData* tolua_ret = (CSystemTimerData*)  Mtolua_new((CSystemTimerData));
        tolua_pushusertype(tolua_S, (void*)tolua_ret,"CSystemTimerData");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'new_local'.",&tolua_err);
    return 0;
#endif
}

// CAsyncSocket
tolua_CocosEasy_class_function_def(CAsyncSocket,new_local,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CAsyncSocket",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CAsyncSocket* tolua_ret = (CAsyncSocket*)  Mtolua_new((CAsyncSocket));
        tolua_pushusertype(tolua_S, (void*)tolua_ret,"CAsyncSocket");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'new_local'.",&tolua_err);
    return 0;
#endif
}


tolua_CocosEasy_class_function_def(CAsyncSocket,connect,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CAsyncSocket",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isnumber(tolua_S,3,0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CAsyncSocket* self = (CAsyncSocket*)  tolua_tousertype(tolua_S,1,0);
        const char* ip = tolua_tostring(tolua_S,2,NULL);
        int port = (int)tolua_tonumber(tolua_S,3,0);
        float timeout = (float)tolua_tonumber(tolua_S,4,0xffff);
        bool ret = self->connect(ip, port, timeout);
        ce_tolua_pushboolean(tolua_S,ret);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'connect'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CAsyncSocket,disconnect,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CAsyncSocket",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CAsyncSocket* self = (CAsyncSocket*)  tolua_tousertype(tolua_S,1,0);
        self->disconnect();
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'disconnect'.",&tolua_err);
    return 0;
#endif
}


tolua_CocosEasy_class_function_def(CAsyncSocket,send,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CAsyncSocket",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isnumber(tolua_S,3,0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CAsyncSocket* self = (CAsyncSocket*)  tolua_tousertype(tolua_S,1,0);
        const char* buff = tolua_tostring(tolua_S,2,NULL);
        unsigned int size = (unsigned int)tolua_tonumber(tolua_S,3,0);
        unsigned int id = self->send(buff, size);
        tolua_pushnumber(tolua_S,id);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'send'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CAsyncSocket,cancelSend,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CAsyncSocket",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CAsyncSocket* self = (CAsyncSocket*)  tolua_tousertype(tolua_S,1,0);
        unsigned int id = (unsigned int)tolua_tonumber(tolua_S,2,0);
        bool ret = self->cancelSend(id);
        ce_tolua_pushboolean(tolua_S, ret);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'send'.",&tolua_err);
    return 0;
#endif
}


tolua_CocosEasy_class_function_def(CAsyncSocket,registerListener,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CAsyncSocket",0,&tolua_err) ||
        (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
        (tolua_isvaluenil(tolua_S,3,&tolua_err) || !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
        (tolua_isvaluenil(tolua_S,4,&tolua_err) || !toluafix_isfunction(tolua_S,4,"LUA_FUNCTION",0,&tolua_err)) ||
        (tolua_isvaluenil(tolua_S,5,&tolua_err) || !toluafix_isfunction(tolua_S,5,"LUA_FUNCTION",0,&tolua_err))
        )
        goto tolua_lerror;
    else
#endif
    {
        CAsyncSocket* self = (CAsyncSocket*)  tolua_tousertype(tolua_S,1,0);
        LUA_FUNCTION connectID = toluafix_ref_function(tolua_S,2,0);
        LUA_FUNCTION sentID = toluafix_ref_function(tolua_S,3,0);
        LUA_FUNCTION recvID = toluafix_ref_function(tolua_S,4,0);
        LUA_FUNCTION breakID = toluafix_ref_function(tolua_S,5,0);
        CAsyncSocket::st_listener listener(connectID, sentID, recvID, breakID);
        self->registerListener(listener);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'registerListener'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CAsyncSocket,lastError,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CAsyncSocket",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CAsyncSocket* self = (CAsyncSocket*)  tolua_tousertype(tolua_S,1,0);
        std::string error = self->lastError();
        tolua_pushstring(tolua_S, error.c_str());
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lastError'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CAsyncSocket,lastErrorEnum,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CAsyncSocket",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CAsyncSocket* self = (CAsyncSocket*)  tolua_tousertype(tolua_S,1,0);
		int error = self->lastErrorEnum();
		tolua_pushnumber(tolua_S,error);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lastErrorEnum'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CAsyncSocket,setWorkTimeForOnce,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CAsyncSocket",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CAsyncSocket* self = (CAsyncSocket*)  tolua_tousertype(tolua_S,1,0);
        float workTime = (float)tolua_tonumber(tolua_S,2,NULL);
        self->setWorkTimeForOnce(workTime);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setWorkTimeForOnce'.",&tolua_err);
    return 0;
#endif
}


tolua_CocosEasy_class_function_def(CAsyncSocket,setHeartbeat,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CAsyncSocket",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
        !tolua_isstring(tolua_S,3,0, &tolua_err) ||
        !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
        !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
        !tolua_isstring(tolua_S,6,0, &tolua_err) ||
        !tolua_isnumber(tolua_S,7,0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CAsyncSocket* self = (CAsyncSocket*)  tolua_tousertype(tolua_S,1,0);
        
        float stimeout = (float)tolua_tonumber(tolua_S,2,0);
        const char* sbuf = tolua_tostring(tolua_S,3,NULL);
        unsigned int slen = (unsigned int) tolua_tonumber(tolua_S,4,0);
        
        float rtimeout = (float)tolua_tonumber(tolua_S,5,0);
        const char* rbuf = tolua_tostring(tolua_S,6,NULL);
        unsigned int rlen = (unsigned int) tolua_tonumber(tolua_S,7,0);
        
        self->setHeartbeat(stimeout, sbuf, slen, rtimeout, rbuf, rlen);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setHeartbeat'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CAsyncSocket,isConnected,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CAsyncSocket",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CAsyncSocket* self = (CAsyncSocket*)  tolua_tousertype(tolua_S,1,0);
        bool ret = self->isConnected();
        ce_tolua_pushboolean(tolua_S, ret);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'isConnected'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CAsyncSocket,isIdle,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CAsyncSocket",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CAsyncSocket* self = (CAsyncSocket*)  tolua_tousertype(tolua_S,1,0);
        bool ret = self->isIdle();
        ce_tolua_pushboolean(tolua_S, ret);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'isIdle'.",&tolua_err);
    return 0;
#endif
}


tolua_CocosEasy_class_function_def(CAsyncSocket,setInnerVersion,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CAsyncSocket",0,&tolua_err) ||
		!!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CAsyncSocket* self = (CAsyncSocket*)  tolua_tousertype(tolua_S,1,0);
		unsigned int version = (unsigned int)tolua_tonumber(tolua_S,2,0);
		self->setInnerVersion(version);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setInnerVersion'.",&tolua_err);
	return 0;
#endif
}

// CResLoaderList
tolua_CocosEasy_class_function_def(CResLoaderList,new_local,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CResLoaderList",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CResLoaderList* tolua_ret = (CResLoaderList*)  Mtolua_new((CResLoaderList));
		tolua_pushusertype(tolua_S,(void*)tolua_ret,"CResLoaderList");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'new_local'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CResLoaderList,new,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CResLoaderList",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CResLoaderList* tolua_ret = (CResLoaderList*)  Mtolua_new((CResLoaderList));
		tolua_pushusertype(tolua_S,(void*)tolua_ret,"CResLoaderList");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CResLoaderList,addImage,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResLoaderList",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CResLoaderList* self = (CResLoaderList*)  tolua_tousertype(tolua_S,1,0);
		const char* png = tolua_tostring(tolua_S,2,NULL);
		const char* plist = tolua_tostring(tolua_S,3,NULL);
		self->addImage(png,plist);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'addImage'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CResLoaderList,addTexture,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResLoaderList",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CResLoaderList* self = (CResLoaderList*)  tolua_tousertype(tolua_S,1,0);
		const char* png = tolua_tostring(tolua_S,2,NULL);
		self->addTexture(png);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'addTexture'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CResLoaderList,addArmature,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResLoaderList",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) 
		)
		goto tolua_lerror;
	else
#endif
	{
		CResLoaderList* self = (CResLoaderList*)  tolua_tousertype(tolua_S,1,0);
		const char* json = tolua_tostring(tolua_S,2,NULL);
		const char* name = tolua_tostring(tolua_S,3,NULL);
		bool onlyConfig = ce_tolua_toboolean(tolua_S,4,NULL);
		self->addArmature(json,name,onlyConfig);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'addArmature'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CResLoaderList,addMusic,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResLoaderList",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CResLoaderList* self = (CResLoaderList*)  tolua_tousertype(tolua_S,1,0);
		const char* music = tolua_tostring(tolua_S,2,NULL);
		self->addMusic(music);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'addMusic'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CResLoaderList,addEffect,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResLoaderList",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CResLoaderList* self = (CResLoaderList*)  tolua_tousertype(tolua_S,1,0);
		const char* effect = tolua_tostring(tolua_S,2,NULL);
		self->addEffect(effect);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'addEffect'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CResLoaderList,addUIJson,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CResLoaderList",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CResLoaderList* self = (CResLoaderList*)  tolua_tousertype(tolua_S,1,0);
		const char* json = tolua_tostring(tolua_S,2,NULL);
		self->addUIJson(json);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'addUIJson'.",&tolua_err);
	return 0;
#endif
}


// plug in
tolua_CocosEasy_class_function_def(CCEPlugin,getDeviceID,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		std::string deviceId = CCEPlugin::getDeviceID();
		tolua_pushstring(tolua_S,deviceId.c_str());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getDeviceID'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,isIOSSimulator,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		unsigned int isSimulator = CCEPlugin::isIOSSimulator();
		tolua_pushnumber(tolua_S,isSimulator);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'isIOSSimulator'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,getOSVersion,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int osVersion = CCEPlugin::getOSVersion();
		tolua_pushnumber(tolua_S,osVersion);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getOSVersion'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,getLatitude,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        double latitude = CCEPlugin::getLatitude();
        tolua_pushnumber(tolua_S,latitude);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getLatitude'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,getLongitude,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        double longitude = CCEPlugin::getLongitude();
        tolua_pushnumber(tolua_S,longitude);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getLongitude'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,locationServicesEnabled,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        bool enabled = CCEPlugin::locationServicesEnabled();
        tolua_pushboolean(tolua_S, enabled);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'locationServicesEnabled'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,requestLocationService,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CCEPlugin::requestLocationService();
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'requestLocationService'.",&tolua_err);
    return 0;
#endif
}

// plug in
tolua_CocosEasy_class_function_def(CCEPlugin,getAndroidSDCardPath,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        std::string deviceId = CCEPlugin::getAndroidSDCardPath();
        tolua_pushstring(tolua_S,deviceId.c_str());
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getAndroidSDCardPath'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,getDeviceLV,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        int deviceLV = CCEPlugin::getDeviceLV();
        tolua_pushnumber(tolua_S,deviceLV);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getDeviceLV'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,isJailBreak,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
    goto tolua_lerror;
    else
#endif
    {
        int isJailBreak = CCEPlugin::isJailBreak();
        tolua_pushnumber(tolua_S,isJailBreak);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'isJailBreak'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,getDeviceModel,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		std::string deviceModel = CCEPlugin::getDeviceModel();
		tolua_pushstring(tolua_S,deviceModel.c_str());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getDeviceModel'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,setClipboardTxt,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* pTxt = tolua_tostring(tolua_S,2,NULL);
		CCEPlugin::setClipboardTxt(pTxt);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setClipboardTxt'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,setIdleTimerDisabled,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		bool disabled = ce_tolua_toboolean(tolua_S,2,0);
		CCEPlugin::setIdleTimerDisabled(disabled);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setIdleTimerDisabled'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,playVibration,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CCEPlugin::playVibration();
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setIdleTimerDisabled'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,messageBox,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isstring(tolua_S,3,0,&tolua_err) ||
        !tolua_isstring(tolua_S,4,0,&tolua_err) ||
        !tolua_isstring(tolua_S,5,0,&tolua_err) ||
        (tolua_isvaluenil(tolua_S,6,&tolua_err) || !toluafix_isfunction(tolua_S,6,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,8,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
        const char* title = ((const char*)  tolua_tostring(tolua_S,2,0));
        const char* msg = ((const char*)  tolua_tostring(tolua_S,3,0));
        const char* button1Title = ((const char*)  tolua_tostring(tolua_S,4,0));
        const char* button2Title = ((const char*)  tolua_tostring(tolua_S,5,0));
        LUA_FUNCTION buttonHandler = (  toluafix_ref_function(tolua_S,6,0));
        CCEPlugin::messageBox(title, msg, button1Title, button2Title, buttonHandler);
        
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'messageBox'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,openURL,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
        const char* url = ((const char*)  tolua_tostring(tolua_S,2,0));
        CCEPlugin::openURL(url);
        
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'openURL'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CCEPlugin,startNotification,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isstring(tolua_S,5,0,&tolua_err) ||
		!tolua_isstring(tolua_S,6,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,7,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,8,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		//E_Noti_T notiType,int repeatDays ,int timeByMin , const char *title , const char *msg ,int tagId
		int noti = tolua_tonumber(tolua_S,2,0);
		E_Noti_T notiType = CCEPlugin::transNotiTFromInt(noti);
		int repeatDays = tolua_tonumber(tolua_S,3,0);
		int timeByMin =  tolua_tonumber(tolua_S,4,0);
		const char* title = ((const char*)  tolua_tostring(tolua_S,5,0));
		const char* msg = ((const char*)  tolua_tostring(tolua_S,6,0));
		int id = tolua_tonumber(tolua_S,7,0);
		CCEPlugin::startNotification(notiType,repeatDays, timeByMin,title,msg,id);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'startNotification'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CCEPlugin,closeAlarmNotification,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCEPlugin",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int id = tolua_tonumber(tolua_S,2,0);
		CCEPlugin::closeAlarmNotification(id);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'closeAlarmNotification'.",&tolua_err);
	return 0;
#endif
}

//ImagePicker
tolua_CocosEasy_class_function_def(ImagePicker,getInstance,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"ImagePicker",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        ImagePicker* tolua_ret = ImagePicker::getInstance();
        int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"ImagePicker");
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(ImagePicker,selectPicAndUpload,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"ImagePicker",0,&tolua_err) ||
        (tolua_isvaluenil(tolua_S,2,&tolua_err) ||
         !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
        !tolua_isstring(tolua_S,3,0,&tolua_err)||
        !tolua_isnoobj(tolua_S,4,&tolua_err)
        )
    goto tolua_lerror;
    else
#endif
    {
        ImagePicker* tolua_ret = (ImagePicker*)tolua_tousertype(tolua_S,1,0);
        LUA_FUNCTION funcID = toluafix_ref_function(tolua_S,2,0);
        string url = tolua_tostring(tolua_S,3,0);
        tolua_ret->selectPicAndUpload(funcID,url);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'selectPicAndUpload'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(ImagePicker,selectPicAndUpload,01)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"ImagePicker",0,&tolua_err) ||
        (tolua_isvaluenil(tolua_S,2,&tolua_err) ||
         !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
        !tolua_isstring(tolua_S,3,0,&tolua_err)||
        !tolua_isstring(tolua_S,4,0,&tolua_err)||
        !tolua_isnoobj(tolua_S,5,&tolua_err)
        )
    goto tolua_lerror;
    else
#endif
    {
        ImagePicker* tolua_ret = (ImagePicker*)tolua_tousertype(tolua_S,1,0);
        LUA_FUNCTION funcID = toluafix_ref_function(tolua_S,2,0);
        string url = tolua_tostring(tolua_S,3,0);
        string extInfo = tolua_tostring(tolua_S,4,0);
        tolua_ret->selectPicAndUpload(funcID,url,extInfo);
    }
    return 1;
tolua_lerror:
    return tolua_CocosEasy_ImagePicker_selectPicAndUpload_00(tolua_S);
}

tolua_CocosEasy_class_function_def(ImagePicker,selectPicAndUpload,02)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"ImagePicker",0,&tolua_err) ||
        (tolua_isvaluenil(tolua_S,2,&tolua_err) ||
         !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
        !tolua_isstring(tolua_S,3,0,&tolua_err)||
        !tolua_isstring(tolua_S,4,0,&tolua_err)||
        !tolua_isnumber(tolua_S,5,0,&tolua_err)||
        !tolua_isnoobj(tolua_S,6,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        ImagePicker* tolua_ret = (ImagePicker*)tolua_tousertype(tolua_S,1,0);
        LUA_FUNCTION funcID = toluafix_ref_function(tolua_S,2,0);
        string url = tolua_tostring(tolua_S,3,0);
        string extInfo = tolua_tostring(tolua_S,4,0);
        int clipWidth = (int) tolua_tonumber(tolua_S,5,0);
        tolua_ret->selectPicAndUpload(funcID,url,extInfo,clipWidth);
    }
    return 1;
tolua_lerror:
    return tolua_CocosEasy_ImagePicker_selectPicAndUpload_01(tolua_S);
}



tolua_CocosEasy_class_function_def(ImagePicker,setTag,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"ImagePicker",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err)||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        ImagePicker* tolua_ret = (ImagePicker*)tolua_tousertype(tolua_S,1,0);
        string tag = tolua_tostring(tolua_S,2,0);
        tolua_ret->setTag(tag);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setTag'.",&tolua_err);
    return 0;
#endif
}


// CCCLipingNode
tolua_CocosEasy_class_function_def(CCClippingNode,create,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCClippingNode* tolua_ret = (CCClippingNode*)  CCClippingNode::create();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCClippingNode");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CCClippingNode,setInverted,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
		bool isInvert = ce_tolua_toboolean(tolua_S,2,0);
		self->setInverted(isInvert);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setInverted'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CCClippingNode,setAlphaThreshold,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
		float threshold = (float) tolua_tonumber(tolua_S,2,0);
		self->setAlphaThreshold(threshold);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setAlphaThreshold'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CCClippingNode,setStencil,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CCClippingNode",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"CCNode",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CCClippingNode* self = (CCClippingNode*)  tolua_tousertype(tolua_S,1,0);
		CCNode* pStencil = (CCNode*)  tolua_tousertype(tolua_S,2,0);
		self->setStencil(pStencil);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setStencil'.",&tolua_err);
	return 0;
#endif
}


// SDK
tolua_CocosEasy_class_function_def(CSDKManager,getInstance,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CSDKManager* tolua_ret = CSDKManager::getInstance();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CSDKManager");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CSDKManager, createRoleAnayasis, 00){
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto  tolua_lerror;
    else
#endif
    {
        unsigned int serverId = tolua_tonumber(tolua_S, 2, 0);
        CSDKManager::getInstance()->createRoleAnayasis(serverId);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'createRoleAnayasis'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager, enterMainSceneAnayasis, 00){
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto  tolua_lerror;
    else
#endif
    {
        unsigned int serverId = tolua_tonumber(tolua_S, 2, 0);
        CSDKManager::getInstance()->enterMainSceneAnayasis(serverId);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'enterMainSceneAnayasis'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CSDKManager,playVideo,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        const char* jsonData = ((const char*)  tolua_tostring(tolua_S,2,0));
        CSDKManager::getInstance()->playVideo(jsonData);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'playVideo'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,login,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CSDKManager::getInstance()->login();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'login'.",&tolua_err);
	return 0;
#endif
}

// getLoginType
tolua_CocosEasy_class_function_def(CSDKManager,getLoginType,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        int loginType = CSDKManager::getInstance()->getLoginType();
        tolua_pushnumber(tolua_S,(int)loginType);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getLoginType'.",&tolua_err);
    return 0;
#endif
}


tolua_CocosEasy_class_function_def(CSDKManager,thirdLogin,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        int loginType = ((int)  tolua_tonumber(tolua_S,2,0));
        CSDKManager::getInstance()->thirdLogin(loginType);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'thirdLogin'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,logout,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CSDKManager::getInstance()->logout();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'logout'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,userCenter,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CSDKManager::getInstance()->userCenter();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'userCenter'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,isLogined,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		bool ret = CSDKManager::getInstance()->isLogined();
		tolua_pushboolean(tolua_S,(int)ret);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'isLogined'.",&tolua_err);
	return 0;
#endif
}
tolua_CocosEasy_class_function_def(CSDKManager,isInited,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		bool ret = CSDKManager::getInstance()->isInited();
		tolua_pushboolean(tolua_S,(int)ret);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'isInited'.",&tolua_err);
	return 0;
#endif
}
tolua_CocosEasy_class_function_def(CSDKManager,hasUserCenter,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		bool ret = CSDKManager::getInstance()->hasUserCenter();
		tolua_pushboolean(tolua_S,(int)ret);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'hasUserCenter'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CSDKManager,getAccountName,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		std::string accountName = CSDKManager::getInstance()->getAccountName();
		tolua_pushstring(tolua_S,accountName.c_str());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getAccountName'.",&tolua_err);
	return 0;
#endif
}
tolua_CocosEasy_class_function_def(CSDKManager,getAccountID,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		std::string accountID = CSDKManager::getInstance()->getAccountID();
		tolua_pushstring(tolua_S,accountID.c_str());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getAccountID'.",&tolua_err);
	return 0;
#endif
}
tolua_CocosEasy_class_function_def(CSDKManager,getSessionID,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		std::string sessionID = CSDKManager::getInstance()->getSessionID();
		tolua_pushstring(tolua_S,sessionID.c_str());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getSessionID'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,getSimCountryISO,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        std::string simCountryISO = CSDKManager::getInstance()->getSimCountryISO();
        tolua_pushstring(tolua_S,simCountryISO.c_str());
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getSimCountryISO'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,getChannelID,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int ret = CSDKManager::getInstance()->getChannelID();
		tolua_pushnumber(tolua_S,ret);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getChannelID'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,getDeviceToken,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
    goto tolua_lerror;
    else
#endif
    {
        std::string ret = CSDKManager::getInstance()->getDeviceToken();
        tolua_pushstring(tolua_S,ret.c_str());
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getDeviceToken'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,showWebPayView,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isstring(tolua_S,3,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,4,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        const char* url = ((const char*)  tolua_tostring(tolua_S,2,0));
        const char* jsonData = ((const char*)  tolua_tostring(tolua_S,3,0));
        CSDKManager::getInstance()->showWebPayView(url, jsonData);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'showWebPayView'.",&tolua_err);
    return 0;
#endif
}

#if CASINO_VOICE
// IM
tolua_CocosEasy_class_function_def(CIMManager,getInstance,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"CIMManager",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CIMManager* tolua_ret = CIMManager::getInstance();
        int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CIMManager");
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CIMManager, update, 00){
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CIMManager",0,&tolua_err) ||
        !tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto  tolua_lerror;
    else
#endif
    {
        int time = tolua_tonumber(tolua_S, 2, 0);
        CIMManager::getInstance()->update(time);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CIMManager, beginRecord, 00){
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CIMManager",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto  tolua_lerror;
    else
#endif
    {
        CIMManager::getInstance()->beginRecord();
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'beginRecord'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CIMManager,stopRecord,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CIMManager",0,&tolua_err) ||
        !tolua_isboolean(tolua_S,2,0,&tolua_err)||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {		bool isUpload = ce_tolua_toboolean(tolua_S,2,0);
            CIMManager::getInstance()->stopRecord(isUpload);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'stopRecord'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CIMManager,enterRoom,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CIMManager",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CIMManager::getInstance()->enterRoom();
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'enterRoom'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CIMManager,downloadRecord,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CIMManager",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        const char* fileID = ((const char*)  tolua_tostring(tolua_S,2,0));
        CIMManager::getInstance()->downloadRecord(fileID);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'downloadRecord'.",&tolua_err);
    return 0;
#endif
}


tolua_CocosEasy_class_function_def(CIMManager,registerUploadScriptHandler,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CIMManager",0,&tolua_err) ||
        (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        LUA_FUNCTION funcID = (  toluafix_ref_function(tolua_S,2,0));
        CIMManager::getInstance()->registerUploadScriptHandler(funcID);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'registerUploadScriptHandler'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CIMManager,registerDownloadScriptHandler,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CIMManager",0,&tolua_err) ||
        (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        LUA_FUNCTION funcID = (  toluafix_ref_function(tolua_S,2,0));
        CIMManager::getInstance()->registerDownloadScriptHandler(funcID);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'registerDownloadScriptHandler'.",&tolua_err);
    return 0;
#endif
}
#endif

// talking data
tolua_CocosEasy_class_function_def(CAnalysis,getInstance,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"CAnalysis",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		CAnalysis* tolua_ret = CAnalysis::getInstance();
		int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
		int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
		toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CAnalysis");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
	return 0;
#endif
}
// talking data
tolua_CocosEasy_class_function_def(CAnalysis,setPlayerID,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
        !tolua_isusertype(tolua_S,1,"CAnalysis",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
        char* playerID = ((char*)  tolua_tostring(tolua_S,2,0));
		if(playerID) CAnalysis::getInstance()->setPlayerID((std::string)playerID);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setPlayerID'.",&tolua_err);
	return 0;
#endif
}
tolua_CocosEasy_class_function_def(CAnalysis,setServerID,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
        !tolua_isusertype(tolua_S,1,"CAnalysis",0,&tolua_err) ||
		!tolua_isnumber(tolua_S, 2, 0, &tolua_err)  ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
        int serverID = ((int)  tolua_tonumber(tolua_S,2,0));
        CAnalysis::getInstance()->setServerID(serverID);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setServerID'.",&tolua_err);
	return 0;
#endif
}
tolua_CocosEasy_class_function_def(CAnalysis,logEvent,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CAnalysis",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isstring(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* id = ((const char*)  tolua_tostring(tolua_S,2,0));
		const char* tag = ((const char*)  tolua_tostring(tolua_S,3,0));
		CAnalysis::getInstance()->logEvent(id,tag);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'logEvent'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CAnalysis,logPurchase,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"CAnalysis",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        const char* extraData = ((const char*)  tolua_tostring(tolua_S,2,0));
        CAnalysis::getInstance()->logPurchase(extraData);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'logPurchase'.", &tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CAnalysis,logRegister,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"CAnalysis",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        const char* extraData = ((const char*)  tolua_tostring(tolua_S,2,0));
        CAnalysis::getInstance()->logRegister(extraData);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'logRegister'.", &tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CAnalysis,logLogin,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"CAnalysis",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        const char* extraData = ((const char*)  tolua_tostring(tolua_S,2,0));
        CAnalysis::getInstance()->logLogin(extraData);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S, "ferror in function 'logLogin'.", &tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CAnalysis,pageviewStart,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CAnalysis",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* pageName = ((const char*)  tolua_tostring(tolua_S,2,0));
		CAnalysis::getInstance()->pageviewStart(pageName);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'pageviewStart'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CAnalysis,pageviewEnd,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CAnalysis",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* pageName = ((const char*)  tolua_tostring(tolua_S,2,0));
		CAnalysis::getInstance()->pageviewEnd(pageName);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'pageviewEnd'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CAnalysis,getAdID,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CAnalysis",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
      const char*idfa = CAnalysis::getInstance()->getAdID();
      tolua_pushstring(tolua_S, idfa);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getAdID'.",&tolua_err);
    return 0;
#endif
}

// preLoadGoods
tolua_CocosEasy_class_function_def(CSDKManager,preLoadGoods,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
        const char* goodsList = ((const char*)  tolua_tostring(tolua_S,2,0));
		CSDKManager::getInstance()->preLoadGoods(goodsList);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'preLoadGoods'.",&tolua_err);
	return 0;
#endif
}

// buy goods
tolua_CocosEasy_class_function_def(CSDKManager,pay,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
        const char* extraData = ((const char*)  tolua_tostring(tolua_S,2,0));
		CSDKManager::getInstance()->pay(extraData);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'pay'.",&tolua_err);
	return 0;
#endif
}

// buy goods
tolua_CocosEasy_class_function_def(CSDKManager,share,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        const char* extraData = ((const char*)  tolua_tostring(tolua_S,2,0));
        CSDKManager::getInstance()->share(extraData);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'share'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,isWXAppInstalled,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        bool bRet = CSDKManager::getInstance()->isWXAppInstalled();
        tolua_pushboolean(tolua_S, bRet);
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'share'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,payWithPayPal,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        const char* extraData = ((const char*)  tolua_tostring(tolua_S,2,0));
        CSDKManager::getInstance()->payWithPayPal(extraData);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'payWithPayPal'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,registerLoginScriptHandler,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		LUA_FUNCTION funcID = (  toluafix_ref_function(tolua_S,2,0));
		CSDKManager::getInstance()->registerLoginScriptHandler(funcID);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'registerLoginScriptHandler'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,registerThirdLoginScriptHandler,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        LUA_FUNCTION funcID = (  toluafix_ref_function(tolua_S,2,0));
        CSDKManager::getInstance()->registerThirdLoginScriptHandler(funcID);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'registerThirdLoginScriptHandler'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,unRegisterThirdLoginScriptHandler,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        CSDKManager::getInstance()->unRegisterThirdLoginScriptHandler();
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'unRegisterThirdLoginScriptHandler'.",&tolua_err);
    return 0;
#endif
}


// miss order
tolua_CocosEasy_class_function_def(CSDKManager,getMissOrderList,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        std::string orderListStr = CSDKManager::getInstance()->getMissOrderList();
        tolua_pushstring(tolua_S, orderListStr.c_str());
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getMissOrderList'.",&tolua_err);
    return 0;
#endif
}

// remove order
tolua_CocosEasy_class_function_def(CSDKManager,getOrderInfoAndRemove,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        !tolua_isstring(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        const char* orderID = ((const char*)  tolua_tostring(tolua_S,2,0));
        std::string orderInfo = CSDKManager::getInstance()->getOrderInfoAndRemove(orderID);
        tolua_pushstring(tolua_S, orderInfo.c_str());
    }
    return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getOrderInfoAndRemove'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,registerBuyScriptHandler,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		LUA_FUNCTION funcID = (  toluafix_ref_function(tolua_S,2,0));
		CSDKManager::getInstance()->registerBuyScriptHandler(funcID);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'registerBuyScriptHandler'.",&tolua_err);
	return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,registerLogoutScriptHandler,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		LUA_FUNCTION funcID = (  toluafix_ref_function(tolua_S,2,0));
		CSDKManager::getInstance()->registerLogoutScriptHandler(funcID);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'registerLogoutScriptHandler'.",&tolua_err);
	return 0;
#endif
}


tolua_CocosEasy_class_function_def(CSDKManager,registerVideoScriptHandler,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        LUA_FUNCTION funcID = (  toluafix_ref_function(tolua_S,2,0));
        CSDKManager::getInstance()->registerVideoScriptHandler(funcID);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'registerVideoScriptHandler'.",&tolua_err);
    return 0;
#endif
}

tolua_CocosEasy_class_function_def(CSDKManager,registerShareScriptHandler,00)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"CSDKManager",0,&tolua_err) ||
        (tolua_isvaluenil(tolua_S,2,&tolua_err) || !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        LUA_FUNCTION funcID = (  toluafix_ref_function(tolua_S,2,0));
        CSDKManager::getInstance()->registerShareScriptHandler(funcID);
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'registerShareScriptHandler'.",&tolua_err);
    return 0;
#endif
}

// ccHSL3F
tolua_CocosEasy_get_set_class_float(ccHSL3F,h);
tolua_CocosEasy_get_set_class_float(ccHSL3F,s);
tolua_CocosEasy_get_set_class_float(ccHSL3F,l);

tolua_CocosEasy_class_function_def(ccHSL3F,new_local,00)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"ccHSL3F",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		ccHSL3F* tolua_ret = (ccHSL3F*)  Mtolua_new((ccHSL3F));
		tolua_pushusertype(tolua_S,(void*)tolua_ret,"ccHSL3F");
		tolua_register_gc(tolua_S,lua_gettop(tolua_S));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'new_local'.",&tolua_err);
	return 0;
#endif
}

/* =================================== open libs  =================================== */

TOLUA_API int tolua_CocosEasy_open( lua_State* tolua_S )
{
	tolua_open(tolua_S);
	tolua_reg_types(tolua_S);
	tolua_module(tolua_S,NULL,0);
	tolua_beginmodule(tolua_S,NULL);
	{
		// =============== global =============== //
		tolua_function(tolua_S,"getTickCount",tolua_CocosEasy_getTickCount00);
		tolua_function(tolua_S,"hashString",tolua_CocosEasy_hashString00);
		tolua_function(tolua_S,"getOSPlatform",tolua_CocosEasy_getOSPlatform00);
		tolua_function(tolua_S,"cchsl3",tolua_CocosEasy_cchsl300);
		tolua_function(tolua_S,"oi_symmetry_encrypt2",tolua_CocosEasy_oi_symmetry_encrypt200);
		tolua_function(tolua_S,"oi_symmetry_decrypt2",tolua_CocosEasy_oi_symmetry_decrypt200);
		tolua_function(tolua_S,"base64Encode",tolua_CocosEasy_base64Encode00);
		tolua_function(tolua_S,"base64Decode",tolua_CocosEasy_base64Decode00);
		
		// Geometry2D
		tolua_function(tolua_S,"g2Vdist",tolua_CocosEasy_g2Vdist00);
		tolua_function(tolua_S,"g2Vray",tolua_CocosEasy_g2Vray00);
		tolua_function(tolua_S,"g2Vequal",tolua_CocosEasy_g2Vequal00);
		tolua_function(tolua_S,"g2Vadd",tolua_CocosEasy_g2Vadd00);
		tolua_function(tolua_S,"g2Vsub",tolua_CocosEasy_g2Vsub00);
		tolua_function(tolua_S,"g2PointInRect",tolua_CocosEasy_g2PointInRect00);
		tolua_function(tolua_S,"angle2Rotate",tolua_CocosEasy_angle2Rotate00);
		tolua_function(tolua_S,"rotate2Angle",tolua_CocosEasy_rotate2Angle00);


		// =============== class =============== //
        // Script Object
        
#ifdef __cplusplus
        tolua_cclass(tolua_S,"CScriptObject","CScriptObject","CCObject",tolua_collect_CScriptObject);
#else
        tolua_cclass(tolua_S,"CScriptObject","CScriptObject","",NULL);
#endif
        tolua_beginmodule(tolua_S,"CScriptObject");
        {
        }
        tolua_endmodule(tolua_S);
        
        
        
        
		// st_DrawInfo
#ifdef __cplusplus
		tolua_cclass(tolua_S,"st_DrawInfo","st_DrawInfo","",tolua_collect_st_DrawInfo);
#else
		tolua_cclass(tolua_S,"st_DrawInfo","st_DrawInfo","",NULL);
#endif
		tolua_beginmodule(tolua_S,"st_DrawInfo");
		{
			tolua_variable(tolua_S,"rgba4b",tolua_CocosEasy_st_DrawInfo_get_rgba4b, tolua_CocosEasy_st_DrawInfo_set_rgba4b);
			tolua_variable(tolua_S,"isSolid",tolua_CocosEasy_st_DrawInfo_get_isSolid, tolua_CocosEasy_st_DrawInfo_set_isSolid);
			tolua_variable(tolua_S,"size",tolua_CocosEasy_st_DrawInfo_get_size, tolua_CocosEasy_st_DrawInfo_set_size);
			tolua_function(tolua_S,"new_local",tolua_CocosEasy_st_DrawInfo_new_local_00);
		}
		tolua_endmodule(tolua_S);

		// graph
#ifdef __cplusplus
		tolua_cclass(tolua_S,"CDebugDraw","CDebugDraw","CCNode",tolua_collect_CDebugDraw);
#else
		tolua_cclass(tolua_S,"CDebugDraw","CDebugDraw","CCNode",NULL);
#endif
		tolua_beginmodule(tolua_S,"CDebugDraw");
		{
			tolua_function(tolua_S,"create",tolua_CocosEasy_CDebugDraw_create_00);
			tolua_function(tolua_S,"drawPoint",tolua_CocosEasy_CDebugDraw_drawPoint_00);
			tolua_function(tolua_S,"drawLine",tolua_CocosEasy_CDebugDraw_drawLine_00);
			tolua_function(tolua_S,"drawRectangle",tolua_CocosEasy_CDebugDraw_drawRectangle_00);
            tolua_function(tolua_S,"drawPoly",tolua_CocosEasy_CDebugDraw_drawPoly_00);

            
		}
		tolua_endmodule(tolua_S);

		// cocosExt
		tolua_function(tolua_S,"createMaskedSprite",tolua_CocosEasy_createMaskedSprite00);

#ifdef __cplusplus
		tolua_cclass(tolua_S,"CArmatureExt","CArmatureExt","CCArmature",tolua_collect_CArmatureExt);
#else
		tolua_cclass(tolua_S,"CArmatureExt","CArmatureExt","CCArmature",NULL);
#endif
		tolua_beginmodule(tolua_S,"CArmatureExt");
		{
			tolua_function(tolua_S,"create",tolua_CocosEasy_CArmatureExt_create_00);
			tolua_function(tolua_S,"play",tolua_CocosEasy_CArmatureExt_play_00);
			tolua_function(tolua_S,"stop",tolua_CocosEasy_CArmatureExt_stop_00);
			tolua_function(tolua_S,"resume",tolua_CocosEasy_CArmatureExt_resume_00);
			tolua_function(tolua_S,"pause",tolua_CocosEasy_CArmatureExt_pause_00);
			tolua_function(tolua_S,"setScriptCurAnimationComplete",tolua_CocosEasy_CArmatureExt_setScriptCurAnimationComplete_00);
			tolua_function(tolua_S,"setScriptAnimationEventOnce",tolua_CocosEasy_CArmatureExt_setScriptAnimationEventOnce_00);
			tolua_function(tolua_S,"getAnimationEventOnceData",tolua_CocosEasy_CArmatureExt_getAnimationEventOnceData_00);
			tolua_function(tolua_S,"isLoop",tolua_CocosEasy_CArmatureExt_isLoop_00);
			tolua_function(tolua_S,"getBoneTexture",tolua_CocosEasy_CArmatureExt_getBoneTexture_00);
			tolua_function(tolua_S,"getBoneTextureRect",tolua_CocosEasy_CArmatureExt_getBoneTextureRect_00);
			tolua_function(tolua_S,"getCurAnimation",tolua_CocosEasy_CArmatureExt_getCurAnimation_00);
			tolua_function(tolua_S,"getAnimationTime",tolua_CocosEasy_CArmatureExt_getAnimationTime_00);
		}
		tolua_endmodule(tolua_S);
		//CCScrollViewExt
#ifdef __cplusplus
		tolua_cclass(tolua_S,"CCScrollViewExt","CCScrollViewExt","ScrollView",tolua_collect_CCScrollViewExt);
#else
		tolua_cclass(tolua_S,"CCScrollViewExt","CCScrollViewExt","ScrollView",NULL);
#endif
		tolua_beginmodule(tolua_S,"CCScrollViewExt");
		{
			tolua_function(tolua_S,"create",tolua_CocosEasy_CCScrollViewExt_create_00);	
			tolua_function(tolua_S,"addScrollPanel",tolua_CocosEasy_CCScrollViewExt_addScrollPanel_00);
			tolua_function(tolua_S,"resetContainerPos",tolua_CocosEasy_CCScrollViewExt_resetContainerPos_00);
			tolua_function(tolua_S,"registerScriptScrollViewEvent",tolua_CocosEasy_CCScrollViewExt_registerScriptScrollViewEvent_00);
			tolua_function(tolua_S,"setPageChangable",tolua_CocosEasy_CCScrollViewExt_setPageChangable_00);
			tolua_function(tolua_S,"resetScrollPanel",tolua_CocosEasy_CCScrollViewExt_resetScrollPanel_00);
			tolua_function(tolua_S,"setScrollPanelPos",tolua_CocosEasy_CCScrollViewExt_setScrollPanelPos_00);
		}
		tolua_endmodule(tolua_S);

	//CUITabViewExt
#ifdef __cplusplus
		tolua_cclass(tolua_S,"CUITabViewExt","CUITabViewExt","CCNode",tolua_collect_CUITabViewExt);
#else
		tolua_cclass(tolua_S,"CUITabViewExt","CUITabViewExt","CCNode",NULL);
#endif
		tolua_beginmodule(tolua_S,"CUITabViewExt");
		{
			tolua_function(tolua_S,"create",tolua_CocosEasy_CUITabViewExt_create_00);
			tolua_function(tolua_S,"addView",tolua_CocosEasy_CUITabViewExt_addView_00);
			tolua_function(tolua_S,"choose",tolua_CocosEasy_CUITabViewExt_choose_00);
			tolua_function(tolua_S,"resetToFirstView",tolua_CocosEasy_CUITabViewExt_resetToFirstView_00);
			tolua_function(tolua_S,"registScriptClickEvent",tolua_CocosEasy_CUITabViewExt_registScriptClickEvent_00);
			tolua_function(tolua_S,"getButton",tolua_CocosEasy_CUITabViewExt_getButton_00);
			tolua_function(tolua_S,"getView",tolua_CocosEasy_CUITabViewExt_getView_00);

			tolua_function(tolua_S,"getCurTabId",tolua_CocosEasy_CUITabViewExt_getCurTabId_00);
			tolua_function(tolua_S,"getCurButton",tolua_CocosEasy_CUITabViewExt_getCurButton_00);
			tolua_function(tolua_S,"getCurView",tolua_CocosEasy_CUITabViewExt_getCurView_00);
			
		}
		tolua_endmodule(tolua_S);


		//CUIPageViewExt
#ifdef __cplusplus
		tolua_cclass(tolua_S,"CUIPageViewExt","CUIPageViewExt","PageView",tolua_collect_CUIPageViewExt);
#else
		tolua_cclass(tolua_S,"CUIPageViewExt","CUIPageViewExt","PageView",NULL);
#endif
		tolua_beginmodule(tolua_S,"CUIPageViewExt");
		{
			tolua_function(tolua_S,"create",tolua_CocosEasy_CUIPageViewExt_create_00);
			tolua_function(tolua_S,"changePage",tolua_CocosEasy_CUIPageViewExt_changePage_00);
			tolua_function(tolua_S,"setScrollCheckFactor",tolua_CocosEasy_CUIPageViewExt_setScrollCheckFactor_00);
		}
		tolua_endmodule(tolua_S);


#ifdef __cplusplus
		tolua_cclass(tolua_S,"CEShake","CEShake","CCActionInterval",tolua_collect_CEShake);
#else
		tolua_cclass(tolua_S,"CEShake","CEShake","CCActionInterval",NULL);
#endif
		tolua_beginmodule(tolua_S,"CEShake");
		{
			tolua_function(tolua_S,"create",tolua_CocosEasy_CEShake_create_00);
		}
		tolua_endmodule(tolua_S);

		//HttpRequestExt
		
#ifdef __cplusplus
		tolua_cclass(tolua_S,"HttpRequestExt","HttpRequestExt","CCObject",tolua_collect_HttpRequestExt);
#else
		tolua_cclass(tolua_S,"HttpRequestExt","HttpRequestExt","CCObject",NULL);
#endif
		tolua_beginmodule(tolua_S,"HttpRequestExt");
		{
			tolua_function(tolua_S,"create",tolua_CocosEasy_HttpRequestExt_create_00);
			tolua_function(tolua_S,"sendGet",tolua_CocosEasy_HttpRequestExt_sendGet_00);
			tolua_function(tolua_S,"setTag",tolua_CocosEasy_HttpRequestExt_setTag_00);
		}
		tolua_endmodule(tolua_S);

		//RichTextExt

#ifdef __cplusplus
		tolua_cclass(tolua_S,"RichTextExt","RichTextExt","Widget",tolua_collect_RichTextExt);
#else
		tolua_cclass(tolua_S,"RichTextExt","RichTextExt","Widget",NULL);
#endif
		tolua_beginmodule(tolua_S,"RichTextExt");
		{
			tolua_function(tolua_S,"create",tolua_CocosEasy_RichTextExt_create_00);

			tolua_function(tolua_S,"setText",tolua_CocosEasy_RichTextExt_setText_00);

			tolua_function(tolua_S,"setTextHorizontalAlignment",tolua_CocosEasy_RichTextExt_setTextHorizontalAlignment_00);

			tolua_function(tolua_S,"setLineSpacing",tolua_CocosEasy_RichTextExt_setLineSpacing_00);

			tolua_function(tolua_S,"setDefaultFontSize",tolua_CocosEasy_RichTextExt_setDefaultFontSize_00);
		}
		tolua_endmodule(tolua_S);

		//UICircleViewBaseExt

#ifdef __cplusplus
		tolua_cclass(tolua_S,"UICircleViewBaseExt","UICircleViewBaseExt","Layout",tolua_collect_UICircleViewBaseExt);
#else
		tolua_cclass(tolua_S,"UICircleViewBaseExt","UICircleViewBaseExt","Layout",NULL);
#endif
		tolua_beginmodule(tolua_S,"UICircleViewBaseExt");
		{
			tolua_function(tolua_S,"turnRight",tolua_CocosEasy_UICircleViewBaseExt_turnRight_00);

			tolua_function(tolua_S,"turnLeft",tolua_CocosEasy_UICircleViewBaseExt_turnLeft_00);
		}
		tolua_endmodule(tolua_S);

		//UICircleViewExt
#ifdef __cplusplus
		tolua_cclass(tolua_S,"UICircleViewExt","UICircleViewExt","UICircleViewBaseExt",tolua_collect_UICircleViewExt);
#else
		tolua_cclass(tolua_S,"UICircleViewExt","UICircleViewExt","UICircleViewBaseExt",NULL);
#endif
		tolua_beginmodule(tolua_S,"UICircleViewExt");
		{
			tolua_function(tolua_S,"create",tolua_CocosEasy_UICircleViewExt_create_00);
			tolua_function(tolua_S,"addCell",tolua_CocosEasy_UICircleViewExt_addCell_00);
			tolua_function(tolua_S,"resetCellPosition",tolua_CocosEasy_UICircleViewExt_resetCellPosition_00);
			tolua_function(tolua_S,"setTargetCellScale",tolua_CocosEasy_UICircleViewExt_setTargetCellScale_00);
			tolua_function(tolua_S,"setTargetOffY",tolua_CocosEasy_UICircleViewExt_setTargetOffY_00);
		}
		tolua_endmodule(tolua_S);
		
		//UIChoosePersonViewExt

#ifdef __cplusplus
		tolua_cclass(tolua_S,"UIChoosePersonViewExt","UIChoosePersonViewExt","UICircleViewBaseExt",tolua_collect_UIChoosePersonViewExt);
#else
		tolua_cclass(tolua_S,"UIChoosePersonViewExt","UIChoosePersonViewExt","UICircleViewBaseExt",NULL);
#endif
		tolua_beginmodule(tolua_S,"UIChoosePersonViewExt");
		{
			tolua_function(tolua_S,"create",tolua_CocosEasy_UIChoosePersonViewExt_create_00);
			tolua_function(tolua_S,"addCell",tolua_CocosEasy_UIChoosePersonViewExt_addCell_00);
			tolua_function(tolua_S,"resetAfterAddCell",tolua_CocosEasy_UIChoosePersonViewExt_resetAfterAddCell_00);
			tolua_function(tolua_S,"registScriptEvent" , tolua_CocosEasy_UIChoosePersonViewExt_registScriptEvent_00);
		}
		tolua_endmodule(tolua_S);

		//CEWheelRotateExt
#ifdef __cplusplus
		tolua_cclass(tolua_S,"CEWheelRotateExt","CEWheelRotateExt","CCNode",tolua_collect_CEWheelRotateExt);
#else
		tolua_cclass(tolua_S,"CEWheelRotateExt","CEWheelRotateExt","CCNode",NULL);
#endif
		tolua_beginmodule(tolua_S,"CEWheelRotateExt");
		{
			tolua_function(tolua_S,"create"							,tolua_CocosEasy_CEWheelRotateExt_create_00);
			tolua_function(tolua_S,"startWithSpeed"					,tolua_CocosEasy_CEWheelRotateExt_startWithSpeed_00);
			tolua_function(tolua_S,"startToAngel"					,tolua_CocosEasy_CEWheelRotateExt_startToAngel_00);
			tolua_function(tolua_S,"stop"							,tolua_CocosEasy_CEWheelRotateExt_stop_00);
			tolua_function(tolua_S,"registerStopScriptHandler"		,tolua_CocosEasy_CEWheelRotateExt_registerStopScriptHandler_00);
		}
		tolua_endmodule(tolua_S);	

		//CEImeChatBox
#ifdef __cplusplus
		tolua_cclass(tolua_S,"CEImeChatBox","CEImeChatBox","Widget",tolua_collect_CEImeChatBox);
#else
		tolua_cclass(tolua_S,"CEImeChatBox","CEImeChatBox","Widget",NULL);
#endif
		tolua_beginmodule(tolua_S,"CEImeChatBox");
		{
			tolua_function(tolua_S,"create"							,tolua_CocosEasy_CEImeChatBox_create_00);
			tolua_function(tolua_S,"show"							,tolua_CocosEasy_CEImeChatBox_show_00);
			tolua_function(tolua_S,"hide"							,tolua_CocosEasy_CEImeChatBox_hide_00);
			tolua_function(tolua_S,"registInputScript"				,tolua_CocosEasy_CEImeChatBox_registInputScript_00);
			tolua_function(tolua_S,"setFontColor"					,tolua_CocosEasy_CEImeChatBox_setFontColor_00);
			tolua_function(tolua_S,"setFontSize"					,tolua_CocosEasy_CEImeChatBox_setFontSize_00);
			tolua_function(tolua_S,"setFontName"					,tolua_CocosEasy_CEImeChatBox_setFontName_00);
			tolua_function(tolua_S,"getStrInput"					,tolua_CocosEasy_CEImeChatBox_getStrInput_00);
			tolua_function(tolua_S,"reset"							,tolua_CocosEasy_CEImeChatBox_reset_00);
		}
		tolua_endmodule(tolua_S);
        
        //CEImeChatBox
#ifdef __cplusplus
        tolua_cclass(tolua_S,"CCWebView","CCWebView","CCObject",tolua_collect_CCWebView);
#else
        tolua_cclass(tolua_S,"CCWebView","CCWebView","CCObject",NULL);
#endif
        tolua_beginmodule(tolua_S,"CCWebView");
        {
            tolua_function(tolua_S,"create"                                 ,tolua_CocosEasy_CCWebView_create_00);
            tolua_function(tolua_S,"create"                                 ,tolua_CocosEasy_CCWebView_create_01);
            tolua_function(tolua_S,"init"                                   ,tolua_CocosEasy_CCWebView_init_00);
            tolua_function(tolua_S,"showWebView"                            ,tolua_CocosEasy_CCWebView_showWebView_00);
            tolua_function(tolua_S,"updateURL"                              ,tolua_CocosEasy_CCWebView_updateURL_00);
            tolua_function(tolua_S,"removeWebView"                          ,tolua_CocosEasy_CCWebView_removeWebView_00);
            tolua_function(tolua_S,"getActivityName"						,tolua_CocosEasy_CCWebView_getActivityName_00);

        }
        tolua_endmodule(tolua_S);

// model 
#ifdef __cplusplus
		tolua_cclass(tolua_S,"CResourceLoader","CResourceLoader","CCObject",tolua_collect_CResourceLoader);
#else
		tolua_cclass(tolua_S,"CResourceLoader","CResourceLoader","CCObject",NULL);
#endif
		tolua_beginmodule(tolua_S,"CResourceLoader");
		{
			tolua_function(tolua_S,"getInstance",tolua_CocosEasy_CResourceLoader_getInstance_00);
			tolua_function(tolua_S,"start",tolua_CocosEasy_CResourceLoader_start_00);
			tolua_function(tolua_S,"setSearchPath",tolua_CocosEasy_CResourceLoader_setSearchPath_00);
			tolua_function(tolua_S,"addResourceLoaderList",tolua_CocosEasy_CResourceLoader_addResourceLoaderList_00);
			tolua_function(tolua_S,"run",tolua_CocosEasy_CResourceLoader_run_00);
			tolua_function(tolua_S,"run",tolua_CocosEasy_CResourceLoader_run_01);
			tolua_function(tolua_S,"loadArmature",tolua_CocosEasy_CResourceLoader_loadArmature_00);
			tolua_function(tolua_S,"unloadArmature",tolua_CocosEasy_CResourceLoader_unloadArmature_00);
			tolua_function(tolua_S,"loadEffect",tolua_CocosEasy_CResourceLoader_loadEffect_00);
			tolua_function(tolua_S,"unloadEffect",tolua_CocosEasy_CResourceLoader_unloadEffect_00);
			tolua_function(tolua_S,"playEffect",tolua_CocosEasy_CResourceLoader_playEffect_00);
			tolua_function(tolua_S,"isRunning",tolua_CocosEasy_CResourceLoader_isRunning_00);
			tolua_function(tolua_S,"getPercent",tolua_CocosEasy_CResourceLoader_getPercent_00);
			tolua_function(tolua_S,"setMaxLoadStepTime",tolua_CocosEasy_CResourceLoader_setMaxLoadStepTime_00);
			tolua_function(tolua_S,"unloadAllResource",tolua_CocosEasy_CResourceLoader_unloadAllResource_00);
		}
		tolua_endmodule(tolua_S);

#ifdef __cplusplus
		tolua_cclass(tolua_S,"CDBManager","CDBManager","CCObject",tolua_collect_CDBManager); 
#else
		tolua_cclass(tolua_S,"CDBManager","CDBManager","CCObject",NULL);
#endif
		tolua_beginmodule(tolua_S,"CDBManager");
		{
			tolua_function(tolua_S,"getInstance",tolua_CocosEasy_CDBManager_getInstance_00);
			tolua_function(tolua_S,"openDB",tolua_CocosEasy_CDBManager_openDB_00);
			tolua_function(tolua_S,"closeDB",tolua_CocosEasy_CDBManager_closeDB_00);
			tolua_function(tolua_S,"exeSql",tolua_CocosEasy_CDBManager_exeSql_00);
		}
		tolua_endmodule(tolua_S);

#ifdef __cplusplus
        tolua_cclass(tolua_S,"CTimerData","CTimerData","CScriptObject",tolua_collect_CTimerData);
#else
        tolua_cclass(tolua_S,"CTimerData","CTimerData","CScriptObject",NULL);
#endif
        tolua_beginmodule(tolua_S,"CTimerData");
        {
            tolua_function(tolua_S,"new_local",tolua_CocosEasy_CTimerData_new_local_00);
            tolua_function(tolua_S,"addTimer",tolua_CocosEasy_CTimerData_addTimer_00);
            tolua_function(tolua_S,"delTimer",tolua_CocosEasy_CTimerData_delTimer_00);
            tolua_function(tolua_S,"clearTimer",tolua_CocosEasy_CTimerData_clearTimer_00);
        }
        tolua_endmodule(tolua_S);
        
#ifdef __cplusplus
        tolua_cclass(tolua_S,"CSystemTimerData","CSystemTimerData","CTimerData",tolua_collect_CSystemTimerData);
#else
        tolua_cclass(tolua_S,"CSystemTimerData","CSystemTimerData","CTimerData",NULL);
#endif
        tolua_beginmodule(tolua_S,"CSystemTimerData");
        {
            tolua_function(tolua_S,"new_local",tolua_CocosEasy_CSystemTimerData_new_local_00);
        }
        tolua_endmodule(tolua_S);
        
        
#ifdef __cplusplus
        tolua_cclass(tolua_S,"CAsyncSocket","CAsyncSocket","CScriptObject",tolua_collect_CAsyncSocket);
#else
        tolua_cclass(tolua_S,"CAsyncSocket","CAsyncSocket","CScriptObject",NULL);
#endif
        tolua_beginmodule(tolua_S,"CAsyncSocket");
        {
            tolua_function(tolua_S,"new_local",tolua_CocosEasy_CAsyncSocket_new_local_00);
            tolua_function(tolua_S,"connect",tolua_CocosEasy_CAsyncSocket_connect_00);
            tolua_function(tolua_S,"disconnect",tolua_CocosEasy_CAsyncSocket_disconnect_00);
            tolua_function(tolua_S,"send",tolua_CocosEasy_CAsyncSocket_send_00);
            tolua_function(tolua_S,"cancelSend",tolua_CocosEasy_CAsyncSocket_cancelSend_00);
            tolua_function(tolua_S,"registerListener",tolua_CocosEasy_CAsyncSocket_registerListener_00);
			tolua_function(tolua_S,"lastError",tolua_CocosEasy_CAsyncSocket_lastError_00);
			tolua_function(tolua_S,"lastErrorEnum",tolua_CocosEasy_CAsyncSocket_lastErrorEnum_00);
            tolua_function(tolua_S,"setWorkTimeForOnce",tolua_CocosEasy_CAsyncSocket_setWorkTimeForOnce_00);
            tolua_function(tolua_S,"setHeartbeat",tolua_CocosEasy_CAsyncSocket_setHeartbeat_00);
            tolua_function(tolua_S,"isConnected",tolua_CocosEasy_CAsyncSocket_isConnected_00);
            tolua_function(tolua_S,"isIdle",tolua_CocosEasy_CAsyncSocket_isIdle_00);
			tolua_function(tolua_S,"setInnerVersion",tolua_CocosEasy_CAsyncSocket_setInnerVersion_00);
        }
        tolua_endmodule(tolua_S);


#ifdef __cplusplus
		tolua_cclass(tolua_S,"CSceneTransitMgr","CSceneTransitMgr","CCObject",tolua_collect_CSceneTransitMgr);
#else
		tolua_cclass(tolua_S,"CSceneTransitMgr","CSceneTransitMgr","CCObject",NULL);
#endif
		tolua_beginmodule(tolua_S,"CSceneTransitMgr");
		{
			tolua_function(tolua_S,"getInstance",tolua_CocosEasy_CSceneTransitMgr_getInstance_00);
			tolua_function(tolua_S,"replaceScene",tolua_CocosEasy_CSceneTransitMgr_replaceScene_00);
			tolua_function(tolua_S,"runWithScene",tolua_CocosEasy_CSceneTransitMgr_runWithScene_00);
			tolua_function(tolua_S,"setLoadingLayer",tolua_CocosEasy_CSceneTransitMgr_setLoadingLayer_00);
		}
		tolua_endmodule(tolua_S);

#ifdef __cplusplus
		tolua_cclass(tolua_S,"CEventManager","CEventManager","CCObject",tolua_collect_CEventManager);
#else
		tolua_cclass(tolua_S,"CEventManager","CEventManager","CCObject",NULL);
#endif
		tolua_beginmodule(tolua_S,"CEventManager");
		{
			tolua_function(tolua_S,"getInstance",tolua_CocosEasy_CEventManager_getInstance_00);
			tolua_function(tolua_S,"registerScriptEvent",tolua_CocosEasy_CEventManager_registerScriptEvent_00);
		}
		tolua_endmodule(tolua_S);

#ifdef __cplusplus
		tolua_cclass(tolua_S,"CELog","CELog","CCObject",tolua_collect_CELog);
#else
		tolua_cclass(tolua_S,"CELog","CELog","CCObject",NULL);
#endif
		tolua_beginmodule(tolua_S,"CELog");
		{
			tolua_function(tolua_S,"getInstance",tolua_CocosEasy_CELog_getInstance_00);
			tolua_function(tolua_S,"write",tolua_CocosEasy_CELog_write_00);
			tolua_function(tolua_S,"createFile",tolua_CocosEasy_CELog_createFile_00);
			tolua_function(tolua_S,"setLogLevel",tolua_CocosEasy_CELog_setLogLevel_00);
			tolua_function(tolua_S,"setCacheMaxChars",tolua_CocosEasy_CELog_setCacheMaxChars_00);
		}
		tolua_endmodule(tolua_S);

#ifdef __cplusplus
		tolua_cclass(tolua_S,"DynamicDownloadExt","DynamicDownloadExt","CCObject",NULL);
#else
		tolua_cclass(tolua_S,"DynamicDownloadExt","DynamicDownloadExt","CCObject",NULL);
#endif
		tolua_beginmodule(tolua_S,"DynamicDownloadExt");
		{
			tolua_function(tolua_S,"getInstance",tolua_CocosEasy_DynamicDownloadExt_getInstance_00);
			tolua_function(tolua_S,"startDownload",tolua_CocosEasy_DynamicDownloadExt_startDownload_00);
			tolua_function(tolua_S,"setScriptEvent",tolua_CocosEasy_DynamicDownloadExt_setScriptEvent_00);
			tolua_function(tolua_S,"isHasZipInDownloadDir",tolua_CocosEasy_DynamicDownloadExt_isHasZipInDownloadDir_00);
			tolua_function(tolua_S,"uncompressDownloadzip",tolua_CocosEasy_DynamicDownloadExt_uncompressDownloadzip_00);
		}
		tolua_endmodule(tolua_S);

#ifdef __cplusplus
		tolua_cclass(tolua_S,"CLoadingTransitLayer","CLoadingTransitLayer","CCLayer",tolua_collect_CLoadingTransitLayer);
#else
		tolua_cclass(tolua_S,"CLoadingTransitLayer","CLoadingTransitLayer","CCLayer",NULL);
#endif
		tolua_beginmodule(tolua_S,"CLoadingTransitLayer");
		{
			tolua_function(tolua_S,"create",tolua_CocosEasy_CLoadingTransitLayer_create_00);
			tolua_function(tolua_S,"enterNextScene",tolua_CocosEasy_CLoadingTransitLayer_enterNextScene_00);
			tolua_function(tolua_S,"registerLoadStepScriptHandler",tolua_CocosEasy_CLoadingTransitLayer_registerLoadStepScriptHandler_00);
			tolua_function(tolua_S,"getNextScene",tolua_CocosEasy_CLoadingTransitLayer_getNextScene_00);
			tolua_function(tolua_S,"startResourceLoad",tolua_CocosEasy_CLoadingTransitLayer_startResourceLoad_00);
		}
		tolua_endmodule(tolua_S);

#ifdef __cplusplus
		tolua_cclass(tolua_S,"CResLoaderList","CResLoaderList","CCObject",tolua_collect_CResLoaderList);
#else
		tolua_cclass(tolua_S,"CResLoaderList","CResLoaderList","CCObject",NULL);
#endif
		tolua_beginmodule(tolua_S,"CResLoaderList");
		{
			tolua_function(tolua_S,"new_local",tolua_CocosEasy_CResLoaderList_new_local_00);
			tolua_function(tolua_S,"new",tolua_CocosEasy_CResLoaderList_new_00);
			tolua_function(tolua_S,"addTexture",tolua_CocosEasy_CResLoaderList_addTexture_00);
			tolua_function(tolua_S,"addImage",tolua_CocosEasy_CResLoaderList_addImage_00);
			tolua_function(tolua_S,"addArmature",tolua_CocosEasy_CResLoaderList_addArmature_00);
			tolua_function(tolua_S,"addMusic",tolua_CocosEasy_CResLoaderList_addMusic_00);
			tolua_function(tolua_S,"addEffect",tolua_CocosEasy_CResLoaderList_addEffect_00);
			tolua_function(tolua_S,"addUIJson",tolua_CocosEasy_CResLoaderList_addUIJson_00);
		}
		tolua_endmodule(tolua_S);


#ifdef __cplusplus
		tolua_cclass(tolua_S,"CCEPlugin","CCEPlugin","",tolua_collect_CCEPlugin);
#else
		tolua_cclass(tolua_S,"CCEPlugin","CCEPlugin","",NULL);
#endif
		tolua_beginmodule(tolua_S,"CCEPlugin");
		{
			tolua_function(tolua_S,"getDeviceID",tolua_CocosEasy_CCEPlugin_getDeviceID_00);
			tolua_function(tolua_S,"isIOSSimulator",tolua_CocosEasy_CCEPlugin_isIOSSimulator_00);
            tolua_function(tolua_S,"getDeviceModel",tolua_CocosEasy_CCEPlugin_getDeviceModel_00);
			tolua_function(tolua_S,"getDeviceLV",tolua_CocosEasy_CCEPlugin_getDeviceLV_00);
            tolua_function(tolua_S,"getOSVersion",tolua_CocosEasy_CCEPlugin_getOSVersion_00);
            tolua_function(tolua_S,"getLatitude",tolua_CocosEasy_CCEPlugin_getLatitude_00);
            tolua_function(tolua_S,"getLongitude",tolua_CocosEasy_CCEPlugin_getLongitude_00);
            tolua_function(tolua_S,"getAndroidSDCardPath",tolua_CocosEasy_CCEPlugin_getAndroidSDCardPath_00);
            tolua_function(tolua_S,"isJailBreak",tolua_CocosEasy_CCEPlugin_isJailBreak_00);
            tolua_function(tolua_S,"messageBox",tolua_CocosEasy_CCEPlugin_messageBox_00);
            tolua_function(tolua_S,"openURL",tolua_CocosEasy_CCEPlugin_openURL_00);
			tolua_function(tolua_S,"startNotification",tolua_CocosEasy_CCEPlugin_startNotification_00);
			tolua_function(tolua_S,"closeAlarmNotification",tolua_CocosEasy_CCEPlugin_closeAlarmNotification_00);
			tolua_function(tolua_S,"setClipboardTxt",tolua_CocosEasy_CCEPlugin_setClipboardTxt_00);
            tolua_function(tolua_S,"setIdleTimerDisabled",tolua_CocosEasy_CCEPlugin_setIdleTimerDisabled_00);
            tolua_function(tolua_S,"playVibration",tolua_CocosEasy_CCEPlugin_playVibration_00);
            tolua_function(tolua_S,"locationServicesEnabled",tolua_CocosEasy_CCEPlugin_locationServicesEnabled_00);
            tolua_function(tolua_S,"requestLocationService",tolua_CocosEasy_CCEPlugin_requestLocationService_00);
		}
		tolua_endmodule(tolua_S);
        
        
        //ImagePicker
        
#ifdef __cplusplus
        tolua_cclass(tolua_S,"ImagePicker","ImagePicker","CCObject",tolua_collect_ImagePicker);
#else
        tolua_cclass(tolua_S,"ImagePicker","ImagePicker","CCObject",NULL);
#endif
        tolua_beginmodule(tolua_S,"ImagePicker");
        {
            tolua_function(tolua_S,"getInstance",tolua_CocosEasy_ImagePicker_getInstance_00);
            tolua_function(tolua_S,"selectPicAndUpload",tolua_CocosEasy_ImagePicker_selectPicAndUpload_00);
            tolua_function(tolua_S,"selectPicAndUpload",tolua_CocosEasy_ImagePicker_selectPicAndUpload_01);
            tolua_function(tolua_S,"selectPicAndUpload",tolua_CocosEasy_ImagePicker_selectPicAndUpload_02);
            tolua_function(tolua_S,"setTag",tolua_CocosEasy_ImagePicker_setTag_00);
        }
        tolua_endmodule(tolua_S);


#ifdef __cplusplus
		tolua_cclass(tolua_S,"CCClippingNode","CCClippingNode","CCNode",tolua_collect_CCEPlugin);
#else
		tolua_cclass(tolua_S,"CCClippingNode","CCClippingNode","",NULL);
#endif
		tolua_beginmodule(tolua_S,"CCClippingNode");
		{
			tolua_function(tolua_S, "create", tolua_CocosEasy_CCClippingNode_create_00);
			tolua_function(tolua_S, "setInverted", tolua_CocosEasy_CCClippingNode_setInverted_00);
			tolua_function(tolua_S, "setAlphaThreshold", tolua_CocosEasy_CCClippingNode_setAlphaThreshold_00);
			tolua_function(tolua_S, "setStencil", tolua_CocosEasy_CCClippingNode_setStencil_00);
		}
		tolua_endmodule(tolua_S);


#ifdef __cplusplus
		tolua_cclass(tolua_S,"CSDKManager","CSDKManager","CCObject",tolua_collect_CSDKManager);
#else
		tolua_cclass(tolua_S,"CSDKManager","CSDKManager","",NULL);
#endif
		tolua_beginmodule(tolua_S,"CSDKManager");
		{
			tolua_function(tolua_S, "getInstance", tolua_CocosEasy_CSDKManager_getInstance_00);
			tolua_function(tolua_S, "login", tolua_CocosEasy_CSDKManager_login_00);
			tolua_function(tolua_S, "logout", tolua_CocosEasy_CSDKManager_logout_00);
			tolua_function(tolua_S, "userCenter", tolua_CocosEasy_CSDKManager_userCenter_00);
            tolua_function(tolua_S, "thirdLogin", tolua_CocosEasy_CSDKManager_thirdLogin_00);
            tolua_function(tolua_S, "getLoginType", tolua_CocosEasy_CSDKManager_getLoginType_00);

			tolua_function(tolua_S, "isLogined", tolua_CocosEasy_CSDKManager_isLogined_00);
			tolua_function(tolua_S, "isInited", tolua_CocosEasy_CSDKManager_isInited_00);
            tolua_function(tolua_S, "hasUserCenter", tolua_CocosEasy_CSDKManager_hasUserCenter_00);
			tolua_function(tolua_S, "getAccountName", tolua_CocosEasy_CSDKManager_getAccountName_00);
			tolua_function(tolua_S, "getAccountID", tolua_CocosEasy_CSDKManager_getAccountID_00);
			tolua_function(tolua_S, "getSessionID", tolua_CocosEasy_CSDKManager_getSessionID_00);
            tolua_function(tolua_S, "getSimCountryISO", tolua_CocosEasy_CSDKManager_getSimCountryISO_00);
			tolua_function(tolua_S, "getChannelID", tolua_CocosEasy_CSDKManager_getChannelID_00);
            tolua_function(tolua_S, "getDeviceToken", tolua_CocosEasy_CSDKManager_getDeviceToken_00);
            tolua_function(tolua_S, "createRoleAnayasis", tolua_CocosEasy_CSDKManager_createRoleAnayasis_00);
            tolua_function(tolua_S, "enterMainSceneAnayasis", tolua_CocosEasy_CSDKManager_enterMainSceneAnayasis_00);
            tolua_function(tolua_S, "playVideo", tolua_CocosEasy_CSDKManager_playVideo_00);
			tolua_function(tolua_S, "registerLoginScriptHandler", tolua_CocosEasy_CSDKManager_registerLoginScriptHandler_00);
            tolua_function(tolua_S, "registerThirdLoginScriptHandler", tolua_CocosEasy_CSDKManager_registerThirdLoginScriptHandler_00);
            tolua_function(tolua_S, "unRegisterThirdLoginScriptHandler", tolua_CocosEasy_CSDKManager_unRegisterThirdLoginScriptHandler_00);
            tolua_function(tolua_S, "registerLogoutScriptHandler", tolua_CocosEasy_CSDKManager_registerLogoutScriptHandler_00);
            tolua_function(tolua_S, "registerVideoScriptHandler", tolua_CocosEasy_CSDKManager_registerVideoScriptHandler_00);
            tolua_function(tolua_S, "registerShareScriptHandler", tolua_CocosEasy_CSDKManager_registerShareScriptHandler_00);
            tolua_function(tolua_S, "preLoadGoods", tolua_CocosEasy_CSDKManager_preLoadGoods_00);
            tolua_function(tolua_S, "pay", tolua_CocosEasy_CSDKManager_pay_00);
            tolua_function(tolua_S, "share", tolua_CocosEasy_CSDKManager_share_00);
            tolua_function(tolua_S, "getMissOrderList", tolua_CocosEasy_CSDKManager_getMissOrderList_00);
            tolua_function(tolua_S, "getOrderInfoAndRemove", tolua_CocosEasy_CSDKManager_getOrderInfoAndRemove_00);
            
            tolua_function(tolua_S, "registerBuyScriptHandler", tolua_CocosEasy_CSDKManager_registerBuyScriptHandler_00);
            
            tolua_function(tolua_S, "showWebPayView", tolua_CocosEasy_CSDKManager_showWebPayView_00);
            tolua_function(tolua_S, "payWithPayPal", tolua_CocosEasy_CSDKManager_payWithPayPal_00);
            tolua_function(tolua_S, "isWXAppInstalled", tolua_CocosEasy_CSDKManager_isWXAppInstalled_00);
		}
		tolua_endmodule(tolua_S);

//IM
#if CASINO_VOICE
#ifdef __cplusplus
        tolua_cclass(tolua_S,"CIMManager","CIMManager","CCObject",tolua_collect_CIMManager);
#else
        tolua_cclass(tolua_S,"CIMManager","CIMManager","",NULL);
#endif
        tolua_beginmodule(tolua_S,"CIMManager");
        {
            tolua_function(tolua_S,"getInstance",tolua_CocosEasy_CIMManager_getInstance_00);
            tolua_function(tolua_S,"update",tolua_CocosEasy_CIMManager_update_00);
            tolua_function(tolua_S,"beginRecord", tolua_CocosEasy_CIMManager_beginRecord_00);
            tolua_function(tolua_S, "stopRecord", tolua_CocosEasy_CIMManager_stopRecord_00);
            tolua_function(tolua_S, "enterRoom", tolua_CocosEasy_CIMManager_enterRoom_00);
            tolua_function(tolua_S, "downloadRecord", tolua_CocosEasy_CIMManager_downloadRecord_00);
			tolua_function(tolua_S, "registerUploadScriptHandler", tolua_CocosEasy_CIMManager_registerUploadScriptHandler_00);
            tolua_function(tolua_S, "registerDownloadScriptHandler", tolua_CocosEasy_CIMManager_registerDownloadScriptHandler_00);
        }
        tolua_endmodule(tolua_S);
#endif

#ifdef __cplusplus
		tolua_cclass(tolua_S,"CAnalysis","CAnalysis","CCObject",tolua_collect_CAnalysis);
#else
		tolua_cclass(tolua_S,"CAnalysis","CAnalysis","",NULL);
#endif
		tolua_beginmodule(tolua_S,"CAnalysis");
		{
			tolua_function(tolua_S, "getInstance", tolua_CocosEasy_CAnalysis_getInstance_00);
            tolua_function(tolua_S, "setPlayerID", tolua_CocosEasy_CAnalysis_setPlayerID_00);
            tolua_function(tolua_S, "setServerID", tolua_CocosEasy_CAnalysis_setServerID_00);
			tolua_function(tolua_S, "logEvent", tolua_CocosEasy_CAnalysis_logEvent_00);
            tolua_function(tolua_S, "logPurchase", tolua_CocosEasy_CAnalysis_logPurchase_00);
            tolua_function(tolua_S, "logRegister", tolua_CocosEasy_CAnalysis_logRegister_00);
            tolua_function(tolua_S, "logLogin", tolua_CocosEasy_CAnalysis_logLogin_00);
			tolua_function(tolua_S, "pageviewStart", tolua_CocosEasy_CAnalysis_pageviewStart_00);
			tolua_function(tolua_S, "pageviewEnd", tolua_CocosEasy_CAnalysis_pageviewEnd_00);
            tolua_function(tolua_S, "getAdID", tolua_CocosEasy_CAnalysis_getAdID_00);
		}
		tolua_endmodule(tolua_S);

		// ccHSL3F
#ifdef __cplusplus
		tolua_cclass(tolua_S,"ccHSL3F","ccHSL3F","",tolua_collect_ccHSL3F);
#else
		tolua_cclass(tolua_S,"ccHSL3F","ccHSL3F","",NULL);
#endif
		tolua_beginmodule(tolua_S,"ccHSL3F");
		{
			tolua_variable(tolua_S,"h",tolua_CocosEasy_ccHSL3F_get_h, tolua_CocosEasy_ccHSL3F_set_h);
			tolua_variable(tolua_S,"s",tolua_CocosEasy_ccHSL3F_get_s, tolua_CocosEasy_ccHSL3F_set_s);
			tolua_variable(tolua_S,"l",tolua_CocosEasy_ccHSL3F_get_l, tolua_CocosEasy_ccHSL3F_set_l);
			tolua_function(tolua_S,"new_local",tolua_CocosEasy_ccHSL3F_new_local_00);
		}
		tolua_endmodule(tolua_S);

        // const arguments
        tolua_constant(tolua_S,"CE_EVENT_APP_HOME",CE_EVENT_APP_HOME);
        tolua_constant(tolua_S,"CE_EVENT_APP_BACK",CE_EVENT_APP_BACK);

	}// global
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    tolua_tdga_open(tolua_S);
#endif
	tolua_endmodule(tolua_S);
	return 1;
}
