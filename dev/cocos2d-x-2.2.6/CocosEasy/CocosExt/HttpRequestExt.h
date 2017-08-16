#ifndef  __CE_HTTP_REQUEST_EXT_H__
#define  __CE_HTTP_REQUEST_EXT_H__

#include "Common/CocosEasyDef.h"



NS_CC_EASY_BEGIN

class HttpRequestExt:public CCObject
{
public:
	static HttpRequestExt *create();
	void sendGet(const std::string &url , int callBackHandler);
	void setTag(const std::string &tag);
	
	//Http Response Callback
	void onHttpRequestCompleted(cocos2d::extension::CCHttpClient *sender , cocos2d::extension::CCHttpResponse *response);
private:
	std::string		m_strTag;
    std::string		m_fileName;
	int				m_callBackHandler;
	void doEventToScript(CCSprite *img , int code,const std::string &tag , const std::string &error);
    void saveFileToLocal(CCImage *pImg);
    std::string factoryDownloadPath(std::string fileName);
};


NS_CC_EASY_END
#endif //__UI_TAB_VIEW_EXT_H__

