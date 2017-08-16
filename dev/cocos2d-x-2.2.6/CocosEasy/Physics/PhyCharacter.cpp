#include "PhyCharacter.h"

NS_CC_EASY_BEGIN

CPhyCharInfo::CPhyCharInfo()
: name(NULL)
, pWorld(NULL)
{

}

bool CPhyCharInfo::isValid()
{
	if(!name || !pWorld) return false;
	return true;
}

CPhyCharacter::CPhyCharacter()
: m_pArmature(NULL)
, m_pBody(NULL)
, m_pWorld(NULL)
{

}

CPhyCharacter* CPhyCharacter::create(CPhyCharInfo* pInfo)
{
	CPhyCharacter *pRet = new CPhyCharacter();
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

bool CPhyCharacter::init( CPhyCharInfo* pInfo )
{
	if(!pInfo || !pInfo->isValid()) return false;
	if(!cocos2d::CCNode::init()) return false;

	// init armature
	char png[128] = {0}; sprintf(png,"%s0.png",pInfo->name);
	char plist[128] = {0}; sprintf(plist,"%s0.plist",pInfo->name);
	char pExportJon[128] = {0}; sprintf(pExportJon,"%s.ExportJson",pInfo->name);
	cocos2d::extension::CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(png, plist, pExportJon);
	m_pArmature = cocos2d::extension::CCArmature::create(pInfo->name);
	m_pArmature->setPosition(pInfo->pos);
	CC_SAFE_RETAIN(m_pArmature);

	// init physical body
	if(!initBody(pInfo->pWorld)) return false;

	return true;
}

void CPhyCharacter::unInit()
{
	CC_SAFE_RELEASE(m_pArmature);
}

bool CPhyCharacter::initBody( CPhysicalWorld* pWorld )
{
	if(!pWorld) return false;
	m_pWorld = pWorld;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(m_pArmature->getPositionX()/PT_RATIO,m_pArmature->getPositionY()/PT_RATIO);
	m_pBody = m_pWorld->createBody(&bodyDef);

	b2PolygonShape shape;
	//shape.SetAsBox(30.0f/(float)PT_RATIO,28.0f/(float)PT_RATIO,b2Vec2(-0.2,-0.2),0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	m_pBody->CreateFixture(&fixtureDef);

	b2Fixture *fixture = m_pBody->GetFixtureList();
	if(fixture != NULL) 
	{
		fixture->SetUserData(static_cast<CPhyCharacter*>(this));
		b2Filter filter = fixture->GetFilterData();
		filter.categoryBits = 1;
		filter.groupIndex = -1;
		fixture->SetFilterData(filter);
	}

	return true;
}






NS_CC_EASY_END