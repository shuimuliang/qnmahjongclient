#ifndef __LUALOADEXTS_H_
#define __LUALOADEXTS_H_

#if defined(_USRDLL)
#define LUA_EXTENSIONS_DLL     __declspec(dllexport)
#else         /* use a DLL library */
#define LUA_EXTENSIONS_DLL
#endif


#ifdef __cplusplus
extern "C" {
#endif
    #include "lauxlib.h"
void LUA_EXTENSIONS_DLL luax_loadexts(lua_State *L);
    
    
#ifdef __cplusplus
}
#endif
#endif // __LUALOADEXTS_H_
