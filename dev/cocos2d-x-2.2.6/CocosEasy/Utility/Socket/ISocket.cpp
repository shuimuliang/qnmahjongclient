#include "ISocket.h"

void second_timeval(float s, timeval* tv)
{
	if (!tv) return;
	tv->tv_sec	= (int)s;
	tv->tv_usec	= (int)((s - (int)s) * 1000000) ;
}

int i_socket_open(void)
{
	return socket_open();
}

int i_socket_close(void)
{
	return socket_close();
}

int i_socket_create(p_socket sock,int inettype, int type)//qiuhe
{
	int err = IO_DONE;
	err = inet_trycreate(sock,inettype, type);//qiuhe
	return err;
}

int i_socket_connect(p_socket sock, const char *address, unsigned short port, float delay)
{
	int err = IO_DONE;
	struct timeval tv; second_timeval(delay, &tv);
	err = inet_tryconnect(sock, address, port, &tv);
	return err;
}

int i_socket_connect_wait(p_socket sock, float delay)
{
	int err = IO_DONE;
	struct timeval tv; second_timeval(delay, &tv);
	err = socket_connect_wait(sock, &tv);
	return err;
}

int i_socket_send(p_socket sock, const char *data, unsigned short start, unsigned int end, size_t *sent, float delay)
{
	int err = IO_DONE;
	if(start < 0 || start > end) return -0xff;//"invalid arguments";
	struct timeval tv; second_timeval(delay, &tv);
	if (start <= end) err = socket_send(sock, &data[start], end-start+1, sent, &tv);
	return err;
}

int i_socket_recv(p_socket sock, char *data, size_t count, size_t *got, float delay)
{
	int err = IO_DONE;
	struct timeval tv; second_timeval(delay, &tv);
	err = socket_recv(sock, data, count, got, &tv, 0);
	return err;
}

int i_socket_seek(p_socket sock, char *data, size_t count, size_t *got, float delay)
{
	int err = IO_DONE;
	struct timeval tv; second_timeval(delay, &tv);
	err = socket_recv(sock, data, count, got, &tv, MSG_PEEK);
	return err;
}

void i_socket_destroy(p_socket ps)
{
	socket_destroy(ps);
}

int i_socket_select(p_socket sock, int sw, float delay)
{
	int err = IO_DONE;
	struct timeval tv; second_timeval(delay, &tv);
	err = socket_waitfd(sock, sw, &tv);
	return err;
}

// ==================== package ==================== //

int inet_trycreate(p_socket ps,int inettype, int type) {//qiuhe
    return socket_create(ps, inettype, type, 0);//qiuhe
}


int inet_tryconnect(p_socket ps, const char *address, unsigned short port, struct timeval* tm)
{
    //////////////////////////////qiuhe
    
    struct addrinfo *result;
    struct addrinfo *res;
    
    struct addrinfo addrCriteria;
    memset(&addrCriteria,0,sizeof(addrCriteria));
    
    char t[256];
    sprintf(t, "%u", port);
    int error = getaddrinfo(address, t, &addrCriteria, &result);
    if (error == 0)
    {
        struct sockaddr_in *sa;
        for (res = result; res != NULL; res = res->ai_next)
        {
            if (AF_INET6 == res->ai_addr->sa_family)
            {
                char buf[128] = {};
                sa = (struct sockaddr_in*)res->ai_addr;
                inet_ntop(AF_INET6, &((reinterpret_cast<struct sockaddr_in6*>(sa))->sin6_addr), buf, 128);
                int err;
                struct sockaddr_in6 svraddr;
                memset(&svraddr, 0, sizeof(svraddr));
                svraddr.sin6_family = AF_INET6;
                svraddr.sin6_port = htons(port);
                if (inet_pton(AF_INET6,buf,&svraddr.sin6_addr) < 0)
                {
                    
                }
                err = socket_connect(ps, (sockaddr *) &svraddr, sizeof(svraddr), tm);
                return err;
                
                break;
            }
            else if (AF_INET == res->ai_addr->sa_family)
            {
            }
        }
    }
    
    
    ////////////////////////////////qiuhe
	struct sockaddr_in remote;
	int err;
	memset(&remote, 0, sizeof(remote));
	remote.sin_family = AF_INET;
	remote.sin_port = htons(port);
	if (strcmp(address, "*")) {
		if (!inet_aton(address, &remote.sin_addr)) {
			struct hostent *hp = NULL;
			struct in_addr **addr;
			err = socket_gethostbyname(address, &hp);
			if (err != IO_DONE) return err; //socket_hoststrerror(err);
			addr = (struct in_addr **) hp->h_addr_list;
			memcpy(&remote.sin_addr, *addr, sizeof(struct in_addr));
		}
	} else remote.sin_family = AF_UNSPEC;
	err = socket_connect(ps, (sockaddr *) &remote, sizeof(remote), tm);
	return err;
}

// ==================== platform cross ==================== //

#define WAITFD_R        1
#define WAITFD_W        2
#define WAITFD_E        4
#define WAITFD_C        (WAITFD_E|WAITFD_W)

#define timeout_iszero(tm)  ( tm == NULL || (tm->tv_sec == 0 && tm->tv_usec == 0) )

int socket_waitfd(p_socket ps, int sw, struct timeval* tm) {

#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
	int ret;
	fd_set rfds, wfds, efds, *rp = NULL, *wp = NULL, *ep = NULL;
	
	if (timeout_iszero(tm)) return IO_TIMEOUT;  /* optimize timeout == 0 case */
	if (sw & WAITFD_R) { 
		FD_ZERO(&rfds); 
		FD_SET(*ps, &rfds);
		rp = &rfds; 
	}
	if (sw & WAITFD_W) { FD_ZERO(&wfds); FD_SET(*ps, &wfds); wp = &wfds; }
	if (sw & WAITFD_C) { FD_ZERO(&efds); FD_SET(*ps, &efds); ep = &efds; }
	
	ret = select(0, rp, wp, ep, tm);
	if (ret == -1) return WSAGetLastError();
	if (ret == 0) return IO_TIMEOUT;
	if (sw == WAITFD_C && FD_ISSET(*ps, &efds)) return IO_CLOSED;
	return IO_DONE;

#else
    int ret;
	struct pollfd pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.fd = *ps;
	pfd.events = POLLERR;

	if (sw & WAITFD_R) { pfd.events |= POLLIN; }
	if (sw & WAITFD_W) { pfd.events |= POLLOUT;}
	ret = poll(&pfd, 1, tm->tv_sec*1000 + tm->tv_usec/1000);

	if (ret == 0) return IO_TIMEOUT;
	if (ret < 0)
	{
		if (errno == EAGAIN) 
			return IO_TIMEOUT;
		else
			return errno;
	}

	return IO_DONE;


// 	int ret;
// 	fd_set rfds, wfds, *rp, *wp;
// 
// 	if (timeout_iszero(tm)) return IO_TIMEOUT;  /* optimize timeout == 0 case */
// 	do {
// 		/* must set bits within loop, because select may have modifed them */
// 		rp = wp = NULL;
// 		if (sw & WAITFD_R) { FD_ZERO(&rfds); FD_SET(*ps, &rfds); rp = &rfds; }
// 		if (sw & WAITFD_W) { FD_ZERO(&wfds); FD_SET(*ps, &wfds); wp = &wfds; }
// 		
// 		ret = select(*ps+1, rp, wp, NULL, tm);
// 	} while (ret == -1 && errno == EINTR);
// 	if (ret == -1) return errno;
// 	if (ret == 0) return IO_TIMEOUT;
// 	if (sw == WAITFD_C && FD_ISSET(*ps, &rfds)) return IO_CLOSED;
// 	return IO_DONE;

#endif
}


void socket_setblocking(p_socket ps) {
#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
	u_long argp = 0;
	ioctlsocket(*ps, FIONBIO, &argp);
#else
	int flags = fcntl(*ps, F_GETFL, 0);
	flags &= (~(O_NONBLOCK));
	fcntl(*ps, F_SETFL, flags);
#endif
}


void socket_setnonblocking(p_socket ps) {

#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
	u_long argp = 1;
	ioctlsocket(*ps, FIONBIO, &argp);
#else
	int flags = fcntl(*ps, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(*ps, F_SETFL, flags);
#endif
}



int socket_open(void) {
#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0); 
	int err = WSAStartup(wVersionRequested, &wsaData );
	if (err != 0) return 0;
	if ((LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0) &&
		(LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)) {
			WSACleanup();
			return 0; 
	}
#else
	signal(SIGPIPE, SIG_IGN);
#endif
	return 1;
}

int socket_close(void) {
#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
	WSACleanup();
#endif
	return 1;
}

void socket_destroy(p_socket ps) {
	if (*ps != SOCKET_INVALID) {
#if ( CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID) /* an endless block may occur on android platform */
		socket_setblocking(ps); /* close can take a long time on WIN32 */
#endif

#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
		closesocket(*ps);
#else
		close(*ps);
#endif
		*ps = SOCKET_INVALID;
	}
}

int socket_create(p_socket ps, int domain, int type, int protocol)
{
	*ps = socket(domain, type, protocol);
	if (*ps != SOCKET_INVALID) return IO_DONE;

#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
	return WSAGetLastError();
#else
	return errno; 
#endif
}


int socket_connect(p_socket ps, sockaddr *addr, socklen_t len, struct timeval* tm) 
{
    int err;
    /* don't call on closed socket */
    if (*ps == SOCKET_INVALID) return IO_CLOSED;

#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
	/* ask system to connect */
	if (connect(*ps, addr, len) == 0) return IO_DONE;
	/* make sure the system is trying to connect */
	err = WSAGetLastError();
	if (err != WSAEWOULDBLOCK && err != WSAEINPROGRESS) return err;
#else
	/* call connect until done or failed without being interrupted */
	do if (connect(*ps, addr, len) == 0) return IO_DONE;
	while ((err = errno) == EINTR);
	/* if connection failed immediately, return error code */
	if (err != EINPROGRESS && err != EAGAIN) return err; 
#endif

    return socket_connect_wait(ps, tm);
}

int socket_connect_wait(p_socket ps, struct timeval* tm)
{
	int err;
	/* don't call on closed socket */
	if (*ps == SOCKET_INVALID) return IO_CLOSED;

	 /* we wait until something happens */
    err = socket_waitfd(ps, WAITFD_C, tm);
    if (err == IO_CLOSED) {

#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  )
        int len = sizeof(err);
        /* give windows time to set the error (yes, disgusting) */
        Sleep(10);
        /* find out why we failed */
        getsockopt(*ps, SOL_SOCKET, SO_ERROR, (char *)&err, &len); 
        /* we KNOW there was an error. if 'why' is 0, we will return
        * "unknown error", but it's not really our fault */
        return err > 0? err: IO_UNKNOWN; 
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
		int len = sizeof(err);
		/* give windows time to set the error (yes, disgusting) */
		Concurrency::wait(10);	
        /* find out why we failed */
        getsockopt(*ps, SOL_SOCKET, SO_ERROR, (char *)&err, &len); 
        /* we KNOW there was an error. if 'why' is 0, we will return
        * "unknown error", but it's not really our fault */
        return err > 0? err: IO_UNKNOWN; 
#else
		if (recv(*ps, (char *) &err, 0, 0) == 0) return IO_DONE;
		else return errno;
#endif

    } else return err;

}


int socket_send(p_socket ps, const char *data, size_t count, size_t *sent, struct timeval* tm)
{
	int err;
	*sent = 0;
	/* avoid making system calls on closed sockets */
	if (*ps == SOCKET_INVALID) return IO_CLOSED;
	/* loop until we send something or we give up on error */
	for ( ;; ) {
#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
		/* try to send something */
		int put = send(*ps, data, (int) count, 0);
#else
		long put = (long) send(*ps, data, count, 0);
#endif
		/* if we sent something, we are done */
		if (put > 0) {
			*sent = put;
			return IO_DONE;
		}

#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
		/* deal with failure */
		err = WSAGetLastError(); 
		/* we can only proceed if there was no serious error */
		if (err != WSAEWOULDBLOCK) return err;
		/* avoid busy wait */
		if ((err = socket_waitfd(ps, WAITFD_W, tm)) != IO_DONE) return err;
#else
		err = errno;
        /* send can't really return 0, but EPIPE means the connection was 
           closed */
        if (put == 0 || err == EPIPE) return IO_CLOSED;
        /* we call was interrupted, just try again */
        if (err == EINTR) continue;
        /* if failed fatal reason, report error */
        if (err != EAGAIN) return err;
        /* wait until we can send something or we timeout */
        if ((err = socket_waitfd(ps, WAITFD_W, tm)) != IO_DONE) return err;
#endif
	} 
	/* can't reach here */
	return IO_UNKNOWN;
}


int socket_recv(p_socket ps, char *data, size_t count, size_t *got, struct timeval* tm, int flags) 
{
	int err;
	*got = 0;
	if (*ps == SOCKET_INVALID) return IO_CLOSED;
	for ( ;; ) {
#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
		int taken = recv(*ps, data, (int) count, flags);
#else
		long taken = (long) recv(*ps, data, count, flags);
#endif
		if (taken > 0) {
			*got = taken;
			return IO_DONE;
		}

#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
		if (taken == 0) return IO_CLOSED;
		err = WSAGetLastError();
		if (err != WSAEWOULDBLOCK) return err;
		if ((err = socket_waitfd(ps, WAITFD_R, tm)) != IO_DONE) return err;

#else
		err = errno;
		if (taken == 0) return IO_CLOSED;
		if (err == EINTR) continue;
		if (err != EAGAIN) return err; 
		if ((err = socket_waitfd(ps, WAITFD_R, tm)) != IO_DONE) return err; 
#endif
	}
	return IO_UNKNOWN;

}


int opt_keepalive(p_socket ps,int interval)
{
	return setsockopt(*ps, SOL_SOCKET, SO_KEEPALIVE, (char *) &interval, sizeof(interval));
}

int opt_nosigpipe(p_socket ps, bool onOff)
{
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
#define SO_NOSIGPIPE MSG_NOSIGNAL
#endif

#ifndef WIN32
	int nosigpipe = onOff ? 1 : 0;
	return setsockopt(*ps, SOL_SOCKET, SO_NOSIGPIPE, &nosigpipe, sizeof(nosigpipe));
#endif
	return 0;
}

int opt_linger(p_socket ps, bool onOff, int linger)
{
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ) // an endless calling error may occur when close called on android 
	return 0;
#else
	struct linger so_linger;
	so_linger.l_onoff = onOff ? 1 : 0;
	so_linger.l_linger = linger;
	return setsockopt(*ps, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));
#endif
}


int socket_gethostbyaddr(const char *addr, socklen_t len, struct hostent **hp) {
	*hp = gethostbyaddr(addr, len, AF_INET);
	if (*hp) return IO_DONE;
#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
	else return WSAGetLastError();
#else	
	else if (h_errno) return h_errno;
	else if (errno) return errno;
	else return IO_UNKNOWN;
#endif
}

int socket_gethostbyname(const char *addr, struct hostent **hp) {
	*hp = gethostbyname(addr);
	if (*hp) return IO_DONE;
#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
	else return  WSAGetLastError();
#else
	else if (h_errno) return h_errno;
	else if (errno) return errno;
	else return IO_UNKNOWN;
#endif
}



const char *socket_hoststrerror(int err) {
	if (err <= 0) return io_strerror(err);
	switch (err) {
#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
	case WSAHOST_NOT_FOUND: return "host not found";
	default: return wstrerror(err); 
#else
	case HOST_NOT_FOUND: return "host not found";
	default: return hstrerror(err);
#endif
	}
}

const char *socket_strerror(int err) {
	if (err <= 0) return io_strerror(err);
	switch (err) {
#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
	case WSAEADDRINUSE: return "address already in use";
	case WSAECONNREFUSED: return "connection refused";
	case WSAEISCONN: return "already connected";
	case WSAEACCES: return "permission denied";
	case WSAECONNABORTED: return "closed";
	case WSAECONNRESET: return "closed";
	case WSAETIMEDOUT: return "timeout";
	default: return wstrerror(err);

#else
	case EADDRINUSE: return "address already in use";
	case EISCONN: return "already connected";
	case EACCES: return "permission denied";
	case ECONNREFUSED: return "connection refused";
	case ECONNABORTED: return "closed";
	case ECONNRESET: return "closed";
	case ETIMEDOUT: return "timeout";
	default: return strerror(errno);

#endif
	}
}


const char *io_strerror(int err) {
	switch (err) {
	case IO_DONE: return NULL;
	case IO_CLOSED: return "closed";
	case IO_TIMEOUT: return "timeout";
	default: return "unknown error"; 
	}
}

#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )

static const char *wstrerror(int err) {
	switch (err) {
	case WSAEINTR: return "Interrupted function call";
	case WSAEACCES: return "Permission denied";
	case WSAEFAULT: return "Bad address";
	case WSAEINVAL: return "Invalid argument";
	case WSAEMFILE: return "Too many open files";
	case WSAEWOULDBLOCK: return "Resource temporarily unavailable";
	case WSAEINPROGRESS: return "Operation now in progress";
	case WSAEALREADY: return "Operation already in progress";
	case WSAENOTSOCK: return "Socket operation on nonsocket";
	case WSAEDESTADDRREQ: return "Destination address required";
	case WSAEMSGSIZE: return "Message too long";
	case WSAEPROTOTYPE: return "Protocol wrong type for socket";
	case WSAENOPROTOOPT: return "Bad protocol option";
	case WSAEPROTONOSUPPORT: return "Protocol not supported";
	case WSAESOCKTNOSUPPORT: return "Socket type not supported";
	case WSAEOPNOTSUPP: return "Operation not supported";
	case WSAEPFNOSUPPORT: return "Protocol family not supported";
	case WSAEAFNOSUPPORT: 
		return "Address family not supported by protocol family"; 
	case WSAEADDRINUSE: return "Address already in use";
	case WSAEADDRNOTAVAIL: return "Cannot assign requested address";
	case WSAENETDOWN: return "Network is down";
	case WSAENETUNREACH: return "Network is unreachable";
	case WSAENETRESET: return "Network dropped connection on reset";
	case WSAECONNABORTED: return "Software caused connection abort";
	case WSAECONNRESET: return "Connection reset by peer";
	case WSAENOBUFS: return "No buffer space available";
	case WSAEISCONN: return "Socket is already connected";
	case WSAENOTCONN: return "Socket is not connected";
	case WSAESHUTDOWN: return "Cannot send after socket shutdown";
	case WSAETIMEDOUT: return "Connection timed out";
	case WSAECONNREFUSED: return "Connection refused";
	case WSAEHOSTDOWN: return "Host is down";
	case WSAEHOSTUNREACH: return "No route to host";
	case WSAEPROCLIM: return "Too many processes";
	case WSASYSNOTREADY: return "Network subsystem is unavailable";
	case WSAVERNOTSUPPORTED: return "Winsock.dll version out of range";
	case WSANOTINITIALISED: 
		return "Successful WSAStartup not yet performed";
	case WSAEDISCON: return "Graceful shutdown in progress";
	case WSAHOST_NOT_FOUND: return "Host not found";
	case WSATRY_AGAIN: return "Nonauthoritative host not found";
	case WSANO_RECOVERY: return "Nonrecoverable name lookup error"; 
	case WSANO_DATA: return "Valid name, no data record of requested type";
	default: return "Unknown error";
	}
}
#endif


#ifdef INET_ATON
int inet_aton(const char *cp, struct in_addr *inp)
{
	unsigned int a = 0, b = 0, c = 0, d = 0;
	int n = 0, r;
	unsigned long int addr = 0;
	r = sscanf(cp, "%u.%u.%u.%u%n", &a, &b, &c, &d, &n);
	if (r == 0 || n == 0) return 0;
	cp += n;
	if (*cp) return 0;
	if (a > 255 || b > 255 || c > 255 || d > 255) return 0;
	if (inp) {
		addr += a; addr <<= 8;
		addr += b; addr <<= 8;
		addr += c; addr <<= 8;
		addr += d;
		inp->s_addr = htonl(addr);
	}
	return 1;
}
#endif

int xint(int i)
{
	char s1[4];
	char s2[4];
	memcpy(s1,&i,4);
	s2[0]=s1[3];
	s2[1]=s1[2];
	s2[2]=s1[1];
	s2[3]=s1[0];
	int r;
	memcpy(&r,s2,4);
	return r;
}
void xlong(long long &i)
{
	char s1[8];
	char s2[8];
    
	memset( s1, 0 , sizeof( s1 ) );
	memset( s2, 0 , sizeof( s2 ) );
    
    
	memcpy(s1,&i,8);
	s2[0]=s1[7];
	s2[1]=s1[6];
	s2[2]=s1[5];
	s2[3]=s1[4];
	s2[4]=s1[3];
	s2[5]=s1[2];
	s2[6]=s1[1];
	s2[7]=s1[0];
	memcpy(&i,s2,8);
    
}
short xshort(short i)
{
	char s1[2];
	char s2[2];
	memcpy(s1,&i,2);
	s2[0]=s1[1];
	s2[1]=s1[0];
	short r;
	memcpy(&r,s2,2);
	return r;
}
