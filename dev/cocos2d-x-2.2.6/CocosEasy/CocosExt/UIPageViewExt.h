#ifndef __UI_PAGE_VIEW_EXT_H__
#define __UI_PAGE_VIEW_EXT_H__

#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN


class CUIPageViewExt : public PageView
{
public:
	CUIPageViewExt();
	static CUIPageViewExt* create();
	virtual void setLeftBoundary(float bound){_leftBoundary = bound;}
	virtual void setRightBoundary(float bound){_rightBoundary = bound;}
	virtual void enableBoundraySlip(bool onOff);
	virtual void setClippingEnabled(bool onOff){PageView::setClippingEnabled(onOff);}
	virtual void enableCurPageBright(bool onOff);
	virtual void setDimColor(ccColor3B color){m_DimCorlor = color;}
	void addEventListenerPageView(CCObject *target, SEL_PageViewEvent selector);
	void changePage(unsigned int index);
	virtual void handleReleaseLogic(const CCPoint &touchPoint);
	void setScrollCheckFactor(float factor);
protected:
	virtual bool init();
	virtual void onSizeChanged();
	virtual void updateBrightAndDim();

protected: // call back
	void pageTurningCallBack(CCObject* pObj,PageViewEventType);

private:
	void setColorAllChild(UIWidget* pRoot,ccColor3B color);
	
protected:
	bool m_BoundarySlip;
	bool m_CurPageBright;
	ccColor3B m_DimCorlor;
	CCObject* m_pTurningListener;
	SEL_PageViewEvent m_pTurningSelector;
	float m_scrollCheckFactor;
};


NS_CC_EASY_END

#endif // !__SCROLL_VIEW_EXT_H__
