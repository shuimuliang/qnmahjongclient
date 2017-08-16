#include "ClientSocket.h"
#include <signal.h>
#include "Utility/Log.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <netdb.h>
#endif

#define INIT_CACHE_SIZE 128

CClientSocket::CClientSocket()
: m_inCache(NULL)
, m_inCapacity(0)
, m_inSize(0)
, m_maxInCapacity(1024*1024) // 1MB
, m_keepAlive(true)
, m_state(CLOSE)
, m_inStart(0)
, m_Port(0)
, m_lastSendTime(0)
, m_lastRecvTime(0)
, m_sock_error(NULL)
{
	m_socket = SOCKET_INVALID;
	setInputCapacity(INIT_CACHE_SIZE);
}


CClientSocket::~CClientSocket()
{
	if(m_socket != SOCKET_INVALID)
	{
		opt_nosigpipe(&m_socket, true);
		Close();
	}
	delete[] m_inCache;
}


SOCKET CClientSocket::createSocket(int inettype)//qiuhe
{
    
    
	if (int err = i_socket_create(&m_socket,inettype, SOCK_STREAM) )//qiuhe
	{
		printError(err, "socket_create");
		i_socket_destroy(&m_socket);
		return SOCKET_INVALID;
	}

	if(m_keepAlive)
	{
		int err = opt_keepalive(&m_socket, 1);
		if(err) printError(err);
	}
	
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	if(int err = opt_nosigpipe(&m_socket, true) ) printError(err, "opt_nosigpipe");
#endif
	
	socket_setnonblocking(&m_socket);

	return m_socket;
}

bool CClientSocket::Connect(float timeout)
{
	if(m_state == CONNECT) return true;
	if(m_state != CLOSE) return false;
    int inettype=AF_INET;
    //////////////////////////////////////////qiuhe
    struct addrinfo *result;
    struct addrinfo *res;
    
    struct addrinfo addrCriteria;
    memset(&addrCriteria,0,sizeof(addrCriteria));
    
    char t[256];
    sprintf(t, "%u", m_Port);
    int error = getaddrinfo(m_IP.c_str(), t, &addrCriteria, &result);
    if (error == 0)
    {
        struct sockaddr_in *sa;
        for (res = result; res != NULL; res = res->ai_next)
        {
            if (AF_INET6 == res->ai_addr->sa_family)
            {
                inettype=AF_INET6;
                break;
            }
            else if (AF_INET == res->ai_addr->sa_family)
            {
                inettype=AF_INET;
            }
        }
    }

    //////////////////////////////////////////qiuhe
	// create socket
	if(m_socket == SOCKET_INVALID && createSocket(inettype) == SOCKET_INVALID)//qiuhe
	{
		printf("create socket failed \n");
		return false;
	}

	// connect
	if(int err = i_socket_connect(&m_socket, m_IP.c_str(), m_Port, timeout) )
	{
		if(err == IO_TIMEOUT)
		{
			m_state = HALF_CONNECT;
		}
		else
			printError(err, "connect");
		return false;
	}
	
	m_state = CONNECT;
	return true;
}



bool CClientSocket::ConnectWait(float timeout /*= 0*/)
{
	if( int err = i_socket_connect_wait(&m_socket, timeout) )
	{
		switch(err)
		{
		case IO_TIMEOUT: 
			{
				m_state = HALF_CONNECT; 
			}
			break;

		default:
			{
				printError(err, "connect wait");
				Close();
			}
			break;
		}
		return false;
	}
	
	m_state = CONNECT;
	return true;
}



void CClientSocket::Close()
{
	i_socket_destroy(&m_socket);
	m_socket = SOCKET_INVALID;
	m_state = CLOSE;
}


bool CClientSocket::Send(const char* pBuf, unsigned int nSize, float timeout, unsigned int& sent)
{
	if(m_state != CONNECT) return false;
	if(!pBuf || nSize == 0) return false;

	// send once
    size_t size = 0;
	int err = i_socket_send(&m_socket, pBuf, 0, nSize-1, &size, timeout);
    sent = (unsigned int)size;

	// success
	if(err == IO_DONE || err == IO_TIMEOUT)
	{
		if(sent > 0)
		{
			updateSendTime();
			//CEDebug("flush message length %d", sent);
		}
	}
	else
	{
		printError(err);
		Close(); 
		return false;
	}


	__CE_MEMORY_CHECK__;
	return true;
}


bool CClientSocket::Receive( char* pBuf, unsigned int expectSize )
{
	if(!Seek(pBuf, expectSize)) return false;
	m_inStart = (m_inStart + expectSize) % m_inCapacity;
	m_inSize -= expectSize;
	return true;
}


bool CClientSocket::Seek( char* pBuf, unsigned int expectSize )
{
	if(m_state != CONNECT && m_inSize < expectSize) return false; 
	if(expectSize <= 0 || !pBuf) return false;

	// need to read message
	if(expectSize > m_inSize)
	{
		tryRead();
		return false;
	}
	
	// copy data 
	if(m_inStart + expectSize > m_inCapacity)
	{
		// first copy
		int copylen = m_inCapacity - m_inStart;
		memcpy(pBuf, &m_inCache[m_inStart], copylen);

		// second copy
		memcpy(&pBuf[copylen], &m_inCache[0], expectSize - copylen);
	}
	else
	{
		memcpy(pBuf, &m_inCache[m_inStart], expectSize);
	}

	__CE_MEMORY_CHECK__;
	return true;
}



void CClientSocket::tryRead()
{
	if(m_state != CONNECT) return;

	// expansion input cache
	if(m_inSize >= m_inCapacity)
	{
		unsigned int allocSize = (unsigned int)(m_inCapacity * 1.5f);
		setInputCapacity(allocSize);
	}
	if(m_inSize >= m_inCapacity) return;
	bool toEnd = readToEndOfCapacity();
	if(!toEnd) return;

	// maybe there are something messages in cache, try another receive
	if(m_inSize >= m_inCapacity) setInputCapacity( (unsigned int) (m_inCapacity * 1.5f) );
	if(m_inSize >= m_inCapacity) return;
	readToEndOfCapacity();
}



bool CClientSocket::readToEndOfCapacity()
{
	unsigned int savelen, savepos;		
	if( m_inStart + m_inSize < m_inCapacity )
		savelen = m_inCapacity - (m_inStart + m_inSize); 
	else
		savelen = m_inCapacity - m_inSize;

	savepos = (m_inStart + m_inSize) % m_inCapacity;
	if(savepos + savelen > m_inCapacity)
	{
		CEFatal("savepos(%d) + savelen(%d) > m_inCapacity(%d)",savepos, savelen, m_inCapacity);
		return false;
	}

	// receive buff
	size_t inlen = 0;
	int err = i_socket_recv(&m_socket, &m_inCache[savepos], savelen, &inlen, SHORT_DELAY);
	if(err && err != IO_TIMEOUT)
	{
		printError(err);
		Close();
		return false;
	}

	
	__CE_MEMORY_CHECK__;

	if(inlen > 0)
	{
		updateRecvTime();
		//CEDebug("received message length %d", inlen);
	}

	m_inSize += inlen;
	if(m_inSize > m_inCapacity){assert(false); return true;}
	return m_inStart + m_inSize >= m_inCapacity;
}


bool CClientSocket::setInputCapacity(unsigned int size)
{
	if(size > m_maxInCapacity || size < m_inSize) return false;
	char* buf = new char[size];
	if(m_inCache)
	{
		// first copy
		unsigned int len = m_inStart + m_inSize <= m_inCapacity ? m_inSize : m_inCapacity - m_inStart; assert(len <= m_inSize);
		memcpy(buf,&m_inCache[m_inStart],len);
		
		// second copy
		if(len != m_inSize) memcpy(&buf[len],&m_inCache[0],m_inSize-len);

		delete[] m_inCache;
		m_inStart = 0;
	}
	m_inCache = buf;
	m_inCapacity = size;

	__CE_MEMORY_CHECK__;
	return true;
}


void CClientSocket::printError(int err, const char* tag)
{
	if(err == IO_DONE) return;
	if(m_sock_error)
	{
		m_sock_error->seterror(err);
	}
// 	if(tag)
// 		CEError("socket error %s, code %d, [%s]",socket_strerror(err), err, tag);
// 	else
// 		CEError("socket error %s, code %d",socket_strerror(err), err);
}

void CClientSocket::printBin( const char* buf, unsigned int length )
{
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
}

void CClientSocket::printSocketInfo()
{
	const char* stateInfo = "";
	switch (m_state)
	{
	case CClientSocket::UNINIT:
		stateInfo = "UNINIT";
		break;
	case CClientSocket::CLOSE:
		stateInfo = "CLOSE";
		break;
	case CClientSocket::CONNECT:
		stateInfo = "CONNECT";
		break;
	}

	printf("state: %s \n",stateInfo);
	printf("\n");
	printf("in start index : %d \n",m_inStart);
	printf("in size: %d bytes \n",m_inSize);
	printf("in capacity: %d bytes \n",m_inCapacity);
}



