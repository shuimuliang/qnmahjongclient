//
//  TalkingDataGA-Lua.h
//  LuaDemo
//
//  Created by liweiqiang on 13-10-24.
//
//

#ifndef __LuaDemo__TalkingDataGA_Lua__
#define __LuaDemo__TalkingDataGA_Lua__

#include <stdio.h>
extern "C" {
#include "lua.h"
}

void tolua_reg_tdga_types (lua_State* tolua_S);
void tolua_tdga_open (lua_State* tolua_S);

#endif /* defined(__LuaDemo__TalkingDataGA_Lua__) */
