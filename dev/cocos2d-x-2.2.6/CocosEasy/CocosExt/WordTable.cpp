#include "WordTable.h"

NS_CC_EASY_BEGIN

CERowWidget::CERowWidget()
: m_columnSpacing(0)
{
}


CERowWidget* CERowWidget::create(float columnSpacing)
{
	CERowWidget *pRet = new CERowWidget();
	if (pRet && pRet->init(columnSpacing))
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


bool CERowWidget::init(float columnSpacing /*= 0*/)
{
	m_columnSpacing = columnSpacing;
	return UIWidget::init();
}


void CERowWidget::addChild(CCNode* pNode, int zOrder, int tag)
{
	UIWidget::addChild(pNode, zOrder, tag);
	UIWidget* child = static_cast<UIWidget*>(pNode);
	const CCSize& curSize = getSize();
	const CCSize& childSize = child->getSize();
	
	// set child property
	const CCPoint& anchor = child->getAnchorPoint();
	float scaleWidth = childSize.width * child->getScaleX();
	float scaleHeight = childSize.height * child->getScaleY();
	float x = curSize.width + scaleWidth * anchor.x;
	float y = scaleHeight * anchor.y;
	child->setPosition(ccp(x,y));

	// update size
	float width = curSize.width + scaleWidth + m_columnSpacing;
	float height = scaleHeight > curSize.height ? scaleHeight : curSize.height;
	setSize(CCSizeMake(width,height));
}

void CERowWidget::setSize(const CCSize &size)
{
	ignoreContentAdaptWithSize(false);
	setContentSize(size);
	UIWidget::setSize(size);
}



CEWordTable::CEWordTable()
: m_maxWidth((float)0xffff)
, m_hAlignment(kCCTextAlignmentCenter)
, m_lineSpacing(2.0f)
, m_columnSpacing(0.0f)
{

}

CEWordTable::~CEWordTable()
{
	
}

CEWordTable* CEWordTable::create()
{
	CEWordTable *pRet = new CEWordTable();
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





void CEWordTable::addChild(CCNode* pNode, int zOrder, int tag)
{
	//UIWidget::addChild(pNode, zOrder, tag);
	UIWidget* child = static_cast<UIWidget*>(pNode);
	
	// check need to new line
	const CCSize& childSize = child->getSize();
	if( m_container.size() == 0 || 
		getLastLineSize().width + childSize.width > m_maxWidth) 
	{
		newLine();
	}

	
	// add child to row 
	CERowWidget* pCurRow = m_container[m_container.size()-1];
	CCSize oldSize = pCurRow->getSize();
	pCurRow->addChild(child, zOrder, tag);
	
	// update word table size
	CCSize newSize = pCurRow->getSize();
	float extY = newSize.height - oldSize.height;
	float width = getSize().width < newSize.width ? newSize.width : getSize().width;
	float height = getSize().height + extY;
	setSize(CCSizeMake(width,height));

	// set row alignment
	switch (m_hAlignment)
	{
	case kCCTextAlignmentLeft: 
		{
			pCurRow->setAnchorPoint(ccp(0,1.0f));
			pCurRow->setPositionX(0);
		}break;

	case kCCTextAlignmentCenter:
		{
			pCurRow->setAnchorPoint(ccp(0.5,1.0f));
			float hWidth = newSize.width /2.0f;
			float hMaxWidth = m_maxWidth / 2.0f;
			float x = hMaxWidth;
			pCurRow->setPositionX(x);
		}break;

	case kCCTextAlignmentRight:
		{
			pCurRow->setAnchorPoint(ccp(1.0f,1.0f));
			pCurRow->setPositionX(m_maxWidth);
		}break;

	default: 
		astBack;
		break;
	}
}



void CEWordTable::setSize(const CCSize &size)
{
	ignoreContentAdaptWithSize(false);
	setContentSize(size);
	UIWidget::setSize(size);
}



CERowWidget* CEWordTable::newLine()
{
	// create newl line
	CERowWidget* pCurRow = createRowWidget();
	UIWidget::addChild(pCurRow,0,0);
	m_container.push_back(pCurRow);
	if(m_container.size() < 2) return pCurRow;

	// set line position
	CERowWidget* pPreRow = m_container[m_container.size()-2];
	float y = pPreRow->getPositionY() - pPreRow->getSize().height - m_lineSpacing;
	pCurRow->setPositionY(y);

	// update size
	setSize(CCSizeMake(_size.width, _size.height + m_lineSpacing));

	return pCurRow;
}



void CEWordTable::clear()

{
	for (unsigned int i=0; i<m_container.size(); ++i)
	{
		CERowWidget* pWidget = m_container[i];
		if(!pWidget) continue;
		pWidget->removeFromParentAndCleanup(true);
	}
	m_container.clear();
	setSize(CCSizeMake(0,0));
}

const CCSize& CEWordTable::getLastLineSize() const
{
	if(m_container.size() == 0) return getSize();
	CERowWidget* pRow = m_container[m_container.size()-1];
	return pRow->getSize();
}

CERowWidget* CEWordTable::createRowWidget()
{
	CERowWidget* pRow = CERowWidget::create(m_columnSpacing);
	if(!pRow) return NULL;
	pRow->setAnchorPoint((ccp(0,1)));
	return pRow;
}

int CEWordTable::getSumOfLine()
{
	return m_container.size();
}











NS_CC_EASY_END