/***************************************************
* 中清龙图
* Copyright (C) 2013 -  All Rights Reserved.
* -----------------------------------------------------
* 当前版本 : 1.0
* 作     者: 刘宗安
* 时     间: 2013-07-29 14:26:06
* 注     释: 信号量

****************************************************/

#ifndef _CCSEM_H_
#define _CCSEM_H_


#include "cocos2d.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8 )
#include "CCPTHreadWinRT.h"
#include "semaphore.h"
#else
#include <pthread.h>
#include <semaphore.h>
#endif

USING_NS_CC;

namespace Engine
{

	static int sem = 0;
	class CCSem
	{
	public:
		CCSem();
		~CCSem(void);
		void post(void);
		void wait(void);
		void init(void);
		void destroy(void);
	private:
		std::string name;
		sem_t* s_pSem;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define CC_SEM_USE_NAMED_SEMAPHORE 1
#else
#define CC_SEM_USE_NAMED_SEMAPHORE 0
#endif


#if !CC_SEM_USE_NAMED_SEMAPHORE
		sem_t s_sem;
#endif
	};

}
#endif
