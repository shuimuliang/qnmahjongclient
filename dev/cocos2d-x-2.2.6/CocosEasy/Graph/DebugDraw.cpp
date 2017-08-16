#include "DebugDraw.h"
#include "Graph.h"

NS_CC_EASY_BEGIN

CDebugDraw::CDebugDraw()
{

}

CDebugDraw* CDebugDraw::create()
{
	CDebugDraw* pRet = new CDebugDraw();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void CDebugDraw::clearStable()
{
	for (unsigned int i=0; i<m_StableList.size(); ++i)
	{
		CC_SAFE_DELETE(m_StableList[i].pGraph);
	}
	m_StableList.clear();
}

void CDebugDraw::clearTick()
{
	for (unsigned int i=0; i<m_TickList.size(); ++i)
	{
		CC_SAFE_DELETE(m_TickList[i].pGraph);
	}
	m_TickList.clear();
}


void CDebugDraw::addGraph( CGraph* pGraph,const st_DrawInfo* pInfo /*= NULL*/ ,bool isStable /*false*/)
{
	if(!pGraph) return;
	st_GraphItem item;
	item.pGraph = pGraph;
	if(pInfo) item.info = *pInfo;

	if(isStable)
		m_StableList.push_back(item);
	else
		m_TickList.push_back(item);
}

void CDebugDraw::draw()
{
	CHECK_GL_ERROR_DEBUG();

	// draw stable list
	for (unsigned int i=0; i<m_StableList.size(); ++i)
	{
		CHECK_GL_ERROR_DEBUG();
		drawGraph(m_StableList[i]);
	}

	// draw tick list
	for (unsigned int i=0; i<m_TickList.size(); ++i)
	{
		CHECK_GL_ERROR_DEBUG();
		drawGraph(m_TickList[i]);
	}
	clearTick();
}

void CDebugDraw::drawGraph( const st_GraphItem& item )
{
	const CGraph* pGraph = item.pGraph;
	const st_DrawInfo& info = item.info;
	const _ccColor4B& rgba = info.rgba4b;

	switch (pGraph->getType())
	{
	case POINT:
		{
			const CPOINT* pPoint = static_cast<const CPOINT*>(pGraph);
			ccDrawColor4B(rgba.r,rgba.g,rgba.b,rgba.a);
			ccPointSize(info.size);
			ccDrawPoint(pPoint->getData());
		}
		break;
	case LINE:
		{
			const CLine* pLine = static_cast<const CLine*>(pGraph);
			ccDrawColor4B(rgba.r,rgba.g,rgba.b,rgba.a);
			glLineWidth(info.size);
			ccDrawLine(pLine->getSrc(),pLine->getDes());
		}break;
	case CIRCLE:
		{
			const CCircle* pCircle = static_cast<const CCircle*>(pGraph);
			ccDrawColor4B(rgba.r,rgba.g,rgba.b,rgba.a);
			glLineWidth(info.size);  
			ccDrawCircle(pCircle->getCircleCenter(), pCircle->getRadius(),0,10,false);
		}break;
	case RECT:
		{
			const CRectangle* pRect = static_cast<const CRectangle*>(pGraph);
			if(info.isSolid)
			{
				ccColor4F color = ccc4FFromccc4B(rgba);
				ccDrawSolidRect(pRect->getBMin(),pRect->getBMax(),color);
			}
			else
			{
				ccDrawColor4B(rgba.r,rgba.g,rgba.b,rgba.a);
				glLineWidth(info.size);  
				ccDrawRect(pRect->getBMin(),pRect->getBMax());
			}
		}break;
	case POLY:
		{
			const CPoly* pPoly = static_cast<const CPoly*>(pGraph);
			if(info.isSolid)
			{
				ccColor4F color = ccc4FFromccc4B(rgba);
				ccDrawSolidPoly(pPoly->getFirst(),pPoly->size(),color);
			}
			else
			{
				ccDrawColor4B(rgba.r,rgba.g,rgba.b,rgba.a);
				glLineWidth(info.size);  
				ccDrawPoly(pPoly->getFirst(),pPoly->size(),false);
			}
		}break;
	default:
		break;
	}
}

void CDebugDraw::drawPoint( const CCPoint& center,const st_DrawInfo* pInfo /*= NULL*/,bool isStable )
{
	st_GraphItem item;
	CPOINT* pGraph = new CPOINT(center);
	addGraph(pGraph,pInfo,isStable);
}

void CDebugDraw::drawLine( const CCPoint& src,const CCPoint& des,const st_DrawInfo* pInfo /*= NULL*/ ,bool isStable)
{
	st_GraphItem item;
	CLine* pGraph = new CLine(src,des);
	addGraph(pGraph,pInfo,isStable);
}

void CDebugDraw::drawCircle( const CCPoint& center,unsigned int radius,const st_DrawInfo* pInfo /*= NULL*/,bool isStable )
{
	st_GraphItem item;
	CCircle* pGraph = new CCircle(center,radius);
	addGraph(pGraph,pInfo,isStable);
}

void CDebugDraw::drawRectangle( const CCRect& rect,const st_DrawInfo* pInfo /*= NULL*/ ,bool isStable)
{
	st_GraphItem item;
	CRectangle* pGraph = new CRectangle(rect);
	addGraph(pGraph,pInfo,isStable);
}

void CDebugDraw::drawPoly( const std::vector<CCPoint>& poly,const st_DrawInfo* pInfo /*= NULL*/,bool isStable )
{
	st_GraphItem item;
	CPoly* pGraph = new CPoly(poly);
	addGraph(pGraph,pInfo,isStable);
}




NS_CC_EASY_END