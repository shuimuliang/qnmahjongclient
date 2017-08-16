#ifndef  __CE_UI_TAB_VIEW_EXT_H__
#define  __CE_UI_TAB_VIEW_EXT_H__

#include "Common/CocosEasyDef.h"



NS_CC_EASY_BEGIN
class CUITabViewExt;

class IClickEventTabView{
public:
	virtual void onClickTabId(unsigned int id)=0;
};

class CUITabViewDataExt
{
public:
	CUITabViewDataExt(Button* btn , Widget* view);
	~CUITabViewDataExt();

	inline Button* getBtn(){return m_pBtn;}
	inline Widget* getView(){return m_pView;}

	void setBtnZOrder(int zOrder);
	void chooseAndSetOrder(int zOrder);
	void unChooseAndSetOrder();
private:
	Button*				m_pBtn;
	Widget*				m_pView;
	int					m_btnOrder;
};

class CUITabViewExt:public CCNode
{
public:
	static CUITabViewExt *create();
	virtual ~CUITabViewExt();

	void addView(Button* btn, Widget* view);
	//return false if there is no item with this id
	bool choose(unsigned int id , bool isByBtn = false);

	void resetToFirstView();
	
	void registClickEvent(IClickEventTabView *pEvent);

	void registScriptClickEvent(int handler);

	void unRegistClickEvent();
	
	Button* getButton(unsigned int id);
	
	Widget* getView(unsigned int id);

	int getCurTabId();
	
	Button* getCurButton();

	Widget* getCurView();

	//event Function
	void touchUpInsideAction(CCObject *pSender, TouchEventType type);
private:
	typedef std::vector<CUITabViewDataExt*>	VecTabViewData;
	VecTabViewData	m_tabViewData;
	int				m_maxBtnZOrder;
	IClickEventTabView *m_pClickEventView;
	int				m_handler;
	int				m_curTabId;
	CUITabViewExt();
	void clearTabViewData();
	void onClickEvent(int id,bool isByBtn = false);
};

NS_CC_EASY_END
#endif //__UI_TAB_VIEW_EXT_H__

