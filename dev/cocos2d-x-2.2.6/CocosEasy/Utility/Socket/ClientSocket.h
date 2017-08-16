#ifndef __CLIENT_SOCKET_H__
#define __CLIENT_SOCKET_H__

#include <stdlib.h>
#include <assert.h>
#include <string>
#include "cocos2d.h"
#include "ISocket.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include <windows.h>
#include <WinSock.h>
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_WP8)
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <unistd.h>

#endif

#ifndef SOCKET 
#define SOCKET int
#endif

#define SHORT_DELAY  0.00001f // 0.01ms


struct socket_error
{
	socket_error() : error(0), tag(false){}
	inline void seterror(int err)
	{
		error = err;
		tag = true;
	}
	inline int geterror(){return error;}
	bool check_clear()
	{
		bool err = tag;
		tag = false;
		return err;
	}
	int error;
	bool tag;
};

class CClientSocket {
public:
	enum STATE{UNINIT=0, CLOSE, HALF_CONNECT, CONNECT};
	CClientSocket(void);
	~CClientSocket();

	bool init(const char* ip, int port){return true;}
	bool Connect(float timeout = 0);
	bool ConnectWait(float timeout = 0);
	void Close();
	bool Send(const char* pBuf, unsigned int nSize, float timeout, unsigned int& sent);
	bool Receive(char* pBuf, unsigned int nSize);
	bool Seek(char* pBuf, unsigned int nSize);
	void printBin(const char* buf, unsigned int len);

	const char* getIP() const {return m_IP.c_str();}
	int getPort() const {return m_Port;}
	void setIP(const char* ip){if(ip){m_IP = ip;}}
	void setPort(unsigned int port){m_Port = port;}
	void setErrorCatcher(socket_error* catcher){m_sock_error = catcher;}
	
	// cache operations
public:
	bool setMaxInputCapacity(unsigned int size){if(m_state != UNINIT){return false;} m_maxInCapacity = size; return true;}
	bool setInputCapacity(unsigned int size);
	void printSocketInfo();
	
	void clearInputCache(){m_inStart = 0; m_inSize = 0;}

	// statistics operations
public:
	void updateRecvTime(){m_lastRecvTime = getTickCount();}
	void updateSendTime(){m_lastSendTime = getTickCount();}
	unsigned int getRecvTime(){return m_lastRecvTime;}
	unsigned int getSendTime(){return m_lastSendTime;}

public:
	unsigned int getInputSize() {return m_inSize; }
	bool isConnect(){return m_state == CONNECT;}
	bool isClose(){return m_state == CLOSE;}
	bool isHalfConnect(){return m_state == HALF_CONNECT;}

protected:
	SOCKET createSocket(int inettype);//qiuhe
	void printError(int err, const char* tag = NULL);
	void tryRead();
	bool readToEndOfCapacity();
	

private:
	SOCKET	m_socket;

	// cache
	char*  m_inCache;
	unsigned int m_maxInCapacity, m_inCapacity , m_inStart , m_inSize;
	
	// property
	std::string m_IP;
	int m_Port;
	bool m_keepAlive;
	STATE m_state;

	// statistics
	unsigned int m_lastSendTime;
	unsigned int m_lastRecvTime;

	// error
	socket_error* m_sock_error;
};






#endif
