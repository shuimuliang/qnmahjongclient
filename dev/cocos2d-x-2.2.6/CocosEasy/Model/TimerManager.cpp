#include "TimerManager.h"

NS_CC_EASY_BEGIN

CTimerData::CTimerData()
: _priority(0)
, _timerID(0)
, _lock(false)
{
	init();
}

CTimerData::~CTimerData()
{
	std::list<CTimer>::iterator it = _timerList.begin();
	for(it; it!=_timerList.end(); ++it)
	{
		it->clearListener();
	}
	_timerList.clear();

	for(unsigned int i=0; i<_cacheList.size(); ++i)
	{
		st_operate& oper = _cacheList[i];
		oper._timer.clearListener();
	}
	_cacheList.clear();
}

void CTimerData::init()
{
	CCScheduler* pScheduler = CCDirector::sharedDirector()->getScheduler();
	pScheduler->unscheduleUpdateForTarget(this);
	pScheduler->scheduleUpdateForTarget(this, _priority, false);
}


void CTimerData::onDestroy()
{
	CCScheduler* pScheduler = CCDirector::sharedDirector()->getScheduler();
	pScheduler->unscheduleUpdateForTarget(this);

	CScriptObject::onDestroy();
}


unsigned int CTimerData::addTimer(float time, bool repeated /*= false*/, CCObject* selector /*= NULL*/, SEL_OnTime onTimeCallBack /*= NULL*/)
{
	if(!selector || !onTimeCallBack) return 0;
	CTimer::st_listener listener(selector, onTimeCallBack);
	return addTimer(time, repeated, listener);
}

unsigned int CTimerData::addTimer(float time, bool repeated /*= false*/, int handler /*= 0*/)
{
	if(!handler) return 0;
	CTimer::st_listener listener(handler);
	return addTimer(time, repeated, listener);
}

unsigned int CTimerData::addTimer(float time, bool repeated, const CTimer::st_listener& listener)
{
	unsigned int id = applyTimerID();
	CTimer timer(id, time, repeated);
	timer.start();
	timer.setListener(listener);

	if(_lock)
	{
		st_operate item(ADD, timer);
		_cacheList.push_back( item );
	}
	else
	{
		_addTimer(timer);
	}
	return id;
}


void CTimerData::clearTimer()
{
	if(_lock)
	{
		CTimer timer(0);
		st_operate item(CLEAR, timer);
		_cacheList.push_back( item );
	}
	else
	{
		_clearTimer();
	}
}



void CTimerData::delTimer(unsigned int id)
{
	if(_lock)
	{
		CTimer timer(id);
		st_operate item(DEL,timer);
		_cacheList.push_back( item );
	}
	else
	{
		_delTimer(id);
	}
}

void CTimerData::_addTimer(const CTimer& timer)
{
	_timerList.push_back(timer);
}

void CTimerData::_delTimer(unsigned int id)
{
	std::list<CTimer>::iterator it = _timerList.begin();
	for(it; it!=_timerList.end(); ++it)
	{
		const CTimer& curTimer = *it;
		if(curTimer.getID() != id) continue;
		it->clearListener();
		_timerList.erase(it);
		return;
	}
}


void CTimerData::_clearTimer()
{
	std::list<CTimer>::iterator it = _timerList.begin();
	for(it; it!=_timerList.end(); ++it)
	{
		it->clearListener();
	}
	_timerList.clear();
}



void CTimerData::update(float dt)
{
	if(_timerList.size() == 0) return;

	_lock = true;

	std::list<CTimer>::iterator it = _timerList.begin();
	for(it; it!=_timerList.end();)
	{
		// step
		CTimer& curTimer = *it;
		curTimer.step(dt);

		// check done
		if (curTimer.isDone())
		{
			curTimer.notify();
		}

		// check repeated
		if(curTimer.isRepeated())
		{
			curTimer.start();
		}

		// delete current timer
		if (curTimer.isDone())
		{
			curTimer.clearListener();
			it = _timerList.erase(it);
		}
		else
		{
			++it;
		}
	}

	_lock = false;

	// deal cache
	for(unsigned int i=0; i<_cacheList.size(); ++i)
	{
		const st_operate& oper = _cacheList[i];
		switch (oper._operate)
		{
		case ADD:
			{
				_addTimer(oper._timer);
			}
			break;
		case DEL:
			{
				_delTimer(oper._timer.getID());
			}
		case CLEAR:
			{
				_clearTimer();
			}
			break;
		default:
			break;
		}
	}
	_cacheList.clear();
}

void CSystemTimerData::init()
{
	_priority = kCCPrioritySystem;
	CTimerData::init();
}

NS_CC_EASY_END