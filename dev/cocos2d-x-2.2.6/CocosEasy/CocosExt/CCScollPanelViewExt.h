#ifndef __CE_CCSCOLL_PANEL_VIEW_EXT_H_
#define __CE_CCSCOLL_PANEL_VIEW_EXT_H_

#include "cocos2d.h"
#include "Common/CocosEasyDef.h"
NS_CC_EASY_BEGIN;

	class CCScollPanelViewExt:public Layout
	{
	public:
		~CCScollPanelViewExt();
		
		static CCScollPanelViewExt* create(const char* srcArrow , const char* srcPanel ,int length);
		void resetPanel(int viewH,int containH);
		void resetStart();
		virtual void eventCallbackFunc(CCObject* sender,ScrollviewEventType eventType);
		inline void setStartY(int startY){m_startY = startY;}

	protected:
		//initializes state of widget.
		bool init(const char* srcArrow , const char* srcPanel ,int length);
	private:
		CCScollPanelViewExt();
		int m_length;
		int m_startY;
		float m_ratio;
		float m_arrowLength;
		ImageView *m_pPanel;
		ImageView *m_pArrow;
	};
NS_CC_EASY_END;
#endif //__CE_CCSCOLL_PANEL_VIEW_EXT_H_


