
#ifndef __CE_DOWNLOAD_MANAGER_H__
#define __CE_DOWNLOAD_MANAGER_H__

#include "Common/CocosEasyDef.h"



class CDownloadList
{
protected:
	struct st_downloadItem
	{
		st_downloadItem(const char* url, unsigned int size)
		{
			if(!url) astBack;
			this->url = url;
			this->size = size;
		}
		std::string url;
		unsigned int size;
	};

public:
	void addDownloadItem(const char* url, unsigned int size)
	{
		if (!url) astBack;
		st_downloadItem item(url,size);
		m_list.push_back(item);
	}
	void clear(){m_list.clear();}
	unsigned int getCount() const {return m_list.size();}

protected:
	std::vector<st_downloadItem> m_list;
};



class CDownloadManager : public CCObject
{
public:
	~CDownloadManager();
	static CDownloadManager* getInstance();
	void end();

protected:
	struct _Message
	{
	public:
		_Message() : id(0), obj(NULL){}
		unsigned int id;
		void* obj;
	};

protected:
	CDownloadManager();
	void download();
	bool uncompress(); 
	bool createDirectory(const char *path);

protected:
	CC_SYNTHESIZE_READONLY(bool,m_isRunning,Running);
	CC_SYNTHESIZE(bool,m_storagePath,StoragePath);
	CC_SYNTHESIZE(unsigned int,m_timeout,Timeout);
	CDownloadList m_downloadList;

	CURL *_curl;
	pthread_t *_tid;

private:
	static CDownloadManager* m_Instance;
};





#endif
