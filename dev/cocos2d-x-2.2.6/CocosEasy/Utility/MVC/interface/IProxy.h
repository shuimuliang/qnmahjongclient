/***************************************************
* 中清龙图
* Copyright (C) 2013 -  All Rights Reserved.
* -----------------------------------------------------
* 当前版本 : 1.0
* 作     者: 刘宗安
* 时     间: 2013-05-27 18:00:27
* 注     释:

****************************************************/
#ifndef __IPROXY_H__
#define __IPROXY_H__
#include <string>
#include "../interface/ISend.h"

using namespace std;

namespace Engine
{

	class Packet;

	class IProxy:public ISend
	{

	public:
		IProxy(){}
		IProxy(const std::string& name);
		virtual ~IProxy(){}
		inline std::string getName(){return m_proxyName;}
		void factoryOpByAvatarId(string avatarId,string op,string content,Packet *p);
		void factoryOp(string op ,string other,Packet *p);
	protected:
		std::string m_proxyName;
	};

}

#endif // __IPROXY_H__



