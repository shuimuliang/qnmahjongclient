#ifndef __CE_TMX_EXT_H__
#define __CE_TMX_EXT_H__

#include "Common/CocosEasyDef.h"
#include "../Utility/GContainer.h"
#include "../Graph/Graph.h"

USING_NS_CC;

NS_CC_EASY_BEGIN

#define TMXPIX_2_PIXEL(pos,heightCount) ccp(pos.x,heightCount-pos.y)
#define TMXTILE_2_PIXEL(pos,tWide,tHeight,heightCount) ccp(pos.x*tWide,(heightCount-pos.y)*tHeight)
#define PIXEL_IN_TMXTILE(pos,tWide,tHeight,heightCount) ccp(pos.x/tileWide,heightCount-pos.y/tHeight)
#define PIXEL_TO_TMXTILE(pos,tWide,tHeight,heightCount) ccp((unsigned int)pos.x/tWide,(unsigned int)heightCount-pos.y/tHeight)

class CTmxMapLayer : public cocos2d::CCLayer
{
public:
	enum emStorageType{NONE=0, ARRAY2D, MAP,};
	CTmxMapLayer();
	~CTmxMapLayer(){unInit();}
	virtual bool init(cocos2d::CCTMXLayer* pTmxLayer,cocos2d::CCTMXTiledMap* pTileMap, emStorageType storeType = NONE);
	virtual void unInit();

public:
	/* pos conversion */
	cocos2d::CCPoint convertToPosInTile(const cocos2d::CCPoint& pos);
	cocos2d::CCPoint convertToTilePos(const cocos2d::CCPoint& pos);
	cocos2d::CCPoint convertToPixelPos(const cocos2d::CCPoint& pos);

	/* whether tile position is in tmx layer */
	bool hasTile(const CCPoint& pos){return (pos.x < m_width && pos.y < m_height && pos.x >=0 && pos.y >=0);}

	/* get tile gid */
	unsigned int tileGIDAt(const cocos2d::CCPoint& tileCoordinate);

	/* get object in position */
	void* getMapObj(const cocos2d::CCPoint& pos);

	/* add object in position */
	bool addMapObj(void* pObj,const cocos2d::CCPoint& pos);

	/* delete object in position */
	void* delMapObj(const cocos2d::CCPoint& pos);

	/* get count of object in container */
	unsigned int size(){return m_list.size();}

private:
	unsigned int getIndex(const cocos2d::CCPoint& pos){return pos.x + pos.y * m_width;}

	// property
protected:
	emStorageType m_storeType;
	unsigned int m_width;
	unsigned int m_height;

	void*** m_pStorage;
	std::map<unsigned int,void*> m_list;

	// quotes
protected:
	cocos2d::CCTMXTiledMap* m_pTileMap;
	cocos2d::CCTMXLayer* m_pTmxLayer;
};

class CTmxMapping : public CCObject
{
public:
	class CTmxMapObj
	{
	public:
		CTmxMapObj(){m_ID = 0; m_Flag = 0xffffffff; m_pTmxMap = NULL;}
	private:
		bool setTiles(std::vector<CCPoint>& tiles,const CCPoint& center);
		void clear(){m_ID = 0; m_OccupyTiles.clear(); m_pTmxMap = NULL;}
	private:
		CC_SYNTHESIZE(unsigned int,m_ID,MapObjID);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<CCPoint>,m_OccupyTiles,OccupyTiles);
		CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCPoint,m_CenterTile,CenterTile);
		CC_SYNTHESIZE(unsigned int,m_Flag,Flag);
		CC_SYNTHESIZE_READONLY(CTmxMapping*,m_pTmxMap,TmxMap);
		friend class CTmxMapping;
	};

public:
#define MAP_TILE_PIX_FUNC(m_CellWide,m_CellHigh,m_HeightCount)\
	CCPoint PixelToTile(const CCPoint& pos) const {return ccp((int)(pos.x/m_CellWide),(int)(pos.y/m_CellHigh));} \
	CCPoint TileToPixel(const CCPoint& pos) const {return ccp(pos.x*m_CellWide,pos.y*m_CellHigh);} \
	CCPoint TileToPixel(const CCPoint& pos,const CCPoint& offset) const {return ccp((pos.x+offset.x)*m_CellWide,(pos.y+offset.y)*m_CellHigh);} \
	CCPoint PixelInTile(const CCPoint& pos) const {return ccp(pos.x/m_CellWide,pos.y/m_CellHigh);} \
	CCPoint TileCenterToPixel(const CCPoint& pos){return TileToPixel(pos,ccp(0.5,0.5));} \
	CCPoint TileTopToPixel(const CCPoint& pos){return TileToPixel(pos,ccp(0.5,1));} \
	CCPoint TileBottomToPixel(const CCPoint& pos){return TileToPixel(pos,ccp(0.5,0));} \
	CCPoint TmxTileToThisTile(const CCPoint& tmx){return ccp( tmx.x, m_HeightCount-tmx.y-1 ); } \
	
public:
	CTmxMapping();
	static CTmxMapping* create(CCSize map, CCSize cell);
	bool init(CCSize map, CCSize cell);

public:
	// base function
	bool hasTile(const CCPoint& tile){return (tile.x < m_WideCount && tile.y < m_HeightCount && tile.x >=0 && tile.y >=0);}
	bool isBoundTile(const CCPoint& tile );
	CTmxMapObj* getObj(const CCPoint& tile){return !hasTile(tile) ? NULL : m_Map[tile.x][tile.y];}
	void searchNodeObj(std::vector<CTmxMapObj*>&list,const CCPoint& tile,unsigned int radius = 0,unsigned int flag = 0xffffffff);
	

	/* pos conversion */
	MAP_TILE_PIX_FUNC(m_CellWide,m_CellHeight,m_HeightCount);

public:
	CGraph* parseGraph(CCDictionary* pDict,const char* center = "center");
	virtual void calcOccupyFull(const CCRect& rect,std::vector<CCPoint>& mTiles);
	virtual void calcOccupyByCorrect(const CCRect& rect,std::vector<CCPoint>& mTiles);
	virtual void calcOccupyByHeight(const CCRect& rect,std::vector<CCPoint>& mTiles);
	virtual void calcOccupyByCell(const CCRect& rect,CCPoint& tile);

public:
	virtual void dragToMap(CTmxMapObj* pObj,std::vector<CCPoint>& tiles,const CCPoint& center);
	virtual void dragFromMap(CTmxMapObj* pObj);
	virtual bool moveTo(CTmxMapObj* pObj,const CCPoint& des);
	virtual bool tryMove(CTmxMapObj* pObj,const CCPoint& desTile,std::vector<CTmxMapObj*>& list);
	std::map<unsigned long long,CTmxMapObj*>& getObjList(){return m_MapObjList;}

protected:
	unsigned long long SNOGen(){assert(m_SerialNo < 0xffffffff); return ++m_SerialNo;}


private:
	CC_SYNTHESIZE(unsigned int,m_WideCount,WideCount); CC_SYNTHESIZE(unsigned int,m_HeightCount,HeightCount);
	CC_SYNTHESIZE(unsigned int,m_CellWide,CellWide); CC_SYNTHESIZE(unsigned int,m_CellHeight,CellHeight);
	CC_SYNTHESIZE(unsigned int,m_Width,Width); CC_SYNTHESIZE(unsigned int,m_Height,Height);
	CVector2D<CTmxMapObj*> m_Map;
	std::map<unsigned long long,CTmxMapObj*> m_MapObjList;
	unsigned long long m_SerialNo;
};


/* recast to check collide in tmx layer
@param[out]	list		the list of collide points
@param[in]	st,ed		start and end points of line
@param[in]	collideEnd	if collideEnd = true then there are only zero or one element in list
@param[out]	return		true:collide, false:not collide
@param[in]	f,pClass	see Geometry2D g2LineCover
*/
extern bool TileRaycast(std::vector<CCPoint>& list,const CCPoint& st,const CCPoint& ed,bool order,unsigned char (*f)(const CCPoint& pos,void* p),void* pData);



NS_CC_EASY_END


#endif