#include "Graph.h"

NS_CC_EASY_BEGIN


void CRectangle::getPoints( std::vector<CCPoint>& list ) const
{
	list.push_back(ccp(m_Rect.getMinX(),m_Rect.getMinY()));
	list.push_back(ccp(m_Rect.getMinX(),m_Rect.getMaxY()));
	list.push_back(ccp(m_Rect.getMaxX(),m_Rect.getMaxY()));
	list.push_back(ccp(m_Rect.getMaxX(),m_Rect.getMinY()));
}


NS_CC_EASY_END