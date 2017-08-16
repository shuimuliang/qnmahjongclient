#include "HttpRequestExt.h"
//#include "Config.h"
NS_CC_EASY_BEGIN


void string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst )
{
    std::string::size_type pos = 0;
    std::string::size_type srclen = strsrc.size();
    std::string::size_type dstlen = strdst.size();
    
    while( (pos=strBig.find(strsrc, pos)) != std::string::npos )
    {
        strBig.replace( pos, srclen, strdst );
        pos += dstlen;
    }
}


void HttpRequestExt::sendGet(const std::string &url,int callBackHandler)
{
    
//    int pos = (int)url.find_last_of("/");
//    m_fileName = url.substr(pos+1,url.length());
    std::string s1 = "";
    std::string s2 = "/";
    std::string s3 = ":";
    std::string urlS = std::string(url);

    string_replace(urlS,s2,s1);
    string_replace(urlS,s3,s1);
    m_fileName = urlS;

    m_callBackHandler = callBackHandler;

	CCHttpRequest* request = new CCHttpRequest();
	// required fields
	request->setUrl(url.c_str());
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setResponseCallback(this, httpresponse_selector(HttpRequestExt::onHttpRequestCompleted));
	// optional fields
	request->setTag("no tag");

	CCHttpClient::getInstance()->send(request);
	// don't forget to release it, pair to new
	request->release();
}

void HttpRequestExt::setTag(const std::string &tag)
{
	m_strTag = tag;
}

void HttpRequestExt::onHttpRequestCompleted(cocos2d::extension::CCHttpClient *sender , cocos2d::extension::CCHttpResponse *response)
{
	//1°¢¥ÌŒÛ¥˙¬Î
	//2°¢∑µªÿÕº∆¨
	CCSprite *pResult = NULL;
	int statusCode = -1;
	std::string tag = "no response";
	std::string error = "no net";

	if (!response)
	{
		doEventToScript(pResult,statusCode,tag,error);
		return ;
	}

	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag())) 
	{
		tag = response->getHttpRequest()->getTag();
	}

	statusCode = response->getResponseCode();
	if (!response->isSucceed()) 
	{
		error = response->getErrorBuffer();
		doEventToScript(pResult,statusCode,tag,error);
		return;
	}

	// dump data
	std::vector<char> *buffer = response->getResponseData();
	CCImage *pImg = new CCImage;
	pImg->initWithImageData(buffer->data(),(int)buffer->size());
	cocos2d::CCTexture2D* texture = new cocos2d::CCTexture2D();
    saveFileToLocal(pImg);
	bool isImg = texture->initWithImage(pImg);
	pImg->release();
	if (!isImg) 
	{ 
		delete texture; 
		doEventToScript(pResult,statusCode,tag,"init texture error");
		return;   
	} 
	pResult = CCSprite::createWithTexture(texture); 
	texture->release(); 
	doEventToScript(pResult,statusCode,tag,error);
}

void HttpRequestExt::saveFileToLocal(CCImage *pImg){
    if (pImg == NULL) return;
    
    //当前账号头像
    std::string stdCurrentAvatarPath = factoryDownloadPath(m_fileName) + ".png";
    pImg->saveToFile(stdCurrentAvatarPath.c_str(),false);
    
//    std::string bufffff(buffer->begin(),buffer->end());
//    CCLog("%s",stdCurrentAvatarPath.c_str());
//    //写入当前账号头像
//    FILE *pCurrentAvatarFile = fopen(stdCurrentAvatarPath.c_str(), "wb");
//    fwrite(bufffff.c_str(), 1,buffer->size(), pCurrentAvatarFile);
//    fclose(pCurrentAvatarFile);
}

std::string HttpRequestExt::factoryDownloadPath(std::string fileName)
{
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string DIR_ROOT = "";
    path += DIR_ROOT + fileName;
    return path;
}

void HttpRequestExt::doEventToScript(CCSprite *img , int code,const std::string &tag,const std::string &error)
{
    CCArray* pArrayArgs = CCArray::createWithCapacity(4);
    
    pArrayArgs->addObject(CCInteger::create(code));
    pArrayArgs->addObject(CCString::create(tag));
    pArrayArgs->addObject(CCString::create(error));
    if(img)
    {
        pArrayArgs->addObject(img);
    }
    CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_callBackHandler,pArrayArgs);
}

HttpRequestExt * HttpRequestExt::create()
{
    HttpRequestExt* result = new HttpRequestExt;
	result->autorelease();
	return result;
}

NS_CC_EASY_END
