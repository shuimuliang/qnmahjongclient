#ifndef __CE_ARMATURE_EXT_H__
#define __CE_ARMATURE_EXT_H__

#include "Common/CocosEasyDef.h"
#include "../Model/ResourceLoader.h"
#include "CocoStudio/Armature/utils/CCTweenFunction.h"


NS_CC_EASY_BEGIN

class CArmatureExt;
union un_AnimationEventTag{unsigned int tag ; struct{bool start:1;bool complete:1;bool loopComplete:1;};};
typedef void (CCObject::*SEL_AnimationEventCallFunc)(CArmatureExt*, MovementEventType, const char *);
#define AnimationEvent_selector(_SELECTOR) (SEL_AnimationEventCallFunc)(&_SELECTOR)

class CArmatureExt : public extension::CCArmature
{
public:
	struct st_AnimEvtItem
	{
		void clear(){memset(this,0,sizeof(st_AnimEvtItem));}
		CCObject* pTarget;
		SEL_AnimationEventCallFunc pCallBack;
		int scriptHandler;
		un_AnimationEventTag tagEvent;
	};
public:
	CArmatureExt();
	~CArmatureExt();
	static CArmatureExt* create(const char* json, const char* name = NULL);
	static CArmatureExt* create(const CResArmature& res);

public:
	void play(const char *animationName,unsigned int framIdx = -1, int durationTo = -1, int durationTween = -1,  int loop = -1, int tweenEasing = extension::TWEEN_EASING_MAX);
	void pause(unsigned int framIdx = -1);
    void resume(unsigned int framIdx = -1);
    void stop(){getAnimation()->stop();}

public:
	// common
	void setBlankImage(const char* name){if(!name) return; m_BlankImage = name;}

	// movement data
	bool isLoop(const char* name);

	// animations
	bool findAnimation(const char* name);
	bool setDftAnimation(const char* name);
	const char* getDftAnimation(){return m_DftAnimation.length() == 0 ? NULL : m_DftAnimation.c_str();}
	const char* getCurAnimation();
	float getAnimationTime(const char* name);

	bool setCurAnimationComplete(CCObject *target,SEL_AnimationEventCallFunc callFunc);
	bool setAnimationEventOnce(const char* name,un_AnimationEventTag tag,CCObject *target,SEL_AnimationEventCallFunc callFunc);
	bool setScriptCurAnimationComplete(int handler);
	bool setScriptAnimationEventOnce(const char* name,un_AnimationEventTag tag,int handler);
	const st_AnimEvtItem* getAnimationEventOnceData(const char* name);

	// bones
	void setBoneImage(const char* boneName, const char* frameName);
	CCTexture2D* getBoneTexture(const char* boneName);
	const CCRect* getBoneTextureRect(const char* boneName);
	void setBoneVisible(const char* boneName,bool isVisible);

protected:
	virtual bool init(const CResArmature& res);
	virtual void movementCallBack(CCArmature* self, MovementEventType type, const char * name);

	// property
protected:
	std::string m_DftAnimation;
	std::string m_BlankImage;
	std::string m_CurAnimation;
	std::map<std::string,st_AnimEvtItem> m_AnimEvtOnceList;
};

NS_CC_EASY_END


#endif