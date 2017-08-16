#include "CCScollPanelViewExt.h"

NS_CC_EASY_BEGIN

CCScollPanelViewExt::CCScollPanelViewExt()
	:m_startY(0)
	,m_ratio(0)
	,m_pArrow(NULL)
	,m_pPanel(NULL)
{
	setAnchorPoint(ccp(1,1));
}

	void CCScollPanelViewExt::resetPanel( int viewH,int containH )
	{
		if(viewH<containH){
			CCAssert(containH!=0,"divide by zero!");
			m_ratio = (float)m_length / (float)containH;
			m_arrowLength = m_length*viewH/containH;
			float scaleY = m_arrowLength/m_pArrow->getContentSize().height;
			m_pArrow->setScaleY(scaleY);
			m_pArrow->setAnchorPoint(ccp(0.5,1));
			setVisible(true);
		}else{
			setVisible(false);
		}
	}

	void CCScollPanelViewExt::eventCallbackFunc(CCObject* sender,ScrollviewEventType eventType)
	{
		ScrollView *pScrollView = dynamic_cast<ScrollView*>(sender);
		CC_ASSERT(pScrollView);
		//计算相对移动百分比
		//计算containner移动的相对位置，以左上为0，0
		UIWidget *pContainer = pScrollView->getInnerContainer();
		float conYToLeftTop = (pContainer->getContentSize().height-pScrollView->getSize().height)+pContainer->getPositionY();
		float movePercent = conYToLeftTop/(pContainer->getContentSize().height - pScrollView->getSize().height);

		movePercent = movePercent<0?0:movePercent;
		movePercent = movePercent>1?1:movePercent;
		m_startY = m_length - (m_length-m_arrowLength)*movePercent;
		m_pArrow->setPositionY(m_startY);

	}

	CCScollPanelViewExt* CCScollPanelViewExt::create( const char* srcArrow , const char* srcPanel ,int length)
	{
		CCScollPanelViewExt *pobSprite = new CCScollPanelViewExt();
		if (pobSprite && pobSprite->init(srcArrow,srcPanel,length))
		{
			pobSprite->autorelease();
			return pobSprite;
		}
		CC_SAFE_DELETE(pobSprite);
		return NULL;
	}


	void CCScollPanelViewExt::resetStart()
	{
		m_startY=m_length;
		m_pArrow->setPositionY(m_startY);
	}

	CCScollPanelViewExt::~CCScollPanelViewExt()
	{
		CC_SAFE_RELEASE(m_pArrow);
		CC_SAFE_RELEASE(m_pPanel);
	}

	bool CCScollPanelViewExt::init(const char* srcArrow , const char* srcPanel ,int length)
	{
		if (Layout::init())
		{
			m_length = length;
			if(srcPanel){
				m_pPanel = ImageView::create();
				m_pPanel->loadTexture(srcPanel);
				m_pPanel->setScaleY((length)/m_pPanel->getContentSize().height);
				m_pPanel->setPosition(ccp(0,length/2));
				m_pPanel->retain();
				addChild(m_pPanel);
			}
			CC_ASSERT(srcArrow);
			m_pArrow = ImageView::create();
			m_pArrow->loadTexture(srcArrow);
			m_pArrow->retain();
			m_pArrow->setAnchorPoint(ccp(0.5,1));
			m_pArrow->setPositionY(length);
			addChild(m_pArrow);
			return true;
		}
		return false;
	}


NS_CC_EASY_END