#include "GUIReaderDataStruWidgetFactory.h"
#include "GUIReaderDataStru.h"
#include "GUIReaderDataStruWidgetTree.h"

cocos2d::extension::GUIDataStruWidgetFactory * cocos2d::extension::GUIDataStruWidgetFactory::INCE = NULL;
cocos2d::extension::GUIDataStruWidgetFactory::GUIDataStruWidgetFactory()
{
	init();
}

cocos2d::extension::GUIDataStruWidgetFactory::~GUIDataStruWidgetFactory()
{

}

void cocos2d::extension::GUIDataStruWidgetFactory::init()
{
	//regist all data
	registIns("Button",GUIDataStruButton::newIns);
	//registIns("CheckBox",GUIDataStruWidget::newIns);
	registIns("ImageView",GUIDataStruImageView::newIns);
	registIns("Label",GUIDataStruLabel::newIns);
	registIns("LabelAtlas",GUIDataStruLabelAtlas::newIns);
	//registIns("LabelBMFont",GUIDataStruWidget::newIns);
	registIns("LoadingBar",GUIDataStruLoadingBar::newIns);
	//registIns("Slider",GUIDataStruWidget::newIns);
	//registIns("TextField",GUIDataStruWidget::newIns);
	registIns("Layout",GUIDataStruLayout::newIns);
	//registIns("ListView",GUIDataStruWidget::newIns);
	//registIns("PageView",GUIDataStruWidget::newIns);
	//registIns("ScrollView",GUIDataStruWidget::newIns);
	       
}

void cocos2d::extension::GUIDataStruWidgetFactory::registIns(const string &className , newInstance *ince)
{
	mapNewIncese[className] = ince;
}

string cocos2d::extension::GUIDataStruWidgetFactory::getDataClassName(const string &name)
{
	std::string convertedClassName = name;
	if (name == "Panel")
	{
		convertedClassName = "Layout";
	}
	else if (name == "TextArea")
	{
		convertedClassName = "Label";
	}
	else if (name == "TextButton")
	{
		convertedClassName = "Button";
	}
	return convertedClassName;
}

cocos2d::extension::GUIDataStruWidgetFactory * cocos2d::extension::GUIDataStruWidgetFactory::getInstance()
{
	if(!INCE){
		INCE= new GUIDataStruWidgetFactory;
	}
	return INCE;
}

void cocos2d::extension::GUIDataStruWidgetFactory::delInstance()
{
	CC_SAFE_DELETE(INCE);
	INCE = NULL;
}

cocos2d::extension::GUIDataStruWidget * cocos2d::extension::GUIDataStruWidgetFactory::newIns(const string &className)
{
	newInstance *pFunc = mapNewIncese[getDataClassName(className)];
	CC_ASSERT(pFunc);
	return pFunc();
}

cocos2d::extension::GUIDataStruWidget * cocos2d::extension::GUIDataStruWidgetFactory::createByJson(const rapidjson::Value &jsonWidget)
{
	string classname = DICTOOL->getStringValue_json(jsonWidget,"classname");
	GUIDataStruWidget *result = newIns(classname.c_str());
	if(!result->load(jsonWidget))
	{
		CC_ASSERT(false);
		delete result;
		result = NULL;
	}
	return result;
}



