#ifndef __UI_CIRCLE_VIEW_BASE_EXT_H__
#define __UI_CIRCLE_VIEW_BASE_EXT_H__

#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN

class UICircleViewBaseExt : public Layout
{
public:
	UICircleViewBaseExt();
	void turnRight();
	void turnLeft();

	virtual ~UICircleViewBaseExt();
	virtual bool init(float width,float height);
	virtual void doSpecialEffect() = 0;
	virtual void doCircleEffect(){};
	virtual void doWhenStopClose(){}
	//override
	virtual bool onTouchBegan(CCTouch *touch, CCEvent *unusedEvent);
	virtual void onTouchMoved(CCTouch *touch, CCEvent *unusedEvent);
	virtual void onTouchEnded(CCTouch *touch, CCEvent *unusedEvent);
	virtual void onTouchCancelled(CCTouch *touch, CCEvent *unusedEvent);

	virtual void checkChildInfo(int handleState,Widget* sender,const CCPoint &touchPoint);

	void handlePressLogic(const CCPoint &touchPoint);
	void handleMoveLogic(const CCPoint &touchPoint);
	void handleReleaseLogic(const CCPoint &touchPoint);
	

	virtual void setAnchorPoint(const CCPoint& anchorPoint);
protected:
	enum E_Scroll_Type{eScrollLeftT,eScrollRightT,eScrollNoneT};
	//const
	static const int		SUM_OF_SHOW_CELLS = 3;
	//touch
	CCPoint					m_touchBeganPoint;
	CCPoint					m_touchMovedPoint;
	CCPoint					m_touchEndedPoint;
	CCPoint					m_touchMovingPoint;
	
	std::vector<Widget*>	m_arrCells;
	E_Scroll_Type			m_srollType;
	Widget*					m_pMoveNode;
	float					m_cellDistance;

	unsigned int			m_curCellId;
	unsigned int			m_sumOfLeft;	
	unsigned int			m_sumOfRight;

	bool					m_IsDoStopClose;

	void clearArrCells();
	void setCurCellOffX(float x);
	void optimizeCurCellOffXByCenter();

	Widget *getLastCell();
	Widget *getNextCell();
	int getLastCellId();
	int getNextCellId();
	int getLeftCellIdOfArr();
	int getRightCellIdOfArr();

	bool isFloatEqual(float a, float b);
private:
	//pos
	float					m_doStopCloseTargetX;
	float					m_curCellOffXByCenter;
	bool					m_bePressed;
	E_Scroll_Type calScrollType();
	void scroll(float dt);
	void setAllCellUnvisible();
	void calDoStopCloseTargetX();
	void doStopClose(float dt);
};


NS_CC_EASY_END

#endif // __UI_CIRCLE_VIEW_BASE_EXT_H__
