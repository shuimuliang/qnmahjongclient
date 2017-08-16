#include "PhysicalWorld.h"

NS_CC_EASY_BEGIN

CPhysicalWorldInfo::CPhysicalWorldInfo()
: gravity(0.0f,-9.8f)
, allowSleep(true)
, debugTag(0)
, filter(NULL)
, listener(NULL)
{
	cocos2d::CCSize size = cocos2d::CCDirector::sharedDirector()->getWinSize();
	bound.setRect(0,0,size.width/PT_RATIO,size.height/PT_RATIO);
}

CPhysicalWorld::CPhysicalWorld()
: m_pWorld(NULL)
, m_pDebug(NULL)
{
}

CPhysicalWorld* CPhysicalWorld::create(CPhysicalWorldInfo* pInfo)
{
	CPhysicalWorld *pRet = new CPhysicalWorld();
	if (pRet && pRet->init(pInfo))
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

bool CPhysicalWorld::init(CPhysicalWorldInfo* pInfo)
{
	//create world without gravity
	m_pWorld = new b2World(pInfo->gravity);
	m_pWorld->SetAllowSleeping(pInfo->allowSleep);

	// owner contact listener
	if(pInfo->filter) m_pWorld->SetContactFilter(pInfo->filter);
	if(pInfo->listener) m_pWorld->SetContactListener(pInfo->listener);
	
	//show collision boxes
	if(pInfo->debugTag)
	{
		m_pDebug = new GLESDebugDraw( PT_RATIO );
		m_pDebug->SetFlags(pInfo->debugTag);	//flags += b2Draw::e_shapeBit;
		m_pWorld->SetDebugDraw(m_pDebug);
	}

	// create bounding box of world
	initBound(pInfo->bound);
	
	// open world step
	scheduleUpdate();

	return cocos2d::CCLayer::init();
}

void CPhysicalWorld::initBound( cocos2d::CCRect& bound )
{
	b2BodyDef bodyDef;
	b2Body* body = m_pWorld->CreateBody(&bodyDef);

	b2EdgeShape edge;
	b2Vec2 v1(bound.getMinX()/PT_RATIO,bound.getMinY()/PT_RATIO);
	b2Vec2 v2(bound.getMinX()/PT_RATIO,bound.getMaxY()/PT_RATIO);
	b2Vec2 v3(bound.getMaxX()/PT_RATIO,bound.getMaxY()/PT_RATIO);
	b2Vec2 v4(bound.getMaxX()/PT_RATIO,bound.getMinY()/PT_RATIO);
	
	edge.Set(v1,v2); edge.m_hasVertex0 = true; edge.m_hasVertex3 = true; edge.m_vertex0 = v4; edge.m_vertex3 = v3;
	body->CreateFixture(&edge,0.0f);

	edge.Set(v2,v3); edge.m_hasVertex0 = true; edge.m_hasVertex3 = true; edge.m_vertex0 = v1; edge.m_vertex3 = v4;
	body->CreateFixture(&edge,0.0f);

	edge.Set(v3,v4); edge.m_hasVertex0 = true; edge.m_hasVertex3 = true; edge.m_vertex0 = v2; edge.m_vertex3 = v1;
	body->CreateFixture(&edge,0.0f);

	edge.Set(v4,v1); edge.m_hasVertex0 = true; edge.m_hasVertex3 = true; edge.m_vertex0 = v3; edge.m_vertex3 = v2;
	body->CreateFixture(&edge,0.0f);
}

void CPhysicalWorld::unInit()
{
	CC_SAFE_DELETE(m_pWorld);
	CC_SAFE_DELETE(m_pDebug);
	unscheduleUpdate();
}

void CPhysicalWorld::update( float delta )
{
	m_pWorld->Step(delta,8,1);
	cocos2d::CCLayer::update(delta);
}

void CPhysicalWorld::draw()
{
	if(m_pDebug)
	{
		cocos2d::ccGLEnableVertexAttribs( cocos2d::kCCVertexAttribFlag_Position );
		kmGLPushMatrix();
		m_pWorld->DrawDebugData();
		kmGLPopMatrix();
	}
	
	cocos2d::CCLayer::draw();
}

b2Body* CPhysicalWorld::createBarrier( cocos2d::CCTexture2D* pTexture,float friction,const cocos2d::CCPoint& pos,float rotation /*= 0.0f*/,float scaleX /*= 0.0f*/,float scaleY /*= 0.0f*/,b2BodyType type /*= b2_staticBody*/ )
{
	if(!pTexture || !m_pWorld) return NULL;
	float centerX = (float)pTexture->getPixelsWide() * scaleX / 2.0f;
	float centerY = (float)pTexture->getPixelsHigh() * scaleY / 2.0f;
	float angle = rotation == 0.0f ? 0.0f :(-rotation) / 180.0f * PI;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(pos.x/PT_RATIO, pos.y/PT_RATIO);
	bodyDef.angle = angle;
	b2Body* body = m_pWorld->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(centerX/PT_RATIO,centerY/PT_RATIO);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.friction = friction;
	body->CreateFixture(&fixtureDef);

	return body;
}








NS_CC_EASY_END