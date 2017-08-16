#ifndef __GUI_READER_DATA_STRU_H__
#define __GUI_READER_DATA_STRU_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <map>
#include <vector>

#include "../GUI/BaseClasses/UIWidget.h"
using namespace std;
USING_NS_CC;

NS_CC_EXT_BEGIN

class GUIDataStru;
class GUIDataStruWidget;

/*
** written by Terry Lu
** manage GUIDataStru
*/
class GUIDataStruManager:public CCObject
{
public:
	GUIDataStruManager();
	virtual ~GUIDataStruManager();
	GUIDataStru *loadDataStru(const char *fileName);
	cocos2d::ui::Widget *createWidget(const char *fileName);
private:
	map<string , GUIDataStru*> m_dataPool;
	void init();
	cocos2d::ui::Widget *widgetByData(GUIDataStruWidget *pData , const string &filePath);
};
/*
** record gui json data,only for WidgetPropertiesReader0300
** version 0.3.0.0~1.0.0.0
*/
class  GUIDataStru
{
public:
	GUIDataStru();
	~GUIDataStru();
	bool loadJson(const rapidjson::Document *jsonDict);
	// get value
	inline string getFilePath(){return m_strFilePath;}
	inline void setFilePath(const string &filePath){m_strFilePath = filePath;}
	inline short getDesignWidth(){return m_designWidth;}
	inline short getDesignHeight(){return m_designHeight;}
	const vector<string> &getPngTextures();
	inline GUIDataStruWidget *getWidgetTree(){return m_pWidgetTree;}
private:
	string m_strFilePath;
	// global info
	short m_designWidth;
	short m_designHeight;
	// texturesPng
	vector<string> m_pngTextures;
	// rootdata of widget tree
	GUIDataStruWidget *m_pWidgetTree;
};

NS_CC_EXT_END
#endif  // __GUI_READER_DATA_TYPE_H__

