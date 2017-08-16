/***************************************************
* 中清龙图
* Copyright (C) 2013 -  All Rights Reserved.
* -----------------------------------------------------
* 当前版本 : 1.0
* 作     者: 刘宗安
* 时     间: 2013-07-01 12:14:41
* 注     释:

****************************************************/


#include "CCPThread.h"
#include <errno.h>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#include <windows.h>
#else
#include <unistd.h>
#include <signal.h>
#endif

namespace Engine
{

	namespace thread_pool
	{
		void handle_quit(int signo)
		{
			CCLog("in qq handle sig %d \n", signo);
			pthread_exit(NULL);

		}
		void *wrapper(void *param) 
		{
			do 
			{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
				signal(SIGQUIT,handle_quit );
#endif
				CCPThread *t = (CCPThread *) param;
				t->run();
				t->isRunning = false;
				t->m_sem.post();
			} while (0);


			return NULL;
		}

		CCPThread::CCPThread(void):isRunning(false)
		{
			m_sem.init();
		} 


		CCPThread::~CCPThread(void)
		{
			if(isRunning)
			{
				CCLog("delete CCPThread have running");
				m_sem.wait();
				m_sem.destroy();
			}
		}

		void CCPThread::run(void)
		{
			CCThread thread;
			thread.createAutoreleasePool();
		}

		void CCPThread::start( void )
		{
			isRunning = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
			int err = pthread_create_with_args(&m_loadingThread, NULL, wrapper, (void *)this);
#else 
			int err = pthread_create(&m_loadingThread, NULL, wrapper, (void *)this);
#endif
			pthread_detach(m_loadingThread);//创建一个线程默认的状态是joinable, 如果一个线程结束运行但没有被join,则它的状态类似于进程中的Zombie Process,即还有一部分资源没有被回收（退出状态码），所以创建线程者应该调用pthread_join来等待线程运行结束，并可得到线程的退出代码，回收其资源（类似于wait,waitpid)
			CC_ASSERT(err == 0);

		}

		int CCPThread::join( void )
		{
			return pthread_join (m_loadingThread, NULL);   
		}

		void CCPThread::cancel( void )
		{
			//Android下不支持这个写法
			//   		int err;
			//   		void * tret;
			//   		CCLog("start cancel thread");
			//   		pthread_cancel(m_loadingThread);
			//   		err=pthread_join(m_loadingThread,&tret);	
			//   		if(err!=0)
			//   		{
			//   			CCLog("can't join thread : %s\n",strerror(err));
			//   		}
			//   		CCLog("thread exit code %d\n",(int)tret);

			/************************************************************************/
			/* 在线程设置为joinable后，可以调用pthread_detach()使之成为detached。但是相反的操作则不可以。还有，如果线程已经调用pthread_join()后，则再调用pthread_detach()则不会有任何效果。
			(5) 前面提到线程可以通过自身执行结束来结束，也可以通过调用pthread_exit()来结束线程的执行。另外，线程甲可以被线程乙被动结束。这个通过调用pthread_cancel()来达到目的。当然，线程也不是被动的被别人结束。它可以通过设置自身的属性来决定如何结束。
			线程的被动结束分为两种，一种是异步终结，另外一种是同步终结。异步终结就是当其他线程调用 pthread_cancel的时候，线程就立刻被结束。而同步终结则不会立刻终结，它会继续运行，直到到达下一个结束点（cancellation point）。当一个线程被按照默认的创建方式创建，那么它的属性是同步终结。
			通过调用pthread_setcanceltype()来设置终结状态。
			int pthread_setcanceltype(int type, int *oldtype);
			state：要设置的状态，可以为PTHREAD_CANCEL_DEFERRED或者为PTHREAD_CANCEL_ASYNCHRONOUS。
			那么前面提到的结束点又如何设置？最常用的创建终结点就是调用pthread_testcancel()的地方。该函数除了检查同步终结时的状态，其他什么也不做。     上面一个函数是用来设置终结状态的。还可以通过下面的函数来设置终结类型，即该线程可不可以被终结：
			int pthread_setcancelstate(int state, int *oldstate);
			state：终结状态，可以为PTHREAD_CANCEL_DISABLE或者PTHREAD_CANCEL_ENABLE。
			(6) 线程的本质。

			在Linux中，新建的线程并不是在原先的进程中，而是系统通过 一个系统调用clone()。该系统copy了一个和原先进程完全一样的进程，并在这个进程中执行线程函数。不过这个copy过程和fork不一样。 copy后的进程和原先的进程共享了所有的变量，运行环境。这样，原先进程中的变量变动在copy后的进程中便能体现出来。                                                                     */
			/************************************************************************/
		}

		void CCPThread::kill( void ) 
		{
			//这个方法只实现了linux平台
			// #if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
			// 		if(pthread_kill(m_loadingThread, 0)!= ESRCH)
			// 		{
			// 			CCLog("thread %d exists!\n", m_loadingThread);
			// 			pthread_kill(m_loadingThread, SIGQUIT);
			// 			CCLog("after kill\n");
			// 		}
			// #endif
		}
		void CCSleep(int sleepMs)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			Sleep(sleepMs);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
			Concurrency::wait(sleepMs);	
#else
			usleep(sleepMs * 1000);   // usleep takes sleep time in us
#endif
		}
	}
}
