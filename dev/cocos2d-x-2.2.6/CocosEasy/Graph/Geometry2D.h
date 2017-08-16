#ifndef __CE_GEOMETRY2D_H__
#define __CE_GEOMETRY2D_H__

#include "Common/CocosEasyDef.h"

USING_NS_CC;

NS_CC_EASY_BEGIN

#ifndef PI
#define PI 3.14159265358979323846f
#endif // !PI

#define angle2Rotate(angle) (-angle * 180.0f / PI)
#define rotate2Angle(rotate) (-rotate * PI / 180.0f)

template<class T> inline void g2Swap(T& a, T& b) { T t = a; a = b; b = t; }
template<class T> inline T g2Min(T a, T b) { return a < b ? a : b; }
template<class T> inline T g2Max(T a, T b) { return a > b ? a : b; }
template<class T> inline T g2Abs(T a) { return a < 0 ? -a : a; }
template<class T> inline T g2Sqr(T a) { return a*a; }
template<class T> inline T g2Clamp(T v, T mn, T mx) { return v < mn ? mn : (v > mx ? mx : v); }
inline float g2Sqrt(float x){return sqrtf(x);}

inline CCPoint g2Vreverse(const CCPoint& v){return CCPoint(-v.x,-v.y);}
inline float g2Vcross(const CCPoint& v1, const CCPoint& v2){return v1.x*v2.y - v2.x*v1.y;}
inline float g2Vdot(const CCPoint& v1, const CCPoint& v2){return v1.x*v2.x + v1.y*v2.y;}
inline CCPoint g2Vmad(const CCPoint& v1, const CCPoint& v2, const float s){return CCPoint(v1.x+v2.x*s, v1.y+v2.y*s);}
inline CCPoint g2Vadd(const CCPoint& v1, const CCPoint& v2){return CCPoint(v1.x+v2.x, v1.y+v2.y);}
inline CCPoint g2Vsub(const CCPoint& v1, const CCPoint& v2){return CCPoint(v1.x-v2.x, v1.y-v2.y);}
inline void g2Vmin(CCPoint& mn, const CCPoint& v){mn.x = g2Min(mn.x,v.x); mn.y = g2Min(mn.y,v.y);}
inline void g2Vmax(CCPoint& mn, const CCPoint& v){mn.x = g2Max(mn.x,v.x); mn.y = g2Max(mn.y,v.y);}
inline float g2VdistSqr(const CCPoint& v1, const CCPoint& v2){float dx = v2.x - v1.x; float dy = v2.y - v1.y; return dx*dx + dy*dy;}
inline float g2Vdist(const CCPoint& v1, const CCPoint& v2){return g2Sqrt(g2VdistSqr(v1,v2));}
inline void g2Vnormalize(CCPoint& v){float d = 1.0f / g2Sqrt(g2Sqr(v.x) + g2Sqr(v.y)); v.x *= d; v.y *= d;}
inline bool g2Vequal(const CCPoint& v1, const CCPoint& v2, float threshold = 0.0f){return (g2Abs(v1.x - v2.x) <= threshold && g2Abs(v1.y - v2.y) <= threshold);}
inline CCPoint g2Vtile(const CCPoint& v){return CCPoint((unsigned int)v.x,(unsigned int)v.y);}

// like a ray, shoot a [dis] length ray from [v] point along [vec] vector, return the destination point 
inline CCPoint g2Vray(const CCPoint& v,const CCPoint& vec,float dis)
{
	float d=vec.getLength(); 
	float r=dis/d; 
	return CCPoint(v.x+vec.x*r, v.y+vec.y*r);
}
// definite proportion and separated points 
inline CCPoint g2Vdpsp(const CCPoint& v1,const CCPoint& v2,float prop){return CCPoint((v1.x+prop*v2.x)/(1+prop),(v1.y+prop*v2.y)/(1+prop));}

/*
@param[out]	pos				cross position when pos is not null
@param[in]	p1,p2			double point of line 1
@param[in]	p3,p4			double point of line 2
*/
bool g2LineCross(const CCPoint &p1,const CCPoint &p2,const CCPoint &p3,const CCPoint &p4,CCPoint* pos = NULL);

/*
@param[out]	pos				cross position when pos is not null
@param[in]	p1				start point of line
@param[in]	p2				end point of line
@param[in]	rect			rectangle including bmin and width height				 					
							3_2
@param[in]	order			| | from 0 to 3, anticlockwise check every edge
							0-1
*/
unsigned int g2LineRectCross(const CCPoint& p1,const CCPoint& p2,const CCRect& rect,unsigned int order = 0, bool collideEnd = false, CCPoint* pos = NULL);



/*
@param[out]	pos				cross position when pos is not null
@param[in]	p1				start point of line
@param[in]	p2				end point of line
@param[in]	poly			polygon
@param[in]	count			points count
@param[in]	order			check index from pt
*/
unsigned int g2LinePolyCross(const CCPoint& p1,const CCPoint& p2,const CCPoint* poly,unsigned int count, unsigned int order, bool collideEnd = false, CCPoint* pos = NULL);



/*
@param[in]	src			rect 1
@param[in]	des			rect 2			
*/
bool g2RectCross(const CCRect& src, const CCRect& des);



bool g2PointInRect(const CCPoint& pt, const CCRect& rect);



/*
@param[out]	list			cells be covered by line
@param[in]	st,ed			The double point of line
@param[in]	f				function for record and control progress. 0 bit: continue/exit, 1 bit: record/not record
*/ 
void g2LineCover(std::vector<CCPoint>& list,const CCPoint& st,const CCPoint& ed,unsigned char (*f)(const CCPoint& pos,void* p) = NULL,void* pClass = NULL);


NS_CC_EASY_END


#endif