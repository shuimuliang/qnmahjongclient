#ifndef __CE_DEBUG_DRAW_H__
#define __CE_DEBUG_DRAW_H__

#include "Common/CocosEasyDef.h"

USING_NS_CC;

NS_CC_EASY_BEGIN

#define COLOR_RED		{255,0,0,0}
#define COLOR_GREEN		{0,255,0,0}
#define COLOR_BLUE		{0,0,255,0}

class CGraph;

struct st_DrawInfo
{
	st_DrawInfo()
	{
		isSolid = true; size = 1;
		_ccColor4B color = COLOR_BLUE;
		rgba4b = color;
	}
	_ccColor4B rgba4b;
	bool isSolid;
	unsigned int size;
}; 

class CDebugDraw : public CCNode
{
public:
	struct st_GraphItem{CGraph* pGraph; st_DrawInfo info;};
	CDebugDraw();
	~CDebugDraw(){unInit();}
	static CDebugDraw* create();

public:
	virtual void clear(){clearStable(); clearTick();}
	virtual void clearStable();
	virtual void clearTick();

public:
	virtual void drawPoint(const CCPoint& center,const st_DrawInfo* pInfo = NULL,bool isStable = false);
	virtual void drawLine(const CCPoint& src,const CCPoint& des,const st_DrawInfo* pInfo = NULL,bool isStable = false);
	virtual void drawCircle(const CCPoint& center,unsigned int radius,const st_DrawInfo* pInfo = NULL,bool isStable = false);
	virtual void drawRectangle(const CCRect& rect,const st_DrawInfo* pInfo = NULL,bool isStable = false);
	virtual void drawPoly(const std::vector<CCPoint>& poly,const st_DrawInfo* pInfo = NULL,bool isStable = false);

private:
	virtual void addGraph(CGraph* pGraph,const st_DrawInfo* pInfo = NULL,bool isStable = false);
	virtual void drawGraph(const st_GraphItem& item );

protected:
	virtual bool init(){return true; setPosition(0,0);}
	virtual void unInit(){clear();}
	virtual void draw();

private:
	std::vector<st_GraphItem> m_StableList;
	std::vector<st_GraphItem> m_TickList;
};



NS_CC_EASY_END



#endif