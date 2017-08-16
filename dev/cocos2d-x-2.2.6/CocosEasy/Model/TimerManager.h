#ifndef __CE_TIMER_MANAGER_H__
#define __CE_TIMER_MANAGER_H__

#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN
typedef void (CCObject::*SEL_OnTime)(unsigned int timerID);
#define on_time_selector(_SELECTOR) (SEL_OnTime)(&_SELECTOR)


class CTimerData : public CScriptObject
{
public:
	class CTimer
	{
	public:
		struct st_listener
		{
			st_listener() : _selector(NULL), _onTimeCallBack(NULL), _onTimeScriptHandler(0) {}
			st_listener(CCObject* selector, SEL_OnTime onTimeCallBack) : _selector(selector), _onTimeCallBack(onTimeCallBack), _onTimeScriptHandler(0) {}
			st_listener(int onTimeScriptHandler) : _selector(NULL), _onTimeCallBack(NULL), _onTimeScriptHandler(onTimeScriptHandler) {}
			CCObject* _selector;
			SEL_OnTime _onTimeCallBack;
			int _onTimeScriptHandler;
		};

		CTimer(unsigned int id, float time = 0, bool repeated = false) 
		{
			m_ID = id;
			m_duration = time;
			m_repeated = repeated;
			m_elapsed = 0.0f;
			m_bFirstTick = false;
		}

		unsigned int getID() const {return m_ID;}

		void setListener(const st_listener& listener)
		{
			clearListener();
			m_listener = listener;
			CC_SAFE_RETAIN(m_listener._selector);
		}

		void clearListener()
		{
			CC_SAFE_RELEASE_NULL(m_listener._selector);
		}

		void notify()
		{
			if(m_listener._selector && m_listener._onTimeCallBack)
			{
				(m_listener._selector->*m_listener._onTimeCallBack)(m_ID);
			}
			if(m_listener._onTimeScriptHandler)
			{
				CCArray* pArrayArgs = CCArray::createWithCapacity(1);
				pArrayArgs->addObject(CCInteger::create(m_ID));
				CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_listener._onTimeScriptHandler,pArrayArgs);
			}
		}

		bool isRepeated() const{return m_repeated;}

		const st_listener& getListener() const {return m_listener;}

		void start()
		{
			m_elapsed = 0.0f;
			m_bFirstTick = true;
		}
		bool isDone(void)
		{
			return m_elapsed >= m_duration;
		}
		void step(float dt)
		{
			if (m_bFirstTick)
			{
				m_bFirstTick = false;
				m_elapsed = 0;
			}
			else
			{
				m_elapsed += dt;
			}
		}

	protected:
		unsigned int m_ID;
		float m_duration;
		bool m_repeated;
		st_listener m_listener;

		float m_elapsed;
		bool  m_bFirstTick;
	};

	enum eOperate{ADD = 0, DEL, CLEAR};
	struct st_operate
	{
		st_operate(eOperate oper, const CTimer& timer)
			: _operate(oper)
			, _timer(timer)
		{
		}
		CTimer _timer;
		eOperate _operate;
	};

public:
	CTimerData();
	~CTimerData();
	virtual void onDestroy();

	unsigned int addTimer(float time, bool repeated, CCObject* selector, SEL_OnTime onTimeCallBack);
	unsigned int addTimer(float time, bool repeated, int handler);
	unsigned int addTimer(float time, bool repeated, const CTimer::st_listener& listener);
	void delTimer(unsigned int id);
	void clearTimer();

public:
	CCObject* autorelease(void){CCAssert(false, "can not support auto release");}
	virtual void update(float dt);

protected:
	virtual void init();
	unsigned int applyTimerID(){return ++_timerID;}
	void _addTimer(const CTimer& timer);
	void _delTimer(unsigned int id);
	void _clearTimer();
	

protected:
	int _priority;
	unsigned int _timerID;
	std::list<CTimer> _timerList;
	std::vector<st_operate> _cacheList;
	bool _lock;
};

class CSystemTimerData : public CTimerData
{
protected:
	virtual void init();
};

NS_CC_EASY_END

#endif
