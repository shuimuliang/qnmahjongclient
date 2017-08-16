#include "CCThreadPool.h"
#include <errno.h>

namespace Engine
{

	namespace thread_pool
	{


		static CCThreadPool *g_sharedThreadPool = NULL;

		CCThreadPool * CCThreadPool::sharedThreadPool()
		{
			if (!g_sharedThreadPool)
			{
				g_sharedThreadPool = new CCThreadPool(5);
			}
			return g_sharedThreadPool;
		}

		CCThreadPool::CCThreadPool(unsigned int size,bool isAutoStartUp):m_running(true),m_threads(NULL),m_runningJobs(0),m_threadNum(0),m_jobs(NULL),m_isStartUp(false)
		{
			m_threadNum = size;
			m_sem.init();

			m_jobs = new CCArray();
			m_jobs->init();

			if(isAutoStartUp)
			{
				startUp();
			}


		}

		void CCThreadPool::startUp()
		{
			if(!m_isStartUp)
			{
				m_threads = new CCThreadTask[m_threadNum];
				for (unsigned int i = 0; i < m_threadNum; i++) 
				{
					m_threads[i].setThreadPool(this);
					m_threads[i].start();
				}
				m_isStartUp = true;
			}

		}

		CCThreadPool::~CCThreadPool(void)
		{
			m_running = false;
			CCLog("start delete CCThreadPool  m_runningJobs:%d",m_runningJobs);
			if(m_threads)
			{
				unsigned int i = 0;
				for (; i < m_threadNum; i++) 
				{
					//防止线程还在等待状态
					m_sem.post();
					m_threads[i].join();
				}
				CCLog("wait all thread stop");
				delete[] m_threads;
				m_threads = NULL;
			}

			m_sem.destroy();
			m_loadMutex.lock();
			m_jobs->removeAllObjects();
			CC_SAFE_RELEASE(m_jobs);
			m_jobs = NULL;
			m_loadMutex.unlock();

		}

		void CCThreadPool::append(CCObject* job) 
		{
			m_loadMutex.lock();
			m_runningJobs++;
			m_jobs->addObject(job);
			m_sem.post();
			m_loadMutex.unlock();
		} 

		void CCThreadPool::runJob() 
		{
			do{
				m_sem.wait();
				m_loadMutex.lock();
				CCRunable *curjob = NULL;
				if (m_jobs && m_jobs->count() > 0) 
				{
					CCObject *cur_obj = m_jobs->lastObject();
					cur_obj->retain();
					curjob = dynamic_cast<CCRunable*>(cur_obj);  
					CC_ASSERT(curjob);
					m_jobs->removeLastObject();
					m_runningJobs--;
				}
				m_loadMutex.unlock();

				if (curjob) 
				{	//succ acquire a Job
					curjob->run();
					CCObject *cur_obj = dynamic_cast<CCObject*>(curjob);
					cur_obj->release();
				} 
			}while(m_running);
		}

		void CCThreadPool::clean()
		{
			m_loadMutex.lock();
			m_jobs->removeAllObjects();
			m_loadMutex.unlock();
		}



		void CCThreadTask::run( void )
		{
			m_pool->runJob();
		}

		CCThreadTask::CCThreadTask():m_pool(NULL)
		{

		}

		void CCThreadTask::setThreadPool( CCThreadPool *pool )
		{
			m_pool = pool;
		}

		CCThreadTask::~CCThreadTask()
		{
			CCLog("~CCThreadTask");
		}

	}
}