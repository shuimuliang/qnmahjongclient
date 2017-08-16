/***************************************************
* 中清龙图
* Copyright (C) 2013 -  All Rights Reserved.
* -----------------------------------------------------
* 当前版本 : 1.0
* 作     者: 刘宗安
* 时     间: 2013-07-29 14:26:06
* 注     释: 信号量

****************************************************/
#include "CCSem.h"
#include <errno.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace Engine
{

	CCSem::CCSem():s_pSem(NULL)
	{
		sem++;
		std::string semName = "sem%d";
		char buf[20];
		sprintf( buf, semName.c_str(), sem);
		name =  std::string(buf);
		//CCLOG("sem:%s",name.c_str());

	}


	CCSem::~CCSem(void)
	{
		if(s_pSem != NULL)
		{
			CCLOG("please destroy CCSem");
		}
	}

	void CCSem::post( void )
	{
		sem_post(s_pSem);
	}

	void CCSem::wait( void )
	{
		if(s_pSem)
		{
			int semWaitRet = sem_wait(s_pSem);
			if( semWaitRet < 0 )
			{
				CCLOG( "CCPThread async thread semaphore error: %s\n", strerror( errno ) );
				return;
			}
		}

	}

	void CCSem::init( void )
	{
		if (s_pSem == NULL)
		{             
#if CC_SEM_USE_NAMED_SEMAPHORE
			s_pSem = sem_open(name.c_str(), O_CREAT, 0644, 0);
			if( s_pSem == SEM_FAILED )
			{
				CCLOG( "CCPThread async thread semaphore init error: %s\n", strerror( errno ) );
				s_pSem = NULL;
				return;
			}
#else
			int semInitRet = sem_init(&s_sem, 0, 0);
			if( semInitRet < 0 )
			{
				CCLOG( "CCPThread async thread semaphore init error: %s\n", strerror( errno ) );
				return;
			}
			s_pSem = &s_sem;
#endif
		}

	}

	void CCSem::destroy( void )
	{
		if(s_pSem)
		{
#if CC_SEM_USE_NAMED_SEMAPHORE
			sem_unlink(name.c_str());
			sem_close(s_pSem);
#else
			sem_destroy(s_pSem);
#endif
			s_pSem = NULL;
		}
	}

}