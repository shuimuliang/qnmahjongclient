#ifndef __UI_CHOOSE_PERSON_VIEW_EXT_H__
#define __UI_CHOOSE_PERSON_VIEW_EXT_H__

#include "Common/CocosEasyDef.h"
#include "UICircleViewBaseExt.h"

NS_CC_EASY_BEGIN

class UIChoosePersonViewExt : public UICircleViewBaseExt
{
public:
	static UIChoosePersonViewExt* create(float width,float height,const std::string& backImgSrc);
	UIChoosePersonViewExt();
	virtual ~UIChoosePersonViewExt();

	void addCell(ImageView *pPortrait ,ImageView *pHalfPortrait);	
	void resetAfterAddCell();
	
	void registScriptEvent(int handler);
	//override
	virtual bool init(float width,float height,const std::string& backImgSrc);
	virtual void doSpecialEffect();
	virtual void doCircleEffect();
	virtual void doWhenStopClose();
private:
	enum E_ScrollCondition{eScrollBeginT = 0,eScrollStopT = 1};
	std::vector<Widget *>	m_arrHalfPortrait;
	Widget*					m_pMoveHalf;
	float					m_cellDistanceHalf;
	int						m_eventHandler;
	bool					m_isScrollBegan;
	void clearArrHalfPortrait();
	void onEventScript(E_ScrollCondition condition);
	
};
NS_CC_EASY_END

#endif // __UI_CIRCLE_VIEW_EXT_H__
