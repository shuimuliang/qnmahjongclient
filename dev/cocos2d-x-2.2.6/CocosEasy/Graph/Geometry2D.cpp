#include "Geometry2D.h"

NS_CC_EASY_BEGIN

bool g2LineCross( const CCPoint &p1,const CCPoint &p2,const CCPoint &p3,const CCPoint &p4,CCPoint* pos /*= NULL*/ )
{
	// rectangle cross
	if(g2Min(p1.x,p2.x) > g2Max(p3.x,p4.x) ||
	   g2Min(p3.x,p4.x) > g2Max(p1.x,p2.x) ||
	   g2Min(p1.y,p2.y) > g2Max(p3.y,p4.y) ||
	   g2Min(p3.y,p4.y) > g2Max(p1.y,p2.y))
	   return false;
	
	// double direct line
	CCPoint v12 = p2 - p1; CCPoint v34 = p4 - p3;
	CCPoint v13 = p3 - p1; CCPoint v14 = p4 - p1; CCPoint v23 = p3 - p2; CCPoint v24 = p4 - p2;

	float cl,cr;
	cl = g2Vcross(v12,v13); if(cl == 0 && g2Vequal(p2,p3)){if(pos) *pos = p2; return true;}
	cr = g2Vcross(v12,v14); if(cr == 0 && g2Vequal(p2,p4)){if(pos) *pos = p2; return true;}
	if(cl * cr > 0) return false;

	cl = g2Vcross(v12,v23); if(cl == 0 && g2Vequal(p1,p3)){if(pos) *pos = p1; return true;}
	cr = g2Vcross(v12,v24); if(cr == 0 && g2Vequal(p1,p4)){if(pos) *pos = p1; return true;}
	if(cl * cr > 0) return false;

	cl = g2Vcross(v34,v13); 
	cr = g2Vcross(v34,v23); 
	if(cl * cr > 0) return false;

	cl = g2Vcross(v34,v14); 
	cr = g2Vcross(v34,v24); 
	if(cl * cr > 0) return false;
	
	if(pos)
	{
		float x1 = p1.x; float y1 = p1.y; float x2 = p2.x; float y2 = p2.y;
		float x3 = p3.x; float y3 = p3.y; float x4 = p4.x; float y4 = p4.y;
		float b1 = (y2 - y1) * x1 + (x1 - x2) * y1;
		float b2 = (y4 - y3) * x3 + (x3 - x4) * y3;
		float D = (x2 - x1) * (y4 - y3) - (x4 - x3) * (y2-y1);
		if(D == 0)	// reclosing completely
		{
			*pos = p3;
		}
		else
		{
			float D1 = b2 * (x2 - x1) - b1 * (x4 - x3);
			float D2 = b2 * (y2 - y1) - b1 * (y4 - y3);
			pos->x = D1 / D; pos->y = D2 / D;
		}
	}
	return true;
}


void g2LineCover(std::vector<CCPoint>& list,const CCPoint& st,const CCPoint& ed,unsigned char (*f)(const CCPoint& pos,void* p),void* pClass)
{
	// get start and end cells
	CCPoint stTile = g2Vtile(st);
	CCPoint edTile = g2Vtile(ed);

	CCPoint vec = ed - st;
	float angle = vec.getAngle(); angle = angle < 0? angle + 2*PI : angle;

	// ensure the number of quadrant and priority direction
	CCPoint dir[2]; unsigned int order = 0;
	if(0<=angle && angle<=PI/4){dir[0] = CCPoint(1,0); dir[1] = CCPoint(0,1); order = 3;}
	else if(PI/4<angle && angle<=PI/2){dir[0] = CCPoint(0,1); dir[1] = CCPoint(1,0); order = 3;}
	else if(PI/2<angle && angle<=3*PI/4){dir[0] = CCPoint(0,1); dir[1] = CCPoint(-1,0); order = 0;}
	else if(3*PI/4<angle && angle<=PI){dir[0] = CCPoint(-1,0); dir[1] = CCPoint(0,1); order = 0;}
	else if(PI<angle && angle<=5*PI/4){dir[0] = CCPoint(-1,0); dir[1] = CCPoint(0,-1); order = 1;}
	else if(5*PI/4<angle && angle<=3*PI/2){dir[0] = CCPoint(0,-1); dir[1] = CCPoint(-1,0); order = 1;}
	else if(3*PI/2<angle && angle<=7*PI/4){dir[0] = CCPoint(0,-1); dir[1] = CCPoint(1,0); order = 2;}
	else if(7*PI/4<angle && angle<=2*PI){dir[0] = CCPoint(1,0); dir[1] = CCPoint(0,-1); order = 2;}
	else{assert(false); return;}

	// find covered path
	CCPoint coreTile = stTile;
	while (true)
	{	
		bool isCovered = false;
		for (unsigned int i=0; i<2; ++i)
		{
			CCPoint curTile = coreTile + dir[i];
			CCRect rect(curTile.x,curTile.y, 1 ,1);
			if(g2LineRectCross(st,ed,rect,order,true))
			{
				isCovered = true;
				coreTile = curTile;
				if(f)
				{
					unsigned char tag = f(rect.origin,pClass);
					if(tag & 0x02){list.push_back(rect.origin);}
					if(tag & 0x01) break;
					return;
				}
				else
				{
					list.push_back(rect.origin);
					break;
				}
			}
		}
		if(!isCovered) return;
	}
}

unsigned int g2LineRectCross( const CCPoint& p1,const CCPoint& p2,const CCRect& rect,unsigned int order /*= 0*/,bool collideEnd /*= false*/, CCPoint* pos /*= NULL*/ )
{
	CCPoint rectangle[4];
	rectangle[0] = CCPoint(rect.getMinX(),rect.getMinY());
	rectangle[1] = CCPoint(rect.getMaxX(),rect.getMinY());
	rectangle[2] = CCPoint(rect.getMaxX(),rect.getMaxY());
	rectangle[3] = CCPoint(rect.getMinX(),rect.getMaxY());
	unsigned int c = g2LinePolyCross(p1,p2,&rectangle[0],4,order,collideEnd,pos); assert(c <= 4);
	return c;
}


unsigned int g2LinePolyCross( const CCPoint& p1,const CCPoint& p2,const CCPoint* poly,unsigned int count, unsigned int order,bool collideEnd /*= false*/, CCPoint* pos /*= NULL*/ )
{
	unsigned int idx1 = order;
	unsigned int idx2 = order + 1;
	unsigned int crossCount = 0;
	for (unsigned int i=0; i<count; ++i,++idx1,++idx2)
	{
		if(idx1 >= count) idx1 = idx1 % count;
		if(idx2 >= count) idx2 = idx2 % count;
		CCPoint* crossPt = pos ? &pos[crossCount] : NULL;
		bool cross = g2LineCross(p1,p2,poly[idx1],poly[idx2],crossPt);
		if(!cross) continue;
		++crossCount; 
		if(collideEnd) break;
	}
	assert(crossCount <= count);
	return crossCount;
}



bool g2RectCross( const CCRect& src, const CCRect& des )
{
	// src rect data
	float srcMinX = src.getMinX();
	float srcMaxX = src.getMaxX();
	float srcMinY = src.getMinY();
	float srcMaxY = src.getMaxY();
	
	// des rect data
	float desMinX = des.getMinX();
	float desMaxX = des.getMaxX();
	float desMinY = des.getMinY();
	float desMaxY = des.getMaxY();

	// rectangle cross
	if( srcMinX > desMaxX ||
		desMinX > srcMaxX ||
		srcMinY > desMaxY ||
		desMinY > srcMaxY)
		return false;

	return true;
}

bool g2PointInRect( const CCPoint& pt, const CCRect& rect )
{
	// src rect data
	float minX = rect.getMinX();
	float maxX = rect.getMaxX();
	float minY = rect.getMinY();
	float maxY = rect.getMaxY();

	if( pt.x < minX ||
		pt.x > maxX ||
		pt.y < minY ||
		pt.y > maxY)
		return false;
	return true;
}



NS_CC_EASY_END