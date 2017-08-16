//
//  EncryptKit.h
//  client
//
//  Created by liulihua on 14-9-23.
//
//

#ifndef __client__EncryptKit__
#define __client__EncryptKit__

#include <iostream>

class CEncryptKit
{
public:
    //tea encrypt and decrypt
    static char* encryptWithTea(const char* buffer);
    static char* decryptWithTea(const char* buffer);
    
    
    
    static void teaTest();

};
#endif /* defined(__client__EncryptKit__) */
