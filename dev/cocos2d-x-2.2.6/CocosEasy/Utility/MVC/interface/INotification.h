#ifndef _I_NOTIFICATION_H_
#define _I_NOTIFICATION_H_

#include "cocos2d.h"
USING_NS_CC;

namespace Engine
{

	class INotification
	{
	public:
		virtual ~INotification()
		{
		}
		/**
		* Get the name of the <code>INotification</code> instance. 
		* No setter, should be set by constructor only
		*/
		virtual const std::string& getName() const  =0;

		/**
		* Set the body of the <code>INotification</code> instance
		*/
		virtual void setBody( void *  ) =0;

		/**
		* Get the body of the <code>INotification</code> instance
		*/
		virtual void * getBody() =0;

		/**
		* Set the type of the <code>INotification</code> instance
		*/
		virtual void setType( const std::string & type ) =0;

		/**
		* Get the type of the <code>INotification</code> instance
		*/
		virtual const std::string& getType() =0;

		/**
		* Get the string representation of the <code>INotification</code> instance
		*/
		virtual std::string toString() =0;

		virtual void autoReleaseObject() = 0;
		virtual CCObject* getCCObjectType()=0;
		virtual void addCCObjectType(CCObject* obj)=0;
		inline void setNum(int num){mIntNum = num;}
		inline int getNum(){return mIntNum;}
		inline void setDouble(const double dNum){mDoubelNum = dNum;}
		inline double getDouble(){return mDoubelNum;}
	protected:
		int mIntNum;
		double mDoubelNum;
	};

}
#endif //_I_NOTIFICATION_H_


