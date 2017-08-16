#ifndef  __CE_CCSCROLL_VIEW_EXT_H__
#define  __CE_CCSCROLL_VIEW_EXT_H__

#include "Common/CocosEasyDef.h"
//  组合CCScrollView
//	a、加滚动条
//	b、仅上下滚动
//	c、可左右翻页 给回调函数

NS_CC_EASY_BEGIN

class CCScollPanelViewExt;
class CCScrollViewExt;

class IScrollViewEventExt
{
public:
	virtual void onChangePage(bool isLeftChange,CCScrollViewExt* pSender) = 0;
};
class CCScrollViewExt:public ScrollView
{
public:
	static CCScrollViewExt* create(CCSize size);

	static CCScrollViewExt* create(float width, float height);

	virtual bool init(float width, float height);

	//only can add once
	void addScrollPanel(const char* srcArrow , const char* srcPanel ,int length);

	void registerScrollViewEvent(IScrollViewEventExt *pEvent);

	void registerScriptScrollViewEvent(int handler);
	
	void setPageChangable(bool canChange);

	void resetScrollPanel();

	void setScrollPanelPos(float x,float y);

	//overide touchEvent
	virtual void handlePressLogic(const CCPoint &touchPoint);
	virtual void handleMoveLogic(const CCPoint &touchPoint);
	virtual void handleReleaseLogic(const CCPoint &touchPoint);

	virtual void onSizeChanged();
	virtual void addChild(CCNode* child, int zOrder, int tag);

	~CCScrollViewExt();
	void resetContainerPos();
	
	void onEventCallBack();

	virtual void update(float dt);

	virtual void onEnter();
private:
	enum E_CAN_MOVE_T{eCanVerticalMoveT,eCanhorizotalMoveT,eCanNotMoveT};
	CCScollPanelViewExt*	m_pScrollPanel;
	E_CAN_MOVE_T			m_canMoveType;
	IScrollViewEventExt*	m_pScrollViewEventExt;
	int						m_scriptScrollViewHandler;
	bool					m_isPageChangable;
	bool					m_isLeft;
	
	CCScrollViewExt();
	
	E_CAN_MOVE_T calCanMoveType(const CCPoint &moveDis);

	void onEvent(bool isLeft);
	

};
NS_CC_EASY_END
#endif //__CE_CCSCROLL_VIEW_EXT_H__

