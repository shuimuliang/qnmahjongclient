#include "UICircleViewBaseExt.h"

NS_CC_EASY_BEGIN
//Scroll judge factor
const int MIN_TOUCH_X_OFF = 1;
const int SCROLL_SPEED = 600;
const float CHILD_FOCUS_CANCEL_OFFSET = 5.0f;
const int JUADGE_FACTOR = 4;
const int DO_STOP_CLOSE_SPEED = 20;

UICircleViewBaseExt::UICircleViewBaseExt()
	:m_srollType(eScrollNoneT)
	,m_curCellOffXByCenter(0)
	,m_doStopCloseTargetX(0.0f)
	,m_pMoveNode(0)
	,m_curCellId(0)
	,m_sumOfLeft(0)
	,m_sumOfRight(0)
	,m_bePressed(false)
	,m_IsDoStopClose(false)
{
}

UICircleViewBaseExt::~UICircleViewBaseExt()
{
	clearArrCells();
	CC_SAFE_RELEASE(m_pMoveNode);
}
bool UICircleViewBaseExt::onTouchBegan(CCTouch *touch, CCEvent *unusedEvent)
{
	bool pass = Layout::onTouchBegan(touch, unusedEvent);
	if (_hitted)
	{
		handlePressLogic(_touchStartPos);
	}
	return pass;
}

void UICircleViewBaseExt::onTouchMoved(CCTouch *touch, CCEvent *unusedEvent)
{
	Layout::onTouchMoved(touch, unusedEvent);
	handleMoveLogic(_touchMovePos);
}

void UICircleViewBaseExt::onTouchEnded(CCTouch *touch, CCEvent *unusedEvent)
{
	Layout::onTouchEnded(touch, unusedEvent);
	handleReleaseLogic(_touchEndPos);
}

void UICircleViewBaseExt::onTouchCancelled(CCTouch *touch, CCEvent *unusedEvent)
{
	Layout::onTouchCancelled(touch, unusedEvent);
	handleReleaseLogic(touch->getLocation());
}


void UICircleViewBaseExt::checkChildInfo(int handleState,Widget* sender,const CCPoint &touchPoint)
{
	//0,1,2,3 touchPressType,e.g:begin,move,cancel
	switch (handleState)
	{
	case 0:
		handlePressLogic(touchPoint);
		break;
	case 1:
		{
			float offset = (sender->getTouchStartPos() - touchPoint).getLength();
			if (offset > CHILD_FOCUS_CANCEL_OFFSET)
			{
				sender->setFocused(false);
				handleMoveLogic(touchPoint);
			}
		}
		break;
	case 2:
		handleReleaseLogic(touchPoint);
		break;
	case 3:
		handleReleaseLogic(touchPoint);
		break;
	}
}
void UICircleViewBaseExt::handlePressLogic(const CCPoint &touchPoint)
{
	m_touchBeganPoint = convertToNodeSpace(touchPoint);
	m_touchMovingPoint = m_touchBeganPoint;    
	m_bePressed = true;
	schedule(schedule_selector(UICircleViewBaseExt::scroll));
}

void UICircleViewBaseExt::handleMoveLogic(const CCPoint &touchPoint)
{
	m_touchMovedPoint = convertToNodeSpace(touchPoint);
	CCPoint delta = m_touchMovedPoint - m_touchMovingPoint;
	m_srollType = calScrollType();
	//record curMovingPoint
	m_touchMovingPoint = m_touchMovedPoint;
}

void UICircleViewBaseExt::handleReleaseLogic(const CCPoint &touchPoint)
{
	m_touchEndedPoint = convertToNodeSpace(touchPoint);
	m_bePressed = false;
	unschedule(schedule_selector(UICircleViewBaseExt::scroll));
	//startToClose
	calDoStopCloseTargetX();
	m_IsDoStopClose = true;
	schedule(schedule_selector(UICircleViewBaseExt::doStopClose));
	m_srollType = eScrollNoneT;
}



void UICircleViewBaseExt::setAnchorPoint(const CCPoint& anchorPoint)
{
	//DO Nothing
}
void UICircleViewBaseExt::clearArrCells()
{
	for(unsigned int i = 0;i<m_arrCells.size();++i)
	{
		CC_SAFE_RELEASE_NULL(m_arrCells.at(i));
	}
	m_arrCells.clear();
}

void UICircleViewBaseExt::setCurCellOffX(float offX)
{	
	m_curCellOffXByCenter = offX;
	optimizeCurCellOffXByCenter();
	m_pMoveNode->setPositionX(offX);
	doSpecialEffect();
	doCircleEffect();
}

void UICircleViewBaseExt::optimizeCurCellOffXByCenter()
{
	if(m_curCellOffXByCenter>m_cellDistance)
	{
		m_curCellOffXByCenter-=m_cellDistance;
	}else if(m_curCellOffXByCenter<=-m_cellDistance)
	{
		m_curCellOffXByCenter+=m_cellDistance;
	}
}



Widget * UICircleViewBaseExt::getLastCell()
{
	int idCurlast = getLastCellId();
	return m_arrCells.at(idCurlast);
}

Widget * UICircleViewBaseExt::getNextCell()
{
	int idCurPlus  = getNextCellId();
	return m_arrCells.at(idCurPlus);
}


int UICircleViewBaseExt::getNextCellId()
{
	return m_curCellId==m_arrCells.size()-1?0:m_curCellId+1;
}

int UICircleViewBaseExt::getLastCellId()
{
	return m_curCellId==0?m_arrCells.size()-1:m_curCellId-1;
}

int UICircleViewBaseExt::getLeftCellIdOfArr()
{
	int result = m_curCellId-m_sumOfLeft;
	while(result<0){
		result+=m_arrCells.size();
	}
	CC_ASSERT(((unsigned)result)<m_arrCells.size());
	return result;
}

int UICircleViewBaseExt::getRightCellIdOfArr()
{
	unsigned int result = m_curCellId+m_sumOfRight;
	while(result>=m_arrCells.size()){
		result-=m_arrCells.size();
	}
	CC_ASSERT(result<m_arrCells.size());
	return result;
}
UICircleViewBaseExt::E_Scroll_Type UICircleViewBaseExt::calScrollType()
{
	E_Scroll_Type result = eScrollNoneT;
	if(m_touchMovedPoint.x-m_touchMovingPoint.x>MIN_TOUCH_X_OFF){
		result = eScrollRightT;
	}else if(m_touchMovedPoint.x-m_touchMovingPoint.x<-MIN_TOUCH_X_OFF)
	{
		result = eScrollLeftT;
	}
	return result;
}

void UICircleViewBaseExt::scroll(float dt)
{
	switch(m_srollType)
	{
	case eScrollLeftT:
		setCurCellOffX(m_pMoveNode->getPositionX()-SCROLL_SPEED*dt);
		break;
	case eScrollRightT:
		setCurCellOffX(m_pMoveNode->getPositionX()+SCROLL_SPEED*dt);
		break;
	}
}


void UICircleViewBaseExt::setAllCellUnvisible()
{
	for(unsigned int i = 0;i<m_arrCells.size();++i)
	{
		m_arrCells.at(i)->setVisible(false);
	}
}
//move to the currentStopCloseCellId
void UICircleViewBaseExt::doStopClose(float dt)
{
	//m_doStopCloseTargetPoint 
	float targetX = m_doStopCloseTargetX;
	//if keep in close ?
	if(fabs(m_pMoveNode->getPositionX()-targetX)<DO_STOP_CLOSE_SPEED){
		setCurCellOffX(targetX);
		unschedule(schedule_selector(UICircleViewBaseExt::doStopClose));
		doWhenStopClose();
		m_IsDoStopClose = false;
		return;
	}
	//move to close
	if(m_pMoveNode->getPositionX()<targetX){
		setCurCellOffX(m_pMoveNode->getPositionX()+DO_STOP_CLOSE_SPEED);	
	}else{
		setCurCellOffX(m_pMoveNode->getPositionX()-DO_STOP_CLOSE_SPEED);
	}
}

//find the point to stop close from position
void UICircleViewBaseExt::calDoStopCloseTargetX()
{
	//assign default value
	m_doStopCloseTargetX = m_pMoveNode->getPositionX();
	if(m_touchBeganPoint.x>=m_touchEndedPoint.x)
	{
		//scrollToLeft
		m_doStopCloseTargetX = floor(m_pMoveNode->getPositionX()/m_cellDistance)*m_cellDistance;
	}else{
		//scrollToRight
		m_doStopCloseTargetX = ceil(m_pMoveNode->getPositionX()/m_cellDistance)*m_cellDistance;
	}
}

bool UICircleViewBaseExt::isFloatEqual(float a, float b)
{
	return fabs(a-b)<0.001f;
}

bool UICircleViewBaseExt::init(float width,float height)
{
	if (Layout::init())
	{
		setTouchEnabled(true);
		setSize(CCSizeMake(width,height));
		m_pMoveNode = Widget::create();
		m_pMoveNode->setSize(CCSizeMake(width,height));
		m_pMoveNode->setTouchEnabled(true);
		setClippingEnabled(true);
		setClippingType(LAYOUT_CLIPPING_SCISSOR);
		m_pMoveNode->retain();
		addChild(m_pMoveNode);
		return true;
	}
	return false;
}

void UICircleViewBaseExt::turnRight()
{
	if(m_IsDoStopClose)return;
	m_doStopCloseTargetX = ceil((m_pMoveNode->getPositionX()+10)/m_cellDistance)*m_cellDistance;
	m_IsDoStopClose = true;
	schedule(schedule_selector(UICircleViewBaseExt::doStopClose));
}

void UICircleViewBaseExt::turnLeft()
{
	if(m_IsDoStopClose)return;
	m_doStopCloseTargetX = floor((m_pMoveNode->getPositionX()-10)/m_cellDistance)*m_cellDistance;
	m_IsDoStopClose = true;
	schedule(schedule_selector(UICircleViewBaseExt::doStopClose));
}

NS_CC_EASY_END