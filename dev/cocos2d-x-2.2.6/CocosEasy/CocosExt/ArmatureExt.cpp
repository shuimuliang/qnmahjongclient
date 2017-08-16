#include "ArmatureExt.h"

NS_CC_EASY_BEGIN


CArmatureExt::CArmatureExt()
{
}


CArmatureExt::~CArmatureExt()
{
	//CCLog("[CArmatureExt::~CArmatureExt] armature destucted");
}


CArmatureExt* CArmatureExt::create( const char* json, const char* name /*= NULL*/ )
{
	CResArmature res(json,name);
	return create(res);
}

CArmatureExt* CArmatureExt::create( const CResArmature& res)
{
	CArmatureExt *pRet = new CArmatureExt();
	if (pRet && pRet->init(res))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool CArmatureExt::init( const CResArmature& res)
{
	const char* json = res.getResName();
	const char* name = res.getArmName();
	CResourceLoader::getInstance()->loadArmature(json);
	return CCArmature::init(name);
}

void CArmatureExt::play( const char *animationName,unsigned int framIdx, int durationTo /*= -1*/, int durationTween /*= -1*/, int loop /*= -1*/, int tweenEasing /*= extension::TWEEN_EASING_MAX*/ )
{
	const char* curAnimation = getCurAnimation();
	const char* newAnimation =  animationName;
	if(!findAnimation(animationName)) newAnimation = getDftAnimation();
	if(!newAnimation){assert(false); return;}

	if(curAnimation && strcmp(curAnimation,newAnimation) == 0 && framIdx == -1) {return;}  
	getAnimation()->play(newAnimation,durationTo,durationTween,loop);
	if(framIdx != -1) getAnimation()->gotoAndPlay(framIdx);
}

void CArmatureExt::pause( unsigned int framIdx /*= -1*/ )
{
	if(framIdx == -1) 
		getAnimation()->pause();
	else
		getAnimation()->gotoAndPause(framIdx);
}

void CArmatureExt::resume( unsigned int framIdx /*= -1*/ )
{
	getAnimation()->resume();
	if(framIdx != -1) getAnimation()->gotoAndPlay(framIdx);
}

void CArmatureExt::setBoneImage(const char* boneName, const char* frameName)
{
	assert(frameName);
	CCBone* bone =  this->getBone(boneName);
	bone->removeDisplay(0);
	CCSkin* weapSkin = CCSkin::createWithSpriteFrameName(frameName);
	bone->addDisplay(weapSkin,0);
	bone->changeDisplayWithIndex(0,true);
}


CCTexture2D* CArmatureExt::getBoneTexture( const char* boneName )
{
	CCBone* bone =  this->getBone(boneName);
	if(!bone) return NULL;
	CCDecorativeDisplay* pDecoDisplay = bone->getDisplayManager()->getCurrentDecorativeDisplay();
	if(!pDecoDisplay) return NULL;
	CCSkin* pSkin = dynamic_cast<CCSkin*>(pDecoDisplay->getDisplay());
	if(!pSkin) return NULL;
	return pSkin->getTexture();
}


const CCRect* CArmatureExt::getBoneTextureRect( const char* boneName )
{
	CCBone* bone =  this->getBone(boneName);
	if(!bone) return NULL;
	CCDecorativeDisplay* pDecoDisplay = bone->getDisplayManager()->getCurrentDecorativeDisplay();
	if(!pDecoDisplay) return NULL;
	CCSkin* pSkin = dynamic_cast<CCSkin*>(pDecoDisplay->getDisplay());
	if(!pSkin) return NULL;
	return &pSkin->getTextureRect();
}


void CArmatureExt::setBoneVisible( const char* boneName,bool isVisible )
{
	CCBone* bone =  this->getBone(boneName);
	int idx = isVisible ? 0 : -1;
	bone->changeDisplayWithIndex(idx,true);
}



bool CArmatureExt::findAnimation( const char* name )
{
	if(!name) return false;
	CCMovementData* pData = getAnimation()->getAnimationData()->getMovement(name);
	return pData != NULL;
}


bool CArmatureExt::setDftAnimation( const char* name )
{
	if(!name) return NULL;
	if(!findAnimation(name)) return false;
	m_DftAnimation = name;
	return true;
}

const char* CArmatureExt::getCurAnimation()
{
	m_CurAnimation = getAnimation()->getCurrentMovementID();
	return m_CurAnimation.length() == 0 ? NULL : m_CurAnimation.c_str();
}

float CArmatureExt::getAnimationTime(const char* name)
{
	const char* animName = name;
	if (!animName) animName = getCurAnimation();
	CCMovementData* pData = getAnimation()->getAnimationData()->getMovement(animName);
	if (!pData) astReturn(0);
	unsigned int frames = pData->duration;
	float scale = pData->scale;
	float interval = CCDirector::sharedDirector()->getAnimationInterval();
	return frames * (interval * (1.0f / scale));
}


const CArmatureExt::st_AnimEvtItem* CArmatureExt::getAnimationEventOnceData(const char* name)
{
	std::map<std::string,st_AnimEvtItem> ::iterator it = m_AnimEvtOnceList.find(name);
	if(it == m_AnimEvtOnceList.end()) return NULL;
	return &it->second;
}


bool CArmatureExt::setAnimationEventOnce( const char* name,un_AnimationEventTag tag,CCObject *target,SEL_AnimationEventCallFunc callFunc )
{
	if( !name || !target || !callFunc || 
		!findAnimation(name) ||
		tag.tag == 0) 
		{assert(false); return false;}

	std::map<std::string,st_AnimEvtItem> ::iterator it = m_AnimEvtOnceList.find(name);
	if(it != m_AnimEvtOnceList.end()) {assert(false); return false;}
	st_AnimEvtItem item = {target,callFunc,0,tag};
	m_AnimEvtOnceList[name] = item;
	getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(CArmatureExt::movementCallBack));
	return true;
}


bool CArmatureExt::setCurAnimationComplete( CCObject *target,SEL_AnimationEventCallFunc callFunc )
{
	const char* name = getCurAnimation();
	un_AnimationEventTag tag; tag.complete = true; tag.loopComplete = true;
	return setAnimationEventOnce(name,tag,target,callFunc);
}


bool CArmatureExt::setScriptAnimationEventOnce( const char* name,un_AnimationEventTag tag, int handler )
{
	if( !name || !findAnimation(name) || handler == 0 || tag.tag == 0) {assert(false); return false;}
	std::map<std::string,st_AnimEvtItem> ::iterator it = m_AnimEvtOnceList.find(name);
	if(it != m_AnimEvtOnceList.end()) {assert(false); return false;}
	st_AnimEvtItem item = {NULL,NULL,handler,tag};
	m_AnimEvtOnceList[name] = item;
	getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(CArmatureExt::movementCallBack));
	return true;
}

bool CArmatureExt::setScriptCurAnimationComplete( int handler )
{
	const char* name = getCurAnimation();
	un_AnimationEventTag tag; tag.complete = true; tag.loopComplete = true;
	return setScriptAnimationEventOnce(name,tag,handler);
}


void CArmatureExt::movementCallBack( CCArmature* self, MovementEventType type, const char * name )
{
	do{// once list
		std::map<std::string,st_AnimEvtItem> ::iterator it = m_AnimEvtOnceList.find(name);
		if(it == m_AnimEvtOnceList.end()) break;
		st_AnimEvtItem item = it->second;
		unsigned int tag = 1; tag = tag << (unsigned int)type;
		if((tag & item.tagEvent.tag) == 0) break;
		m_AnimEvtOnceList.erase(it);
		if(item.pTarget && item.pCallBack) (item.pTarget->*item.pCallBack)(this,type,name);
		if(item.scriptHandler)
		{
			CCArray* pArrayArgs = CCArray::createWithCapacity(2);
			pArrayArgs->addObject(CCInteger::create(type));
			pArrayArgs->addObject(CCString::create(name));
			CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(item.scriptHandler,pArrayArgs);
		}
	}while (0);

	if(m_AnimEvtOnceList.size() != 0) return;
	getAnimation()->setMovementEventCallFunc(NULL,NULL);
}



bool CArmatureExt::isLoop( const char* name )
{
	if(!name) return false;
	CCMovementData* pData = getAnimation()->getAnimationData()->getMovement(name);
	if(!pData) return false;
	return pData->loop;
}



NS_CC_EASY_END



