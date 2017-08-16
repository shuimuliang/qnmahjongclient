/***************************************************
* 中清龙图
* Copyright (C) 2013 -  All Rights Reserved.
* -----------------------------------------------------
* 当前版本 : 1.0
* 作     者: 刘宗安
* 时     间: 2013-07-01 17:31:21
* 注     释: 线程池实现

****************************************************/

#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include "cocos2d.h"
#include "CCRunable.h"
#include "CCMutex.h"
#include "CCPThread.h"
#include <queue>
USING_NS_CC;

namespace Engine
{
	namespace thread_pool
	{
		class CCThreadTask;
		class CCThreadPool : public CCObject
		{
		public:
			CCThreadPool(unsigned int size,bool isAutoStartUp = true);
			~CCThreadPool(void);
			void startUp();
			void append(CCObject* job);
			void clean();
			static CCThreadPool *sharedThreadPool();
			friend class CCThreadTask;
		private:
			//std::queue<CCRunable*> m_jobs;
			CCArray *m_jobs;
			unsigned int m_threadNum;
			volatile unsigned int m_runningJobs;
			bool m_running;
			bool m_isStartUp;
			CCMutex m_loadMutex;
			void runJob();
			CCThreadTask* m_threads;
			CCSem m_sem;

		};


		class CCThreadTask : public thread_pool::CCPThread
		{
		public:
			CCThreadTask();
			~CCThreadTask();
			virtual void run(void);//线程开始
			void setThreadPool(CCThreadPool *pool);
		private:
			CCThreadPool *m_pool;  
		};
	}
}
#endif//_THREAD_POOL_H_