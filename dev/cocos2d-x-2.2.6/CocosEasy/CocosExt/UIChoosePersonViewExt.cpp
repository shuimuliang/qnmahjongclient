#include "UIChoosePersonViewExt.h"

NS_CC_EASY_BEGIN
const float HALF_SCALE = 0.5f;
const float HALF_NODE_Y = 450.0f;

UIChoosePersonViewExt* UIChoosePersonViewExt::create(float width,float height,const std::string& backImgSrc)
{
	UIChoosePersonViewExt* widget = new UIChoosePersonViewExt();
	if (widget && widget->init(width,height,backImgSrc))
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return NULL;
}

bool UIChoosePersonViewExt::init(float width,float height,const std::string& backImgSrc)
{
	if(UICircleViewBaseExt::init(width,height)){
		CCSprite *pBack = CCSprite::create(backImgSrc.c_str());
		float scale = height/pBack->getContentSize().height;
		pBack->setScale(20.0f);
		pBack->setPosition(ccp(0,height));
		pBack->setOpacity(128);
		addNode(pBack,2,2);
		
		m_pMoveHalf = Widget::create();
		addChild(m_pMoveHalf,1,1);
		m_pMoveHalf->retain();
		m_pMoveHalf->setPositionY(HALF_NODE_Y);
		m_pMoveNode->setZOrder(3);
		return true;
	}
	return false;
}

void UIChoosePersonViewExt::addCell(ImageView *pPortrait ,ImageView *pHalfPortrait)
{
	CC_ASSERT(pPortrait&&pHalfPortrait);
	m_cellDistance = 520;
	m_cellDistanceHalf = m_cellDistance * HALF_SCALE;
	//add portrait
	pPortrait->setAnchorPoint(ccp(0.5,0));
	m_pMoveNode->addChild(pPortrait);
	m_arrCells.push_back(pPortrait);
	pPortrait->retain();
	pPortrait->setZOrder(1000);
	//add halfPortrait
	pHalfPortrait->setAnchorPoint(ccp(0.5,0));
	m_pMoveHalf->addChild(pHalfPortrait);
	m_arrHalfPortrait.push_back(pHalfPortrait);
	pHalfPortrait->retain();
	pHalfPortrait->setZOrder(-1000);
	float tempScale = m_cellDistance/pHalfPortrait->getContentSize().width*0.5;
	pHalfPortrait->setScale(tempScale);
}

void UIChoosePersonViewExt::resetAfterAddCell()
{
	CC_ASSERT(m_arrCells.size()==m_arrHalfPortrait.size());
	//check m_curCellId
	if(m_curCellId>=m_arrCells.size()){
		m_curCellId = 0;
		return;
	}
	unsigned int arrCellLength = m_arrCells.size();


	m_pMoveNode->setPosition(CCPointZero);
	for(unsigned int i = 0;i<arrCellLength;++i)
	{
		Widget *cell = m_arrCells.at(i);
		cell->setPositionX(m_cellDistance*i + m_cellDistance /2);
		Widget *halfCell = m_arrHalfPortrait.at(arrCellLength -i -1);
		halfCell->setPositionX(m_cellDistanceHalf*i + m_cellDistanceHalf/2);
	}
	//left have one Cell
	(*m_arrCells.rbegin())->setPositionX((m_arrCells.at(0))->getPositionX()-m_cellDistance);
	m_sumOfLeft = 1;
	m_sumOfRight = m_arrCells.size()-2;

	setSize(CCSize(m_cellDistance,getContentSize().height));
}

void UIChoosePersonViewExt::doSpecialEffect()
{
	float halfViewSize = m_cellDistance/2;
	for(unsigned int i =0;i<m_arrCells.size();++i)
	{
		Widget *pCell = m_arrCells.at(i);
		float curXForView = pCell->getPositionX()+m_pMoveNode->getPositionX();

		if(curXForView>-m_cellDistance/2&&curXForView<1.5*m_cellDistance)
		{
			float diffForMid = fabs(curXForView-m_cellDistance/2);
			float ratio = 1-diffForMid/m_cellDistance;
			float tempColor = 255*ratio;
			pCell->setColor(ccc3(tempColor , tempColor , tempColor));
			float tempScale = HALF_SCALE+(1-HALF_SCALE)*ratio;
			pCell->setScale(tempScale);
			float tempY = pCell->getContentSize().height*(1-tempScale);
			pCell->setPositionY(tempY);
		}
	}
}

void UIChoosePersonViewExt::doWhenStopClose()
{
	m_isScrollBegan = false;
	onEventScript(eScrollStopT);


}

void UIChoosePersonViewExt::doCircleEffect()
{
	if(!m_isScrollBegan)
	{
		m_isScrollBegan = true;
		onEventScript(eScrollBeginT);
	}
	Widget *pNextCell = getNextCell();
	Widget *pLastCell = getLastCell();
	Widget *pMoveCell = NULL;
	Widget *pCurCell = m_arrCells.at(m_curCellId);
	float curXForView = pCurCell->getPositionX()+m_pMoveNode->getPositionX();
	m_pMoveHalf->setPositionX(-m_pMoveNode->getPositionX()*HALF_SCALE);

	if(curXForView>m_cellDistance)
	{
		//如果CurCell的x屏幕相对坐标>屏幕宽/2，则lastCell变成当前，最右边的cell移到最左边
		pMoveCell= m_arrCells.at(getRightCellIdOfArr());
		pMoveCell->setPositionX(pMoveCell->getPositionX()-m_cellDistance*(m_arrCells.size()));
		//change the curCellId
		m_curCellId = getLastCellId();
	}else if(curXForView<0)
	{
		//如果CurCell的x屏幕相对坐标<-屏幕宽/2，则nextCell变成当前，最左边的cell移动到最右边
		pMoveCell = m_arrCells.at(getLeftCellIdOfArr());
		pMoveCell->setPositionX(pMoveCell->getPositionX()+m_cellDistance*(m_arrCells.size()));
		//change the curCellId
		m_curCellId = getNextCellId();
	}
	if(isFloatEqual(curXForView,m_cellDistanceHalf/2)){
		return;
	}
	if(curXForView>m_cellDistance/2)
	{
		int nextIdd = getNextCellId();
		m_arrHalfPortrait[m_curCellId]->setPositionX(m_arrHalfPortrait.at(nextIdd)->getPositionX()+m_cellDistanceHalf);
	}else if(curXForView<m_cellDistanceHalf){
		int lastId = getLastCellId();
		m_arrHalfPortrait[m_curCellId]->setPositionX(m_arrHalfPortrait.at(lastId)->getPositionX()-m_cellDistanceHalf);
	}
}

UIChoosePersonViewExt::~UIChoosePersonViewExt()
{
	clearArrHalfPortrait();
	CC_SAFE_RELEASE(m_pMoveHalf);
}

void UIChoosePersonViewExt::clearArrHalfPortrait()
{
	for(unsigned int i = 0;i<m_arrHalfPortrait.size();++i)
	{
		CC_SAFE_RELEASE_NULL(m_arrHalfPortrait.at(i));
	}
	m_arrHalfPortrait.clear();
}

UIChoosePersonViewExt::UIChoosePersonViewExt()
:m_pMoveHalf(NULL)
,m_eventHandler(-1)
,m_isScrollBegan(false)
{

}

void UIChoosePersonViewExt::registScriptEvent(int handler)
{
	m_eventHandler = handler;
}

void UIChoosePersonViewExt::onEventScript(E_ScrollCondition condition)
{
	if(m_eventHandler <= 0) return;

	CCArray* pArrayArgs = CCArray::createWithCapacity(1);
	pArrayArgs->addObject(CCInteger::create(condition));
	pArrayArgs->addObject(CCInteger::create(m_curCellId));
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_eventHandler,pArrayArgs);
}

NS_CC_EASY_END