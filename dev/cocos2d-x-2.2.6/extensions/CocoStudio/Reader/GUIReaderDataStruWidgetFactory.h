#ifndef __GUI_READER_DATA_STRU_WiDGET_FACTORY_H__
#define __GUI_READER_DATA_STRU_WiDGET_FACTORY_H__
#include <map>
#include <string>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GUIReaderDataStruWidgetTree.h"
using namespace std;
USING_NS_CC;
NS_CC_EXT_BEGIN
typedef GUIDataStruWidget *newInstance();
class GUIDataStruWidgetFactory
{
public:
	static GUIDataStruWidgetFactory *getInstance();
	static void delInstance();
	~GUIDataStruWidgetFactory();
	GUIDataStruWidget *createByJson(const rapidjson::Value &jsonWidget);
	//needed to delete it by client
	GUIDataStruWidget *newIns(const string &className);
	void registIns(const string &className , newInstance *ince);
	string getDataClassName(const string &name);
private:
	typedef map<string , newInstance*> MapNewInstance;
	MapNewInstance mapNewIncese;
	static GUIDataStruWidgetFactory *INCE;
	GUIDataStruWidgetFactory();
	void init();
};

NS_CC_EXT_END
#endif  // __GUI_READER_DATA_STRU_WiDGET_FACTORY_H__

