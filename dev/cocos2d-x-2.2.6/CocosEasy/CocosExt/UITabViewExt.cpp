#include "UITabViewExt.h"
NS_CC_EASY_BEGIN

CUITabViewDataExt::CUITabViewDataExt(Button* btn , Widget* view):m_btnOrder(0)
{
	m_pBtn = btn;
	m_pBtn->retain();
	m_pView = view;
	m_pView->retain();
}

CUITabViewDataExt::~CUITabViewDataExt()
{	
	CC_SAFE_RELEASE(m_pView);
	CC_SAFE_RELEASE(m_pBtn);
}

void CUITabViewDataExt::chooseAndSetOrder(int zOrder)
{
	if(m_pBtn&&m_pView)
	{
		m_pBtn->setTouchEnabled(false);
		m_pBtn->setBright(true);
//		m_pBtn->setZOrder(zOrder);
		m_pView->setVisible(true);
        m_pView->setEnabled(true);
	}
}

void CUITabViewDataExt::unChooseAndSetOrder()
{
	if(m_pBtn&&m_pView)
	{
		m_pBtn->setTouchEnabled(true);
		m_pBtn->setBright(false);
//		m_pBtn->setZOrder(m_btnOrder);
		m_pView->setVisible(false);
        m_pView->setEnabled(false);
	}
}

void CUITabViewDataExt::setBtnZOrder(int zOrder)
{
//	m_btnOrder = zOrder;
}

CUITabViewExt * CUITabViewExt::create()
{
	CUITabViewExt *result = new CUITabViewExt;
	result->autorelease();
	return result;
}

CUITabViewExt::~CUITabViewExt()
{
	clearTabViewData();
}

CUITabViewExt::CUITabViewExt()
	:m_maxBtnZOrder(-1000)
	,m_pClickEventView(NULL)
	,m_handler(0)
	,m_curTabId(0)
{
	
}

void CUITabViewExt::addView(Button* btn, Widget* view)
{
	//addToData
	CUITabViewDataExt* pData = new CUITabViewDataExt(btn,view);
	
	m_maxBtnZOrder = m_maxBtnZOrder<btn->getZOrder()?btn->getZOrder()+1:m_maxBtnZOrder;
	pData->setBtnZOrder(btn->getZOrder());
	//btn init
	if(m_tabViewData.empty()){
		pData->chooseAndSetOrder(1);
	}else{
		pData->unChooseAndSetOrder();
	}
	m_tabViewData.push_back(pData);
	//register event
	btn->addTouchEventListener(this, toucheventselector(CUITabViewExt::touchUpInsideAction));
}

bool CUITabViewExt::choose(unsigned int id , bool isByBtn)
{
	if(id>=m_tabViewData.size())return false;
	for(unsigned int i = 0;i<m_tabViewData.size();++i)
	{
		CUITabViewDataExt *pData = m_tabViewData.at(i);
		if(id == i)
		{
			onClickEvent(id,isByBtn);
			pData->chooseAndSetOrder(m_maxBtnZOrder);
		}else
		{
			pData->unChooseAndSetOrder();
		}
	}
	return true;
}

void CUITabViewExt::resetToFirstView()
{
	choose(0,false);
}

void CUITabViewExt::registClickEvent(IClickEventTabView *pEvent)
{
	m_pClickEventView = pEvent;
}

void CUITabViewExt::unRegistClickEvent()
{
	m_pClickEventView = NULL;
}

Button * CUITabViewExt::getButton(unsigned int id)
{
	Button* result = NULL;
	if(id<m_tabViewData.size())
	{
		result = m_tabViewData.at(id)->getBtn();
	}
	return result;
}

Widget* CUITabViewExt::getView(unsigned int id)
{
	Widget* result = NULL;
	if(id<m_tabViewData.size())
	{
		result = m_tabViewData.at(id)->getView();
	}
	return result;
}

void CUITabViewExt::clearTabViewData()
{
	for(unsigned int i = 0;i<m_tabViewData.size();++i)
	{
		delete m_tabViewData.at(i);
	}
	m_tabViewData.clear();
}

void CUITabViewExt::touchUpInsideAction(CCObject *sender, TouchEventType type)
{
	if(type!= TOUCH_EVENT_ENDED)return;
	
	for(unsigned int i = 0;i<m_tabViewData.size();++i)
	{
		CUITabViewDataExt *pData = m_tabViewData.at(i);
		if(pData->getBtn()==sender)
		{
			onClickEvent(i,true);
			pData->chooseAndSetOrder(m_maxBtnZOrder);
		}else
		{
			pData->unChooseAndSetOrder();
		}
	}
}

void CUITabViewExt::registScriptClickEvent(int handler)
{
	m_handler = handler;
}

void CUITabViewExt::onClickEvent(int id,bool isByBtn )
{
	m_curTabId = id;
	if(m_handler)
	{
		CCArray* pArrayArgs = CCArray::createWithCapacity(2);
		pArrayArgs->addObject(CCInteger::create(id));
		pArrayArgs->addObject(CCBool::create(isByBtn));
		CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_handler,pArrayArgs);
	}else{
		if(m_pClickEventView)
		{
			m_pClickEventView->onClickTabId(id);
		}
	}
}

int CUITabViewExt::getCurTabId()
{
	return m_curTabId;
}

Button* CUITabViewExt::getCurButton()
{
	return getButton(m_curTabId);
}

Widget* CUITabViewExt::getCurView()
{
	return getView(m_curTabId);
}


NS_CC_EASY_END