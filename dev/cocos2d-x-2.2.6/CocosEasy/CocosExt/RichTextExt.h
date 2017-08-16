#ifndef  __CE_RICH_TEXT_EXT_H__
#define  __CE_RICH_TEXT_EXT_H__
#include "Common/CocosEasyDef.h"


using namespace std;
NS_CC_EASY_BEGIN
//
//1\eg:<p><font size="18" color="red">dasfdsa</font><font size="20" color="green">hello world</font><img scale = "0.5" src=\"arrow.png\"></img></p>
//<p>next para</p>
//2\需要设置宽度
//3\格式必须遵守xml格式
//warning:you can't make Chinese and English words to one para
enum E_RICHTEXT_LABEL_T
{
    eParaType,
    eFontSizeType,
    eFontColorType,
    eTextType,
    eImgType,
    eScaleType,
    eRichtextTypeMax,
};
class SingleRichTextData;
class CEWordTable;

class RichTextExt: public UIWidget
{
public:
    static RichTextExt *create(float width, string fontName);
    virtual ~RichTextExt();
    void setTextHorizontalAlignment(CCTextAlignment alignment);
    CCTextAlignment transFerToAlignment(int alignment);
    void setLineSpacing(float lineSpacing);
    void setColumnSpacing(float columnSpacing);
    void setText(const string& str);
	void setDefaultFontSize(float size);
	void setDefaultFontName(string fontName);
	int getSumOfLine();
	float getCurViewWidth();
protected:
    string          m_strText;
    string          m_fontName;
    float           m_viewWidth;
    //need to reset
    float           m_curLineHeight;
    float           m_curFontSize;
    CCSize          m_curViewSize;
    ccColor3B       m_curFontColor;
    string          m_curFontName;
    //render
    CEWordTable*    m_pContainer;
    float           m_curScale;
    RichTextExt();
    bool init(float width, string fontName);
    void initContainer();
    void reset();
    //analyze
    void analyze(const string& str);
    void analysePara(tinyxml2::XMLElement *pElementPara);
    void analyseSingleEleInPara(tinyxml2::XMLElement *pSingleEleInPara);
    void analyseFontType(tinyxml2::XMLElement* pFontEle);
    void analyseImgType(tinyxml2::XMLElement* pImgEle);
    //combine
    void combineAllText();
    void addText(const string &text);
    void addImg(const string &imgSrc);
    void addNewLine();

    void setCurLineHeight(float height);
    void updatePosition();
    void setViewSizeWidth(float width);
	
private:
	float		m_defaultFontSize;
    typedef vector<SingleRichTextData *> RichTextDataVec;
    typedef RichTextDataVec::iterator RichTextDataVecItr;
    RichTextDataVec m_vecRichTextData;
  
	void showStrForError();
    void clearVecRichTextData();

    void addFontSizeTypeRichTextData(int fontSize);
    void addFontColorTypeRichTextData(const char *strColor);
    void addTextTypeRichTextData(const char *pText);

    Label *createLabel(const string &text);
};

class SingleRichTextData
{
public:
    SingleRichTextData(const E_RICHTEXT_LABEL_T& type , const string& strMsg , const int& intMsg);
    SingleRichTextData(const E_RICHTEXT_LABEL_T& type , const string& strMsg , const int& intMsg, const float& floatMsg);
    void setFloatMsg(float msg);
    E_RICHTEXT_LABEL_T  m_type;
    string              m_strMsg;
    int                 m_iMsg;
    float               m_fMsg;
};

namespace RichTextTool{
	string createColorSec(const string &strContent , const string &strColor);
}

NS_CC_EASY_END
#endif //__CE_RICH_TEXT_EXT_H__

