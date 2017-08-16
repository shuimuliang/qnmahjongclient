
#include "AsyncSocket.h"
#include "CocosExt/CCLString.h"
#include <cells/md5.h>

#define HEAD_LEN 4
#define VERSION_LEN 4
#define RANDOM_LEN 4
#define HEAD_VERSION_LEN 12
#define MD5_STRING_LEN 4
#define SUM_HEAD_LEN HEAD_LEN + VERSION_LEN + RANDOM_LEN + MD5_STRING_LEN
#define DEF_MAX_PACKAGE_LEN 1024*1024

#define SOCKET_RPM  100

CAsyncSocket::CAsyncSocket()
: m_pSocket(NULL)
, m_state(IDLE)
, m_recvBuffer(NULL)
, m_recvBufferSize(0)
, m_curTick(0)
, m_onceWorkTime(0)
, m_startConnectTime(0)
, m_connectTimeout(0)
, m_innerVersion(0)
, m_lastTick(0)
, m_rpm(SOCKET_RPM)
, m_sentID(0)
{
	m_pSocket = new CClientSocket();
	m_pSocket->setErrorCatcher(&m_sock_error);

	CCScheduler* pScheduler = CCDirector::sharedDirector()->getScheduler();
	pScheduler->unscheduleUpdateForTarget(this);
	pScheduler->scheduleUpdateForTarget(this,kCCPrioritySystem,false);

}

CAsyncSocket::~CAsyncSocket()
{
	CC_SAFE_DELETE(m_pSocket);
	CC_SAFE_DELETE_ARRAY(m_recvBuffer);

	clearMessages();
}


void CAsyncSocket::onDestroy()
{
	CCScheduler* pScheduler = CCDirector::sharedDirector()->getScheduler();
	pScheduler->unscheduleUpdateForTarget(this);
	CScriptObject::onDestroy();
}



void CAsyncSocket::update(float dt)
{
	// get current tick
	m_curTick = getTickCount();
	if(m_curTick - m_lastTick < m_rpm) return;
	m_lastTick = m_curTick;

	// check work time for once
	if(m_onceWorkTime > 0 && m_state != IDLE)
	{
		if(m_curTick - m_startConnectTime > m_onceWorkTime)
		{
			m_sock_error.seterror(IO_TIMEOUT);
			changeState(IDLE);
		}
	}

	// dispatch state
	switch (m_state)
	{
	case CAsyncSocket::IDLE:{idle(); break;}
	case CAsyncSocket::CONNECTING:{connecting(); break;}
	case CAsyncSocket::WORK:{work(); break;}
	default: break;
	}
}


#define CHECK_SOCKET_ERROR (m_sock_error.check_clear())

void CAsyncSocket::changeState(eState state)
{
	// set state
	eState preState = m_state;
	m_state = state;

	// event
	switch (state)
	{
	case IDLE:
		{
			if(preState != IDLE)
			{
				m_pSocket->Close();
				m_pSocket->clearInputCache();
				clearMessages();
				didDisconnect();
			}
		}
		break;

	case CONNECTING:
		{

		}
		break;

	case WORK:
		{
			if(preState != WORK)
			{
				didConnect();
			}
		}
		break;
	default:
		break;
	}
}

bool CAsyncSocket::connect(const char* ip, int port, float timeout)
{
	if(m_state != IDLE) return true;
	m_pSocket->setIP(ip);
	m_pSocket->setPort(port);

	bool ret = m_pSocket->Connect(SHORT_DELAY);
	if(ret)
	{
		changeState(WORK);
		return true;
	}
	
	if(CHECK_SOCKET_ERROR)
	{
		return false;
	}

	// set time out
	m_startConnectTime = getTickCount();
	m_connectTimeout = (unsigned int)(timeout * 1000.0f);

	changeState(CONNECTING);
	return true;
}


void CAsyncSocket::disconnect()
{
	if(m_state == IDLE) return;
	changeState(IDLE);
}


unsigned int CAsyncSocket::send(const char* pBuf, unsigned int nSize)
{
	// add head
	int len = nSize;
	len = xint(len);

	// version
	int iVersion = m_innerVersion;
	iVersion = xint(iVersion);

	// random
	int iRand = rand();
	iRand = xint(iRand);

	// concat head
	char concat[HEAD_VERSION_LEN] = {0};
	memcpy(concat,&len,HEAD_LEN);
	memcpy(concat+HEAD_LEN,&iVersion,VERSION_LEN);
	memcpy(concat+HEAD_LEN + VERSION_LEN,&iRand,RANDOM_LEN);

	// md5
	md5_state_t state;
	md5_byte_t digest[24];
	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)concat, HEAD_VERSION_LEN);
	md5_finish(&state, digest);

	//concat all 
	char head[SUM_HEAD_LEN] = {0};
	memcpy(head,&len,HEAD_LEN);
	memcpy(head+HEAD_LEN,&iVersion,VERSION_LEN);
	memcpy(head+HEAD_LEN + VERSION_LEN,&iRand,RANDOM_LEN);
	memcpy(head+HEAD_LEN + VERSION_LEN + RANDOM_LEN,digest,MD5_STRING_LEN);
	
	// uuid
	unsigned int id = applySentID();

	// save message
	st_Msg* msg = new st_Msg(id, head, SUM_HEAD_LEN, pBuf, nSize);
    printBin(msg->msg(), SUM_HEAD_LEN + nSize);
	m_messageList.push_back(msg);
	return id;
}


bool CAsyncSocket::cancelSend(unsigned int id)
{
	bool cancelled = false;
	std::list<st_Msg*>::iterator it = m_messageList.begin();
	for (it; it != m_messageList.begin(); ++it)
	{
		st_Msg* pMsg = (*it);
		if(pMsg->id() == id)
		{
			cancelled = true;
			delete(pMsg);
			m_messageList.erase(it);
			break;
		}
		else if(pMsg->id() > id)
		{
			break;
		}
	}
	return cancelled;
}



void CAsyncSocket::setHeartbeat(float stimeout, const char* sbuf, unsigned int slen, float rtimeout, const char* rbuf, unsigned int rlen)
{
	if(!sbuf || !rbuf) return;
	m_sHB.update((unsigned int)(stimeout*1000.f), sbuf, slen);
	m_rHB.update((unsigned int)(rtimeout*1000.f), rbuf, rlen);
	__CE_MEMORY_CHECK__;
}



void CAsyncSocket::setInnerVersion(unsigned int version)
{
	m_innerVersion = version;
}



void CAsyncSocket::idle()
{
	return;
}

void CAsyncSocket::connecting()
{
	if(m_pSocket->ConnectWait(SHORT_DELAY))
	{
        m_pSocket->updateRecvTime();
		changeState(WORK);
	}

	if(CHECK_SOCKET_ERROR)
	{
		changeState(IDLE);
		return;
	}

	// check timeout
	if(m_curTick - m_startConnectTime > m_connectTimeout)
	{
		m_sock_error.seterror(IO_TIMEOUT);
		changeState(IDLE);
	}
}

void CAsyncSocket::work()
{
	// flush
	flush();
	if(CHECK_SOCKET_ERROR)
	{
		changeState(IDLE);
		return;
	}

	// receive
    bool hasRecv = false;
    do
    {
        hasRecv = receive();
        if(CHECK_SOCKET_ERROR)
        {
            changeState(IDLE);
            return;
        }
    }
    while(hasRecv);
	

	// check received timeout
	checkRecvTimeout();
	if(CHECK_SOCKET_ERROR)
	{
		changeState(IDLE);
		return;
	}
}

void CAsyncSocket::flush()
{
	// flush data
	if(m_messageList.size() > 0)
	{
		std::list<st_Msg*>::iterator it = m_messageList.begin();
		while (it!=m_messageList.end())
		{
			st_Msg* pMsg = (*it);
			const char* pBuf = NULL;
			unsigned int size = pMsg->msg(&pBuf);
			unsigned int sent = 0;
			if(!m_pSocket->Send(pBuf, size, SHORT_DELAY, sent))
			{
				break;
			}
			if(pMsg->sent(sent) == 0)
			{
				m_sentList.push_back(pMsg->id());
				delete(pMsg);
				it = m_messageList.erase(it);
			}
			else
			{
				break;
			}
		}
	}

	// check send heartbeat time
	if(m_sHB.isValid())
	{
		unsigned int sendTime = m_pSocket->getSendTime();
		if(m_curTick >= sendTime && m_curTick - sendTime > m_sHB._timeout)
		{
			if(send(m_sHB._msg, m_sHB._len)) 
			{
				m_pSocket->updateSendTime();
			}
		}
	}

	// notify for sent
	if(m_sentList.size() > 0)
	{
		for (unsigned int i = 0; i < m_sentList.size(); i++)
		{
			didSent(m_sentList[i]);
		}
		m_sentList.clear();
	}
}

bool CAsyncSocket::receive()
{
	// read message head
	char head[SUM_HEAD_LEN] = {0};
	int len = SUM_HEAD_LEN;
	if(!m_pSocket->Seek(&head[0],len)) return false;
	memcpy(&len,&head[0],HEAD_LEN);
	len = xint(len);

	// check is message body length valid
	if(len <= 0 || len >= DEF_MAX_PACKAGE_LEN)
	{
		if(len <= 0) {CEError("message head size(%d) little than 0",len);}
		else{CEError("message head size(%d) is larger than %d", len, DEF_MAX_PACKAGE_LEN);}
		m_sock_error.seterror(SOCK_ERR_BAD_MESSAGE);
		return false;
	}

	// set receive buffer size
	restReceiveBufferSize(SUM_HEAD_LEN + len);
	if(m_recvBufferSize < (unsigned int)(SUM_HEAD_LEN + len))
	{
		CEError("received buffer length(%d) < message length(%d)", m_recvBufferSize, HEAD_LEN + len);
		m_sock_error.seterror(SOCK_ERR_INVALID_RECEIVER);
		return false;
	}

	// try read message(head + body)
	if(!m_pSocket->Receive(m_recvBuffer, SUM_HEAD_LEN + len))
	{
		//CEError("unknown error");
		//m_sock_error.seterror(SOCK_ERR_UNKNOW);
		return false;
	}

	// update received time
	m_pSocket->updateRecvTime();

	// check heartbeat
	bool isHB = false;
	if(m_rHB.isValid())
	{
		if(len == m_rHB._len && (memcmp(&m_recvBuffer[SUM_HEAD_LEN], m_rHB._msg, m_rHB._len) == 0) )
		{
			 isHB = true;
		}
	}

	// dispatch
	if(!isHB)
	{
		didReceive(&m_recvBuffer[SUM_HEAD_LEN], len);
	}
    printBin(m_recvBuffer, SUM_HEAD_LEN + len);
    
    return true;
}



void CAsyncSocket::printBin( const char* buf, unsigned int length )
{
#if COCOS2D_DEBUG > 0
    char line[20], p;
    int j = 0;
    unsigned int i;
    
    line[0]=0;
    
    printf("\n");
    printf("%-5d|  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  0123456789ABCDEF\n",length);
    printf("-----|------------------------------------------------------------------\n");
    
    for(i=0;i<length;i++)
    {
        if(i%16 == 0)
        {
            if (j>0) {line[j]=0; printf("%s",line); j=0;}
            if (i/16 != 0)
                printf( "\n%-5d|  ", i );
            else
                printf( "%-5d|  ", i );
        }
        p = buf[i];
        printf("%X%X ",(unsigned char)p/16,(unsigned char)p%16);
        if (p>='!' && p<='~' && p!='%') line[j] = p;
        else line[j] = '.';
        j++;
        
    }
    i=i%16; for (i;i<16;i++) printf ("   ");
    if (j>0) {line[j]=0; printf("%s",line); j=0;}
    printf("\n\n");
#endif
}




void CAsyncSocket::restReceiveBufferSize(unsigned int size)
{
	if(size <= m_recvBufferSize) {
		return;
	}
	char* buf = new char[size];
	if(m_recvBuffer)
	{
		memcpy(buf,m_recvBuffer,m_recvBufferSize);
		delete[] m_recvBuffer;
	}
	m_recvBuffer = buf;
	m_recvBufferSize = size;
}

void CAsyncSocket::didConnect()
{
	if(m_listener._selector && m_listener._connectCallBack)
	{
		(m_listener._selector->*m_listener._connectCallBack)();
	}

	if(m_listener._connectScriptHandler)
	{
		CCArray* pArrayArgs = CCArray::createWithCapacity(0);
		CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(
			m_listener._connectScriptHandler, pArrayArgs);
	}
}


void CAsyncSocket::didSent(unsigned int id)
{
	if(m_listener._selector && m_listener._sentCallBack)
	{
		(m_listener._selector->*m_listener._sentCallBack)(id);
	}

	if(m_listener._sentScriptHandler)
	{
		CCArray* pArrayArgs = CCArray::createWithCapacity(1);
		pArrayArgs->addObject(CCInteger::create(id));
		CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(
			m_listener._sentScriptHandler, pArrayArgs);
	}
}


void CAsyncSocket::didReceive(const char* buf, unsigned int len)
{
	if(m_listener._selector && m_listener._recvCallBack)
	{
		(m_listener._selector->*m_listener._recvCallBack)(buf, len);
	}

	if(m_listener._recvScriptHandler)
	{
		CCArray* pArrayArgs = CCArray::createWithCapacity(2);
		pArrayArgs->addObject(CCLString::create((const unsigned char*)buf,len));
		pArrayArgs->addObject(CCInteger::create(len));
		CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(
			m_listener._recvScriptHandler, pArrayArgs);
	}
}

void CAsyncSocket::didDisconnect()
{
	if(m_listener._selector && m_listener._disConnectCallBack)
	{
		(m_listener._selector->*m_listener._disConnectCallBack)();
	}

	if(m_listener._disConnectScriptHandler)
	{
		CCArray* pArrayArgs = CCArray::createWithCapacity(0);
		CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(
			m_listener._disConnectScriptHandler, pArrayArgs);
	}
}

std::string CAsyncSocket::lastError()
{
	if(m_sock_error.error == 0)
	{
		return "";
	}

	switch (m_sock_error.error)
	{
	case SOCK_ERR_INVALID_RECEIVER: {return "invalid receive buffer, size is too small"; break;}
	case SOCK_ERR_BAD_MESSAGE:{return "receive a message with a large length"; break;}
	case SOCK_ERR_HB_TIMEOUT:{return "heart beat timeout"; break;}
	default:
		return socket_strerror(m_sock_error.error);
		break;
	}
	return "";
}

int CAsyncSocket::lastErrorEnum()
{
	return m_sock_error.error;
}

unsigned int CAsyncSocket::applySentID()
{
	m_sentID += 1;
	return m_sentID;
}

void CAsyncSocket::clearMessages()
{
	std::list<st_Msg*>::iterator it = m_messageList.begin();
	for (it; it!=m_messageList.end(); ++it)
	{
		delete(*it);
	}
	m_messageList.clear();
}

void CAsyncSocket::checkRecvTimeout()
{
	if(!m_rHB.isValid()) return;

	unsigned int recvTime = m_pSocket->getRecvTime();
	if(m_curTick >= recvTime && m_curTick - recvTime > m_rHB._timeout)
	{
		CEError("heart beat for received time out, ready to close. (timeout:%d)", (m_curTick - recvTime) / 1000);
		m_sock_error.seterror(SOCK_ERR_HB_TIMEOUT);
		return;
	}
}



