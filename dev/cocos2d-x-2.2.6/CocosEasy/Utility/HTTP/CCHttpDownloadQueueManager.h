/***************************************************
* 中清龙图
* Copyright (C) 2013 -  All Rights Reserved.
* -----------------------------------------------------
* 当前版本 : 1.0
* 作     者: 刘宗安
* 时     间: 2013-07-02 11:56:55
* 注     释: http下载管理器

****************************************************/
#ifndef _CCHTTP_DOWNLOAD_QUEUE_H_
#define _CCHTTP_DOWNLOAD_QUEUE_H_

#include "cocos2d.h"
#include "../ThreadPool/CCThreadPool.h"
#include "curl/curl.h"
USING_NS_CC;

namespace Engine
{

#define DEFAULT_DOWNLOAD_POOL_COUNT 5
#define HTTP_DOWNLOAD_TIMEOUT 20
#define HTTP_DOWNLOAD_CONNECTTIMEOUT 10

	using namespace thread_pool;

	class CCDownloadQueueWrapper;
	class CCHttpDownloadThread;
	class CCDownloadListener;
	class CCHttpDownloadQueueManager : public CCObject
	{
	private:
		CCHttpDownloadQueueManager(int poolCount = DEFAULT_DOWNLOAD_POOL_COUNT);
		~CCHttpDownloadQueueManager(void);
	public:
		/**
		* 添加下载列表
		*/
		void addRequest(CCObject *wrapper);

		void startUp();

		void clean();
	public:

		static CCHttpDownloadQueueManager* getInstance();
		static void end();

	private:
		CCThreadPool pool;
	};



	/***************************************************
	* 中清龙图
	* Copyright (C) 2013 -  All Rights Reserved.
	* -----------------------------------------------------
	* 当前版本 : 1.0
	* 作     者: 刘宗安
	* 时     间: 2013-07-02 11:56:55
	* 注     释: 下载包装器

	****************************************************/
	using namespace std;
	class CCDownloadQueueWrapper : public CCObject
	{
	public:
		CCDownloadQueueWrapper(const string &url,const string &downloadSdacrdPath,CCDownloadListener *downloadListener);
		~CCDownloadQueueWrapper();
		static CCDownloadQueueWrapper* create(string url, string downloadSdacrdPath,CCDownloadListener *downloadListener);

		friend class CCHttpDownloadQueueManager;
		friend class CCHttpDownloadThread;
	private:
		/**
		* http地址
		*/
		string url;
		/**
		* 下载路径
		*/
		string downloadSdacrdPath;

		/**
		* 下载监听器
		*/
		CCDownloadListener *downloadListener;

		/**
		 * 自动解压
		 */
		bool m_bIsSupportUnCompress;

		/**
		 * 续传
		 */
		bool m_bIsSupportResume;

		/**
		 * 自动删除
		 */
		bool m_AutoDelete;
	public:
		CCDownloadListener* getDownloadListener();
		string& getUrl();
		string& getDownloadPath();
		void setUnCompress();
		void setDownloadResume();
		void setAutoDelete();

	};

	class CCHttpDownloadThread : public CCRunable,public CCObject
	{
	public:
		CCHttpDownloadThread(CCDownloadQueueWrapper *downloadQueueWrapper);
		virtual ~CCHttpDownloadThread();
		virtual void run();
		CCDownloadQueueWrapper * getDownloadQueueWrapper();
		long getLocalFileLenth(const char* filename);
	private:
		CCDownloadQueueWrapper *downloadQueueWrapper;
		CURL *m_curl;
		bool startDownload();

		void loadComplete( string &tmpDownloadPath, string &downloadPath, long localFileTime );

		void modifyLastTime( long lastmodified, string &downloadPath );

		void uncompress( string &tmpDownloadPath );


		void createDir( string &downloadPath );

	};

	class CCDownloadListener
	{
	public:
		/**
		* http开始下载 
		*/
		virtual void onStart(string downLoadPath) = 0;

		/**
		* http下载完成 
		*/
		virtual void onComplete(string downLoadPath) = 0;

		/**
		* http进度  返回 0-- 100
		* scanf("%lld",&a);
		* printf("%lld",a);
		* 使用无符号数时，将"%lld"改成"%llu"即可。
		*/
		virtual void onProcess(int number , string downLoadPath) = 0;

		/**
		* http错误
		*/
		virtual void onFail(string errors,std::string url) = 0;
	};

}

#endif //_CCHTTP_DOWNLOAD_QUEUE_H_
