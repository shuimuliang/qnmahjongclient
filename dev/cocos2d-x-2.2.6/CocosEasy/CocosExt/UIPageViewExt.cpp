#include "UIPageViewExt.h"

NS_CC_EASY_BEGIN


CUIPageViewExt::CUIPageViewExt()
: m_BoundarySlip(false)
, m_CurPageBright(false)
, m_pTurningListener(NULL)
, m_pTurningSelector(NULL)
,  m_scrollCheckFactor(2.0f)
{
	m_DimCorlor = ccGRAY;
}


CUIPageViewExt* cocoseasy::CUIPageViewExt::create()
{
	CUIPageViewExt *pRet = new CUIPageViewExt();

	if (pRet && pRet->init())
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


bool CUIPageViewExt::init()
{
	if(!PageView::init()) return false;
	PageView::addEventListenerPageView(this,
		pagevieweventselector(CUIPageViewExt::pageTurningCallBack));
	return true;
}


void CUIPageViewExt::onSizeChanged()
{
	float rightBoundray = _rightBoundary;
	PageView::onSizeChanged();
	if(!m_BoundarySlip) return;
	if(_rightBoundary != rightBoundray){assert(false);}
	_rightBoundary = rightBoundray;
}

void CUIPageViewExt::enableBoundraySlip( bool onOff )
{
	m_BoundarySlip = true;
	if(m_BoundarySlip)
	{
		_leftBoundary = getSize().width / 2.0f;
		_rightBoundary = getSize().width - _leftBoundary;
	}
	else
	{
		_leftBoundary = 0;
		_rightBoundary = getSize().width;
	}
}

void CUIPageViewExt::enableCurPageBright( bool onOff )
{
	if(m_CurPageBright == onOff) return;
	m_CurPageBright = onOff;
	updateBrightAndDim();
}

void CUIPageViewExt::updateBrightAndDim()
{
	ccArray* arrayPages = _pages->data;
	int length = arrayPages->num;
	for (int i = 0; i < length; i++)
	{
		UIWidget* child = (UIWidget*)(arrayPages->arr[i]);
		if(m_CurPageBright)
		{
			if(i == getCurPageIndex())
				setColorAllChild(child,ccWHITE);
			else
				setColorAllChild(child,m_DimCorlor);
		}
		else
			child->setColor(ccWHITE);
	}
}

void CUIPageViewExt::addEventListenerPageView( CCObject *target, SEL_PageViewEvent selector )
{
	m_pTurningListener = target;
	m_pTurningSelector = selector;
}

void CUIPageViewExt::pageTurningCallBack( CCObject* pObj ,PageViewEventType type)
{
    
	// set bright and dim
	if(m_CurPageBright)
	{
		updateBrightAndDim();
	}

	if(m_pTurningListener && m_pTurningSelector)
	{
		(m_pTurningListener->*m_pTurningSelector)(this, type);
	}
}

void CUIPageViewExt::setColorAllChild( UIWidget* pRoot,ccColor3B color )
{
	if(!pRoot) return;
	pRoot->setColor(color);
	cocos2d::ccArray* list = pRoot->getChildren()->data;
	unsigned int length = list->num;
	for (unsigned int i = 0; i < length; i++)
	{
		UIWidget* child = (UIWidget*)(list->arr[i]);
		setColorAllChild(child,color);
	}
}

void CUIPageViewExt::changePage( unsigned int idx )
{
	if (idx < 0 || idx >= (int)(_pages->count())) return;
	_curPageIdx = idx;
	Widget* curPage = static_cast<Widget*>(_pages->objectAtIndex(idx));
	float distance = -(curPage->getPosition().x);
	movePages(distance);
}

void CUIPageViewExt::handleReleaseLogic(const CCPoint &touchPoint)
{
	if (_pages->count() <= 0)
	{
		return;
	}
	Widget* curPage = static_cast<Widget*>(_pages->objectAtIndex(_curPageIdx));
	if (curPage)
	{
		CCPoint curPagePos = curPage->getPosition();
		int pageCount = _pages->count();
		float curPageLocation = curPagePos.x;
		float pageWidth = getSize().width;
		float boundary = pageWidth/m_scrollCheckFactor;
		if (curPageLocation <= -boundary)
		{
			if (_curPageIdx >= pageCount-1)
			{
				scrollPages(-curPageLocation);
			}
			else
			{
				scrollToPage(_curPageIdx+1);
			}
		}
		else if (curPageLocation >= boundary)
		{
			if (_curPageIdx <= 0)
			{
				scrollPages(-curPageLocation);
			}
			else
			{
				scrollToPage(_curPageIdx-1);
			}
		}
		else
		{
			scrollToPage(_curPageIdx);
		}
	}
}

void CUIPageViewExt::setScrollCheckFactor(float factor)
{
	m_scrollCheckFactor = factor;
}




NS_CC_EASY_END