//
//  Base64.h
//  client
//
//  Created by liulihua on 14-9-23.
//
//

#ifndef __client__Base64__
#define __client__Base64__

#include <iostream>

void *base64Decode(
                      const char *inputBuffer,
                      size_t length,
                      size_t *outputLength);

char *base64Encode(
                      const void *inputBuffer,
                      size_t length,
                      bool separateLines,
                      size_t *outputLength);


#endif /* defined(__client__Base64__) */
