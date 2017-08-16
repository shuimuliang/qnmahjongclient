//
//  EncryptKit.cpp
//  client
//
//  Created by liulihua on 14-9-23.
//
//

#include "EncryptKit.h"
#include "Base64.h"
#include "oi_tea.h"
#include <stdlib.h>

//#define SHOW_LOG

#ifdef SHOW_LOG
#define PRINT_HEX(str) do{ \
printf("%s[%d]:",__FUNCTION__,__LINE__);\
if (NULL == str){ printf("NULL\n"); break;}\
for(int i = 0;i < strlen((const char *)str);++i) printf("%2x ",str[i]);\
printf("\n");\
}while(0)
#else
#define PRINT_HEX(str) ((void)0)
#endif


const BYTE *pKey = (const BYTE *)"\x29\x0a\xa2\xbd\x77\xeb\x9d\xed\x4b\x01\x17\x73\xe8\x8f\x0b\x77";

char* CEncryptKit::encryptWithTea(const char* buffer){
    
    if(NULL == buffer) return NULL;
    
    const BYTE *pOriginBuffer = (const BYTE *)buffer;
	int nOriginBufferLength = strlen((const char *)pOriginBuffer);
	int nEncryptBufferLength = oi_symmetry_encrypt2_len(nOriginBufferLength);
    
	BYTE *pEncryptBuffer = (BYTE *)malloc(sizeof(BYTE)*nEncryptBufferLength);
	memset(pEncryptBuffer, 0, sizeof(BYTE)*nEncryptBufferLength);
	
    PRINT_HEX(pOriginBuffer);

	if (pEncryptBuffer)
		oi_symmetry_encrypt2(pOriginBuffer, nOriginBufferLength, pKey, pEncryptBuffer, &nEncryptBufferLength);
	else
		return NULL;
    
    PRINT_HEX(pEncryptBuffer);

	size_t nBase64Length = 0;
	char *pBse64Buffer = base64Encode(pEncryptBuffer, nEncryptBufferLength, true, &nBase64Length);
    
    PRINT_HEX(pBse64Buffer);

	if (pBse64Buffer)
	{
		free(pEncryptBuffer);
	}
	return pBse64Buffer;
}

char* CEncryptKit::decryptWithTea(const char* buffer){
    if (NULL == buffer) return NULL;
    
    size_t outputLength;
    
    PRINT_HEX(buffer);
    
    BYTE *pOriginBuffer = (BYTE *)base64Decode(buffer, strlen((const char *)buffer), &outputLength);
    int nOriginBufferLength =  (int)outputLength;

    PRINT_HEX(pOriginBuffer);

    int nDecryptBufferLength = nOriginBufferLength ;
    BYTE *pDecryptBuffer = (BYTE *)malloc(sizeof(BYTE)*nDecryptBufferLength);
    memset(pDecryptBuffer, 0, sizeof(BYTE)*nDecryptBufferLength);
    
    if (pDecryptBuffer){
        BOOL isSucc = oi_symmetry_decrypt2(pOriginBuffer, nOriginBufferLength, pKey, pDecryptBuffer, &nDecryptBufferLength);
        if (!isSucc){
            printf("tea decode failed");
            return NULL;
        }
    }
    
    PRINT_HEX(pDecryptBuffer);

    if (pOriginBuffer){
        free(pOriginBuffer);
    }
    return (char*)pDecryptBuffer;
}

void CEncryptKit::teaTest(){
    char str[] = "hello world!";
    char* encrptStr = CEncryptKit::encryptWithTea(str);
    
    printf("************  encrptStr : %s \n",encrptStr);
    
    char* deEncrptStr = CEncryptKit::decryptWithTea(encrptStr);
    
    printf("************  deEncrptStr : %s \n",deEncrptStr);
    
    //need free memory
    free(encrptStr);
    free(deEncrptStr);
}