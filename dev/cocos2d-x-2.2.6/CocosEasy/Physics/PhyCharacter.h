#pragma once;

#include "Box2D/Box2D.h"
#include "PhysicalWorld.h"

NS_CC_EASY_BEGIN

class CPhyCharInfo
{
public:
	CPhyCharInfo();
	virtual bool isValid();
	const char* name;
	CPhysicalWorld* pWorld;
	cocos2d::CCPoint pos;
	float width,height;
	b2Vec2 center;
};

class CPhyCharacter : public cocos2d::CCNode
{
public:
	CPhyCharacter();
	~CPhyCharacter(){unInit();}
	static CPhyCharacter* create(CPhyCharInfo* pInfo);

protected:
	virtual bool init(CPhyCharInfo* pInfo);
	virtual void unInit();

protected:
	bool initBody(CPhysicalWorld* pWorld);

	// models
protected:
	cocos2d::extension::CCArmature* m_pArmature;

	// quotes
protected:
	CPhysicalWorld* m_pWorld;
	b2Body* m_pBody;
};

NS_CC_EASY_END