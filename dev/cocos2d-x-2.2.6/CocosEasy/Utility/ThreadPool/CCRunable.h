/***************************************************
* 中清龙图
* Copyright (C) 2013 -  All Rights Reserved.
* -----------------------------------------------------
* 当前版本 : 1.0
* 作     者: 刘宗安
* 时     间: 2013-07-01 13:42:46
* 注     释: 线程接口

****************************************************/
#ifndef _CCRUNABLE_H_
#define _CCRUNABLE_H_

#include "cocos2d.h"
USING_NS_CC;

namespace Engine
{
	namespace thread_pool
	{
		class CCRunable
		{
		public:
			virtual void run() = 0;
		};
	}
}
#endif //_CCRUNABLE_H_

