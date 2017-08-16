//
//  ImagePicker.cpp
//  Casino
//
//  Created by liulihua on 15/1/14.
//
//

#include "ImagePicker.h"
NS_CC_EASY_BEGIN
ImagePicker* ImagePicker::m_Instance = NULL;

ImagePicker* ImagePicker::getInstance()
{
    if(!m_Instance)
    {
        m_Instance = new ImagePicker();
    }
    return m_Instance;
}

void ImagePicker::selectPicAndUpload(int callBackHandler,const std::string url){
    selectPicAndUpload(callBackHandler, url,"", 0);
}
void ImagePicker::selectPicAndUpload(int callBackHandler,
                                     const std::string url,
                                     const std::string extInfo){
    selectPicAndUpload(callBackHandler, url,extInfo, 0);
}

//if clipWidth==0 : origin size
void ImagePicker::selectPicAndUpload(int callBackHandler,
                        const std::string url,
                        const std::string extInfo,
                        int clipWidth){
    m_callBackHandler = callBackHandler;
    m_URL = url;
    m_extInfo = extInfo;
    selectPic(clipWidth);
}

void ImagePicker::selectPicFinished(const std::string filePath){
    if ( ! m_URL.empty() ){
        uploadData(filePath, m_URL);
    }else{
        CCSprite *pResult = NULL;
        if (filePath.empty())
        {
            doEventToScript(pResult,pickPicFailed,"" , m_strTag,"init texture error");
            return;
        }
        pResult = CCSprite::create(filePath.c_str());
        doEventToScript(pResult,pickPicSucc,"",m_strTag,"");
    }
}
void ImagePicker::selectPicCanceled(){
    doEventToScript(NULL,pickPicCancel,"","","");
}
void ImagePicker::setTag(const std::string &tag)
{
    m_strTag = tag;
}

void ImagePicker::uploadData(const std::string filePath, const std::string url){
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(CCHttpRequest::kHttpPostFile);
    request->setFilePath(filePath);
    request->setExtInfo(m_extInfo);
    request->setResponseCallback(this, httpresponse_selector(ImagePicker::onHttpRequestCompleted));
    request->setTag(m_strTag.c_str());
    CCHttpClient::getInstance()->send(request);
    request->release();
}
void ImagePicker::onHttpRequestCompleted(cocos2d::extension::CCHttpClient *sender , cocos2d::extension::CCHttpResponse *response)
{
    //1°¢¥ÌŒÛ¥˙¬Î
    //2°¢∑µªÿÕº∆¨
    CCSprite *pResult = NULL;
    ePickPictureStatus statusCode = pickPicFailed;
    std::string tag = "no response";
    std::string error = "no net";
    
    if (!response)
    {
        doEventToScript(pResult,statusCode,"", tag,error);
        return ;
    }
    
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        tag = response->getHttpRequest()->getTag();
    }
    
    if( response->getResponseCode() == 200 ){
        statusCode = pickPicSucc;
    }else{
        statusCode = pickPicFailed;
    }
    if (!response->isSucceed())
    {
        error = response->getErrorBuffer();
        doEventToScript(pResult,statusCode,"",tag,error);
        return;
    }
    
    std::string localFilePath = response->getHttpRequest()->getFilePath();
    if (0 != localFilePath.length())
    {
        pResult = CCSprite::create(localFilePath.c_str());
        tag = localFilePath;
    }
    
    // path
    std::vector<char> *buffer = response->getResponseData();
    std::string result(buffer->begin(),buffer->end());
    doEventToScript(pResult,statusCode,result,tag,error);
}

void ImagePicker::doEventToScript(CCSprite *img , ePickPictureStatus code, const std::string result, const std::string &tag,const std::string &error)
{
    CCArray* pArrayArgs = CCArray::createWithCapacity(4);
    
    pArrayArgs->addObject(CCInteger::create(code));
    pArrayArgs->addObject(CCString::create(result));
    pArrayArgs->addObject(CCString::create(tag));
    pArrayArgs->addObject(CCString::create(error));
    if(img)
    {
        pArrayArgs->addObject(img);
    }
    CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_callBackHandler,pArrayArgs);
}

unsigned char* ImagePicker::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
{
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(pszFileName, "rb", pSize);
	return pBuffer;
}







NS_CC_EASY_END