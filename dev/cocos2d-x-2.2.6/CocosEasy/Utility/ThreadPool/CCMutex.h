/***************************************************
* 中清龙图
* Copyright (C) 2013 -  All Rights Reserved.
* -----------------------------------------------------
* 当前版本 : 1.0
* 作     者: 刘宗安
* 时     间: 2013-07-01 16:39:31
* 注     释: 线程锁

****************************************************/
#ifndef _CCMUTEX_H_
#define _CCMUTEX_H_

#include "cocos2d.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8 )
#include "CCPTHreadWinRT.h"
#else
#include <pthread.h>
#include <semaphore.h>
#endif
USING_NS_CC;

namespace Engine
{
	namespace thread_pool
	{
		class CCMutex :	public CCObject
		{
		private:
			pthread_mutex_t this_lock;
		public:
			CCMutex(void);
			~CCMutex(void);
			void lock(void); 
			void unlock(void);
		};
	}
}
#endif //_CCMUTEX_H_