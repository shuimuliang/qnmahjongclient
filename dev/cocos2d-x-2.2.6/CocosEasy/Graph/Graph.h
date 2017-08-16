#ifndef __CE_GRAPH_H__
#define __CE_GRAPH_H__

#include "Common/CocosEasyDef.h"
#include "Geometry2D.h"

USING_NS_CC;

NS_CC_EASY_BEGIN


enum eGraphType{UNKNOW=0,POINT,LINE,CIRCLE,RECT,POLY,};

class CGraph
{
public:
	CGraph(eGraphType type){m_Type = type;}
	bool isLine() const {return m_Type == LINE;}

protected:
	CC_SYNTHESIZE(eGraphType,m_Type,Type);
	CC_SYNTHESIZE_PASS_BY_REF(CCPoint,m_Center,Center);
};

class CPOINT : public CGraph
{
public:
	CPOINT(const CCPoint& pt) : CGraph(POINT){m_Data = pt; setCenter(m_Data);}
private:
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCPoint,m_Data,Data);
};

class CLine : public CGraph
{
public:
	CLine(const CCPoint& src,const CCPoint& des) : CGraph(LINE){m_Src = src; m_Des = des; setCenter(m_Src);}
	float getSubX() const{return (m_Des.x - m_Src.x);}
	float getSubY() const{return (m_Des.y - m_Src.y);}
private:
	CC_SYNTHESIZE_PASS_BY_REF(CCPoint,m_Src,Src);
	CC_SYNTHESIZE_PASS_BY_REF(CCPoint,m_Des,Des);
};

class CCircle : public CGraph
{
public: 
	CCircle(const CCPoint& center,float radius) : CGraph(CIRCLE){m_CirCenter = center; m_Radius = radius; setCenter(m_CirCenter);}
private:
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCPoint,m_CirCenter,CircleCenter);
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(float,m_Radius,Radius);
};

class CRectangle : public CGraph
{
public: 
	CRectangle(const CCRect& rect) : CGraph(RECT){m_Rect = rect; setCenter(m_Rect.origin);}
	virtual void getPoints(std::vector<CCPoint>& list) const;
	CCPoint getBMin() const{return ccp(m_Rect.getMinX(),m_Rect.getMinY());}
	CCPoint getBMax() const{return ccp(m_Rect.getMaxX(),m_Rect.getMaxY());}
private:
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCRect,m_Rect,Data);
};

class CPoly : public CGraph
{
public:
	CPoly() : CGraph(POLY){}
	CPoly(const std::vector<CCPoint>& poly) : CGraph(POLY)
	{
		m_Poly = poly; 
		m_pFirst = m_Poly.size() == 0 ? NULL : &m_Poly[0];
		if(m_pFirst) setCenter(*m_pFirst);
	}
	unsigned int size() const{return m_Poly.size();}

private:
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<CCPoint>,m_Poly,Data);
	CC_SYNTHESIZE_READONLY(CCPoint*,m_pFirst,First);
};

class CRaycast : public CLine
{
public:
	CRaycast(const CCPoint& src,const CCPoint& vec,float dis) : CLine(ccp(0,0),ccp(0,0))
	{
		m_Src = src;
		m_Vector = vec;
		m_Distance = dis;
		m_Des = g2Vray(m_Src,m_Vector,m_Distance);
	}
protected:
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCPoint,m_Vector,Vector);
	CC_SYNTHESIZE_READONLY(float,m_Distance,Distance);
};

NS_CC_EASY_END

#endif