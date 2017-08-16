//
//  lua_cjson.h
//  HelloLua
//
//  Created by yang zhu on 13-1-18.
//
//

#ifndef __LUA_CJSON_H_
#define __LUA_CJSON_H_

#define ENABLE_CJSON_GLOBAL 1

#ifndef USE_INTERNAL_FPCONV
#define USE_INTERNAL_FPCONV
#endif


#include <assert.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "lua.h"
#include "lauxlib.h"
#include "strbuf.h"
#include "fpconv.h"
int luaopen_cjson(lua_State *l);
int luaopen_cjson_safe(lua_State *l);

#ifdef __cplusplus
}
#endif
#endif // __LUA_CJSON_H_