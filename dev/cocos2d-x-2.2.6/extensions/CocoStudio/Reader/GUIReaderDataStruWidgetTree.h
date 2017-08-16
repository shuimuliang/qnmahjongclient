#ifndef __GUI_READER_DATA_STRU_WIDGET_TREE_H__
#define __GUI_READER_DATA_STRU_WIDGET_TREE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <map>
#include <vector>
#ifndef byte
#define byte char
#endif
using namespace std;
USING_NS_CC;
NS_CC_EXT_BEGIN

/**
 * define a create function for a specific type, such as CCLayer
 * @__TYPE__ class type to add create(), such as CCLayer
 */
#define NEWINS_FUNC(__TYPE__) \
static GUIDataStruWidget* newIns() \
{ \
    return new __TYPE__(); \
}
class GUIDataStruWidget;


/*
** widget data
** 1.options
** 2.children
*/
class GUIDataStruWidget
{
public:
	typedef vector<GUIDataStruWidget*> ArrChildren;
    NEWINS_FUNC(GUIDataStruWidget);
    bool load(const rapidjson::Value &jsonWidget);
    virtual ~GUIDataStruWidget();
	//get value
	inline string getClassName(){return className;}
	inline string getClassType(){return classType;}
	ArrChildren &getChildren();
	virtual void readWidget(ui::Widget* pWidget , const string &filePath);
protected:
    virtual void readWidgetByColor(ui::Widget* pWidget);
    ArrChildren m_arrChildren;
    virtual bool loadOptions(const rapidjson::Value &jsonOption);
    virtual bool loadchildren(const rapidjson::Value &jsonWidget);

protected:
    //global
    string className;

protected:
    //options
    string __type;
    string classType;
    string name;
    string customProperty;
    byte tag;
    bool touchAble;
    bool useMergedTexture;
    bool visible;
    //position
    byte ZOrder;
    int x;
    int y;
    float anchorPointX;
    float anchorPointY;
    float positionPercentX;
    float positionPercentY;
    byte positionType;
    //transform
    bool flipX;
    bool flipY;
    float rotation;
    //size
    short height;
    short width;
    bool ignoreSize;
    float scaleX;
    float scaleY;
    float sizePercentX;
    float sizePercentY;
    byte sizeType;
    //show
    byte colorB;
    byte colorG;
    byte colorR;
    byte opacity;
};

class GUIDataStruLayout : public GUIDataStruWidget
{
public:
    NEWINS_FUNC(GUIDataStruLayout);
	virtual void readWidget(ui::Widget* pWidget , const string &filePath);
protected:
    virtual bool loadOptions(const rapidjson::Value &jsonOption);
protected:
	bool adaptScreen;
    //backGound
    string backGroundImage;
    bool backGroundScale9Enable;
    byte bgColorB;
    byte bgColorG;
    byte bgColorR;
    byte bgColorOpacity;
    byte bgStartColorR;
    byte bgStartColorG;
    byte bgStartColorB;
    byte bgEndColorR;
    byte bgEndColorG;
    byte bgEndColorB;
    float vectorX;
    float vectorY;
    float capInsetsX;
    float capInsetsY;
    short capInsetsHeight;
    short capInsetsWidth;
	bool clipAble;
	byte colorType;
	string backGroundImageDataPath;
	byte layoutType;
};

class GUIDataStruButton : public GUIDataStruWidget
{
public:
    NEWINS_FUNC(GUIDataStruButton);
	virtual void readWidget(ui::Widget* pWidget , const string &filePath);
protected:
    virtual bool loadOptions(const rapidjson::Value &jsonOption);
protected:
	float capInsetsX;
	float capInsetsY;
	short capInsetsHeight;
	short capInsetsWidth;
	bool disabled;
	bool scale9Enable;
	short scale9Height;
	short scale9Width;
	string text;
	byte textColorB;
	byte textColorG;
	byte textColorR;
	byte fontSize;
	//we use png only
	string normalDataPath;
	string pressedDataPath;
	string disabledDataPath;


};

class GUIDataStruImageView : public GUIDataStruWidget
{
public:
	NEWINS_FUNC(GUIDataStruImageView);
	virtual void readWidget(ui::Widget* pWidget , const string &filePath);
protected:
	virtual bool loadOptions(const rapidjson::Value &jsonOption);
protected:
	float capInsetsX;
	float capInsetsY;
	short capInsetsHeight;
	short capInsetsWidth;
	bool scale9Enable;
	short scale9Height;
	short scale9Width;
	//we only use png
	string fileNameDataPath;
};
class GUIDataStruLabelAtlas : public GUIDataStruWidget
{
public:
	NEWINS_FUNC(GUIDataStruLabelAtlas);
	virtual void readWidget(ui::Widget* pWidget , const string &filePath);
protected:
	virtual bool loadOptions(const rapidjson::Value &jsonOption);
protected:
	string charMapFile;
	int itemHeight;
	int itemWidth;
	string startCharMap;
	string stringValue;
	//we only use png
	string charMapFileDataPath;
};

class GUIDataStruLabel : public GUIDataStruWidget
{
public:
	NEWINS_FUNC(GUIDataStruLabel);
	virtual void readWidget(ui::Widget* pWidget , const string &filePath);
protected:
	virtual bool loadOptions(const rapidjson::Value &jsonOption);
protected:
	int areaHeight;
	int areaWidth;
	string fontFile;
	string fontName;
	byte fontSize;
	byte hAlignment;
	byte vAlignment;
	string text;
	bool touchScaleEnable;
};

class GUIDataStruLoadingBar : public GUIDataStruWidget
{
public:
	NEWINS_FUNC(GUIDataStruLoadingBar);
	virtual void readWidget(ui::Widget* pWidget , const string &filePath);
protected:
	virtual bool loadOptions(const rapidjson::Value &jsonOption);
protected:
	float capInsetsX;
	float capInsetsY;
	short capInsetsHeight;
	short capInsetsWidth;
	byte direction;
	byte percent;
	bool scale9Enable;
	string texture;
	//we use png only
	string textureDataPath;
};

NS_CC_EXT_END
#endif  // __GUI_READER_DATA_STRU_WIDGET_TREE_H__

