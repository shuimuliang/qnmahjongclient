/*
qqcrypt.h


QQ加密
hyj@tencent.com
1999/12/24
2002/03/27

  实现下列算法:
  Hash算法: MD5,已实现
  对称算法: DES,未实现
  非对称算法: RSA,未实现


2002/06/14
	修改v3解密函数，去掉v3加密函数

*/

#ifndef WIN32 
#ifndef WP8
typedef unsigned char BYTE;
typedef char BOOL;
#endif
#endif

//typedef unsigned long DWORD;
//typedef char DWORD[4];
#define TRUE 1
#define FALSE 0


#ifndef _INCLUDED_OICQCRYPT_H_
#define _INCLUDED_OICQCRYPT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#pragma comment(lib,"Ws2_32.lib")
#else 
#ifdef WP8
#include <Winsock2.h>
#pragma comment(lib,"Ws2_32.lib")
#else
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#endif // WP8
#endif


#define MD5_DIGEST_LENGTH	16
#define ENCRYPT_PADLEN		18
#define	CRYPT_KEY_SIZE		16

#if __cplusplus
extern "C" {
#endif

/************************************************************************************************
	Hash函数
************************************************************************************************/
/*
	输入const BYTE *inBuffer、int length
	输出BYTE *outBuffer
	其中length可为0,outBuffer的长度为MD5_DIGEST_LENGTH(16byte)
*/
//void Md5HashBuffer( BYTE *outBuffer, const BYTE *inBuffer, int length);



/************************************************************************************************
	对称加密底层函数
************************************************************************************************/
//pOutBuffer、pInBuffer均为8byte, pKey为16byte
void TeaEncryptECB(const BYTE *pInBuf, const BYTE *pKey, BYTE *pOutBuf);
void TeaDecryptECB(const BYTE *pInBuf, const BYTE *pKey, BYTE *pOutBuf);
void TeaEncryptECB3(const BYTE *pInBuf, const BYTE *pKey, BYTE *pOutBuf);
void TeaDecryptECB3(const BYTE *pInBuf, const BYTE *pKey, BYTE *pOutBuf);



/************************************************************************************************
	QQ对称加密第一代函数
************************************************************************************************/

/*pKey为16byte*/
/*
	输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
	输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数,至少应预留nInBufLen+17;
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
void oi_symmetry_encrypt(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);

/*pKey为16byte*/
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数; *pOutBufLen为接收缓冲区的长度
		特别注意*pOutBufLen应预置接收缓冲区的长度!
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度,至少应预留nInBufLen-10;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
BOOL oi_symmetry_decrypt(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);







/************************************************************************************************
	QQ对称加密第二代函数
************************************************************************************************/

/*pKey为16byte*/
/*
	输入:nInBufLen为需加密的明文部分(Body)长度;
	输出:返回为加密后的长度(是8byte的倍数);
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
int oi_symmetry_encrypt2_len(int nInBufLen);


/*pKey为16byte*/
/*
	输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
	输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数,至少应预留nInBufLen+17;
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
void oi_symmetry_encrypt2(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);



/*pKey为16byte*/
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数; *pOutBufLen为接收缓冲区的长度
		特别注意*pOutBufLen应预置接收缓冲区的长度!
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度,至少应预留nInBufLen-10;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
BOOL oi_symmetry_decrypt2(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);


/************************************************************************************************
	QQ对称解密第三代函数
************************************************************************************************/

/*pKey为16byte*/
/*
	输入:nInBufLen为需加密的明文部分(Body)长度;
	输出:返回为加密后的长度(是8byte的倍数);
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
//int oi_symmetry_encrypt3_len(int nInBufLen);


/*pKey为16byte*/
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数; *pOutBufLen为接收缓冲区的长度
		特别注意*pOutBufLen应预置接收缓冲区的长度!
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度,至少应预留nInBufLen-10;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
//BOOL qq_symmetry_decrypt3(const BYTE* pInBuf, int nInBufLen, BYTE chMainVer, BYTE chSubVer, DWORD dwUin, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);

#if __cplusplus
}
#endif
#endif // #ifndef _INCLUDED_OICQCRYPT_H_


