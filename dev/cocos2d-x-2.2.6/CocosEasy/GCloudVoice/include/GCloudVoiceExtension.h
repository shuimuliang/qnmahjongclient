/*******************************************************************************\
 ** gcloud_voice:GCloudVoiceExtension.h
 ** Created by CZ(cz.devnet@gmail.com) on 16/8/1
 **
 **  Copyright © 2016年 apollo. All rights reserved.
 \*******************************************************************************/

#ifndef gcloud_voice_GCloudVoiceExtension_h_
#define gcloud_voice_GCloudVoiceExtension_h_

#include "GCloudVoiceErrno.h"

namespace gcloud_voice
{
    
    class IGCloudVoiceEngine;
    /**
     * Extension API for voice engine
     */
    class IGCloudVoiceEngineExtension
    {
    public:
        /**
         * Don't play voice of the member.
         * 
         * @param member : member to forbid
         * @param bEnable : do forbid if it is true
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno ForbidMemberVoice(int member, bool bEnable) = 0;
        
        /**
         * Open Voice Engine's logcat
         *
         * @param enable: open logcat if it is true
         */
        virtual void EnableLog(bool enable) = 0;
        
        /**
         * Get micphone's volume
         *
         * @return : micphone's volume , if return value>0, means you have said something capture by micphone
         */
        virtual int GetMicLevel() = 0;
        
        /**
         * Get speaker's volume
         *
         * @return : speaker's volume, value is equal you Call SetSpeakerVolume param value
         */
        virtual int GetSpeakerLevel() = 0;

        /**
         * set sepaker's volume
         *
		 * @param vol: setspeakervolume, 
		 * Android & IOS, value range is 0-800, 100 means original voice volume, 50 means only 1/2 original voice volume, 200 means double original voice volume
		 * Windows value range is 0x0-0xFFFF, suggested value bigger than 0xff00, then you can hear you speaker sound
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
		virtual GCloudVoiceErrno SetSpeakerVolume(int vol) = 0;
        
        /**
         * Test wheather microphone is available
         *
         * @return : if success return GCLOUD_VOICE_SUCC, , means have detect micphone device,failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno TestMic() = 0;
        
        /**
         * Get voice message's info
         * 
         * @param filepath: file path should be "sdcard/your_dir/your_file_name" be sure to use "/" not "\"
         * @param bytes: on return for file's size
         * @param sendons: on return for voice's length
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno GetFileParam(const char *filepath, unsigned int* bytes, float *seconds) = 0;
        
        /**
         * Capture microphone audio data by IGCloudVoiceNotify::OnRecording
         * 
         * @param bCapture : true/false - start/stop capturing audio data
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno CaptureMicrophoneData(bool bCapture) = 0;

    public:
        /**
         * YOU SHOULD NOT INVOKE THIS ONE.
         */
        virtual int invoke ( unsigned int    nCmd,
                            unsigned int    nParam1,
                            unsigned int    nParam2,
                            unsigned int    *pOutput ) = 0;
    };
} // end of gcloud_voice


#endif /* gcloud_voice_GCloudVoiceExtension_h_ */
