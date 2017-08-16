#ifndef __UI_CIRCLE_VIEW_EXT_H__
#define __UI_CIRCLE_VIEW_EXT_H__

#include "Common/CocosEasyDef.h"
#include "UICircleViewBaseExt.h"

NS_CC_EASY_BEGIN


class UICircleViewExt : public UICircleViewBaseExt
{
public:

	static UICircleViewExt* create(float width,float height);
	virtual ~UICircleViewExt();
	void addCell(Widget* cell);
	void resetCellPosition();

	void setTargetCellScale(float scale);
	void setTargetOffY(float offY);
	float getTargetCellScale();
	float getTargetOffY();

	virtual void doCircleEffect();
	virtual void doSpecialEffect();
private:

	float			m_targetCellScale;
	float			m_targetOffY;

	UICircleViewExt();
};


NS_CC_EASY_END

#endif // __UI_CIRCLE_VIEW_EXT_H__
