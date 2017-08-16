#include "UICircleViewExt.h"

NS_CC_EASY_BEGIN
const int TARGET_OFFY = 0;
const float TARGET_CELL_SCALE = 0.5f;


UICircleViewExt* UICircleViewExt::create(float width,float height)
{
	UICircleViewExt* widget = new UICircleViewExt();
	if (widget && widget->init(width,height))
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return NULL;
}

UICircleViewExt::UICircleViewExt()
	:m_targetCellScale(TARGET_CELL_SCALE)
	,m_targetOffY(TARGET_OFFY)
{
}

UICircleViewExt::~UICircleViewExt()
{
	
}

void UICircleViewExt::addCell(Widget* cell)
{
	CCAssert(cell,"cell can't be NULL");
	if(!cell)return;
	m_arrCells.push_back(cell);
	cell->retain();
	m_pMoveNode->addChild(cell);

	cell->setAnchorPoint(ccp(0.5,0));
	//add or not??
	resetCellPosition();
}

void UICircleViewExt::resetCellPosition()
{
	//check sumOfcells
	if(m_arrCells.size()<SUM_OF_SHOW_CELLS+1)return;

	//check m_curCellId
	if(m_curCellId>=m_arrCells.size()){
		m_curCellId = 0;
		return;
	}
	m_pMoveNode->setPosition(CCPointZero);
	int midId = m_arrCells.size()/2;
	m_cellDistance = _size.width/2;
	m_sumOfLeft	= midId;
	m_sumOfRight	= m_arrCells.size()-midId-1;//not include currCellId
	for(unsigned int i = 0;i<m_sumOfRight+1;++i)
	{
		Widget *cell = m_arrCells.at(i);
		cell->setPositionX(m_cellDistance*(i+1));
		cell->setScale(m_targetCellScale);
	}

	for(unsigned int i = m_arrCells.size()-1,m=0;m<m_sumOfLeft;--i)
	{
		Widget *cell = m_arrCells.at(i);
		cell->setPositionX(int(-m_cellDistance*m));
		cell->setScale(m_targetCellScale);
		++m;
	}
	
	//pCurCell reset
	Widget *pCurCell	= m_arrCells.at(m_curCellId);
	pCurCell->setScale(1.0f*0.9f);
	pCurCell->setPositionY(m_targetOffY);
	pCurCell->setZOrder(10);
	pCurCell->setTouchEnabled(true);

	Widget *pNextCell = getNextCell();
	pNextCell->setTouchEnabled(false);
	pNextCell->setZOrder(5);

	Widget *pLastCell = getLastCell();
	pLastCell->setTouchEnabled(false);
	pLastCell->setZOrder(5);
}

void UICircleViewExt::doSpecialEffect()
{
	float viewX = -m_pMoveNode->getPositionX();
	float halfViewSize = _size.width/2;
	for(unsigned int i =0;i<m_arrCells.size();++i)
	{
		Widget *pCell = m_arrCells.at(i);
		float cellX = pCell->getPositionX();
		pCell->setVisible(false);
		if(cellX>viewX&&cellX<viewX+_size.width)
		{
			float diffForMid = fabs(cellX-viewX-halfViewSize);
			float ratio = 1-diffForMid/halfViewSize;
			float tempScale = m_targetCellScale+(1-m_targetCellScale)*ratio;
			float tempOffY = m_targetOffY*ratio;
			pCell->setScale(tempScale*0.9f);
			pCell->setPositionY(tempOffY);
		}
	}
}




void UICircleViewExt::setTargetCellScale(float scale)
{
	m_targetCellScale = scale;
}

void UICircleViewExt::setTargetOffY(float offY)
{
	m_targetOffY = offY;
}

float UICircleViewExt::getTargetCellScale()
{
	return m_targetCellScale;
}

float UICircleViewExt::getTargetOffY()
{
	return m_targetOffY;
}

void UICircleViewExt::doCircleEffect()
{
	Widget *pNextCell = getNextCell();
	Widget *pLastCell = getLastCell();
	Widget *pMoveCell = NULL;
	float moveX = 0;
	if(pNextCell->getPositionX()-pNextCell->getContentSize().width/2+m_pMoveNode->getPositionX()>_size.width){
		//when the right cell outSideView, change the rightest cell to the leftest 
		pMoveCell= m_arrCells.at(getRightCellIdOfArr());
		pMoveCell->setPositionX(pMoveCell->getPositionX()-m_cellDistance*(m_arrCells.size()));
		//change the curCellId
		m_curCellId = getLastCellId();
	}else if(pLastCell->getPositionX()+pLastCell->getContentSize().width/2+m_pMoveNode->getPositionX()<0){
		//when the left cell outside view ,change the leftest cell to the rightest
		pMoveCell = m_arrCells.at(getLeftCellIdOfArr());
		pMoveCell->setPositionX(pMoveCell->getPositionX()+m_cellDistance*(m_arrCells.size()));
		//change the curCellId
		m_curCellId = getNextCellId();
	}

	Widget *pNewNextCell = getNextCell();
	Widget *pNewLastCell = getLastCell();
	Widget *pNewCurCell = m_arrCells.at(m_curCellId);
	pNewCurCell->setZOrder(10);
	pNewCurCell->setTouchEnabled(true);
	pNewCurCell->setVisible(true);
	pNewNextCell->setZOrder(5);
	pNewNextCell->setTouchEnabled(false);
	pNewNextCell->setVisible(true);
	pNewLastCell->setZOrder(5);
	pNewLastCell->setTouchEnabled(false);
	pNewLastCell->setVisible(true);
}



NS_CC_EASY_END