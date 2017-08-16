#ifndef __CE_COMM_INCLUDE_H__
#define __CE_COMM_INCLUDE_H__

// C
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>
#include <errno.h>

// C++
#include <map>
#include <string>
#include <vector>
#include <stack>
#ifdef __GNUC__
//#include <ext/hash_map>
//#define HashMap __gnu_cxx::map
	#define HashMap std::map
#else
#include <hash_map>
#define HashMap std::hash_map
#endif

unsigned int getTickCount();
unsigned int hashString(const char* str);
void wait(unsigned int ms);




// common define
#ifndef PI
#define PI 3.14159265358979323846
#endif


#if defined(WP8) && defined(_WP8)
	#include "CCPThreadWinRT.h"
	#include <ppl.h>
	#include <ppltasks.h>
using namespace concurrency;
#else
	#include <pthread.h>
#endif

#include <curl/curl.h>

#ifdef WIN32
#define __CE_MEMORY_CHECK__		//_CrtCheckMemory();
#else
#define __CE_MEMORY_CHECK__
#endif

#endif