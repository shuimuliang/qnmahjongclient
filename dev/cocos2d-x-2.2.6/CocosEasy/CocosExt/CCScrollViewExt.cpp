#include "CCScrollViewExt.h"
#include "CCScollPanelViewExt.h"
#include "CCStdC.h"

NS_CC_EASY_BEGIN
const float SCROLL_DEACCEL_RATE = 0.95f; 
const float SCROLL_DEACCEL_DIST = 1.0f;
const float BOUNCE_DURATION		= 0.15f;
const float INSET_RATIO			= 0.2f;

const CCPoint SCROLLDIR_UP = CCPoint(0.0f, 1.0f);
const CCPoint SCROLLDIR_DOWN = CCPoint(0.0f, -1.0f);
const CCPoint SCROLLDIR_LEFT = CCPoint(-1.0f, 0.0f);
const CCPoint SCROLLDIR_RIGHT = CCPoint(1.0f, 0.0f);

static const float AUTOSCROLLMAXSPEED = 1000.0f;
static const float PAGE_CHECK_DIS = 20.0f;

static float convertDistanceFromPointToInch(float pointDis)
{
	float factor = ( CCEGLView::sharedOpenGLView()->getScaleX() + CCEGLView::sharedOpenGLView()->getScaleY() ) / 2;
	return pointDis * factor / CCDevice::getDPI();
}

CCScrollViewExt::CCScrollViewExt()
	:m_canMoveType(eCanNotMoveT)
	,m_pScrollViewEventExt(NULL)
	,m_pScrollPanel(NULL)
	,m_scriptScrollViewHandler(0)
	,m_isPageChangable(true)
{
	_childFocusCancelOffset = 5.0f;
}

CCScrollViewExt::~CCScrollViewExt()
{
	m_pScrollViewEventExt = NULL;
	CC_SAFE_RELEASE(m_pScrollPanel);
}

CCScrollViewExt* CCScrollViewExt::create(float width, float height)
{
	CCScrollViewExt * pRet = new CCScrollViewExt();
	if (pRet && pRet->init(width,height))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;	
}

CCScrollViewExt* CCScrollViewExt::create(CCSize size)
{
	return create(size.width,size.height);
}

bool CCScrollViewExt::init( float width, float height )
{
	if (!ScrollView::init()) return false;
	setSize(CCSizeMake(width,height));
	setTouchEnabled(true);
	setBounceEnabled(true);
	resetContainerPos();
	setDirection(SCROLLVIEW_DIR_BOTH);
	return true;
}

void CCScrollViewExt::addScrollPanel(const char* srcArrow , const char* srcPanel ,int length)
{
	m_pScrollPanel = CCScollPanelViewExt::create(srcArrow,srcPanel,length);
	CC_SAFE_RETAIN(m_pScrollPanel);
	addChild(m_pScrollPanel,100,100);
	m_pScrollPanel->setAnchorPoint(getAnchorPoint());
	m_pScrollPanel->setPositionX(_size.width);
	m_pScrollPanel->resetPanel(_size.height,_innerContainer->getContentSize().height);
	m_pScrollPanel->resetStart();
	addEventListenerScrollView(m_pScrollPanel,scrollvieweventselector(CCScollPanelViewExt::eventCallbackFunc));
}

CCScrollViewExt::E_CAN_MOVE_T CCScrollViewExt::calCanMoveType(const CCPoint &moveDis)
{
	E_CAN_MOVE_T result = eCanNotMoveT;
	if(moveDis.getLength()<_childFocusCancelOffset)return result;
	float ratio = fabs(moveDis.x/moveDis.y);
	if(ratio>1.2)
	{
		result = eCanhorizotalMoveT;
	}else
	{
		result = eCanVerticalMoveT;
	}
	return result;
}

void CCScrollViewExt::registerScrollViewEvent(IScrollViewEventExt *pEvent)
{
	m_pScrollViewEventExt = pEvent;
}

void CCScrollViewExt::resetContainerPos()
{
	_innerContainer->setPosition(CCPoint(0, _size.height - _innerContainer->getSize().height));
}


void CCScrollViewExt::registerScriptScrollViewEvent(int handler)
{
	m_scriptScrollViewHandler = handler;
}

void CCScrollViewExt::onEvent(bool isLeft)
{
	m_isLeft = isLeft;
	if(m_scriptScrollViewHandler)
	{
		CCAction *pAction =CCCallFunc::create(this,callfunc_selector(CCScrollViewExt::onEventCallBack));
		runAction(pAction);
	}else
	{
		if(m_pScrollViewEventExt)
		{
			m_pScrollViewEventExt->onChangePage(isLeft,this);
		}
	}
}

void CCScrollViewExt::handlePressLogic(const CCPoint &touchPoint)
{   
	_touchBeganPoint = convertToNodeSpace(touchPoint);
	_touchMovingPoint = _touchBeganPoint; 
	startRecordSlidAction();
	_bePressed = true;
}

void CCScrollViewExt::handleMoveLogic(const CCPoint &touchPoint)
{
	_touchMovedPoint = convertToNodeSpace(touchPoint);
	CCPoint delta = _touchMovedPoint - _touchMovingPoint;
	_touchMovingPoint = _touchMovedPoint;
	if(m_canMoveType==eCanNotMoveT&&_bouncing == false)
	{
		m_canMoveType = calCanMoveType(delta);
	}
	if(_bouncing||_autoScroll){
		
	}
	switch (m_canMoveType)
	{
	case eCanVerticalMoveT: // vertical
		{
			scrollChildren(0.0f, delta.y);
			setDirection(SCROLLVIEW_DIR_VERTICAL);
			break;
		}
	case eCanhorizotalMoveT: // horizontal
		{
			if(m_isPageChangable)
			{
				setDirection(SCROLLVIEW_DIR_HORIZONTAL);
				scrollChildren(delta.x, 0.0f);
			}else{
				setDirection(SCROLLVIEW_DIR_NONE);
			}
			break;
		}
	default:
		setDirection(SCROLLVIEW_DIR_NONE);
		break;
	}
}

void CCScrollViewExt::handleReleaseLogic(const CCPoint &touchPoint)
{
	_touchEndedPoint = convertToNodeSpace(touchPoint);
	if(_innerContainer->getPositionX()<-PAGE_CHECK_DIS/*-_size.width/2*/)
	{
		onEvent(true);
		resetContainerPos();
	}else if(_innerContainer->getPositionX()>PAGE_CHECK_DIS/*_size.width/2*/){
		onEvent(false);
		resetContainerPos();
	}else{
		endRecordSlidAction();
	}
	m_canMoveType = eCanNotMoveT;
	_bePressed = false;
}

void CCScrollViewExt::onSizeChanged()
{
	Layout::onSizeChanged();
	_topBoundary = _size.height;
	_rightBoundary = _size.width;
	float bounceBoundaryParameterX = _size.width / 1.0f;
	float bounceBoundaryParameterY = _size.height / 1.0f;
	_bounceTopBoundary = _size.height - bounceBoundaryParameterY;
	_bounceBottomBoundary = bounceBoundaryParameterY;
	_bounceLeftBoundary = bounceBoundaryParameterX;
	_bounceRightBoundary = _size.width - bounceBoundaryParameterX;
	CCSize innerSize = _innerContainer->getSize();
	float orginInnerSizeWidth = innerSize.width;
	float orginInnerSizeHeight = innerSize.height;
	float innerSizeWidth = MAX(orginInnerSizeWidth, _size.width);
	float innerSizeHeight = MAX(orginInnerSizeHeight, _size.height);
	_innerContainer->setSize(CCSize(innerSizeWidth, innerSizeHeight));
	_innerContainer->setPosition(CCPoint(0, _size.height - _innerContainer->getSize().height));
}

void CCScrollViewExt::addChild(CCNode* child, int zOrder, int tag)
{
	if(dynamic_cast<CCScollPanelViewExt *>(child)!=NULL)
	{
		Layout::addChild(child,zOrder,tag);
	}else
	{
		ScrollView::addChild(child,zOrder,tag);
	}
}

void CCScrollViewExt::setPageChangable(bool canChange)
{
	m_isPageChangable = canChange;
}

void CCScrollViewExt::onEventCallBack()
{
	CCArray* pArrayArgs = CCArray::createWithCapacity(2);
	pArrayArgs->addObject(CCBool::create(m_isLeft));
	pArrayArgs->addObject(this);
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_scriptScrollViewHandler,pArrayArgs);
}


void CCScrollViewExt::setScrollPanelPos(float x,float y)
{
	CC_ASSERT(m_pScrollPanel);
	if(m_pScrollPanel)
	{
		m_pScrollPanel->setPosition(ccp(x,y));
	}
}

void CCScrollViewExt::resetScrollPanel()
{
	CC_ASSERT(m_pScrollPanel);
	if(m_pScrollPanel)
	{
		m_pScrollPanel->resetPanel(_size.width,getInnerContainerSize().height);
	}
}

void CCScrollViewExt::update(float dt)
{
	ScrollView::update(dt);
}

void CCScrollViewExt::onEnter()
{
	Layout::onEnter();
	scheduleUpdate();
	startRecordSlidAction();
}
NS_CC_EASY_END