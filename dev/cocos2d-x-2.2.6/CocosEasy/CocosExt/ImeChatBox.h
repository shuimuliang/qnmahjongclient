#ifndef __CE_IME_CHAT_BOX_H_
#define __CE_IME_CHAT_BOX_H_

#include "cocos2d.h"
#include "Common/CocosEasyDef.h"
USING_NS_CC;

NS_CC_EASY_BEGIN;

class RichTextExt;
/**
* 1.show ime 
* 2.transfer events 
* 3.text filter
*/
class CEImeChatBox:public Widget , public CCEditBoxDelegate
{
public:
	static CEImeChatBox *create(CCSize size , int maxLength , CCScale9Sprite *pSpr);
	CEImeChatBox();
	~CEImeChatBox();

	void show();
	void hide();

	void registInputScript(int editBoxEditingDidBeginHandle, int editBoxEditingDidEndHandler , int insertTextHandler , int editBoxReturnHandler);

	void setFontColor(ccColor3B color);
	void setFontSize(float fontSize);
	void setFontName(const char *name);
	void setMaxLength(int maxLength);
	string getStrInput();
	void reset();
protected:
	///////////////////////////
	//CCEditBoxDelegate interface
	///////////////////////////
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);

protected:
	virtual bool init(CCSize size , CCScale9Sprite *pSpr);
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float delta);
	
private:
	bool		m_isDebug;
	
	//input script handler
	int			m_editBoxEditingDidBeginHandler; 
	int			m_editBoxEditingDidEndHandler; 
	int			m_insertTextHandler; 
	int			m_editBoxReturnHandler;
		
	// ‰»ÎøÚ
	CCEditBox	*m_pEditBox;
	RichTextExt	*m_pLabelInput;
	RichTextExt	*m_pPlaceholder;

	string		m_strInput;
    string		m_placeholder;
	
	float		m_fBlinkInteval;
	float		m_fViewWidth;

	int			m_maxLength;
	void deleteChar();
	void updateVisible();
	string textFilter(const char *pChar);
	void log(const string& str);
	void executeScriptSimple(int handler);
};
NS_CC_EASY_END;
#endif //__CE_IME_CHAT_BOX_H_


