#ifndef __CE_LUA_COCOS_EASY_H__
#define __CE_LUA_COCOS_EASY_H__

#include "Common/CocosEasyDef.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

TOLUA_API int tolua_CocosEasy_open(lua_State* tolua_S);



// ================================ define ================================ //
#define ce_tolua_toboolean(L, narg, def) (tolua_toboolean(L, narg, (bool)def ) != 0)
#define ce_tolua_pushboolean(L, value) tolua_pushboolean(L, (bool)value )

#define tolua_collect_def(classname) \
	static int tolua_collect_##classname (lua_State* tolua_S) \
{\
	classname* self = (classname*) tolua_tousertype(tolua_S,1,0); \
	Mtolua_delete(self); \
	return 0; \
}\

#define tolua_collect_script_object_def(classname) \
static int tolua_collect_##classname (lua_State* tolua_S) \
{\
classname* self = (classname*) tolua_tousertype(tolua_S,1,0); \
self->destroy(); \
return 0; \
}\

#define tolua_CocosEasy_class_func_string(classname,funcname,num) tolua_CocosEasy_##classname##_##funcname##_##num 
#define tolua_CocosEasy_class_function_def(classname,funcname,num) static int tolua_CocosEasy_class_func_string(classname,funcname,num)(lua_State* tolua_S)

#define tolua_CocosEasy_get_class_variable(classname,variable) static int tolua_CocosEasy_##classname##_get_##variable(lua_State* tolua_S)
#define tolua_CocosEasy_set_class_variable(classname,variable) static int tolua_CocosEasy_##classname##_set_##variable(lua_State* tolua_S)

#define tolua_CocosEasy_get_set_class_variable(classname,variable,typeVariable,tofunc,pushfunc,def) \
	tolua_CocosEasy_get_class_variable(classname,variable) \
{ \
	classname* self = (classname*)  tolua_tousertype(tolua_S,1,0); \
	pushfunc(tolua_S, (typeVariable)self->variable ); \
	return 1; \
} \
	tolua_CocosEasy_set_class_variable(classname,variable) \
{\
	classname* self = (classname*)  tolua_tousertype(tolua_S,1,0); \
	self->variable = (typeVariable) tofunc(tolua_S,2,def); \
	return 0; \
}\

#define tolua_CocosEasy_get_set_class_int(classname,variable) \
	tolua_CocosEasy_get_set_class_variable(classname,variable,int tolua_tonumber,tolua_pushnumber,0)

#define tolua_CocosEasy_get_set_class_uint(classname,variable) \
	tolua_CocosEasy_get_set_class_variable(classname,variable,unsigned int,tolua_tonumber,tolua_pushnumber,0)

#define tolua_CocosEasy_get_set_class_string(classname,variable) \
	tolua_CocosEasy_get_set_class_variable(classname,variable,const char*,tolua_tostring,tolua_pushstring,NULL)

#define tolua_CocosEasy_get_set_class_float(classname,variable) \
	tolua_CocosEasy_get_set_class_variable(classname,variable,float,ce_tolua_toboolean,tolua_pushnumber,0.0f)

#define tolua_CocosEasy_get_set_class_bool(classname,variable) \
	tolua_CocosEasy_get_set_class_variable(classname,variable,bool,ce_tolua_toboolean,ce_tolua_pushboolean,false)

#define tolua_CocosEasy_get_set_class_reference(classname,variable,typeVariable) \
	tolua_CocosEasy_get_class_variable(classname,variable) \
{ \
	classname* self = (classname*)  tolua_tousertype(tolua_S,1,0); \
	tolua_pushusertype(tolua_S,(void*)&self->variable, #typeVariable); \
	return 1; \
} \
	tolua_CocosEasy_set_class_variable(classname,variable) \
{\
	classname* self = (classname*)  tolua_tousertype(tolua_S,1,0); \
	self->variable = *( (typeVariable*)tolua_tousertype(tolua_S,2,0) ); \
	return 0; \
}\

#endif