/***************************************************
* 中清龙图
* Copyright (C) 2013 -  All Rights Reserved.
* -----------------------------------------------------
* 当前版本 : 1.0
* 作     者: 刘宗安
* 时     间: 2013-07-01 12:13:02
* 注     释:

****************************************************/

#ifndef _CCPTHREAD_H_
#define _CCPTHREAD_H_

#include "cocos2d.h"
#include "CCRunable.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8 )
#include "CCPTHreadWinRT.h"
#else
#include <pthread.h>
#include <semaphore.h>
#endif
#include "CCSem.h"
USING_NS_CC;

namespace Engine
{
	namespace thread_pool
	{

		class CCPThread : public CCRunable,public CCObject
		{
		public:
			CCPThread(void);
			~CCPThread(void);
			int join(void);//准备先不使用这个方法
			virtual void run(void);//需要复写的方法
			void start(void);//线程开始
			void cancel(void);
			void kill(void);
			bool isRunning;
			CCSem m_sem;
		private:

			pthread_t m_loadingThread;





		};

		void CCSleep(int sleepMs);
	}
}
#endif//_CCPTHREAD_H_

