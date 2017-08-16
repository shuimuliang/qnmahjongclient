#ifndef __CCNETFACADE_H_
#define __CCNETFACADE_H_

#include "core/Facade.h"
#include "cocos2d.h"
#include <queue>
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8 )
#include "CCPTHreadWinRT.h"
#else
#include "pthread.h"
#endif
#include "pattern/obsever/Notification.h"

using namespace std;
USING_NS_CC;

namespace Engine
{

	class CCNetFacade : public Facade
	{
	public:
		~CCNetFacade();
		static CCNetFacade * getInstance();
		static void release();
		void sendNotificationToMainThread(Notification &noti);
		void sendNotificationToMainThread(const string& name);
		void sendNotificationToMainThreadFromLoading(const string& name);
		void sendNotificationToMainThreadFromLoading(Notification &noti);
		void update();
		inline void resume(){m_bIsRunUpdate = true;}
		inline void pause(){m_bIsRunUpdate = false;}
	private:
		CCNetFacade();
		static pthread_mutex_t m_mutex_receive_queue;
		typedef queue<Notification> QueueNotification;
		static QueueNotification m_queueNotification;
		static QueueNotification m_queueNotificationFromLoading;
		bool m_bIsRunUpdate;
		//网络线程
		void updateNet();
		//loading线程
		void updateLoading();
	};

}

#endif // __CCNETFACADE_H_

