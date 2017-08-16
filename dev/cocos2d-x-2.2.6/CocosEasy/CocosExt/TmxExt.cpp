#include "TmxExt.h"
#include "../Graph/Geometry2D.h"

USING_NS_CC_EASY;

NS_CC_EASY_BEGIN

CTmxMapLayer::CTmxMapLayer()
: m_pTileMap(NULL)
, m_pTmxLayer(NULL)
, m_storeType(NONE)
, m_pStorage(NULL)
, m_width(0)
, m_height(0)
{

}

bool CTmxMapLayer::init( cocos2d::CCTMXLayer* pTmxLayer,cocos2d::CCTMXTiledMap* pTileMap ,emStorageType storeType)
{
	if(!cocos2d::CCLayer::init()) return false;
	if(!pTmxLayer || !pTileMap) return false;

	m_pTileMap = pTileMap;
	m_pTmxLayer = pTmxLayer;
	m_storeType = storeType;
	cocos2d::CCSize size = m_pTmxLayer->getLayerSize();
	m_width = size.width; m_height = size.height;
	
	if(m_storeType == ARRAY2D)
	{
		m_pStorage = new void**[m_width];
		for (unsigned int i=0; i<m_width; ++i)
		{
			m_pStorage[i] = new void*[m_height];
			memset(m_pStorage[i], 0, m_height*sizeof(void*));
		}
	}
	return true;
}

void CTmxMapLayer::unInit()
{
	if(m_storeType == ARRAY2D) delete[] m_pStorage;
}

unsigned int CTmxMapLayer::tileGIDAt( const cocos2d::CCPoint& tileCoordinate )
{
	//if(!hasTile(tileCoordinate)) return 0;
	return m_pTmxLayer->tileGIDAt(tileCoordinate);
}

cocos2d::CCPoint CTmxMapLayer::convertToPosInTile( const cocos2d::CCPoint& pos )
{
	cocos2d::CCSize tile = m_pTmxLayer->getMapTileSize();
	float x = pos.x  / tile.width;
	float y = m_height - pos.y / tile.height;
	return cocos2d::CCPoint(x,y);
}


cocos2d::CCPoint CTmxMapLayer::convertToTilePos( const cocos2d::CCPoint& pos )
{
	cocos2d::CCSize tile = m_pTmxLayer->getMapTileSize();
	unsigned int x = pos.x  / tile.width;
	unsigned int y = m_height - pos.y / tile.height;
	return cocos2d::CCPoint(x,y);
}

cocos2d::CCPoint CTmxMapLayer::convertToPixelPos( const cocos2d::CCPoint& pos )
{
	cocos2d::CCSize tile = m_pTmxLayer->getMapTileSize();
	float x = pos.x * tile.width;
	float y = (m_height - pos.y) * tile.height;
	return cocos2d::CCPoint(x,y);
}

void* CTmxMapLayer::getMapObj( const cocos2d::CCPoint& pos )
{
	switch (m_storeType)
	{
	case cocoseasy::CTmxMapLayer::ARRAY2D:
		{
			unsigned int x = pos.x;
			unsigned int y = pos.y;
			if(x >= m_width || y >= m_height) return NULL;
			return (m_pStorage[x])[y];
		}
		break;
	case cocoseasy::CTmxMapLayer::MAP:
		{
			unsigned int gid = tileGIDAt(pos);
			if(!gid) return false;
			std::map<unsigned int,void*>::iterator it = m_list.find(getIndex(pos));
			if(it == m_list.end()) return NULL;
			return it->second;
		}
		break;
	default:
		break;
	}
	return NULL;
}

bool CTmxMapLayer::addMapObj( void* pObj,const cocos2d::CCPoint& pos )
{
	if(getMapObj(pos)) return true;

	unsigned int gid = tileGIDAt(pos);
	if(!gid) return false;

	switch (m_storeType)
	{
	case cocoseasy::CTmxMapLayer::ARRAY2D:
		{
			unsigned int x = pos.x;
			unsigned int y = pos.y;
			assert(x >= m_width || y >= m_height);
			(m_pStorage[x])[y] = pObj;
		}
	case cocoseasy::CTmxMapLayer::MAP:
		{
			m_list[getIndex(pos)] = pObj;
		}
		break;
	default:
		return false;
		break;
	}
	return true;
}

void* CTmxMapLayer::delMapObj( const cocos2d::CCPoint& pos )
{
	unsigned int gid = tileGIDAt(pos);
	if(!gid) return NULL;
	void* pObj = NULL;

	switch (m_storeType)
	{
	case cocoseasy::CTmxMapLayer::ARRAY2D:
		{
			unsigned int x = pos.x;
			unsigned int y = pos.y;
			if(x >= m_width || y >= m_height) return NULL;
			(m_pStorage[x])[y] = NULL;
		}
	case cocoseasy::CTmxMapLayer::MAP:
		{
			std::map<unsigned int,void*>::iterator it = m_list.find(getIndex(pos));
			if(it == m_list.end()) break;
			pObj = it->second;
			m_list.erase(it);
		}
		break;
	default:
		break;
	}
	return pObj;
}


bool CTmxMapping::CTmxMapObj::setTiles( std::vector<CCPoint>& tiles,const CCPoint& centerTile )
{
	m_OccupyTiles = tiles;
	m_CenterTile = centerTile;
	return true;
}



CTmxMapping::CTmxMapping()
: m_Width(0)
, m_Height(0)
, m_CellWide(0)
, m_CellHeight(0)
, m_WideCount(0)
, m_HeightCount(0)
, m_SerialNo(0)
{

}


CTmxMapping* CTmxMapping::create( CCSize map, CCSize cell )
{
	CTmxMapping* pScene = new CTmxMapping();                                
	if(pScene && pScene->init(map,cell))       
	{                                                           
		pScene->autorelease();                                  
		return pScene;                                          
	}                                                           
	CC_SAFE_DELETE(pScene);                                     
	return NULL;               
}


bool CTmxMapping::init( CCSize map, CCSize cell )
{
	m_WideCount = map.width; m_HeightCount = map.height;
	m_CellWide = cell.width; m_CellHeight = cell.height;
	m_Width = m_WideCount * m_CellWide; m_Height = m_HeightCount * m_CellHeight;
	m_Map.reserve(m_WideCount,m_HeightCount,NULL);
	return true;
}




bool CTmxMapping::isBoundTile( const CCPoint& tile )
{
	unsigned int x = (unsigned int)tile.x;
	unsigned int y = (unsigned int)tile.y;
	if(x == 0 || x == m_WideCount-1 || y == 0 || y == m_HeightCount-1) return true;
	return false;
}


void CTmxMapping::searchNodeObj(std::vector<CTmxMapObj*>& list, const CCPoint& tile,unsigned int radius /*= 0*/,unsigned int flag /*= 0xffffffff*/ )
{
	unsigned int w = (radius + 1)*2 - 1;
	list.reserve(w*w);

	CCRect rect;
	rect.origin = CCPoint(tile.x - radius, tile.y - radius);
	rect.size = CCPoint(w,w);
	for (int i=rect.getMinX(); i<rect.getMaxX(); ++i)
	{
		for (int j=rect.getMinY(); j<rect.getMaxY(); ++j)
		{
			CCPoint tilePos(i,j);
			CTmxMapObj* pObj = static_cast<CTmxMapObj*>(getObj(tilePos));
			if(!pObj) continue;
			if(!pObj || !(pObj->getFlag()&flag)) continue;;
			list.push_back(pObj);
		}
	}
}


void CTmxMapping::dragToMap( CTmxMapObj* pObj,std::vector<CCPoint>& tiles,const CCPoint& centerTile)
{
	if(!pObj || pObj->m_ID){assert(false); return;}

	// generate id and save object
	pObj->m_ID = SNOGen();
	std::map<unsigned long long,CTmxMapObj*>::iterator it = m_MapObjList.find(pObj->m_ID);
	while (it != m_MapObjList.end())
	{
		assert(false);
		pObj->m_ID = SNOGen();
		it = m_MapObjList.find(pObj->m_ID);
	}
	m_MapObjList[pObj->m_ID] = pObj;

	// add object to map
	if(!pObj->setTiles(tiles,centerTile)) {assert(false); return;}
	for (unsigned int i=0; i<pObj->m_OccupyTiles.size(); ++i)
	{
		const CCPoint& tile = pObj->m_OccupyTiles[i];
		if(m_Map[tile.x][tile.y]){assert(false); return;}
		m_Map[tile.x][tile.y] = pObj;
	}
	pObj->m_pTmxMap = this;
}

void CTmxMapping::dragFromMap( CTmxMapObj* pObj )
{
	// remove and clear id 
	if(!pObj || !pObj->m_ID){assert(false); return;}
	std::map<unsigned long long,CTmxMapObj*>::iterator it = m_MapObjList.find(pObj->m_ID);
	if(it != m_MapObjList.end()) m_MapObjList.erase(it);

	// remove from map
	for (unsigned int i=0; i<pObj->m_OccupyTiles.size(); ++i)
	{
		const CCPoint& tile = pObj->m_OccupyTiles[i];
		if(!m_Map[tile.x][tile.y] || m_Map[tile.x][tile.y] != pObj){assert(false); return;}
		m_Map[tile.x][tile.y] = NULL;
	}
	pObj->clear();
}



bool CTmxMapping::moveTo( CTmxMapObj* pObj,const CCPoint& desTile )
{
	if(!pObj || pObj->getTmxMap() != this) {assert(false); return false;}
	std::vector<CTmxMapObj*> list;
	if(!tryMove(pObj,desTile,list)) return false;
	CCPoint vec = desTile - pObj->m_CenterTile;
	if(abs((int)vec.x) < 1 && abs((int)vec.y) < 1) return true;

	// clear map cells
	for (unsigned int i=0; i<pObj->m_OccupyTiles.size(); ++i)
	{
		// erase src tile
		CCPoint& src = pObj->m_OccupyTiles[i]; 
		if(!m_Map[src.x][src.y] || m_Map[src.x][src.y] != pObj) {assert( false); return false;}
		m_Map[src.x][src.y] = NULL;
	}

	// occupy map cell
	for (unsigned int i=0; i<pObj->m_OccupyTiles.size(); ++i)
	{
		// set des tile
		CCPoint& src = pObj->m_OccupyTiles[i];
		CCPoint des = src + vec;
		if(m_Map[des.x][des.y]){assert(false); return false;}
		m_Map[des.x][des.y] = pObj;
		src = des;
	}

	pObj->m_CenterTile = desTile;
	return true;
}


bool CTmxMapping::tryMove( CTmxMapObj* pObj,const CCPoint& desTile,std::vector<CTmxMapObj*>& list )
{
	if(!pObj || !hasTile(desTile)) return false;
	bool canMove = true;
	CCPoint vec = desTile - pObj->m_CenterTile;
	if(abs((int)vec.x) < 1 && abs((int)vec.y) < 1) return canMove;
	for (unsigned int i=0; i<pObj->m_OccupyTiles.size(); ++i)
	{
		CCPoint& src = pObj->m_OccupyTiles[i];
		CCPoint des = src + vec;
		CTmxMapObj* pDes = m_Map[des.x][des.y];
		if(!pDes || pDes == pObj) continue;
		canMove = false;
		list.push_back(pDes);
	}
	return canMove;
}



CGraph* CTmxMapping::parseGraph( CCDictionary* pDict,const char* k_center /*= "center"*/ )
{
	if(!pDict) return NULL;

	// parse rect
	CCRect rect;
	rect.origin.x = pDict->valueForKey("x")->intValue();
	rect.origin.y = pDict->valueForKey("y")->intValue();
	rect.size.width = pDict->valueForKey("width")->intValue();
	rect.size.height = pDict->valueForKey("height")->intValue();

	// parse center
	CCPoint center = rect.origin;
	CCArray* arr = (CCArray*)pDict->objectForKey("points");

	// rectangle center
	if(!arr) center = ccp(rect.getMidX(),rect.getMinY());
	
	// self define center
	const CCString* pStrCenter = pDict->valueForKey(k_center);
	if(pStrCenter && pStrCenter->length()){/* todo center = ? */}

	// parse points
	std::vector<CCPoint> points;
	if(arr) // points cover
	{
		bool trans = !g2Vequal(center,rect.origin);
		CCObject* pObj = NULL;
		CCARRAY_FOREACH(arr, pObj)
		{
			CCDictionary* str = (CCDictionary*)pObj;
			CCPoint pt;
			pt.x = str->valueForKey("x")->intValue();
			pt.y = str->valueForKey("y")->intValue();
			if(trans) pt = rect.origin + pt - center;
			points.push_back(pt);
		}
	}

	CGraph* pGraph = NULL;
	switch (points.size())
	{
	case 0: // rectangle
		pGraph = new CRectangle(rect);
		break;
	case 2: // line
		pGraph = new CLine(points[0]+center,points[1]+center);
		break;
	default:
		assert(points.size() > 2);
		pGraph = new CPoly(points);
		break;
	}

	pGraph->setCenter(center);
	return pGraph;
}


void CTmxMapping::calcOccupyFull( const CCRect& rect,std::vector<CCPoint>& mTiles )
{
	CCPoint bmin = PixelToTile(rect.origin); CCPoint bmax = PixelToTile(ccp(rect.getMaxX(),rect.getMaxY()));
	for (int x = bmin.x; x <= (int)bmax.x; ++x)
	{
		if(x < 0 || x > (int)m_WideCount-1) continue;
		for (int y = bmin.y; y <= (int)bmax.y; ++y)
		{
			if(y < 0 || y > (int)m_HeightCount-1) continue;
			mTiles.push_back(ccp(x,y));
		}
	}
}



void CTmxMapping::calcOccupyByCorrect( const CCRect& rect,std::vector<CCPoint>& mTiles )
{
	CCPoint bTileMin(0xffff,0xffff),bTileMax;
	std::vector<CCPoint> list;
	CCPoint bmin = rect.origin; CCPoint bmax = ccp(rect.getMaxX(),rect.getMaxY());
	bTileMin = PixelToTile(bmin); bTileMax = PixelToTile(bmax);
	unsigned int uX = (unsigned int)bmax.x; unsigned int uY = (unsigned int)bmax.y;
	if((float)uX == bmax.x && uX % m_CellWide == 0) --bTileMax.x;
	if((float)uY == bmax.y && uY % m_CellHeight == 0) --bTileMax.y;

	for (unsigned int i = bTileMin.x; i <= bTileMax.x; ++i)
		for (unsigned int j = bTileMin.y; j <= bTileMax.y; ++j)
			mTiles.push_back(ccp(i,j));
}


void CTmxMapping::calcOccupyByHeight( const CCRect& rect,std::vector<CCPoint>& mTiles )
{
	CCPoint bmin = rect.origin; CCPoint bmax = ccp(rect.getMaxX(),rect.getMaxY());
	CCPoint footPos = ccp( (bmin.x + bmax.x)/2.0f , bmin.y );
	CCPoint bottomTile = PixelToTile(footPos);
	float topTileY = bmax.y / m_CellHeight;
	unsigned int uY = (unsigned int)bmax.y;
	if((float)uY == bmax.y && uY % m_CellHeight == 0) --topTileY; assert(topTileY >= bottomTile.y);
	for (unsigned int i = bottomTile.y; i <= topTileY; ++i)
	{
		mTiles.push_back(ccp(bottomTile.x,i));
	}
}

void CTmxMapping::calcOccupyByCell( const CCRect& rect,CCPoint& tile )
{
	CCPoint bmin = rect.origin; CCPoint bmax = ccp(rect.getMaxX(),rect.getMaxY());
	CCPoint footPos = ccp( (bmin.x + bmax.x)/2.0f , bmin.y );
	tile = PixelToTile(footPos);
}




bool TileRaycast(std::vector<CCPoint>& list,const CCPoint& st,const CCPoint& ed,bool order,unsigned char (*f)(const CCPoint& pos,void* p),void* pData)
{
	std::vector<CCPoint> tilelist;
	g2LineCover(tilelist,st,ed,f,pData);
	for (unsigned int i=0; i<tilelist.size(); ++i)
	{
		const CCPoint& tile = tilelist[i];
		CCPoint points[4];
		unsigned int c = g2LineRectCross(st,ed,CCRect(tile.x,tile.y,1,1),0,false,points); assert(c);
		if(!order) // needn't to order
		{
			for (unsigned int i=0; i<c; ++i){list.push_back(points[i]);}
		}
		else      // need to order
		{
			std::vector<float> tmp; tmp.reserve(c+1);
			list.push_back(points[0]);
			tmp.push_back(points[0].getDistance(st));
			for (unsigned int i=1; i<c; ++i)
			{
				bool insert = false;
				float dis = points[i].getDistance(st);
				for (unsigned int j=0; j<list.size(); ++j)
				{
					if(dis > tmp[j]) continue;
					list.insert(list.begin()+j,points[i]);
					tmp.insert(tmp.begin()+j,dis);
					insert = true;
					break;
				}
				if(insert) continue;
				list.push_back(points[i]);
				tmp.push_back(dis);
			}
		}
	}
	return list.size() != 0;
}



NS_CC_EASY_END