#include "GUIReaderDataStruWidgetTree.h"
#include "GUIReaderDataStruWidgetFactory.h"

#define READ_JSON_INT(_JSON_,_CONSTANT_)\
	CC_ASSERT(DICTOOL->checkObjectExist_json(_JSON_ , #_CONSTANT_));\
	_CONSTANT_ = DICTOOL->getIntValue_json(_JSON_,#_CONSTANT_,0);

#define READ_JSON_STR(_JSON_,_CONSTANT_)\
	CC_ASSERT(DICTOOL->checkObjectExist_json(_JSON_ , #_CONSTANT_));\
	_CONSTANT_ = DICTOOL->getStringValue_json(_JSON_,#_CONSTANT_,"");

#define READ_JSON_BOOL(_JSON_,_CONSTANT_)\
	CC_ASSERT(DICTOOL->checkObjectExist_json(_JSON_ , #_CONSTANT_));\
	_CONSTANT_ = DICTOOL->getBooleanValue_json(_JSON_,#_CONSTANT_,false);

#define READ_JSON_FLOAT(_JSON_,_CONSTANT_)\
	CC_ASSERT(DICTOOL->checkObjectExist_json(_JSON_ , #_CONSTANT_));\
	_CONSTANT_ = DICTOOL->getFloatValue_json(_JSON_,#_CONSTANT_,0);

bool cocos2d::extension::GUIDataStruWidget::load(const rapidjson::Value &jsonWidget)
{
	
	className = DICTOOL->getStringValue_json(jsonWidget , "classname");
	name = "null";
	const rapidjson::Value &jsonOption = DICTOOL->getSubDictionary_json(jsonWidget , "options");
	bool result = loadOptions(jsonOption);
	if(!result)return result;

	result = loadchildren(jsonWidget);
	return result;
}

bool cocos2d::extension::GUIDataStruWidget::loadOptions(const rapidjson::Value &jsonOption)
{
	READ_JSON_STR(jsonOption,__type);
	READ_JSON_STR(jsonOption ,name);
	READ_JSON_STR(jsonOption ,customProperty);
	READ_JSON_INT(jsonOption ,tag);
	READ_JSON_BOOL(jsonOption,touchAble);
	READ_JSON_BOOL(jsonOption,useMergedTexture);
	READ_JSON_BOOL(jsonOption,visible);

	//position
	READ_JSON_INT(jsonOption ,ZOrder);
	READ_JSON_INT(jsonOption ,x);
	READ_JSON_INT(jsonOption ,y);
	
	READ_JSON_FLOAT(jsonOption,anchorPointX);
	READ_JSON_FLOAT(jsonOption,anchorPointY);
	READ_JSON_FLOAT(jsonOption,positionPercentX);
	READ_JSON_FLOAT(jsonOption,positionPercentY);
	READ_JSON_INT(jsonOption , positionType);
	//transform
	READ_JSON_BOOL(jsonOption,flipX);
	READ_JSON_BOOL(jsonOption,flipY);
	READ_JSON_FLOAT(jsonOption,rotation);
	//size
	READ_JSON_FLOAT(jsonOption , height);
	READ_JSON_FLOAT(jsonOption , width);
	READ_JSON_BOOL(jsonOption,ignoreSize);
	scaleX = DICTOOL->getFloatValue_json(jsonOption , "scaleX" , 1.0f);
	scaleY = DICTOOL->getFloatValue_json(jsonOption , "scaleY" , 1.0f);
	READ_JSON_FLOAT(jsonOption,sizePercentX);
	READ_JSON_FLOAT(jsonOption,sizePercentY);
	READ_JSON_INT(jsonOption ,sizeType);

	//show
	READ_JSON_INT(jsonOption , colorB);
	READ_JSON_INT(jsonOption , colorG);
	READ_JSON_INT(jsonOption , colorR);
	READ_JSON_INT(jsonOption , opacity);
	
	return true;
}

bool cocos2d::extension::GUIDataStruWidget::loadchildren(const rapidjson::Value &jsonWidget)
{
	int childrenCount = DICTOOL->getArrayCount_json(jsonWidget , "children");
	for (int i = 0; i < childrenCount; ++i)
	{
		const rapidjson::Value& subData = DICTOOL->getDictionaryFromArray_json(jsonWidget, "children", i);
		GUIDataStruWidget *pDataStru = GUIDataStruWidgetFactory::getInstance()->createByJson(subData);
		CC_ASSERT(pDataStru);
		if(pDataStru)
		{
			m_arrChildren.push_back(pDataStru);
		}
	}
	return true;
}

cocos2d::extension::GUIDataStruWidget::~GUIDataStruWidget()
{
	//delete children
	for(std::size_t i = 0;i<m_arrChildren.size();++i)
	{
		delete m_arrChildren.at(i);
	}
	m_arrChildren.clear();
}

void cocos2d::extension::GUIDataStruWidget::readWidget(ui::Widget* widget , const string &filePath)
{
	widget->setName(name.c_str());
	widget->ignoreContentAdaptWithSize(ignoreSize);

	widget->setSizeType((ui::SizeType)sizeType);
	widget->setPositionType((ui::PositionType)positionType);

	widget->setSizePercent(ccp(sizePercentX, sizePercentY));
	widget->setPositionPercent(ccp(positionPercentX, positionPercentY));

	widget->setSize(CCSizeMake(width, height));

	widget->setTag(tag);

	// miss action
	//widget->setActionTag(DICTOOL->getIntValue_json(options, "actiontag"));

	//int actionTag = DICTOOL->getIntValue_json(options, "actionTag");
	//widget->setUserObject(cocostudio::timeline::TimelineActionData::create(actionTag));

	widget->setTouchEnabled(touchAble);
	const char* cname = name.c_str();
	const char* widgetName = cname?cname:"default";
	widget->setName(widgetName);
	widget->setPosition(ccp(x,y));

	widget->setScaleX(scaleX);

	widget->setScaleY(scaleY);

	widget->setRotation(rotation);

	widget->setVisible(visible);

	widget->setZOrder(ZOrder);
}

cocos2d::extension::GUIDataStruWidget::ArrChildren & cocos2d::extension::GUIDataStruWidget::getChildren()
{
	return m_arrChildren;
}

void cocos2d::extension::GUIDataStruWidget::readWidgetByColor(ui::Widget *widget)
{
	widget->setOpacity(opacity);

	widget->setColor(ccc3(colorR, colorG, colorB));

	widget->setAnchorPoint(ccp(anchorPointX, anchorPointY));

	widget->setFlipX(flipX);
	widget->setFlipY(flipY);
}

bool cocos2d::extension::GUIDataStruLayout::loadOptions(const rapidjson::Value &jsonOption)
{
	if(!GUIDataStruWidget::loadOptions(jsonOption)) return false;
	READ_JSON_BOOL(jsonOption,adaptScreen);
	READ_JSON_STR(jsonOption,backGroundImage);
	READ_JSON_BOOL(jsonOption,backGroundScale9Enable);
	bgColorR = DICTOOL->getIntValue_json(jsonOption, "bgColorR",150);
	bgColorG = DICTOOL->getIntValue_json(jsonOption, "bgColorG",200);
	bgColorB = DICTOOL->getIntValue_json(jsonOption, "bgColorB",255);

	bgStartColorR = DICTOOL->getIntValue_json(jsonOption, "bgStartColorR",255);
	bgStartColorG = DICTOOL->getIntValue_json(jsonOption, "bgStartColorG",255);
	bgStartColorB = DICTOOL->getIntValue_json(jsonOption, "bgStartColorB",255);

	bgEndColorR = DICTOOL->getIntValue_json(jsonOption, "bgEndColorR",150);
	bgEndColorG = DICTOOL->getIntValue_json(jsonOption, "bgEndColorG",200);
	bgEndColorB = DICTOOL->getIntValue_json(jsonOption, "bgEndColorB",255);
	
	READ_JSON_FLOAT(jsonOption,vectorX);
	vectorY = DICTOOL->getFloatValue_json(jsonOption, "vectorY",-0.5f);
	bgColorOpacity = DICTOOL->getIntValue_json(jsonOption, "bgColorOpacity",100);
	READ_JSON_FLOAT(jsonOption,capInsetsX);
	READ_JSON_FLOAT(jsonOption,capInsetsY);
	READ_JSON_INT(jsonOption ,capInsetsHeight);
	READ_JSON_INT(jsonOption , capInsetsWidth);
	READ_JSON_BOOL(jsonOption , clipAble);
	READ_JSON_INT(jsonOption , colorType);
	//backGroundImageData Path
	const rapidjson::Value& backGroundImageData = DICTOOL->getSubDictionary_json(jsonOption, "backGroundImageData");
	backGroundImageDataPath = DICTOOL->getStringValue_json(backGroundImageData ,"path","");
	READ_JSON_INT(jsonOption , layoutType);
	return true;
}

void cocos2d::extension::GUIDataStruLayout::readWidget(ui::Widget* widget , const string &filePath)
{
	// super read
	GUIDataStruWidget::readWidget(widget,filePath);
	// transfer to panel
	ui::Layout* panel = (ui::Layout*)widget;
	// set size
	float w = 0, h = 0;
	if (adaptScreen)
	{
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		w = screenSize.width;
		h = screenSize.height;
	}
	else
	{
		w = width;
		h = height;
	}
	panel->setSize(CCSizeMake(w, h));
	panel->setClippingEnabled(clipAble);
	panel->setBackGroundImageScale9Enabled(backGroundScale9Enable);

	//set color
	panel->setBackGroundColorVector(ccp(vectorX, vectorY));
	panel->setBackGroundColorType(ui::LayoutBackGroundColorType(colorType));
	panel->setBackGroundColor(ccc3(bgStartColorR, bgStartColorG, bgStartColorB),ccc3(bgEndColorR, bgEndColorG, bgEndColorB));
	panel->setBackGroundColor(ccc3(bgColorR, bgColorG, bgColorB));
	panel->setBackGroundColorOpacity(bgColorOpacity);
	
	if(!backGroundImageDataPath.empty())
	{
		panel->setBackGroundImage((filePath+backGroundImageDataPath).c_str());
	}
	
	panel->setBackGroundImageColor(ccc3(colorR, colorG, colorB));

	panel->setBackGroundImageOpacity(opacity);

	if (backGroundScale9Enable)
	{
		panel->setBackGroundImageCapInsets(CCRectMake(capInsetsX, capInsetsY, capInsetsWidth, capInsetsHeight));
	}
	panel->setLayoutType((ui::LayoutType)layoutType);

	GUIDataStruWidget::readWidgetByColor(widget);
}

bool cocos2d::extension::GUIDataStruButton::loadOptions(const rapidjson::Value &jsonOption)
{
	if(!GUIDataStruWidget::loadOptions(jsonOption)) return false;
	READ_JSON_FLOAT(jsonOption,capInsetsX);
	READ_JSON_FLOAT(jsonOption,capInsetsY);
	capInsetsWidth = DICTOOL->getFloatValue_json(jsonOption, "capInsetsWidth", 1.0);
	capInsetsHeight = DICTOOL->getFloatValue_json(jsonOption, "capInsetsHeight", 1.0);
	READ_JSON_BOOL(jsonOption,disabled);
	READ_JSON_BOOL(jsonOption,scale9Enable);
	READ_JSON_INT(jsonOption ,scale9Height);
	READ_JSON_INT(jsonOption ,scale9Width);
	READ_JSON_STR(jsonOption,text);
	textColorR = DICTOOL->getIntValue_json(jsonOption, "textColorR",255);
	textColorG = DICTOOL->getIntValue_json(jsonOption, "textColorG",255);
	textColorB = DICTOOL->getIntValue_json(jsonOption, "textColorB",255);
	fontSize = DICTOOL->getFloatValue_json(jsonOption, "fontSize", 14);
	//we use png only
	const rapidjson::Value &jsonNormal = DICTOOL->getSubDictionary_json(jsonOption,"normalData");
	CC_ASSERT(DICTOOL->checkObjectExist_json(jsonNormal,"path"));
	normalDataPath = DICTOOL->getStringValue_json(jsonNormal , "path","");

	const rapidjson::Value &jsonPressed = DICTOOL->getSubDictionary_json(jsonOption,"pressedData");
	CC_ASSERT(DICTOOL->checkObjectExist_json(jsonPressed,"path"));
	pressedDataPath = DICTOOL->getStringValue_json(jsonPressed , "path","");

	const rapidjson::Value &jsondisabled = DICTOOL->getSubDictionary_json(jsonOption,"disabledData");
	CC_ASSERT(DICTOOL->checkObjectExist_json(jsondisabled,"path"));
	disabledDataPath = DICTOOL->getStringValue_json(jsondisabled , "path","");
	return true;
}

void cocos2d::extension::GUIDataStruButton::readWidget(ui::Widget* widget , const string &filePath)
{
	GUIDataStruWidget::readWidget(widget, filePath);

	ui::Button* button = (ui::Button*)widget;
	button->setScale9Enabled(scale9Enable);

	if(!normalDataPath.empty())
	{
		button->loadTextureNormal((filePath+normalDataPath).c_str());
	}
	
	if(!pressedDataPath.empty())
	{
		button->loadTexturePressed((filePath+pressedDataPath).c_str());
	}
	if(!disabledDataPath.empty())
	{
		button->loadTextureDisabled((filePath + disabledDataPath).c_str());
	}
	
	if (scale9Enable)
	{
		button->setCapInsets(CCRectMake(capInsetsX, capInsetsY, capInsetsWidth, capInsetsHeight));
		button->setSize(CCSizeMake(scale9Width,scale9Height));
	}

	button->setTitleText(text);
	button->setTitleFontName("");

	button->setTitleColor(ccc3(textColorR,textColorG,textColorB));

	button->setTitleFontSize(fontSize);

	GUIDataStruWidget::readWidgetByColor(widget);
}

bool cocos2d::extension::GUIDataStruImageView::loadOptions(const rapidjson::Value &jsonOption)
{
	if(!GUIDataStruWidget::loadOptions(jsonOption)) return false;
	READ_JSON_FLOAT(jsonOption,capInsetsX);
	READ_JSON_FLOAT(jsonOption,capInsetsY);
	capInsetsWidth = DICTOOL->getFloatValue_json(jsonOption, "capInsetsWidth", 1.0);
	capInsetsHeight = DICTOOL->getFloatValue_json(jsonOption, "capInsetsHeight", 1.0);
	
	READ_JSON_BOOL(jsonOption,scale9Enable);
	scale9Width = DICTOOL->getFloatValue_json(jsonOption , "scale9Width", 80.0f);
	scale9Height = DICTOOL->getFloatValue_json(jsonOption , "scale9Height", 80.0f);
	
	//we use png only
	const rapidjson::Value &jsonFileData = DICTOOL->getSubDictionary_json(jsonOption,"fileNameData");
	CC_ASSERT(DICTOOL->checkObjectExist_json(jsonFileData,"path"));
	fileNameDataPath = DICTOOL->getStringValue_json(jsonFileData , "path","");
	return true;
}

void cocos2d::extension::GUIDataStruImageView::readWidget(ui::Widget* widget , const string &filePath)
{
	GUIDataStruWidget::readWidget(widget, filePath);

	ui::ImageView* imageView = (ui::ImageView*)widget;

	if(!fileNameDataPath.empty())
	{
		imageView->loadTexture((filePath + fileNameDataPath).c_str());
	}
	
	imageView->setScale9Enabled(scale9Enable);

	if (scale9Enable)
	{
		imageView->setSize(CCSizeMake(scale9Width, scale9Height));
		imageView->setCapInsets(CCRectMake(capInsetsX,capInsetsY,capInsetsWidth,capInsetsHeight));
	}

	GUIDataStruWidget::readWidgetByColor(widget);
}

bool cocos2d::extension::GUIDataStruLabelAtlas::loadOptions(const rapidjson::Value &jsonOption)
{
	if(!GUIDataStruWidget::loadOptions(jsonOption)) return false;
	READ_JSON_STR(jsonOption,charMapFile);
	itemWidth = DICTOOL->getIntValue_json(jsonOption, "itemWidth",24);
	itemHeight = DICTOOL->getIntValue_json(jsonOption,"itemHeight",32);
	READ_JSON_STR(jsonOption,startCharMap);
	READ_JSON_STR(jsonOption,stringValue);
	//we only use png
	const rapidjson::Value &charMapFileData = DICTOOL->getSubDictionary_json(jsonOption,"charMapFileData");
	CC_ASSERT(DICTOOL->checkObjectExist_json(charMapFileData,"path"));
	charMapFileDataPath = DICTOOL->getStringValue_json(charMapFileData , "path","");
	return true;
}

void cocos2d::extension::GUIDataStruLabelAtlas::readWidget(ui::Widget* widget , const string &filePath)
{
	GUIDataStruWidget::readWidget(widget, filePath);

	ui::LabelAtlas* labelAtlas = (ui::LabelAtlas*)widget;
	
	if(!charMapFileDataPath.empty())
	{

		string fullPath = filePath ;
		fullPath.append(charMapFileDataPath);
		labelAtlas->setProperty(stringValue.c_str(),fullPath.c_str(),itemWidth,itemHeight,startCharMap);
	}
	GUIDataStruWidget::readWidgetByColor(widget);
}

bool cocos2d::extension::GUIDataStruLabel::loadOptions(const rapidjson::Value &jsonOption)
{
	if(!GUIDataStruWidget::loadOptions(jsonOption)) return false;
	READ_JSON_INT(jsonOption,areaHeight);
	READ_JSON_INT(jsonOption,areaWidth);
	READ_JSON_STR(jsonOption,fontFile);
	READ_JSON_STR(jsonOption,fontName);
	fontSize = DICTOOL->getIntValue_json(jsonOption, "fontSize",20);
	READ_JSON_INT(jsonOption,hAlignment);
	READ_JSON_INT(jsonOption,vAlignment);
	READ_JSON_STR(jsonOption,text);
	READ_JSON_BOOL(jsonOption,touchScaleEnable);
	return true;
}

void cocos2d::extension::GUIDataStruLabel::readWidget(ui::Widget* widget , const string &filePath)
{
	GUIDataStruWidget::readWidget(widget, filePath);
	ui::Label* label = (ui::Label*)widget;
	
	label->setTouchScaleChangeEnabled(touchScaleEnable);
	label->setText(text);
	label->setFontName(fontName);
	label->setFontSize(fontSize);
	if(areaWidth>1&&areaHeight>1){
		label->setTextAreaSize(CCSizeMake(areaWidth,areaHeight));
	}
	label->setTextHorizontalAlignment((CCTextAlignment)hAlignment);
	label->setTextVerticalAlignment((CCVerticalTextAlignment)vAlignment);

	GUIDataStruWidget::readWidgetByColor(widget);
}

bool cocos2d::extension::GUIDataStruLoadingBar::loadOptions(const rapidjson::Value &jsonOption)
{
	if(!GUIDataStruWidget::loadOptions(jsonOption)) return false;
	READ_JSON_FLOAT(jsonOption,capInsetsX);
	READ_JSON_FLOAT(jsonOption,capInsetsY);
	capInsetsWidth = DICTOOL->getFloatValue_json(jsonOption, "capInsetsWidth", 1.0);
	capInsetsHeight = DICTOOL->getFloatValue_json(jsonOption, "capInsetsHeight", 1.0);
	READ_JSON_INT(jsonOption, direction);
	percent = DICTOOL->getIntValue_json(jsonOption, "percent",100);
	READ_JSON_BOOL(jsonOption,scale9Enable);
	READ_JSON_STR(jsonOption, texture);
	//we use png only
	const rapidjson::Value &textureData = DICTOOL->getSubDictionary_json(jsonOption,"textureData");
	CC_ASSERT(DICTOOL->checkObjectExist_json(textureData,"path"));
	textureDataPath = DICTOOL->getStringValue_json(textureData , "path","");
	return true;
}

void cocos2d::extension::GUIDataStruLoadingBar::readWidget(ui::Widget* widget , const string &filePath)
{
	GUIDataStruWidget::readWidget(widget, filePath);
	ui::LoadingBar* loadingBar = (ui::LoadingBar*)widget;

	if(!textureDataPath.empty())
	{
		loadingBar->loadTexture((filePath + textureDataPath).c_str());
	}
	
	loadingBar->setScale9Enabled(scale9Enable);

	if (scale9Enable)
	{
		loadingBar->setCapInsets(CCRectMake(capInsetsX , capInsetsY , capInsetsWidth , capInsetsHeight));
		loadingBar->setSize(CCSizeMake(width, height));
	}

	loadingBar->setDirection(ui::LoadingBarType(direction));
	loadingBar->setPercent(percent);

	GUIDataStruWidget::readWidgetByColor(widget);
}
