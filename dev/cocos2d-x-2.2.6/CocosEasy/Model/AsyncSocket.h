#ifndef __AYNC_SOCKET_H__
#define __AYNC_SOCKET_H__

#include "Utility/Socket/ClientSocket.h"
#include "Utility/Log.h"
#include "Common/CocosEasyDef.h"

typedef void (CCObject::*SEL_ASock_didConnect)();
#define asocket_didconnect_selector(_SELECTOR) (SEL_ASock_didConnect)(&_SELECTOR)

typedef void (CCObject::*SEL_ASock_didSent)(unsigned int);
#define asocket_didsent_selector(_SELECTOR) (SEL_ASock_didSent)(&_SELECTOR)

typedef void (CCObject::*SEL_ASock_didReceive)(const char* buf, unsigned int len);
#define asocket_didreceive_selector(_SELECTOR) (SEL_ASock_didReceive)(&_SELECTOR)

typedef void (CCObject::*SEL_ASock_didDisconnect)();
#define asocket_diddisconnect_selector(_SELECTOR) (SEL_ASock_didDisconnect)(&_SELECTOR)

class CAsyncSocket : public CScriptObject
{
public:
	enum eState{IDLE = 0, CONNECTING, WORK};
	enum {
		SOCK_ERR_NONE = 0,
		SOCK_ERR_UNKNOW = -3,
		SOCK_ERR_INVALID_RECEIVER = -0xf001,
		SOCK_ERR_BAD_MESSAGE = -0xf0002,
		SOCK_ERR_HB_TIMEOUT = -0xf0003,
	};

	struct st_heartBeat
	{
		st_heartBeat() : _msg(NULL), _len(0){}
		st_heartBeat(unsigned int timeout, const char* msg, unsigned int len)
		{
			update(timeout, msg, len);
		}
		
		void update(unsigned int timeout, const char* msg, unsigned int len)
		{
			if(!msg || len == 0) return;
			CC_SAFE_DELETE_ARRAY(_msg);
			_len = len;
			_timeout = timeout;
			_msg = new char[len + 1];
			memcpy((void*)_msg, msg, len);
		}

		void destroy()
		{
			CC_SAFE_DELETE_ARRAY(_msg);
			_len = 0;
		}
		bool isValid(){return _len > 0;}

		unsigned int _timeout; // ms
		const char* _msg;
		unsigned int _len;
	};

	struct st_listener
	{
		st_listener() 
		: _selector(NULL)
		, _connectCallBack(NULL)
		, _sentCallBack(NULL)
		, _recvCallBack(NULL)
		, _disConnectCallBack(NULL)
		, _connectScriptHandler(0)
		, _sentScriptHandler(0)
		, _recvScriptHandler(0)
		, _disConnectScriptHandler(0) {}
		st_listener(CCObject* selector, SEL_ASock_didConnect connectCallBack, SEL_ASock_didSent sentCallBack, SEL_ASock_didReceive recvCallBack, SEL_ASock_didDisconnect disConnectCallBack)
		{
			_connectScriptHandler = _sentScriptHandler = _recvScriptHandler = _disConnectScriptHandler = 0;
			_selector = selector;
			_connectCallBack = connectCallBack;
			_sentCallBack = sentCallBack;
			_recvCallBack = recvCallBack;
			_disConnectCallBack = disConnectCallBack;
		}
		st_listener(int connectScriptHandler, int sentScriptHandler, int recvScriptHandler, int disConnectScriptHandler)
		{
			_selector = NULL; _sentCallBack = NULL; _recvCallBack = NULL; _disConnectCallBack = NULL;
			_connectScriptHandler = connectScriptHandler;
			_sentScriptHandler = sentScriptHandler;
			_recvScriptHandler = recvScriptHandler;
			_disConnectScriptHandler = disConnectScriptHandler;
		}

		CCObject* _selector;
		SEL_ASock_didConnect _connectCallBack;
		SEL_ASock_didSent _sentCallBack;
		SEL_ASock_didReceive _recvCallBack;
		SEL_ASock_didDisconnect _disConnectCallBack;
		
		int _connectScriptHandler;
		int _sentScriptHandler;
		int _recvScriptHandler;
		int _disConnectScriptHandler;
	};

	struct  st_Msg
	{
	public:
		st_Msg(unsigned int id, const char* head, unsigned int headSize, const char* msg, unsigned int size)
		{
			_id = id;
			_index = 0;
			unsigned int count = size + headSize;
			if(size == 0 || headSize == 0)
			{
				_msg = NULL;
				_size = 0;
			}
			else
			{
				_size = count;
				_msg = (const char*)malloc(_size * sizeof(const char));
				memcpy((void*)_msg, head, headSize);
				memcpy((void*)&_msg[headSize], msg, size);
			}
		}

		~st_Msg()
		{
			if(_msg) free((void*)_msg);
			_msg = NULL;
			_size = 0;
			_index = 0;
		}

		unsigned int id() const {return _id;}

		unsigned int msg(const char** p) const
		{
			if(p)
			{
				*p = _index >= _size ? NULL : &_msg[_index];
			}
			return _size - _index;
		}
		unsigned int sent(unsigned int size)
		{
			unsigned int rest = _size - _index;
			if(size > rest)
			{
				astReturn(0);
				return rest;
			}
			else
			{
				_index += size;
				return _size - _index;
			}
		}

		const char* msg(){return _msg;}

	private:
		unsigned int _id;
		const char* _msg;
		unsigned int _size;
		unsigned int _index;
	};

public:
	CAsyncSocket();
	~CAsyncSocket();
	virtual void onDestroy();

	// base operations
public:
	bool connect(const char* ip, int port, float timeout = 2.0f);
	void disconnect();
	unsigned int send(const char* pBuf, unsigned int nSize);
	bool cancelSend(unsigned int id);
	void registerListener(st_listener& listener){m_listener = listener;}
	std::string lastError();
	int lastErrorEnum();
	void setWorkTimeForOnce(float workTime){m_onceWorkTime = (unsigned int)(workTime * 1000.0f);}
	void setHeartbeat(float stimeout, const char* sbuf, unsigned int slen, float rtimeout, const char* rbuf, unsigned int rlen);
	void setInnerVersion(unsigned int version);

	bool isConnected(){return m_state == WORK;}
	bool isIdle(){return m_state == IDLE;}

public:
	virtual void update(float dt);
	
protected:
	void idle();
	void connecting();
	void work();


protected:
	void flush();
	bool receive();
	void changeState(eState state);
	void checkRecvTimeout();

private:
	void restReceiveBufferSize(unsigned int size);
	void didConnect();
	void didSent(unsigned int id);
	void didReceive(const char* buf,unsigned int len);
	void didDisconnect();
    void printBin( const char* buf, unsigned int length );

private:
	unsigned int applySentID();
	void clearMessages();

protected:
	// base property
	CClientSocket* m_pSocket;
	eState m_state;
	unsigned int m_onceWorkTime;
	unsigned int m_rpm;
	unsigned int m_innerVersion;

	// heart beat
	st_heartBeat m_sHB, m_rHB;

	// receiver
	char* m_recvBuffer;
	unsigned int m_recvBufferSize;

	// error
	socket_error m_sock_error;

	// listener
	st_listener m_listener;

	// message list
	std::list<st_Msg*> m_messageList;

	// temp
private:
	unsigned int m_curTick;
	unsigned int m_lastTick;
	unsigned int m_startConnectTime;
	unsigned int m_connectTimeout;
	std::vector<unsigned int> m_sentList;
	unsigned int m_sentID;
};

#endif