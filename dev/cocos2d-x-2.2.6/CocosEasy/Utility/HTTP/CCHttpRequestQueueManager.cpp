#include "CCHttpRequestQueueManager.h"
#include "../Utils.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <vector>

/***************************************************
* 中清龙图
* Copyright (C) 2013 -  All Rights Reserved.
* -----------------------------------------------------
* 当前版本 : 1.0
* 作     者: 刘宗安
* 时     间: 2013-07-02 14:21:32
* 注     释: CCHttpRequestQueueManager管理器

****************************************************/

namespace Engine
{

	// Callback function used by libcurl for collect response data
	size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		std::vector<char> *recvBuffer = (std::vector<char>*)stream;
		size_t sizes = size * nmemb;

		// add data to the end of recvBuffer
		// write data maybe called more than once in a single request
		recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);

		return sizes;
	}

	static CCHttpRequestQueueManager* ince = NULL;
	CCHttpRequestQueueManager::CCHttpRequestQueueManager(int poolCount):pool(poolCount,true)
	{   

	}


	CCHttpRequestQueueManager::~CCHttpRequestQueueManager(void)
	{

	}

	void CCHttpRequestQueueManager::addRequest( CCHttpRequestThread *wrapper )
	{
		pool.append(wrapper);
	}

	void CCHttpRequestQueueManager::startUp()
	{
		pool.startUp();
	}

	void CCHttpRequestQueueManager::clean()
	{
		pool.clean();
	}

	CCHttpRequestQueueManager* CCHttpRequestQueueManager::getInstance()
	{
		if(!ince)
		{
			ince = new CCHttpRequestQueueManager(2);
		}
		return ince;
	}

	CCRequestQueueWrapper* CCHttpRequestThread::getRequestQueueWrapper()
	{
		return requestQueueWrapper;
	}

	string& CCRequestQueueWrapper::getUrl()
	{
		return m_url;
	}

	void CCRequestQueueWrapper::setRequestType( HttpRequestType type )
	{
		m_type = type;
	}

	map<string,string>& CCRequestQueueWrapper::getHeader()
	{
		return m_header;
	}

	void CCRequestQueueWrapper::addHeader( string &key,string &value )
	{
		m_header[key] = value;
	}

	void CCRequestQueueWrapper::addData( string key,string value )
	{
		m_data[key] = value;
	}

	map<string,string>& CCRequestQueueWrapper::getData()
	{
		return m_data;
	}

	/***************************************************
	* 中清龙图
	* Copyright (C) 2013 -  All Rights Reserved.
	* -----------------------------------------------------
	* 当前版本 : 1.0
	* 作     者: 刘宗安
	* 时     间: 2013-07-02 14:21:15
	* 注     释: CCHttpRequestThread实现

	****************************************************/
	CCHttpRequestThread::CCHttpRequestThread(CCRequestQueueWrapper *requestQueueWrapper):requestQueueWrapper(NULL),m_curl(NULL),m_responseCode(0)
	{
		CC_ASSERT(requestQueueWrapper);
		this->requestQueueWrapper = requestQueueWrapper;
		this->requestQueueWrapper->retain();
	}

	CCHttpRequestThread::~CCHttpRequestThread()
	{
		CC_SAFE_RELEASE(requestQueueWrapper);
	}

	void CCHttpRequestThread::run()
	{
		CCThread thread;
		thread.createAutoreleasePool();
		CC_ASSERT(requestQueueWrapper);
		//CC_ASSERT(requestQueueWrapper->getRequestListener());
		this->startRequest();
	}



	bool CCHttpRequestThread::startRequest()
	{
		CURLcode code = CURL_LAST;
		unsigned  char tryTimes = 3;
		do 
		{
			m_curl = curl_easy_init();
			if (! m_curl)
			{
				CCLOG("can not init curl");
				return false;
			}

			if(requestQueueWrapper->getRequestListener())
			{
				requestQueueWrapper->getRequestListener()->onStart(requestQueueWrapper->getUrl());
			}

			CCLOG( "start request ....  %s",requestQueueWrapper->getUrl().c_str() );
			// Request pacakge
			CURLcode res;

			code = curl_easy_setopt(m_curl, CURLOPT_URL, requestQueueWrapper->m_url.c_str());
			if (code != CURLE_OK) 
			{
				break;
			}

			code = curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
			if (code != CURLE_OK) {
				break;	
			}

			code = curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData);
			if (code != CURLE_OK) 
			{
				break;
			}
			code = curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_responseData);
			if (code != CURLE_OK) 
			{
				break;
			}

			code = curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, requestQueueWrapper->getUrl().c_str());
			if (code != CURLE_OK)
			{
				break;
			}

			code = curl_easy_setopt(m_curl,CURLOPT_COOKIEFILE,"cookiefile.txt");
			if (code != CURLE_OK)
			{
				break;
			}
			code = curl_easy_setopt(m_curl,CURLOPT_COOKIEJAR,"cookiefile.txt");   
			if (code != CURLE_OK)
			{
				break;
			}
			code = curl_easy_setopt(m_curl, CURLOPT_COOKIE, ""); 
			if (code != CURLE_OK)
			{
				break;
			}


			/* handle custom header data */
			/* create curl linked list */
			struct curl_slist *cHeaders=NULL;
			/* get custom header data (if set) */
			map<string,string> headers=requestQueueWrapper->getHeader();

			if(!headers.empty())
			{
				for ( map<string,string>::iterator m1_Iter = headers.begin( ); m1_Iter != headers.end( ); m1_Iter++ )
				{
					CC_ASSERT(m1_Iter->first != "");
					CC_ASSERT(m1_Iter->second != "");
					cHeaders=curl_slist_append(cHeaders,(m1_Iter->first+"="+m1_Iter->second).c_str());
				}

				/* set custom headers for curl */
				code = curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, cHeaders);
				if (code != CURLE_OK) 
				{
					break;
				}
			}

			map<string,string> datas=requestQueueWrapper->getData();
			switch (requestQueueWrapper->m_type)
			{
			case CCRequestQueueWrapper::kHttpGet: // HTTP GET
				{
					break;
				}	 
			case CCRequestQueueWrapper::kHttpPost: // HTTP POST
				{
					if(!datas.empty())
					{
						string test("");
						//string str("");
						for ( map<string,string>::iterator m1_Iter = datas.begin( ); m1_Iter != datas.end( ); m1_Iter++ )
						{
							if(m1_Iter != datas.begin())
							{
								test += "&"+(m1_Iter->first+"="+m1_Iter->second);
							}
							else
							{
								test += (m1_Iter->first+"="+m1_Iter->second);
							}
						}
						const char *postData = test.c_str();
						requestQueueWrapper->setRequestData(postData,strlen(postData));
					}

					code = curl_easy_setopt(m_curl, CURLOPT_POST, 1);
					if (code != CURLE_OK) {
						break;
					}

					if(!requestQueueWrapper->m_data.empty())
					{
						code = curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, requestQueueWrapper->getRequestData());
						if (code != CURLE_OK) {
							break;
						}


						code = curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, requestQueueWrapper->getRequestDataSize());
						if (code != CURLE_OK) {
							break;
						}

					}

					break;
				}
			default:
				CCAssert(true, "CCRequestQueueWrapper: unkown request type, only GET and POSt are supported");
				break;
			}

			res = curl_easy_perform(m_curl);
			curl_easy_cleanup(m_curl);
			m_curl = NULL;
			curl_slist_free_all(cHeaders);
			CCLOG( "end request ....  %s",requestQueueWrapper->getUrl().c_str() );
			if (res != 0)
			{
				
				tryTimes--;
				CCLOG("error when request package");
				if(tryTimes > 0)
				{
					continue;
				}else
				{
					if(requestQueueWrapper->getRequestListener())
					{
						requestQueueWrapper->getRequestListener()->onFail(requestQueueWrapper->m_url,200,"");
					}
					return false;
				}
			}

			std::string responseText(m_responseData.begin(),m_responseData.end());
			if(requestQueueWrapper->getRequestListener())
			{
				requestQueueWrapper->getRequestListener()->onComplete(requestQueueWrapper->getUrl(),responseText,200);
			}

			//CCLOG("succeed requesting package %s", requestQueueWrapper->url.c_str());
			break;
		} while (tryTimes > 0);

		if(m_curl)
		{
			curl_easy_cleanup(m_curl);
			m_curl = NULL;
		}
		return true;
	}



	//Configure curl's timeout property
	bool CCHttpRequestThread::configureCURL(CURL *handle)
	{
		if (!handle) {
			return false;
		}

		int32_t code;
		code = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, m_errorBuffer.c_str());
		if (code != CURLE_OK) {
			return false;
		}
		// 	code = curl_easy_setopt(handle, CURLOPT_TIMEOUT, CCHttpClient::getInstance()->getTimeoutForRead());
		// 	if (code != CURLE_OK) {
		// 		return false;
		// 	}
		// 	code = curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, CCHttpClient::getInstance()->getTimeoutForConnect());
		// 	if (code != CURLE_OK) {
		// 		return false;
		// 	}

		code = curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, HTTP_REQUEST_TIMEOUT );//接收数据时超时设置，如果10秒内数据未接收完，直接退出
		if (code != CURLE_OK) {
			return false;
		}

		code = curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, HTTP_REQUEST_CONNECTTIMEOUT );//连接超时，这个数值如果设置太短可能导致数据请求不到就断开了
		if (code != CURLE_OK) {
			return false;
		}

		return true;
	}

	/***************************************************
	* 中清龙图
	* Copyright (C) 2013 -  All Rights Reserved.
	* -----------------------------------------------------
	* 当前版本 : 1.0
	* 作     者: 刘宗安
	* 时     间: 2013-07-02 11:56:55
	* 注     释: 下载包装器

	****************************************************/
	CCRequestQueueWrapper* CCRequestQueueWrapper::create( string url,HttpRequestType type,CCRequestListener *requestListener )
	{
		CCRequestQueueWrapper *pRet = new CCRequestQueueWrapper(url,type,requestListener);
		if (pRet)
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			CC_SAFE_DELETE(pRet);
			return NULL;
		}
	}

	CCRequestQueueWrapper::CCRequestQueueWrapper(const string& url,HttpRequestType type,CCRequestListener *requestListener ):m_requestListener(NULL)
	{
		this->m_url = url;
		this->m_type = type;
		if(requestListener)
		{
			this->m_requestListener=requestListener;
			CCObject* object = dynamic_cast<CCObject*>(m_requestListener);
			CC_ASSERT(object);
			object->retain();
		}
	}

	CCRequestQueueWrapper::~CCRequestQueueWrapper()
	{
		if(m_requestListener)
		{
			CCObject* object = dynamic_cast<CCObject*>(m_requestListener);
			CC_ASSERT(object);
			CC_SAFE_RELEASE(object);
		}

	}


	CCRequestListener* CCRequestQueueWrapper::getRequestListener()
	{
		return m_requestListener;
	}

}