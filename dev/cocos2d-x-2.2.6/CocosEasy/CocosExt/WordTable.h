#ifndef  __CE_WORD_TABLE_H__
#define  __CE_WORD_TABLE_H__
#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN

class CERowWidget : public UIWidget
{
public:
	CERowWidget();
	static CERowWidget* create(float columnSpacing = 0);
	virtual void addChild(CCNode* child, int zOrder, int tag);
	virtual void setSize(const CCSize &size);

protected:
	virtual bool init(float columnSpacing = 0);

protected:
	CC_SYNTHESIZE(float, m_columnSpacing, ColumnSpacing);
};

class CEWordTable : public UIWidget
{
protected:
	

public:
	CEWordTable();
	~CEWordTable();
	static CEWordTable* create();


public:
	virtual void addChild(CCNode* child, int zOrder, int tag);
	virtual void setSize(const CCSize &size);
	
protected:
	//virtual void onSizeChanged();

public:
	void clear();
	const CCSize& getLastLineSize() const;
	virtual CERowWidget* newLine();
	int getSumOfLine();
protected:
	CERowWidget* createRowWidget();

protected: 
	std::vector< CERowWidget* > m_container;

private:
	CC_SYNTHESIZE(float,m_maxWidth,MaxWidth);
	CC_SYNTHESIZE(CCTextAlignment,m_hAlignment,TextHorizontalAlignment);
	CC_SYNTHESIZE(float, m_lineSpacing, LineSpacing);
	CC_SYNTHESIZE(float, m_columnSpacing, ColumnSpacing);

};


NS_CC_EASY_END
#endif //__CE_RICH_TEXT_EXT_H__
