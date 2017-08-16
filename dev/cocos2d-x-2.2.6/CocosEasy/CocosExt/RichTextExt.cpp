#include "RichTextExt.h"
#include "WStrExt.h"
#include "WordTable.h"

NS_CC_EASY_BEGIN
//
const char* TAG_PARA	= "p";
const char* TAG_FONT	= "font";
const char* TAG_SIZE	= "size";
const char* TAG_COLOR	= "color";
const char* TAG_IMAGE   = "img";
const char* TAG_SRC		= "src";
const char* TAG_SCALE   = "scale";

//define default
const string	DEFAULT_COLOR		= "#ffffff";
int getHex( char c )
{
	//if <10
	int temp = c - '0';
	if(temp<10&&temp>-1){
		return temp;
	}
	temp = c - 'A';
	if(temp<6&&temp>-1){
		return temp+10;
	}
	temp = c-'a';
	if(temp<6&&temp>-1){
		return  temp+10;
	}
	return -1;
}	

ccColor3B transFromString( const string& cStr )
{
	//"#00ff00"
	const int NUMBER_OF_COLOR = 3;
	if(cStr.size()<NUMBER_OF_COLOR*2+1) return ccBLUE;	
	int result[NUMBER_OF_COLOR];
	for(int i =0 ;i<NUMBER_OF_COLOR;++i)
	{
		char tempC = cStr.at(i);
		result[i]=getHex(cStr.at(2*i+1))*16+getHex(cStr.at(2*i+2));
	}
	return ccc3(result[0],result[1],result[2]);
}

RichTextExt * RichTextExt::create(float width,string fontName)
{
	RichTextExt* widget = new RichTextExt();
	if (widget && widget->init(width,fontName))
	{
		widget->autorelease();
		widget->setAnchorPoint(ccp(0,1));
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return NULL;
}

bool RichTextExt::init(float width,string fontName)
{
	if (Widget::init())
	{
		//init
		m_viewWidth = width;
		m_fontName = fontName;
		initContainer();
		//reset
		reset();
		return true;
	}
	return false;
}

void RichTextExt::initContainer()
{
	m_pContainer = CEWordTable::create();
	m_pContainer->retain();
	addChild(m_pContainer);
	m_pContainer->setAnchorPoint(CCPointZero);
	m_pContainer->setMaxWidth(m_viewWidth);
}

void RichTextExt::reset()
{
	m_curLineHeight = 0.0f;
	m_curFontSize = m_defaultFontSize;
	m_curFontColor = ccWHITE;
	m_curFontName = m_fontName;
	m_curViewSize.setSize(0,0);
	if(m_pContainer){
		m_pContainer->clear();
	}
}

RichTextExt::RichTextExt()
	:m_pContainer(NULL)
	,m_curScale(1.0f)
	,m_defaultFontSize(20)
{

}

RichTextExt::~RichTextExt()
{
	clearVecRichTextData();
	CC_SAFE_RELEASE(m_pContainer);
}

void RichTextExt::setText(const string& str)
{
	reset();
	clearVecRichTextData();
	m_strText = str;
	if(str.empty())return;
	analyze(str);
	combineAllText();
}

void RichTextExt::analyze(const string& str)
{
	//if not xml 
	if(str.at(0)!='<')
	{
		showStrForError();
		return;
	}
	//读取xml解析
	tinyxml2::XMLDocument *pDocument = new tinyxml2::XMLDocument;
	tinyxml2::XMLError result = pDocument->Parse(str.c_str(),str.size());
	if(result != tinyxml2::XML_SUCCESS)
	{
		CCLOG("[RichTextExt::analyze]:xml parse error");
		pDocument->PrintError();
		showStrForError();
		return;
	}
	//分段解析
	tinyxml2::XMLElement *pSinglePara = pDocument->FirstChildElement(TAG_PARA);
	for(;pSinglePara!=0;pSinglePara = pSinglePara->NextSiblingElement(TAG_PARA))
	{
		analysePara(pSinglePara);
	}
	delete pDocument;
}

void RichTextExt::showStrForError()
{
	m_vecRichTextData.push_back(new SingleRichTextData(eParaType , "" , -1));
	addTextTypeRichTextData(m_strText.c_str());
}

void RichTextExt::analysePara(tinyxml2::XMLElement *pElementPara)
{
	m_vecRichTextData.push_back(new SingleRichTextData(eParaType,"",-1));

	addTextTypeRichTextData(pElementPara->GetText());

	tinyxml2::XMLElement *pSingleEle = pElementPara->FirstChildElement();
	for(;pSingleEle!=NULL;pSingleEle = pSingleEle->NextSiblingElement())
	{
		analyseSingleEleInPara(pSingleEle);
	}
}

void RichTextExt::analyseSingleEleInPara(tinyxml2::XMLElement *pSingleEleInPara)
{
	const string& tag = pSingleEleInPara->Name();
	if(TAG_FONT == tag){
		analyseFontType(pSingleEleInPara);
	}else if(TAG_IMAGE == tag){
		analyseImgType(pSingleEleInPara);
	}else{
		CCLOG("[RichTextExt::analyseSingleEleInPara]:tag:(%s ) is not support",tag.c_str());
	}
}

void RichTextExt::analyseFontType(tinyxml2::XMLElement* pFontEle)
{
	if(!pFontEle)return;
	//analyse fontSize
	addFontSizeTypeRichTextData(pFontEle->IntAttribute(TAG_SIZE));
	//analyse fontColor
	addFontColorTypeRichTextData(pFontEle->Attribute(TAG_COLOR));
	//analyse text
	addTextTypeRichTextData(pFontEle->GetText());
	//addDefault
	addFontSizeTypeRichTextData(m_defaultFontSize);
	addFontColorTypeRichTextData(DEFAULT_COLOR.c_str());
}

void RichTextExt::analyseImgType(tinyxml2::XMLElement* pImgEle)
{
	if(!pImgEle)return;
	float scale = 1.0f;
	if(pImgEle->Attribute(TAG_SCALE)){
		scale = pImgEle->DoubleAttribute(TAG_SCALE);
		m_vecRichTextData.push_back(new SingleRichTextData(eScaleType,"",0,scale));
	}
	string imgSrc = pImgEle->Attribute(TAG_SRC);
	if(imgSrc.empty())return;
	m_vecRichTextData.push_back(new SingleRichTextData(eImgType,imgSrc,0));
}

void RichTextExt::clearVecRichTextData()
{
	for(RichTextDataVecItr itr = m_vecRichTextData.begin();itr!=m_vecRichTextData.end();++itr){
		delete (*itr);
	}
	m_vecRichTextData.clear();
}

void RichTextExt::addFontSizeTypeRichTextData(int fontSize)
{
	if(fontSize>0){
		m_vecRichTextData.push_back(new SingleRichTextData(eFontSizeType,"",fontSize));
	}
}

void RichTextExt::addFontColorTypeRichTextData(const char *strColor)
{
	if(strColor)
	{
		m_vecRichTextData.push_back(new SingleRichTextData(eFontColorType,strColor,-1));
	}
}
	
void RichTextExt::addTextTypeRichTextData(const char *pText)
{
	if(pText)
	{
		m_vecRichTextData.push_back(new SingleRichTextData(eTextType,pText,-1));
	}
}

void RichTextExt::combineAllText()
{
	CCLabelTTF *pLabelCur = CCLabelTTF::create("A", m_fontName.c_str(), m_defaultFontSize);
	m_curLineHeight = pLabelCur->getContentSize().height;
	for(RichTextDataVecItr itr = m_vecRichTextData.begin();itr != m_vecRichTextData.end();++itr)\
	{
		switch((*itr)->m_type){
		case eParaType:
			{
				m_pContainer->newLine();
				m_curViewSize.height+=m_curLineHeight;
			}
			break;
		case eFontSizeType:
			m_curFontSize = (*itr)->m_iMsg;
			break;
		case eFontColorType:
			m_curFontColor =transFromString((*itr)->m_strMsg);
			break;
		case eTextType:
			{
				addText((*itr)->m_strMsg);
			}
			break;
		case eImgType:
			{
				addImg((*itr)->m_strMsg);
				m_curScale = 1.0f;
			}
			break;
		case eScaleType:
			{
				m_curScale = (*itr)->m_fMsg;
			}
			break;
		} 
                      	}
	m_curViewSize = m_pContainer->getSize();
	m_pContainer->setPositionY(m_curViewSize.height);
	setContentSize(CCSizeMake(m_viewWidth,m_curViewSize.height));
}

void RichTextExt::addText(const string &text)
{
	if(text.empty())return;

	Label* textRenderer = createLabel(text);
	setCurLineHeight(textRenderer->getContentSize().height);
	//cal current texRendererWidth
	float textRendererWidth = textRenderer->getContentSize().width;
	//计算 剩余多的宽度
	float _leftSpaceWidth = m_pContainer->getLastLineSize().width+textRendererWidth - m_viewWidth;
	if (_leftSpaceWidth > 0.0f)
	{
		//如果有剩余宽度
		setViewSizeWidth(m_viewWidth);
		//转成宽字符
		WStringExt curText = text;
		int stringLength = curText.length();
		//计算最短字符长度
		int curLength = 1;
		WStringExt curWords;
		for(; curLength<stringLength;++curLength)
		{
			curText.substr(0,curLength,curWords);
			char* pCharTemp = new char[curWords.length()*3+3];
			curWords.getUtf8(pCharTemp);
			
			Label* curRenderer = createLabel(pCharTemp);
			delete[] pCharTemp;
			bool isLargerThanViewWidth = m_pContainer->getLastLineSize().width + curRenderer->getContentSize().width >m_viewWidth;
			if(isLargerThanViewWidth){
				break;
			}
		}

		//如果只有一个字母就超框了，则直接换行
		if(curLength <1){
			return;
		}
		else if(curLength == 1){
			addNewLine();
			addText(text);
			return;
		}

		curText.substr(0,curLength-1,curWords);
		char* pCharTemp = new char[curWords.length()*3+3];
		curWords.getUtf8(pCharTemp);
		Label* curRenderer = createLabel(pCharTemp);
		delete[] pCharTemp;

		setCurLineHeight(curRenderer->getContentSize().height);
		m_pContainer->addChild(curRenderer,0,0);

		if(curLength>stringLength)
			return;
		addNewLine();

		WStringExt cutWords;
		curText.substr(curLength-1, curText.length(),cutWords);
		pCharTemp = new char[cutWords.length()*3+3];
		cutWords.getUtf8(pCharTemp);
		string pCharStr(pCharTemp);
		delete[] pCharTemp;
		addText(pCharStr);
	}
	else
	{
		m_pContainer->addChild(textRenderer,0,0);
	}
}

void RichTextExt::addImg(const string &imgSrc)
{
	UIImageView *pImg = UIImageView::create();
	pImg->loadTexture(imgSrc.c_str());
	pImg->setScale(m_curScale);
	//将图片加上
	m_pContainer->addChild(pImg,0,0);
}

Label * RichTextExt::createLabel(const string &text)
{
	Label *result = Label::create();
	result->setText(text);
	result->setAnchorPoint(CCPointZero);
	result->setFontSize(m_curFontSize);
	result->setFontName(m_fontName);
	result->setColor(m_curFontColor);
	return result;
}

void RichTextExt::addNewLine()
{
	m_curViewSize.height+=m_curLineHeight;
	m_curLineHeight = 0;
	m_pContainer->newLine();
}

void RichTextExt::setCurLineHeight(float height)
{
	if(m_curLineHeight<height){
		m_curLineHeight = height;
	}
}

void RichTextExt::updatePosition()
{

}

void RichTextExt::setViewSizeWidth(float width)
{
	m_curViewSize.width = m_curViewSize.width>width?m_curViewSize.width:width;
}

void RichTextExt::setTextHorizontalAlignment(CCTextAlignment alignment)
{
	m_pContainer->setTextHorizontalAlignment(alignment);
}

void RichTextExt::setLineSpacing(float lineSpacing)
{
	m_pContainer->setLineSpacing(lineSpacing);
}

void RichTextExt::setColumnSpacing(float columnSpacing)
{
	m_pContainer->setColumnSpacing(columnSpacing);
}

cocos2d::CCTextAlignment RichTextExt::transFerToAlignment(int alignment)
{
	CCTextAlignment result = kCCTextAlignmentCenter;
	switch(alignment){
	case 0:
		result = kCCTextAlignmentLeft;
		break;
	case 1:
		result = kCCTextAlignmentCenter;
		break;
	case 2:
		result = kCCTextAlignmentRight;
		break;
	}
	return result;
}

void RichTextExt::setDefaultFontSize( float size )
{
	m_defaultFontSize = size;
}

void RichTextExt::setDefaultFontName( string fontName )
{
	m_fontName = fontName;
}

int RichTextExt::getSumOfLine()
{
	return m_pContainer->getSumOfLine(); 
}

float RichTextExt::getCurViewWidth()
{
	return m_curViewSize.width;
}

SingleRichTextData::SingleRichTextData(const E_RICHTEXT_LABEL_T& type ,const string& strMsg ,const int& intMsg)
	:m_type(type),
	 m_strMsg(strMsg),
	 m_iMsg(intMsg)
{
}

SingleRichTextData::SingleRichTextData(const E_RICHTEXT_LABEL_T& type ,const string& strMsg ,const int& intMsg,const float& floatMsg)
	:m_type(type),
	m_strMsg(strMsg),
	m_iMsg(intMsg),
	m_fMsg(floatMsg)
{

}

void SingleRichTextData::setFloatMsg(float msg)
{
	m_fMsg = msg;
}

namespace RichTextTool{

string createColorSec(const string &strContent , const string &strColor)
{
	return "<font color=\"" + strColor + "\">"+strContent + "</font>";
}

}


NS_CC_EASY_END