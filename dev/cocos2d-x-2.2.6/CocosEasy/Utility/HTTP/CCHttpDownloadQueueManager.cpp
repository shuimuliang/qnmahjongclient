#include "CCHttpDownloadQueueManager.h"
#include "../Utils.h"
#include "curl/curl.h"
#include "curl/easy.h"
#include <string.h>
#include <stdio.h>
#include <vector>
#include <sys/stat.h>
#include <cstring>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#include <sys/utime.h>
#else
#include "utime.h"
#endif
#include "time.h"
/***************************************************
* 中清龙图
* Copyright (C) 2013 -  All Rights Reserved.
* -----------------------------------------------------
* 当前版本 : 1.0
* 作     者: 刘宗安
* 时     间: 2013-07-02 14:21:32
* 注     释: CCHttpDownloadQueueManager管理器

****************************************************/

namespace Engine
{
	static CCHttpDownloadQueueManager* ince = NULL;

	struct header_values {
		time_t lastmodified;
		std::string etag;
		std::string file;
		FILE *fp;
		bool isAppend;
	};

	CCHttpDownloadQueueManager::CCHttpDownloadQueueManager(int poolCount):pool(poolCount,true)
	{   

	}


	CCHttpDownloadQueueManager::~CCHttpDownloadQueueManager(void)
	{

	}

	void CCHttpDownloadQueueManager::addRequest( CCObject *wrapper )
	{
		pool.append(wrapper);
	}

	void CCHttpDownloadQueueManager::startUp()
	{
		pool.startUp();
	}

	void CCHttpDownloadQueueManager::clean()
	{
		pool.clean();
	}

	CCHttpDownloadQueueManager* CCHttpDownloadQueueManager::getInstance()
	{
		if(!ince)
		{
			ince = new CCHttpDownloadQueueManager();
		}
		return ince;

	}

	void CCHttpDownloadQueueManager::end()
	{
		CC_SAFE_DELETE(ince);
	}

	CCDownloadQueueWrapper* CCHttpDownloadThread::getDownloadQueueWrapper()
	{
		return downloadQueueWrapper;
	}

	string& CCDownloadQueueWrapper::getDownloadPath()
	{
		return downloadSdacrdPath;
	}

	string& CCDownloadQueueWrapper::getUrl()
	{
		return url;
	}

	void CCDownloadQueueWrapper::setUnCompress()
	{
		m_bIsSupportUnCompress = true;
	}

	void CCDownloadQueueWrapper::setDownloadResume()
	{
		m_bIsSupportResume = true;
	}

	void CCDownloadQueueWrapper::setAutoDelete()
	{
		m_AutoDelete = true;
	}


	/***************************************************
	* 中清龙图
	* Copyright (C) 2013 -  All Rights Reserved.
	* -----------------------------------------------------
	* 当前版本 : 1.0
	* 作     者: 刘宗安
	* 时     间: 2013-07-02 14:21:15
	* 注     释: CCHttpDownloadThread实现

	****************************************************/
	CCHttpDownloadThread::CCHttpDownloadThread(CCDownloadQueueWrapper *downloadQueueWrapper):downloadQueueWrapper(NULL),m_curl(NULL)
	{
		this->downloadQueueWrapper = downloadQueueWrapper;
		this->downloadQueueWrapper->retain();
	}

	CCHttpDownloadThread::~CCHttpDownloadThread()
	{
		CC_SAFE_RELEASE(downloadQueueWrapper);
	}

	void CCHttpDownloadThread::run()
	{
		CCThread thread;
		thread.createAutoreleasePool();
		CC_ASSERT(downloadQueueWrapper);
		CC_ASSERT(downloadQueueWrapper->downloadListener);
		this->startDownload();
	}


	static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
	{
		header_values* values=(header_values*)userdata;
		FILE *fp = (FILE*)values->fp;
		size_t written = fwrite(ptr, size, nmemb, fp);
		return written;
	}


	static size_t handle_headers(void * ptr, size_t size, size_t nmemb, void * data) 
	{
		char * header = new char[size*nmemb + 1];
		header_values *values = (header_values *)data;

		memcpy(header, ptr, size*nmemb);
		header[size*nmemb] = '\0';

		if (!utils::mystrncasecmp("Last-Modified:", header, 14)) 
		{
			values->lastmodified = curl_getdate(header+14, NULL);
			CCLOG("handle_headers: got last-modified %s (%d)", header+14, values->lastmodified);

// 			if(values->isAppend)
// 			{
// 				char buf[200]={};
// 				sprintf(buf,"%ld",values->lastmodified);
// 				FILE* file = fopen(values->file.c_str(), "wb"); //追加方式打开 
// 				if(!file)
// 				{
// 					CCLog("create file :%s fail",values->file.c_str());
// 				}else
// 				{
// 					fwrite(buf,1,strlen(buf),file);
// 					fclose(file);
// 				}
// 			}
// 			//修改文件的创建和修改时间
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
// 			struct _utimbuf modify_times;
// #else
// 			struct utimbuf modify_times;
// #endif
// 			time_t modify = values->lastmodified;
// 			struct tm  tmm = *localtime(&modify);
// 			modify_times.modtime =mktime(&tmm);
// 			modify_times.actime =mktime(&tmm);
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
// 			int result = _utime(values->file.c_str(),&modify_times);
// #else
// 			int result = utime(values->file.c_str(),&modify_times);
// #endif
// 			CC_ASSERT(result != -1);


			//values->fp = fopen(values->file.c_str(), "ab+"); //追加方式打开  
		
		} else if (!utils::mystrncasecmp("ETag:",header, 5)) 
		{
			values->etag = std::string(header+5);
			//utils::trim(values->etag);
			CCLOG("handle_headers: got etag %s", values->etag.c_str());
		}

		delete[] header;


		return size * nmemb;
	}
	static int progressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
	{
		if(nowDownloaded > 0.0000000001)
		{
			CCHttpDownloadThread* thread = (CCHttpDownloadThread*)(ptr);
			thread->getDownloadQueueWrapper()->getDownloadListener()->onProcess((int)(nowDownloaded/totalToDownload*100),thread->getDownloadQueueWrapper()->getDownloadPath());
		}
		return 0;
	}
	long CCHttpDownloadThread::getLocalFileLenth(const char* filename)
	{
		std::string fullPath =  filename;

		FILE *fp = fopen(fullPath.c_str(), "r");
		if (!fp)
		{
			return 0;
		}
		fseek(fp, 0, SEEK_END);
		long length = ftell(fp);
		fclose(fp);
		return length; 

	}

	void CCHttpDownloadThread::createDir( string &downloadPath )
	{
		string dir = downloadPath ;
		size_t spos = downloadPath.find_last_of(".");
		if(spos != std::string::npos)
		{
			size_t pos = downloadPath.find_last_of("/");
			size_t ppos = downloadPath.find_last_of("\\");


			if (pos != std::string::npos)
			{
				dir = downloadPath.substr(0,pos);
				utils::mkdirs(dir.c_str());
			}

			if (ppos != std::string::npos)
			{
				dir = downloadPath.substr(0,ppos);
				utils::mkdirs(dir.c_str());
			}
			//CCLOG("create folder:%s",dir.c_str());
		}else
		{
			utils::mkdirs(dir.c_str());
		}
	}

	void CCHttpDownloadThread::uncompress( string &tmpDownloadPath )
	{
		size_t zipPos = tmpDownloadPath.find(".zip");
		size_t zipDpos = tmpDownloadPath.find(".ZIP");
		if (zipPos != std::string::npos || zipDpos != std::string::npos)
		{
			utils::uncompress(tmpDownloadPath);
		}	 
	}

	void CCHttpDownloadThread::loadComplete( string &tmpDownloadPath, string &downloadPath, long lastmodified )
	{
		bool success = utils::crename(tmpDownloadPath.c_str(),downloadPath.c_str());
		if(!success)
		{
			CCLOG("file rename fail:%s",downloadPath.c_str());
		}

		if(downloadQueueWrapper->m_bIsSupportUnCompress)
		{
			this->uncompress(downloadPath);
		}

		downloadQueueWrapper->downloadListener->onComplete(downloadPath);

		if(downloadQueueWrapper->m_bIsSupportResume && lastmodified != 0 && lastmodified != -1)
		{
			modifyLastTime(lastmodified, downloadPath);
		}
	}

	void CCHttpDownloadThread::modifyLastTime( long lastmodified, string &downloadPath )
	{
		//修改文件的创建和修改时间
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		struct _utimbuf modify_times;
#else
		struct utimbuf modify_times;
#endif
		time_t modify = lastmodified;
		struct tm  tmm = *localtime(&modify);
		modify_times.modtime =mktime(&tmm);
		modify_times.actime =mktime(&tmm);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		int result = _utime(downloadPath.c_str(),&modify_times);
#else
		int result = utime(downloadPath.c_str(),&modify_times);
#endif
		//CC_ASSERT(result != -1);
	}

	bool CCHttpDownloadThread::startDownload()
	{
		unsigned  char tryTimes = 3;
		CURLcode code = CURL_LAST;
		string downloadPath = downloadQueueWrapper->downloadSdacrdPath;


		CCLOG( "start download ....  %s",downloadQueueWrapper->getUrl().c_str() );
		string tmpDownloadPath = downloadPath + "_tmp";
		if(!downloadQueueWrapper->m_bIsSupportResume&&CCFileUtils::sharedFileUtils()->isFileExist(tmpDownloadPath.c_str()))
		{
			utils::deleteFile(tmpDownloadPath.c_str());
		}
		createDir(downloadPath);

		//header_values hdrs = { 0, "",tmpDownloadPath+"_time",0,false};
		header_values hdrs = { 0, "",tmpDownloadPath,0,false};

		//服务器版本时间戳
		long serverModifyTime = -1L;
		//本地文件版本时间戳
		time_t lastmodified = 0;
		hdrs.isAppend = downloadQueueWrapper->m_bIsSupportResume;
		if(downloadQueueWrapper->m_bIsSupportResume && CCFileUtils::sharedFileUtils()->isFileExist(tmpDownloadPath.c_str()))  
		{  
			hdrs.fp = fopen(tmpDownloadPath.c_str(), "ab+"); //追加方式打开 
			struct stat file_state;
			stat(tmpDownloadPath.c_str(),&file_state);
			lastmodified = file_state.st_mtime;
		}else  
		{  
			hdrs.fp = fopen(tmpDownloadPath.c_str(), "wb"); //创建方式打开  
			lastmodified = 0L;
		} 

		if (! hdrs.fp)
		{
			CCLOG("can not create file %s", tmpDownloadPath.c_str());
			return false;
		}
		m_curl = curl_easy_init();

		struct curl_slist *cHeaders=NULL;

		long retcode = 0;
		do 
		{
			if (! m_curl)
			{
				CCLOG("can not init curl");
				break;
			}
			downloadQueueWrapper->downloadListener->onStart(downloadPath);

			CURLcode res;
			code = curl_easy_setopt(m_curl, CURLOPT_URL, downloadQueueWrapper->url.c_str());
			if (code != CURLE_OK) 
			{
				break;
			}


			//string time_modifiy = "if-modified-since:"+convertToString(file_state.st_mtime);
// 			string time_modifiy = "if-modified-since:Fri, 08 Nov 2010 02:12:27 GMT";
// 			cHeaders=curl_slist_append(cHeaders,time_modifiy.c_str());
// 			code = curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, cHeaders);
// 			/* free the linked list for header data */
// 			if (code != CURLE_OK) 
// 			{
// 				break;
// 			}
			
			if (lastmodified != 0 && downloadQueueWrapper->m_bIsSupportResume) 
			{
				code = curl_easy_setopt(m_curl, CURLOPT_TIMECONDITION, CURL_TIMECOND_IFMODSINCE);
				if (code != CURLE_OK) 
				{
					break;
				}
				code = curl_easy_setopt(m_curl, CURLOPT_TIMEVALUE, lastmodified);
				if (code != CURLE_OK) 
				{
					break;
				}
			}

			code = curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &hdrs);
			if (code != CURLE_OK) 
			{
				break;
			}

			code = curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
			if (code != CURLE_OK) 
			{
				break;
			}

			code = curl_easy_setopt(m_curl, CURLOPT_MAXREDIRS, 10);
			if (code != CURLE_OK) 
			{
				break;
			}

			code = curl_easy_setopt(m_curl, CURLOPT_FAILONERROR, 1);
			if (code != CURLE_OK) 
			{
				break;
			}


			code = curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, handle_headers);
			if (code != CURLE_OK) 
			{
				break;
			}

			code = curl_easy_setopt(m_curl, CURLOPT_FILETIME, 1L);
			if (code != CURLE_OK) 
			{
				break;
			}

			code = curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
			if (code != CURLE_OK) 
			{
				break;
			}
			//CURLOPT_TIMECONDITION
			//如果在CURLOPT_TIMEVALUE指定的某个时间以后被编辑过，则使用CURL_TIMECOND_IFMODSINCE返回页面，如果没有被修改过，并且CURLOPT_HEADER为true，则返回一个"304 Not Modified"的header，CURLOPT_HEADER为false，则使用CURL_TIMECOND_ISUNMODSINCE，默认值为CURL_TIMECOND_IFMODSINCE
			//code = curl_easy_setopt(m_curl, CURLOPT_TIMECONDITION, CURL_TIMECOND_IFMODSINCE);
			//if (code != CURLE_OK) 
			{
				//break;
			}
			

			curl_slist * custom_headers = NULL;
			string etag;
			if (etag.length() > 0) {
				char buf[200]={};
				sprintf(buf,"If-None-Match: %s",etag.c_str());
				//custom_headers = curl_slist_append(custom_headers, utils::strprintf("If-None-Match: %s", etag.c_str()).c_str());
				custom_headers = curl_slist_append(custom_headers, buf);
				curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, custom_headers);
				curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &hdrs);
				curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, handle_headers);
			}

	
			code =  curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, downLoadPackage);
			if (code != CURLE_OK) 
			{
				break;
			}
			code = curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &hdrs);
			if (code != CURLE_OK) 
			{
				break;
			}
			// 如果本地文件没版本则不要请求rang范围
			if (0 != lastmodified 
				&& downloadQueueWrapper->m_bIsSupportResume)
			{
				long localFileLenth = getLocalFileLenth(tmpDownloadPath.c_str()); //已经下载的大小  
				code = curl_easy_setopt(m_curl,CURLOPT_RESUME_FROM, localFileLenth);
				if (code != CURLE_OK) 
				{
					break;
				}
			}

			code = curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, false);
			if (code != CURLE_OK) 
			{
				break;
			}
			code = curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, progressFunc);
			if (code != CURLE_OK) 
			{
				break;
			}
			code = curl_easy_setopt(m_curl, CURLOPT_PROGRESSDATA,this);
			if (code != CURLE_OK) 
			{
				break;
			}

			//code = curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, HTTP_DOWNLOAD_TIMEOUT );//接收数据时超时设置，如果10秒内数据未接收完，直接退出
			if (code != CURLE_OK)
			{
				break;
			}

			//code = curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, HTTP_DOWNLOAD_CONNECTTIMEOUT );//连接超时，这个数值如果设置太短可能导致数据请求不到就断开了
			if (code != CURLE_OK) 
			{
				break;
			}
			res = curl_easy_perform(m_curl);

			fseek(hdrs.fp, 0L,SEEK_END);
			int size = ftell(hdrs.fp);
			long totalSize = 0;
			curl_easy_getinfo(m_curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &totalSize);

			curl_easy_getinfo(m_curl,CURLINFO_FILETIME,&serverModifyTime);

			bool isError = false;
			if(size == 0)
			{
				CCLog("%s: download fail",downloadPath.c_str());
				isError = true;
			}

			CCLOG( "end download ....  %s",downloadPath.c_str() );


			//判断获取响应的http地址是否存在,若存在则返回200,400以上则为不存在,一般不存在为404错误  
			code = curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE , &retcode);    
			if (CURLE_OK == code && retcode >= 400)  
			{  
				tryTimes--;
				CCLOG("error when download package");
				if(cHeaders)
				{
					curl_slist_free_all(cHeaders);
					cHeaders = NULL;
				}

				fclose(hdrs.fp);
				//下面的删除代码无效
				utils::deleteFile(tmpDownloadPath);
				hdrs.fp  = fopen(tmpDownloadPath.c_str(), "wb");
				lastmodified = 0L;
				if (m_curl)
				{
					curl_easy_cleanup(m_curl);
					m_curl = curl_easy_init();
				}

				if(tryTimes > 0)
				{
					continue;
				}else
				{
					downloadQueueWrapper->downloadListener->onFail(downloadPath,downloadQueueWrapper->url);
					break;

				}
			}  
		
			CCLOG("succeed downloading package %s", downloadQueueWrapper->url.c_str());
			break;
		} while (tryTimes > 0);
		if (hdrs.fp)
		{
			int err = fclose(hdrs.fp);
			if(err)
			{
				CCLog("Could not close the file %s \n",downloadPath.c_str()); 
				downloadQueueWrapper->downloadListener->onFail(downloadPath,downloadQueueWrapper->url);
			}
			else
			{
				hdrs.fp = NULL;

				if(retcode < 400)
				{
					bool isDownloadExist = CCFileUtils::sharedFileUtils()->isFileExist(downloadPath.c_str());
					if(retcode == 304 && isDownloadExist)  
					{
						//使用以前的文件
						string tmpFileName = CCFileUtils::sharedFileUtils()->fullPathForFilename(tmpDownloadPath.c_str());
						utils::deleteFile(tmpFileName.c_str());
						downloadQueueWrapper->downloadListener->onComplete(downloadPath);
						return true;
					}
					if(isDownloadExist)
					{
						string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename(downloadPath.c_str());
						utils::deleteFile(fileName.c_str());
					}
					loadComplete(tmpDownloadPath, downloadPath, hdrs.lastmodified);
					if(downloadQueueWrapper->m_AutoDelete)
					{
						utils::deleteFile(downloadPath);
					}
					
				}else
				{
					utils::deleteFile(tmpDownloadPath);
				}
			}
			

		}
		if (m_curl) 
		{
			curl_easy_cleanup(m_curl);
			m_curl = NULL;
		}

		if (cHeaders)
		{
			curl_slist_free_all(cHeaders);
			cHeaders = NULL;
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
	CCDownloadQueueWrapper* CCDownloadQueueWrapper::create( string url, string downloadSdacrdPath,CCDownloadListener *downloadListener )
	{
		CCDownloadQueueWrapper *pRet = new CCDownloadQueueWrapper(url,downloadSdacrdPath,downloadListener);
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

	CCDownloadQueueWrapper::CCDownloadQueueWrapper(const string& url,const string& downloadSdacrdPath,CCDownloadListener *adownloadListener )
		:downloadListener(NULL),
		m_bIsSupportUnCompress(false),
		m_bIsSupportResume(false),
		m_AutoDelete(false)
	{
		this->url = url;
		this->downloadSdacrdPath = downloadSdacrdPath;
		downloadListener=adownloadListener;
		CCObject* object = dynamic_cast<CCObject*>(downloadListener);
		CC_ASSERT(object);
		object->retain();
	}

	CCDownloadQueueWrapper::~CCDownloadQueueWrapper()
	{
		CCObject* object = dynamic_cast<CCObject*>(downloadListener);
		CC_ASSERT(object);
		CC_SAFE_RELEASE(object);
	}


	CCDownloadListener* CCDownloadQueueWrapper::getDownloadListener()
	{
		return downloadListener;
	}

}