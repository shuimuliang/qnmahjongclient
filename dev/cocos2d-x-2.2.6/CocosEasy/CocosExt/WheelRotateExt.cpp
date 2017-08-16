#include "WheelRotateExt.h"

NS_CC_EASY_BEGIN
static const int ARR_INTEVAL[] = {1,1,1,1,1,2,3,5,8,13,21};
static const int ARR_MAX = 11;
float getMode(float v,float fac){
	float result = v;
	while(result>fac)
	{
		result -= fac;
	}
	return result;
}
CEWheelRotateExt * CEWheelRotateExt::create(ImageView *pSpr , float degreeSpeed )
{
	CEWheelRotateExt *pResult = new CEWheelRotateExt(pSpr , degreeSpeed);
	pResult->autorelease();
	return pResult;
}

CEWheelRotateExt::~CEWheelRotateExt()
{
	CC_SAFE_RELEASE(m_pMainSpr);
}

void CEWheelRotateExt::startWithSpeed(int fIntevalSpeed)
{
	m_degreeSpeed = 10.0f;
	m_accSpeed = fIntevalSpeed;
	m_isToTarget = false;
	m_curClock = 0;
	m_cunAngel = m_pMainSpr->getRotation();
	unscheduleUpdate();
	scheduleUpdate();
}

void CEWheelRotateExt::startToAngel(int fIntevalSpeed , int fAngelAcc , int targetAngel)
{
	m_isToTarget = true;
	m_curClock = 0;
	m_targetAngel = targetAngel;
	m_degreeSpeed = 10.0f;
	m_accSpeed =(m_degreeSpeed*m_degreeSpeed)/(8*120);
	m_fAngelAcc = fAngelAcc;
	m_cunAngel	= m_pMainSpr->getRotation();
	m_arrIndex = -1;
	unscheduleUpdate();
	scheduleUpdate();
}

void CEWheelRotateExt::stop()
{
	unscheduleUpdate();

}

void CEWheelRotateExt::update(float delta)
{
	if(m_isToTarget)
	{
		updateToTarget(delta);
	}else{
		updateSpeed(delta);
	}
	if(fabs(getMode(m_cunAngel,60))<= m_degreeSpeed){
		doMusicScriptEvent();
	}
}

void CEWheelRotateExt::onEnter()
{
	CCNode::onEnter();
}

void CEWheelRotateExt::onExit()
{
	CCNode::onExit();
	unscheduleUpdate();
}

CEWheelRotateExt::CEWheelRotateExt(ImageView *pSpr , float degreeSpeed)
	:m_accSpeed(0),
	m_fAngelAcc(0),
	m_targetAngel(0),
	m_isToTarget(false),
	m_pMainSpr(NULL),
	m_stopScriptHandler(-1),
	m_musicScripHandler(-1),
	m_arrIndex(-1)
{
	CC_ASSERT(pSpr);
	m_pMainSpr = pSpr;
	m_pMainSpr->retain();
	m_cunAngel = m_pMainSpr->getRotation();
	m_degreeSpeed = degreeSpeed;
}

void CEWheelRotateExt::updateSpeed(float delta)
{
	m_cunAngel += m_degreeSpeed;
	m_pMainSpr->setRotation(m_cunAngel);
	
}

void CEWheelRotateExt::updateToTarget(float delta)
{
	if(m_arrIndex>=0)
	{
		if(m_degreeSpeed<m_accSpeed+5.0f)
		{
			stop();
			doStopScriptEvent();
			return;
		}
		m_degreeSpeed -= m_accSpeed;
	}else if(abs(int(m_cunAngel - m_targetAngel))%360<m_degreeSpeed)
	{
			m_arrIndex = 0;	
			
	}
	m_cunAngel += m_degreeSpeed;
	m_cunAngel = m_cunAngel>720?m_cunAngel-360:m_cunAngel;
	m_pMainSpr->setRotation(m_cunAngel);
}

void CEWheelRotateExt::registerScriptHandler(int stopHandler,int musicHandler)
{
	m_stopScriptHandler = stopHandler;
	m_musicScripHandler = musicHandler;
}

void CEWheelRotateExt::doStopScriptEvent()
{
	if(m_stopScriptHandler <= 0 )return;
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_stopScriptHandler,CCArray::create());
}

void CEWheelRotateExt::doMusicScriptEvent()
{
	if(m_musicScripHandler <= 0 )return;
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_musicScripHandler,CCArray::create());
}

NS_CC_EASY_END