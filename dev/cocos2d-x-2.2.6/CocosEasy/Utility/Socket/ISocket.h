#ifndef __CE_ISOCKET_H__
#define __CE_ISOCKET_H__

#include "Common/CocosEasyDef.h"

#ifndef SOCKET 
#define SOCKET int
#endif

enum {
	IO_DONE = 0,        /* operation completed successfully */
	IO_TIMEOUT = -1,    /* operation timed out */
	IO_CLOSED = -2,     /* the connection has been closed */
	IO_UNKNOWN = -3
};


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#include <WinSock.h>
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#include <WinSock2.h>
#endif


#if ( (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) )
/* error codes */
#include <errno.h>
/* close function */
#include <unistd.h>
/* fnctnl function and associated constants */
#include <fcntl.h>
/* struct sockaddr */
#include <sys/types.h>
/* socket function */
#include <sys/socket.h>
/* struct timeval */
#include <sys/time.h>
/* gethostbyname and gethostbyaddr functions */
#include <netdb.h>
/* sigpipe handling */
#include <signal.h>
/* IP stuff*/
#include <netinet/in.h>
#include <arpa/inet.h>
/* TCP options (nagle algorithm disable) */
#include <netinet/tcp.h>
/* use poll instead of select*/
#include <poll.h>
#endif

#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
typedef int socklen_t;
typedef SOCKET t_socket;
typedef t_socket *p_socket;
#define SOCKET_INVALID (INVALID_SOCKET)
#else
typedef int t_socket;
typedef t_socket *p_socket;
#define SOCKET_INVALID (-1)
#endif

#ifdef _WIN32
#define INET_ATON
#endif

#if ( (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) )
static const char *wstrerror(int err);
#endif

// ==================== interface ==================== //
int i_socket_open(void);
int i_socket_close(void);
int i_socket_create(p_socket ps,int inettype,int type);//qiuhe
int i_socket_connect(p_socket sock, const char *address, unsigned short port, float delay);
int i_socket_connect_wait(p_socket sock, float delay);
int i_socket_send(p_socket sock, const char *data, unsigned short start, unsigned int end, size_t *sent, float delay);
int i_socket_recv(p_socket sock, char *data, size_t count, size_t *got, float delay);
int i_socket_seek(p_socket sock, char *data, size_t count, size_t *got, float delay);
void i_socket_destroy(p_socket ps);
int i_socket_select(p_socket sock, int sw, float delay);

// ==================== package ==================== //
int inet_trycreate(p_socket ps,int inettype, int type);//qiuhe
int inet_tryconnect(p_socket ps, const char *address, unsigned short port, struct timeval* tm);
							
// ==================== platform cross ==================== //
int socket_waitfd(p_socket ps, int sw, struct timeval* tm);
void socket_setblocking(p_socket ps);
void socket_setnonblocking(p_socket ps);

int socket_open(void);
int socket_close(void);
void socket_destroy(p_socket ps);
int socket_create(p_socket ps, int domain, int type, int protocol);
int socket_connect(p_socket ps, sockaddr *addr, socklen_t addr_len, struct timeval* tm);
int socket_connect_wait(p_socket ps, struct timeval* tm);
int socket_send(p_socket ps, const char *data, size_t count, size_t *sent, struct timeval* tm);
int socket_recv(p_socket ps, char *data, size_t count, size_t *got, struct timeval* tm, int flags = 0);

int opt_keepalive(p_socket ps,int interval);
int opt_nosigpipe(p_socket ps,bool onOff);
int opt_linger(p_socket ps,bool onOff, int linger);

int socket_gethostbyaddr(const char *addr, socklen_t len, struct hostent **hp);
int socket_gethostbyname(const char *addr, struct hostent **hp);

const char *socket_hoststrerror(int err);
const char *socket_strerror(int err);
const char *io_strerror(int err);

#ifdef INET_ATON
int inet_aton(const char *cp, struct in_addr *inp);
#endif

int xint(int i);
void xlong(long long &i);
short xshort(short i);


#endif