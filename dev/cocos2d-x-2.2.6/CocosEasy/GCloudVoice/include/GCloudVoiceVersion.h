//
//  GCloudVoiceVersion.h
//  gcloud_voice
//
//  Created by cz on 15/10/19.
//  Copyright © 2015年 gcloud. All rights reserved.
//

#ifndef gcloud_voice_GCloudVoiceVersion_h_
#define gcloud_voice_GCloudVoiceVersion_h_
namespace gcloud_voice
{
enum {
    VER_MAJOR = 1,
    VER_MINOR = 1,
    VER_FIX   = 5,
    
    VER_SVN   = 143653,
    
    VER_BUF_LEN = 1024,
};

char * gcloud_voice_version();
}
#endif /* gcloud_voice_GCloudVoiceVersion_h_ */
