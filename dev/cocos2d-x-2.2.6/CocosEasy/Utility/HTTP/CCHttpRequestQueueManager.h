#ifndef _CCHTTP_REQUEST_QUEUE_H_
#define _CCHTTP_REQUEST_QUEUE_H_



#include "cocos2d.h"
#include "../ThreadPool/CCThreadPool.h"
#include "curl/curl.h"
USING_NS_CC;

namespace Engine
{
#define DEFAULT_POOL_COUNT 8
#define HTTP_REQUEST_TIMEOUT 10
#define HTTP_REQUEST_CONNECTTIMEOUT 5

	using namespace thread_pool;

	class CCRequestQueueWrapper;
	class CCHttpRequestThread;
	class CCRequestListener;
	class CCHttpRequestQueueManager : public CCObject
	{
	public:
		CCHttpRequestQueueManager(int poolCount = DEFAULT_POOL_COUNT);
		~CCHttpRequestQueueManager(void);

		/**
		* 添加下载列表
		*/
		void addRequest(CCHttpRequestThread *wrapper);

		void startUp();

		void clean();
	public:
		static CCHttpRequestQueueManager* getInstance();
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
	class CCRequestQueueWrapper : public CCObject
	{
	public:
		typedef enum
		{
			kHttpGet,
			kHttpPost,
			kHttpUnkown,
		} HttpRequestType;
		CCRequestQueueWrapper(const string &url,HttpRequestType type,CCRequestListener *requestListener);
		~CCRequestQueueWrapper();
		static CCRequestQueueWrapper* create(string url,HttpRequestType type,CCRequestListener *requestListener);

		/** Use this enum type as param in setReqeustType(param) */


		friend class CCHttpRequestQueueManager;
		friend class CCHttpRequestThread;
	private:
		/**
		* http地址
		*/
		string m_url;


		/**
		* 下载监听器
		*/
		CCRequestListener *m_requestListener;

		/**
		* 请求类型
		*/
		HttpRequestType m_type;

		/**
		* 头信息
		*/
		map<string,string> m_header;


		map<string,string> m_data;

		std::vector<char>  m_requestData; 

	public:
		CCRequestListener* getRequestListener();
		string& getUrl();
		string& getRequestPath();
		void setRequestType(HttpRequestType type);
		map<string,string>& getHeader();

		/**
		* 添加头信息
		*/
		void addHeader(string &key,string &value);

		/**
		* 添加消息体
		*/
		void addData(string key,string value);

		/**
		* 得到数据
		*/
		map<string,string>& getData();

		/** Get the request data pointer back */
		inline char* getRequestData()
		{
			return &(m_requestData.front());
		}

		/** Option field. You can set your post data here
		*/
		inline void setRequestData(const char* buffer, unsigned int len)
		{
			m_requestData.assign(buffer, buffer + len);
		};

		/** Get the size of request data back */
		inline int getRequestDataSize()
		{
			return m_requestData.size();
		}



	};

	class CCHttpRequestThread : public CCRunable,public CCObject
	{
	public:
		CCHttpRequestThread(CCRequestQueueWrapper *requestQueueWrapper);
		virtual ~CCHttpRequestThread();
		virtual void run();
		CCRequestQueueWrapper * getRequestQueueWrapper();
	private:
		CCRequestQueueWrapper *requestQueueWrapper;
		CURL *m_curl;
		std::vector<char>   m_responseData;  /// the returned raw data. You can also dump it as a string
		int                 m_responseCode;    /// the status code returned from libcurl, e.g. 200, 404
		std::string         m_errorBuffer;

	private:
		bool startRequest();
		bool configureCURL(CURL *handle);
	};

	class CCRequestListener
	{
	public:
		/**
		* http开始下载 
		*/
		virtual void onStart(string url) = 0;

		/**
		* http下载完成 
		*/
		virtual void onComplete(string url,string reponseData,int code) = 0;


		/**
		* http错误
		*/
		virtual void onFail(std::string url,int code,string errors) = 0;
	};

}


#endif //_CCHTTP_REQUEST_QUEUE_H_
