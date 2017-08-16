

#include "oi_tea.h"


/*
	void TeaEncryptECB(BYTE *pInBuf, BYTE *pKey, BYTE *pOutBuf);
	void TeaDecryptECB(BYTE *pInBuf, BYTE *pKey, BYTE *pOutBuf);
*/

typedef unsigned int WORD32;

const WORD32 DELTA = 0x9e3779b9;

#define ROUNDS 16
#define LOG_ROUNDS 4


/*pOutBuffer、pInBuffer均为8byte, pKey为16byte*/
void TeaEncryptECB(const BYTE *pInBuf, const BYTE *pKey, BYTE *pOutBuf)
{
	WORD32 y, z;
	WORD32 sum;
	WORD32 k[4];
	int i;

	/*plain-text is TCP/IP-endian;*/

	/*GetBlockBigEndian(in, y, z);*/
	y = ntohl(*((WORD32*)pInBuf));
	z = ntohl(*((WORD32*)(pInBuf+4)));
	/*TCP/IP network byte order (which is big-endian).*/

	for ( i = 0; i<4; i++)
	{
		/*now key is TCP/IP-endian;*/
		k[i] = ntohl(*((WORD32*)(pKey+i*4)));
	}

	sum = 0;
	for (i=0; i<ROUNDS; i++)
	{   
		sum += DELTA;
		y += (z << 4) + k[0] ^ z + sum ^ (z >> 5) + k[1];
		z += (y << 4) + k[2] ^ y + sum ^ (y >> 5) + k[3];
	}



	*((WORD32*)pOutBuf) = htonl(y);
	*((WORD32*)(pOutBuf+4)) = htonl(z);
	

	/*now encrypted buf is TCP/IP-endian;*/
}

/*pOutBuffer、pInBuffer均为8byte, pKey为16byte*/
void TeaDecryptECB(const BYTE *pInBuf, const BYTE *pKey, BYTE *pOutBuf)
{
	WORD32 y, z, sum;
	WORD32 k[4];
	int i;

	/*now encrypted buf is TCP/IP-endian;*/
	/*TCP/IP network byte order (which is big-endian).*/
	y = ntohl(*((WORD32*)pInBuf));
	z = ntohl(*((WORD32*)(pInBuf+4)));

	for ( i=0; i<4; i++)
	{
		/*key is TCP/IP-endian;*/
		k[i] = ntohl(*((WORD32*)(pKey+i*4)));
	}

	sum = DELTA << LOG_ROUNDS;
	for (i=0; i<ROUNDS; i++)
	{
		z -= (y << 4) + k[2] ^ y + sum ^ (y >> 5) + k[3]; 
		y -= (z << 4) + k[0] ^ z + sum ^ (z >> 5) + k[1];
		sum -= DELTA;
	}

	*((WORD32*)pOutBuf) = htonl(y);
	*((WORD32*)(pOutBuf+4)) = htonl(z);

	/*now plain-text is TCP/IP-endian;*/
}


/*pOutBuffer、pInBuffer均为8byte, pKey为16byte*/
void TeaEncryptECB3(const BYTE *pInBuf, const BYTE *pKey, BYTE *pOutBuf)
{
	WORD32 y, z;
	WORD32 sum;
	WORD32 k[4];
	int i;

	/*plain-text is TCP/IP-endian;*/

	/*GetBlockBigEndian(in, y, z);*/
	y = ntohl(*((WORD32*)pInBuf));
	z = ntohl(*((WORD32*)(pInBuf+4)));
	/*TCP/IP network byte order (which is big-endian).*/

	for ( i = 0; i<4; i++)
	{
		/*now key is TCP/IP-endian;*/
		k[i] = ntohl(*((WORD32*)(pKey+i*4)));
	}

	sum = 0;
	for (i=0; i<13; i++)
	{   
		sum += DELTA;
		y += (z << 4) + k[0] ^ z + sum ^ (z >> 5) + k[1];
		z += (y << 4) + k[2] ^ y + sum ^ (y >> 5) + k[3];
	}



	*((WORD32*)pOutBuf) = htonl(y);
	*((WORD32*)(pOutBuf+4)) = htonl(z);
	

	/*now encrypted buf is TCP/IP-endian;*/
}

/*pOutBuffer、pInBuffer均为8byte, pKey为16byte*/
void TeaDecryptECB3(const BYTE *pInBuf, const BYTE *pKey, BYTE *pOutBuf)
{
	WORD32 y, z, sum;
	WORD32 k[4];
	int i;

	/*now encrypted buf is TCP/IP-endian;*/
	/*TCP/IP network byte order (which is big-endian).*/
	y = ntohl(*((WORD32*)pInBuf));
	z = ntohl(*((WORD32*)(pInBuf+4)));

	for ( i=0; i<4; i++)
	{
		/*key is TCP/IP-endian;*/
		k[i] = ntohl(*((WORD32*)(pKey+i*4)));
	}

	/*13=8+5*/
	sum = DELTA << 3;
	for (i=1; i<=5; i++)
	{
		sum += DELTA;
	}


	for (i=0; i<13; i++)
	{
		z -= (y << 4) + k[2] ^ y + sum ^ (y >> 5) + k[3]; 
		y -= (z << 4) + k[0] ^ z + sum ^ (z >> 5) + k[1];
		sum -= DELTA;
	}

	*((WORD32*)pOutBuf) = htonl(y);
	*((WORD32*)(pOutBuf+4)) = htonl(z);

	/*now plain-text is TCP/IP-endian;*/
}




#define SALT_LEN 2
#define ZERO_LEN 7

/*pKey为16byte*/
/*
	输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
	输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数;
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
void oi_symmetry_encrypt(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen)
{
	
	int nPadSaltBodyZeroLen/*PadLen(1byte)+Salt+Body+Zero的长度*/;
	int nPadlen;
	BYTE src_buf[8], zero_iv[8], *iv_buf;
	int src_i, i, j;

	/*根据Body长度计算PadLen,最小必需长度必需为8byte的整数倍*/
	nPadSaltBodyZeroLen = nInBufLen/*Body长度*/+1+SALT_LEN+ZERO_LEN/*PadLen(1byte)+Salt(2byte)+Zero(7byte)*/;
	if((nPadlen=nPadSaltBodyZeroLen%8) != 0) /*len=nSaltBodyZeroLen%8*/
	{
		/*模8余0需补0,余1补7,余2补6,...,余7补1*/
		nPadlen=8-nPadlen;
	}

	/*srand( (unsigned)time( NULL ) ); 初始化随机数*/
	/*加密第一块数据(8byte),取前面10byte*/
	src_buf[0] = ((BYTE)rand()) & 0x0f8/*最低三位存PadLen,清零*/ | (BYTE)nPadlen;
	src_i = 1; /*src_i指向src_buf下一个位置*/

	while(nPadlen--)
		src_buf[src_i++]=(BYTE)rand(); /*Padding*/

	/*come here, i must <= 8*/

	memset(zero_iv, 0, 8);
	iv_buf = zero_iv; /*make iv*/

	*pOutBufLen = 0; /*init OutBufLen*/

	for (i=1;i<=SALT_LEN;) /*Salt(2byte)*/
	{
		if (src_i<8)
		{
			src_buf[src_i++]=(BYTE)rand();
			i++; /*i inc in here*/
		}

		if (src_i==8)
		{
			/*src_i==8*/
			
			for (j=0;j<8;j++) /*CBC XOR*/
				src_buf[j]^=iv_buf[j];
			/*pOutBuffer、pInBuffer均为8byte, pKey为16byte*/
			TeaEncryptECB(src_buf, pKey, pOutBuf);
			src_i=0;
			iv_buf=pOutBuf;
			*pOutBufLen+=8;
			pOutBuf+=8;
		}
	}

	/*src_i指向src_buf下一个位置*/

	while(nInBufLen)
	{
		if (src_i<8)
		{
			src_buf[src_i++]=*(pInBuf++);
			nInBufLen--;
		}

		if (src_i==8)
		{
			/*src_i==8*/
			
			for (i=0;i<8;i++) /*CBC XOR*/
				src_buf[i]^=iv_buf[i];
			/*pOutBuffer、pInBuffer均为8byte, pKey为16byte*/
			TeaEncryptECB(src_buf, pKey, pOutBuf);
			src_i=0;
			iv_buf=pOutBuf;
			*pOutBufLen+=8;
			pOutBuf+=8;
		}
	}

	/*src_i指向src_buf下一个位置*/

	for (i=1;i<=ZERO_LEN;)
	{
		if (src_i<8)
		{
			src_buf[src_i++]=0;
			i++; /*i inc in here*/
		}

		if (src_i==8)
		{
			/*src_i==8*/
			
			for (j=0;j<8;j++) /*CBC XOR*/
				src_buf[j]^=iv_buf[j];
			/*pOutBuffer、pInBuffer均为8byte, pKey为16byte*/
			TeaEncryptECB(src_buf, pKey, pOutBuf);
			src_i=0;
			iv_buf=pOutBuf;
			*pOutBufLen+=8;
			pOutBuf+=8;
		}
	}

}

/*pKey为16byte*/
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数;
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
BOOL oi_symmetry_decrypt(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen)
{

	int nPadLen, nPlainLen;
	BYTE dest_buf[8];
	const BYTE *iv_buf;
	int dest_i, i, j;

	
	if ((nInBufLen%8) || (nInBufLen<16)) return FALSE;
	

	TeaDecryptECB(pInBuf, pKey, dest_buf);

	nPadLen = dest_buf[0] & 0x7/*只要最低三位*/;

	/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
	i = nInBufLen-1/*PadLen(1byte)*/-nPadLen-SALT_LEN-ZERO_LEN; /*明文长度*/
	if (*pOutBufLen<i) return FALSE;
	*pOutBufLen = i;
	if (*pOutBufLen < 0) return FALSE;
	

	iv_buf = pInBuf; /*init iv*/
	nInBufLen -= 8;
	pInBuf += 8;

	dest_i=1; /*dest_i指向dest_buf下一个位置*/


	/*把Padding滤掉*/
	dest_i+=nPadLen;

	/*dest_i must <=8*/

	/*把Salt滤掉*/
	for (i=1; i<=SALT_LEN;)
	{
		if (dest_i<8)
		{
			dest_i++;
			i++;
		}

		if (dest_i==8)
		{
			/*dest_i==8*/
			TeaDecryptECB(pInBuf, pKey, dest_buf);
			for (j=0; j<8; j++)
				dest_buf[j]^=iv_buf[j];
		
			iv_buf = pInBuf;
			nInBufLen -= 8;
			pInBuf += 8;
	
			dest_i=0; /*dest_i指向dest_buf下一个位置*/
		}
	}

	/*还原明文*/

	nPlainLen=*pOutBufLen;
	while (nPlainLen)
	{
		if (dest_i<8)
		{
			*(pOutBuf++)=dest_buf[dest_i++];
			nPlainLen--;
		}
		else if (dest_i==8)
		{
			/*dest_i==8*/
			TeaDecryptECB(pInBuf, pKey, dest_buf);
			for (i=0; i<8; i++)
				dest_buf[i]^=iv_buf[i];
		
			iv_buf = pInBuf;
			nInBufLen -= 8;
			pInBuf += 8;
	
			dest_i=0; /*dest_i指向dest_buf下一个位置*/
		}
	}

	/*校验Zero*/
	for (i=1;i<=ZERO_LEN;)
	{
		if (dest_i<8)
		{
			if(dest_buf[dest_i++]) return FALSE;
			i++;
		}
		else if (dest_i==8)
		{
			/*dest_i==8*/
			TeaDecryptECB(pInBuf, pKey, dest_buf);
			for (j=0; j<8; j++)
				dest_buf[j]^=iv_buf[j];
		
			iv_buf = pInBuf;
			nInBufLen -= 8;
			pInBuf += 8;
	
			dest_i=0; /*dest_i指向dest_buf下一个位置*/
		}
	
	}

	return TRUE;
}

/* ///////////////////////////////////////////////////////////////////////////////////////////// */

/*pKey为16byte*/
/*
	输入:nInBufLen为需加密的明文部分(Body)长度;
	输出:返回为加密后的长度(是8byte的倍数);
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
int oi_symmetry_encrypt2_len(int nInBufLen)
{
	
	int nPadSaltBodyZeroLen/*PadLen(1byte)+Salt+Body+Zero的长度*/;
	int nPadlen;

	/*根据Body长度计算PadLen,最小必需长度必需为8byte的整数倍*/
	nPadSaltBodyZeroLen = nInBufLen/*Body长度*/+1+SALT_LEN+ZERO_LEN/*PadLen(1byte)+Salt(2byte)+Zero(7byte)*/;
	if((nPadlen=nPadSaltBodyZeroLen%8) != 0) /*len=nSaltBodyZeroLen%8*/
	{
		/*模8余0需补0,余1补7,余2补6,...,余7补1*/
		nPadlen=8-nPadlen;
	}

	return nPadSaltBodyZeroLen+nPadlen;
}


/*pKey为16byte*/
/*
	输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
	输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数;
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
void oi_symmetry_encrypt2(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen)
{
	
	int nPadSaltBodyZeroLen/*PadLen(1byte)+Salt+Body+Zero的长度*/;
	int nPadlen;
	BYTE src_buf[8], iv_plain[8], *iv_crypt;
	int src_i, i, j;

	/*根据Body长度计算PadLen,最小必需长度必需为8byte的整数倍*/
	nPadSaltBodyZeroLen = nInBufLen/*Body长度*/+1+SALT_LEN+ZERO_LEN/*PadLen(1byte)+Salt(2byte)+Zero(7byte)*/;
	if((nPadlen=nPadSaltBodyZeroLen%8) != 0) /*len=nSaltBodyZeroLen%8*/
	{
		/*模8余0需补0,余1补7,余2补6,...,余7补1*/
		nPadlen=8-nPadlen;
	}

	/*srand( (unsigned)time( NULL ) ); 初始化随机数*/
	/*加密第一块数据(8byte),取前面10byte*/
	src_buf[0] = ((BYTE)rand()) & 0x0f8/*最低三位存PadLen,清零*/ | (BYTE)nPadlen;
	src_i = 1; /*src_i指向src_buf下一个位置*/

	while(nPadlen--)
		src_buf[src_i++]=(BYTE)rand(); /*Padding*/

	/*come here, src_i must <= 8*/

	for ( i=0; i<8; i++)
		iv_plain[i] = 0;
	iv_crypt = iv_plain; /*make zero iv*/

	*pOutBufLen = 0; /*init OutBufLen*/

	for (i=1;i<=SALT_LEN;) /*Salt(2byte)*/
	{
		if (src_i<8)
		{
			src_buf[src_i++]=(BYTE)rand();
			i++; /*i inc in here*/
		}

		if (src_i==8)
		{
			/*src_i==8*/

			for (j=0;j<8;j++) /*加密前异或前8个byte的密文(iv_crypt指向的)*/
				src_buf[j]^=iv_crypt[j];

			/*pOutBuffer、pInBuffer均为8byte, pKey为16byte*/
			/*加密*/
			TeaEncryptECB(src_buf, pKey, pOutBuf);

			for (j=0;j<8;j++) /*加密后异或前8个byte的明文(iv_plain指向的)*/
				pOutBuf[j]^=iv_plain[j];

			/*保存当前的iv_plain*/
			for (j=0;j<8;j++)
				iv_plain[j]=src_buf[j];

			/*更新iv_crypt*/
			src_i=0;
			iv_crypt=pOutBuf;
			*pOutBufLen+=8;
			pOutBuf+=8;
		}
	}

	/*src_i指向src_buf下一个位置*/

	while(nInBufLen)
	{
		if (src_i<8)
		{
			src_buf[src_i++]=*(pInBuf++);
			nInBufLen--;
		}

		if (src_i==8)
		{
			/*src_i==8*/
			
			for (j=0;j<8;j++) /*加密前异或前8个byte的密文(iv_crypt指向的)*/
				src_buf[j]^=iv_crypt[j];
			/*pOutBuffer、pInBuffer均为8byte, pKey为16byte*/
			TeaEncryptECB(src_buf, pKey, pOutBuf);

			for (j=0;j<8;j++) /*加密后异或前8个byte的明文(iv_plain指向的)*/
				pOutBuf[j]^=iv_plain[j];

			/*保存当前的iv_plain*/
			for (j=0;j<8;j++)
				iv_plain[j]=src_buf[j];

			src_i=0;
			iv_crypt=pOutBuf;
			*pOutBufLen+=8;
			pOutBuf+=8;
		}
	}

	/*src_i指向src_buf下一个位置*/

	for (i=1;i<=ZERO_LEN;)
	{
		if (src_i<8)
		{
			src_buf[src_i++]=0;
			i++; /*i inc in here*/
		}

		if (src_i==8)
		{
			/*src_i==8*/
			
			for (j=0;j<8;j++) /*加密前异或前8个byte的密文(iv_crypt指向的)*/
				src_buf[j]^=iv_crypt[j];
			/*pOutBuffer、pInBuffer均为8byte, pKey为16byte*/
			TeaEncryptECB(src_buf, pKey, pOutBuf);

			for (j=0;j<8;j++) /*加密后异或前8个byte的明文(iv_plain指向的)*/
				pOutBuf[j]^=iv_plain[j];

			/*保存当前的iv_plain*/
			for (j=0;j<8;j++)
				iv_plain[j]=src_buf[j];

			src_i=0;
			iv_crypt=pOutBuf;
			*pOutBufLen+=8;
			pOutBuf+=8;
		}
	}

}


/*pKey为16byte*/
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数; *pOutBufLen为接收缓冲区的长度
		特别注意*pOutBufLen应预置接收缓冲区的长度!
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度,至少应预留nInBufLen-10;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
BOOL oi_symmetry_decrypt2(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen)
{

	int nPadLen, nPlainLen;
	BYTE dest_buf[8], zero_buf[8];
	const BYTE *iv_pre_crypt, *iv_cur_crypt;
	int dest_i, i, j;
	const BYTE *pInBufBoundary;
	int nBufPos;
	nBufPos = 0;


	
	if ((nInBufLen%8) || (nInBufLen<16)) return FALSE;
	

	TeaDecryptECB(pInBuf, pKey, dest_buf);

	nPadLen = dest_buf[0] & 0x7/*只要最低三位*/;

	/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
	i = nInBufLen-1/*PadLen(1byte)*/-nPadLen-SALT_LEN-ZERO_LEN; /*明文长度*/
	if ((*pOutBufLen<i) || (i<0)) return FALSE;
	*pOutBufLen = i;
	
	pInBufBoundary = pInBuf + nInBufLen; /*输入缓冲区的边界，下面不能pInBuf>=pInBufBoundary*/

	
	for ( i=0; i<8; i++)
		zero_buf[i] = 0;

	iv_pre_crypt = zero_buf;
	iv_cur_crypt = pInBuf; /*init iv*/

	pInBuf += 8;
	nBufPos += 8;

	dest_i=1; /*dest_i指向dest_buf下一个位置*/


	/*把Padding滤掉*/
	dest_i+=nPadLen;

	/*dest_i must <=8*/

	/*把Salt滤掉*/
	for (i=1; i<=SALT_LEN;)
	{
		if (dest_i<8)
		{
			dest_i++;
			i++;
		}
		else if (dest_i==8)
		{
			/*解开一个新的加密块*/

			/*改变前一个加密块的指针*/
			iv_pre_crypt = iv_cur_crypt;
			iv_cur_crypt = pInBuf; 

			/*异或前一块明文(在dest_buf[]中)*/
			for (j=0; j<8; j++)
			{
				if( (nBufPos + j) >= nInBufLen)
					return FALSE;
				dest_buf[j]^=pInBuf[j];
			}

			/*dest_i==8*/
			TeaDecryptECB(dest_buf, pKey, dest_buf);

			/*在取出的时候才异或前一块密文(iv_pre_crypt)*/

			
			pInBuf += 8;
			nBufPos += 8;
	
			dest_i=0; /*dest_i指向dest_buf下一个位置*/
		}
	}

	/*还原明文*/

	nPlainLen=*pOutBufLen;
	while (nPlainLen)
	{
		if (dest_i<8)
		{
			*(pOutBuf++)=dest_buf[dest_i]^iv_pre_crypt[dest_i];
			dest_i++;
			nPlainLen--;
		}
		else if (dest_i==8)
		{
			/*dest_i==8*/

			/*改变前一个加密块的指针*/
			iv_pre_crypt = iv_cur_crypt;
			iv_cur_crypt = pInBuf; 

			/*解开一个新的加密块*/

			/*异或前一块明文(在dest_buf[]中)*/
			for (j=0; j<8; j++)
			{
				if( (nBufPos + j) >= nInBufLen)
					return FALSE;
				dest_buf[j]^=pInBuf[j];
			}

			TeaDecryptECB(dest_buf, pKey, dest_buf);

			/*在取出的时候才异或前一块密文(iv_pre_crypt)*/
		
			
			pInBuf += 8;
			nBufPos += 8;
	
			dest_i=0; /*dest_i指向dest_buf下一个位置*/
		}
	}

	/*校验Zero*/
	for (i=1;i<=ZERO_LEN;)
	{
		if (dest_i<8)
		{
			if(dest_buf[dest_i]^iv_pre_crypt[dest_i]) return FALSE;
			dest_i++;
			i++;
		}
		else if (dest_i==8)
		{
			/*改变前一个加密块的指针*/
			iv_pre_crypt = iv_cur_crypt;
			iv_cur_crypt = pInBuf; 

			/*解开一个新的加密块*/

			/*异或前一块明文(在dest_buf[]中)*/
			for (j=0; j<8; j++)
			{
				if( (nBufPos + j) >= nInBufLen)
					return FALSE;
				dest_buf[j]^=pInBuf[j];
			}

			TeaDecryptECB(dest_buf, pKey, dest_buf);

			/*在取出的时候才异或前一块密文(iv_pre_crypt)*/

			
			pInBuf += 8;
			nBufPos += 8;
			dest_i=0; /*dest_i指向dest_buf下一个位置*/
		}
	
	}

	return TRUE;
}







