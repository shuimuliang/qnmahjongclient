#pragma once;

#include "Common/CocosEasyDef.h"
#include "Box2D/Box2D.h"
#include "GLES-Render.h"

NS_CC_EASY_BEGIN

#ifndef PI
#define PI 3.14159265358979323846f
#endif // !PI


class CPhysicalWorldInfo
{
public:
	CPhysicalWorldInfo();
	b2Vec2 gravity;
	bool allowSleep;
	uint32 debugTag;
	b2ContactFilter* filter;
	b2ContactListener* listener;
	cocos2d::CCRect bound;
};

class CPhysicalWorld : public cocos2d::CCLayer
{
public:
	CPhysicalWorld();
	~CPhysicalWorld(){unInit();}
	
	/* create a physical world by info*/
	static CPhysicalWorld* create(CPhysicalWorldInfo* pInfo);

	/* display physical shape in this world */
	virtual void draw();

	/* for world step */
	virtual void update(float delta);

	/* create bounding box barrier by texture */
	virtual b2Body* createBarrier(cocos2d::CCTexture2D* pTexture,float friction = 0.2f,
		const cocos2d::CCPoint& pos = cocos2d::CCPoint(0,0),
		float rotation = 0.0f,float scaleX = 0.0f,float scaleY = 0.0f,b2BodyType type = b2_staticBody);

	/* equal to b2world::CreateBody */
	b2Body* createBody(const b2BodyDef* def){return m_pWorld->CreateBody(def);}

protected:
	virtual bool init(CPhysicalWorldInfo* pInfo);
	void unInit();

private:
	void initBound(cocos2d::CCRect& bound);

protected:
	b2World *m_pWorld;
	GLESDebugDraw* m_pDebug;
};


NS_CC_EASY_END