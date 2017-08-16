//
//  ImagePicker.h
//  Casino
//
//  Created by liulihua on 15/1/14.
//
//

#ifndef __Casino__ImagePicker__
#define __Casino__ImagePicker__

#include <stdio.h>
#include "Common/CocosEasyDef.h"

enum ePickPictureStatus{
    pickPicSucc = 0,
    pickPicFailed	= 1,
    pickPicCancel = 2,
};

NS_CC_EASY_BEGIN

class ImagePicker:public CCObject
{
public:
    static ImagePicker* getInstance();
    
    // select local pic
    void selectPicAndUpload(int callBackHandler,const std::string url);
    void selectPicAndUpload(int callBackHandler,
                            const std::string url,
                            const std::string extInfo);
    //if clipWidth<=0 : origin size
    void selectPicAndUpload(int callBackHandler,
                            const std::string url,
                            const std::string extInfo,
                            int clipWidth);
    void setTag(const std::string &tag);
    
    void selectPic(int clipWidth);

	void selectPicFinished(const std::string filePath);

	unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);
private:
    std::string		m_strTag;
    int				m_callBackHandler;
    std::string     m_URL;
    std::string     m_extInfo;
    void doEventToScript(CCSprite *img , ePickPictureStatus code, const std::string url, const std::string &tag,const std::string &error);
    void uploadData(const std::string filePath, const std::string url);
    void printBin( const char* buf, unsigned int length );
    
    void selectPicCanceled();

    //Http Response Callback
    void onHttpRequestCompleted(cocos2d::extension::CCHttpClient *sender , cocos2d::extension::CCHttpResponse *response);

    static ImagePicker* m_Instance;

};

NS_CC_EASY_END
#endif /* defined(__Casino__ImagePicker__) */
