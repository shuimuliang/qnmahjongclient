#ifndef __TOLUA_CCHTTPCLIENT__
#define __TOLUA_CCHTTPCLIENT__

extern "C" {
#include "tolua++.h"
#include "tolua_fix.h"
}
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

//######################################## CCHttpClient ##########################
//CCHttpClient::getInstance
static int tolua_CCHttpClient_getInstance(lua_State *l){
	tolua_pushusertype(l, CCHttpClient::getInstance(), "CCHttpClient");
	return 1;
}
//CCHttpClient::getTimeoutForConnect
static int tolua_CCHttpClient_getTimeoutForConnect(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpClient", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpClient.getTimeoutForConnect'.",&err);
		return 0;
	}
#endif
	CCHttpClient *o = (CCHttpClient *)tolua_tousertype(l, 1, NULL);
	tolua_pushnumber(l, o? o->getTimeoutForConnect() : -1);
	return 1;
}
//CCHttpClient::setTimeoutForConnect
static int tolua_CCHttpClient_setTimeoutForConnect(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpClient", 0, &err) || !tolua_isnumber(l, 2, 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpClient.setTimeoutForConnect'.",&err);
		return 0;
	}
#endif
	CCHttpClient *o = (CCHttpClient *)tolua_tousertype(l, 1, NULL);
	if(o){ o->setTimeoutForConnect(tolua_tonumber(l, 2, -1));}
	tolua_pushusertype(l, o, "CCHttpClient");
	return 1;
}
//CCHttpClient::getTimeoutForRead
static int tolua_CCHttpClient_getTimeoutForRead(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpClient", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpClient.getTimeoutForRead'.",&err);
		return 0;
	}
#endif
	CCHttpClient *o = (CCHttpClient *)tolua_tousertype(l, 1, NULL);
	tolua_pushnumber(l, o? o->getTimeoutForRead() : -1);
	return 1;
}
//CCHttpClient::setTimeoutForRead
static int tolua_CCHttpClient_setTimeoutForRead(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpClient", 0, &err) || !tolua_isnumber(l, 2, 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpClient.setTimeoutForRead'.",&err);
		return 0;
	}
#endif
	CCHttpClient *o = (CCHttpClient *)tolua_tousertype(l, 1, NULL);
	if(o){ o->setTimeoutForRead(tolua_tonumber(l, 2, -1));}
	tolua_pushusertype(l, o, "CCHttpClient");
	return 1;
}
//CCHttpClient::send
static int tolua_CCHttpClient_send(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpClient", 0, &err) || !tolua_isusertype(l, 2, "CCHttpRequest", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpClient.send'.",&err);
		return 0;
	}
#endif
	CCHttpClient *o = (CCHttpClient *)tolua_tousertype(l, 1, NULL);
	CCHttpRequest *req = (CCHttpRequest *)tolua_tousertype(l, 2, NULL);
	if(o && req){ o->send(req);}
	tolua_pushusertype(l, o, "CCHttpClient");
	return 1;
}
//######################################## CCHttpRequest ##########################
//CCHttpRequest::open(const char *url = NULL, int type = kHttpGet, const char *postData = NULL)
static int tolua_CCHttpRequest_open(lua_State *l){
	CCHttpRequest *req = new CCHttpRequest();
	// URL
	const char *s = tolua_tostring(l, 2, NULL);
	if(s){ req->setUrl(s);}
	req->setRequestType((CCHttpRequest::HttpRequestType)(int)tolua_tonumber(l, 3, CCHttpRequest::kHttpGet));
	// Post Data
	s = tolua_tostring(l, 4, NULL);
	if(s){ req->setRequestData(s, strlen(s));}
	tolua_pushusertype(l, req, "CCHttpRequest");
	return 1;
}
//CCHttpRequest::sendWithHandler
static int tolua_CCHttpRequest_sendWithHandler(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err) || !toluafix_isfunction(l, 2, "LUA_FUNCTION", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.getRequestType'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	int hnd = toluafix_ref_function(l, 2, 0);
	LuaEventHandler *h = LuaEventHandler::create(l)->handle(o, hnd);
	tolua_pushusertype(l, h, "LuaEventHandler");
	return 1;
}
//CCHttpRequest::getRequestType
static int tolua_CCHttpRequest_getRequestType(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.getRequestType'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	tolua_pushnumber(l, o? o->getRequestType() : CCHttpRequest::kHttpUnkown);
	return 1;
}
//CCHttpRequest::setRequestType
static int tolua_CCHttpRequest_setRequestType(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err) || !tolua_isnumber(l, 2, 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.setRequestType'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	if(o){ o->setRequestType((CCHttpRequest::HttpRequestType)(int)tolua_tonumber(l, 2, CCHttpRequest::kHttpGet));}
	tolua_pushusertype(l, o, "CCHttpRequest");
	return 1;
}
//CCHttpRequest::getUrl
static int tolua_CCHttpRequest_getUrl(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.getUrl'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	tolua_pushstring(l, o? o->getUrl() : NULL);
	return 1;
}
//CCHttpRequest::setUrl
static int tolua_CCHttpRequest_setUrl(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err) || !tolua_isstring(l, 2, 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.setUrl'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	if(o){ o->setUrl(tolua_tostring(l, 2, ""));}
	tolua_pushusertype(l, o, "CCHttpRequest");
	return 1;
}
//CCHttpRequest::getRequestData
static int tolua_CCHttpRequest_getRequestData(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.getRequestData'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	tolua_pushstring(l, o? o->getRequestData() : NULL);
	return 1;
}
//CCHttpRequest::setRequestData
static int tolua_CCHttpRequest_setRequestData(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err) || !tolua_isstring(l, 2, 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.setRequestData'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	if(o){
		const char *d = tolua_tostring(l, 2, "");
		o->setRequestData(d, strlen(d));
	}
	tolua_pushusertype(l, o, "CCHttpRequest");
	return 1;
}
//CCHttpRequest::getRequestDataSize
static int tolua_CCHttpRequest_getRequestDataSize(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.getRequestDataSize'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	tolua_pushnumber(l, o? o->getRequestDataSize() : 0);
	return 1;
}
//CCHttpRequest::getTag
static int tolua_CCHttpRequest_getTag(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.getTag'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	tolua_pushstring(l, o? o->getTag() : NULL);
	return 1;
}
//CCHttpRequest::setTag
static int tolua_CCHttpRequest_setTag(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err) || !tolua_isstring(l, 2, 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.setTag'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	if(o){ o->setTag(tolua_tostring(l, 2, ""));}
	tolua_pushusertype(l, o, "CCHttpRequest");
	return 1;
}
//CCHttpRequest::getUserData
static int tolua_CCHttpRequest_getUserData(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.getUserData'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	tolua_pushuserdata(l, o? o->getUserData() : NULL);
	return 1;
}
//CCHttpRequest::setUserData
static int tolua_CCHttpRequest_setUserData(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err) || !tolua_isuserdata(l, 2, 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.setUserData'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	if(o){ o->setUserData(tolua_touserdata(l, 2, NULL));}
	tolua_pushusertype(l, o, "CCHttpRequest");
	return 1;
}
//CCHttpRequest::getTarget
static int tolua_CCHttpRequest_getTarget(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.getTarget'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	tolua_pushusertype(l, o? o->getTarget() : NULL, "CCObject");
	return 1;
}
//CCHttpRequest::getHeaders
static int tolua_CCHttpRequest_getHeaders(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.getHeaders'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	if(o){
		std::vector<std::string> hs = o->getHeaders();
		CCArray *ds = CCArray::create();
		for(std::vector<std::string>::iterator it = hs.begin(); it != hs.end(); it ++){
			ds->addObject(CCString::create(*it));
		}
		tolua_pushusertype(l, ds, "CCArray");
	}
	return 1;
}
//CCHttpRequest::setHeaders
static int tolua_CCHttpRequest_setHeaders(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpRequest", 0, &err) || !tolua_isusertype(l, 2, "CCArray", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpRequest.setHeaders'.",&err);
		return 0;
	}
#endif
	CCHttpRequest *o = (CCHttpRequest *)tolua_tousertype(l, 1, NULL);
	CCArray *ds = (CCArray *)tolua_tousertype(l, 2, NULL);
	if(o && ds){
		std::vector<std::string> hs;
		CCObject *_o;
		CCString *s;
		CCARRAY_FOREACH(ds, _o){
			s = dynamic_cast<CCString *>(_o);
			if(s){
				hs.push_back(s->getCString());
			}
		}
		o->setHeaders(hs);
	}
	tolua_pushusertype(l, o, "CCHttpRequest");
	return 1;
}

//######################################## CCHttpResponse ##########################
//CCHttpResponse::getHttpRequest
static int tolua_CCHttpResponse_getHttpRequest(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpResponse", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpResponse.getHttpRequest'.",&err);
		return 0;
	}
#endif
	CCHttpResponse *o = (CCHttpResponse *)tolua_tousertype(l, 1, NULL);
	tolua_pushusertype(l, o? o->getHttpRequest() : NULL, "CCHttpRequest");
	return 1;
}
//CCHttpResponse::isSucceed
static int tolua_CCHttpResponse_isSucceed(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpResponse", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpResponse.isSucceed'.",&err);
		return 0;
	}
#endif
	CCHttpResponse *o = (CCHttpResponse *)tolua_tousertype(l, 1, NULL);
	tolua_pushboolean(l, o? o->isSucceed() : false);
	return 1;
}
//CCHttpResponse::setSucceed
static int tolua_CCHttpResponse_setSucceed(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpResponse", 0, &err) || !tolua_isnumber(l, 2, 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpResponse.setSucceed'.",&err);
		return 0;
	}
#endif
	CCHttpResponse *o = (CCHttpResponse *)tolua_tousertype(l, 1, NULL);
	if(o){ o->setSucceed(tolua_toboolean(l, 2, 0) == 1);}
	tolua_pushusertype(l, o, "CCHttpResponse");
	return 1;
}
//CCHttpResponse::getResponseData
static int tolua_CCHttpResponse_getResponseData(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpResponse", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpResponse.getResponseData'.",&err);
		return 0;
	}
#endif
	CCHttpResponse *o = (CCHttpResponse *)tolua_tousertype(l, 1, NULL);
	std::string s;
	if(o){
		std::vector<char> *data = o->getResponseData();
		size_t l = data->size();
		char *cs = new char[l + 1];
		cs[l] = '\0';
		size_t i = 0;
		for(std::vector<char>::iterator it = data->begin(); it < data->end(); it ++){
			cs[i] = *it;
			i ++;
		}
		s = cs;
		delete cs;
	}
	tolua_pushstring(l, s.c_str());
	return 1;
}
//CCHttpResponse::setResponseData
static int tolua_CCHttpResponse_setResponseData(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpResponse", 0, &err) || !tolua_isstring(l, 2, 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpResponse.setResponseData'.",&err);
		return 0;
	}
#endif
	CCHttpResponse *o = (CCHttpResponse *)tolua_tousertype(l, 1, NULL);
	const char *cs = tolua_tostring(l, 2, NULL);
	if(o && cs){
		std::vector<char> data;
		for(size_t i = 0; i < strlen(cs); i ++){
			data.push_back(cs[i]);
		}
		o->setResponseData(&data);
	}
	tolua_pushusertype(l, o, "CCHttpResponse");
	return 1;
}
//CCHttpResponse::getResponseCode
static int tolua_CCHttpResponse_getResponseCode(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpResponse", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpResponse.getResponseCode'.",&err);
		return 0;
	}
#endif
	CCHttpResponse *o = (CCHttpResponse *)tolua_tousertype(l, 1, NULL);
	tolua_pushnumber(l, o? o->getResponseCode() : 0);
	return 1;
}
//CCHttpResponse::setResponseCode
static int tolua_CCHttpResponse_setResponseCode(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpResponse", 0, &err) || !tolua_isnumber(l, 2, 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpResponse.setResponseCode'.",&err);
		return 0;
	}
#endif
	CCHttpResponse *o = (CCHttpResponse *)tolua_tousertype(l, 1, NULL);
	if(o){ o->setResponseCode(tolua_tonumber(l, 2, 0));}
	tolua_pushusertype(l, o, "CCHttpResponse");
	return 1;
}
//CCHttpResponse::getErrorBuffer
static int tolua_CCHttpResponse_getErrorBuffer(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpResponse", 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpResponse.getErrorBuffer'.",&err);
		return 0;
	}
#endif
	CCHttpResponse *o = (CCHttpResponse *)tolua_tousertype(l, 1, NULL);
	tolua_pushstring(l, o? o->getErrorBuffer() : NULL);
	return 1;
}
//CCHttpResponse::setErrorBuffer
static int tolua_CCHttpResponse_setErrorBuffer(lua_State *l){
#ifndef TOLUA_RELEASE
	tolua_Error err;
	if(!tolua_isusertype(l, 1, "CCHttpResponse", 0, &err) || !tolua_isstring(l, 2, 0, &err)){
		tolua_error(l,"#ferror in function 'CCHttpResponse.setErrorBuffer'.",&err);
		return 0;
	}
#endif
	CCHttpResponse *o = (CCHttpResponse *)tolua_tousertype(l, 1, NULL);
	if(o){ o->setErrorBuffer(tolua_tostring(l, 2, ""));}
	tolua_pushusertype(l, o, "CCHttpResponse");
	return 1;
}


#endif //__TOLUA_CCHTTPCLIENT__