#include "CCMutex.h"

namespace Engine
{

	namespace thread_pool
	{
		CCMutex::CCMutex(void) { pthread_mutex_init(&this_lock, NULL); }
		CCMutex::~CCMutex(void) { pthread_mutex_destroy(&this_lock); }
		void CCMutex::lock(void) { pthread_mutex_lock(&this_lock); }
		void CCMutex::unlock(void) { pthread_mutex_unlock(&this_lock); }
	}

}