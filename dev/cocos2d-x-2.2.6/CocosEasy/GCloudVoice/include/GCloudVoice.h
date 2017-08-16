/*******************************************************************************\
 ** gcloud_voice:GCloudVoice.h
 ** Created by CZ(cz.devnet@gmail.com) on 16/8/1
 **
 **  Copyright © 2016年 apollo. All rights reserved.
 \*******************************************************************************/

#ifndef gcloud_voice_GCloudVoice_h_
#define gcloud_voice_GCloudVoice_h_

#include "GCloudVoiceErrno.h"
#include "GCloudVoiceExtension.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#if defined(WIN32) || defined(_WIN32)
	#ifdef GCLOUD_VOICE_EXPORTS
		#define GCLOUD_VOICE_API __declspec(dllexport)
	#else
		#define GCLOUD_VOICE_API __declspec(dllimport)
	#endif
#else
	#define GCLOUD_VOICE_API
#endif

namespace gcloud_voice
{
    /**
     * IGCloudVoiceNotify is a notify for voice engine. You should implemtation it to get the message.
     */
    class GCLOUD_VOICE_API IGCloudVoiceNotify
    {
    protected:
        IGCloudVoiceNotify();
        
    public:
        // Real-Time Callback
        /**
         * Callback when JoinXxxRoom successful or failed.
         *
         * @param code : a GCloudVoiceCompleteCode code . You should check this first.
         * @param roomName : name of your joining, should be 0-9A-Za-Z._- and less than 127 bytes
         * @param memberID : if success, return the memberID
         */
        virtual void OnJoinRoom(GCloudVoiceCompleteCode code, const char *roomName, int memberID) ;
        
        /**
         * Callback when dropped from the room
         *
         * @param code : a GCloudVoiceCompleteCode code . You should check this first.
         * @param roomName : name of your joining, should be 0-9A-Za-Z._- and less than 127 bytes
         * @param memberID : if success, return the memberID
         */
        virtual void OnStatusUpdate(GCloudVoiceCompleteCode status, const char *roomName, int memberID) ;
        
        /**
         * Callback when QuitRoom successful or failed.
         *
         * @param code : a GCloudVoiceCompleteCode code . You should check this first.
         * @param roomName : name of your joining, should be 0-9A-Za-Z._- and less than 127 bytes
         */
        virtual void OnQuitRoom(GCloudVoiceCompleteCode code, const char *roomName) ;
        
        /**
         * Callback when someone saied or silence in the same room.
         *
         * @param count: count of members who's status has changed.
         * @param members: a int array composed of [memberid_0, status,memberid_1, status ... memberid_2*count, status]
         * here, status could be 0, 1, 2. 0 meets silence and 1/2 means saying
         */
        virtual void OnMemberVoice	(const unsigned int *members, int count) ;
        
        // Voice Message Callback
        /**
         * Callback when upload voice file successful or failed.
         *
         * @param code: a GCloudVoiceCompleteCode code . You should check this first.
         * @param filePath: file to upload
         * @param fileID: if success ,get back the id for the file.
         */
        virtual void OnUploadFile(GCloudVoiceCompleteCode code, const char *filePath, const char *fileID) ;
        
        /**
         * Callback when download voice file successful or failed.
         *
         * @param code: a GCloudVoiceCompleteCode code . You should check this first.
         * @param filePath: file to download to .
         * @param fileID: if success ,get back the id for the file.
         */
        virtual void OnDownloadFile(GCloudVoiceCompleteCode code, const char *filePath, const char *fileID) ;
        
        /**
         * Callback when finish a voice file play end.
         *
         * @param code: a GCloudVoiceCompleteCode code . You should check this first.
         * @param filePath: file had been plaied.
         */
        virtual void OnPlayRecordedFile(GCloudVoiceCompleteCode code,const char *filePath) ;
        
        /**
         * Callback when query message key successful or failed.
         *
         * @param code: a GCloudVoiceCompleteCode code . You should check this first.
         */
        virtual void OnApplyMessageKey(GCloudVoiceCompleteCode code) ;
        
        // Translate
        /**
         * Callback when translate voice to text successful or failed.
         *
         * @param code: a GCloudVoiceCompleteCode code . You should check this first.
         * @param fileID : file to translate
         * @param result : the destination text of the destination language.
         */
        virtual void OnSpeechToText(GCloudVoiceCompleteCode code, const char *fileID, const char *result) ;

        /**
         * Callback when client is using microphone recording audio
         *
         * @param pAudioData : audio data pointer 
         * @param nDataLength : audio data length
         * @param result : void
         */
        virtual void OnRecording(const unsigned char* pAudioData, unsigned int nDataLength);
    };
   
    
    class IGCloudVoiceEngine : public IGCloudVoiceEngineExtension
    {
    public:
        
        /**
         * Destination language to translate to.
         */
         
        enum GCloudLanguage {
            China       = 0,
            Korean      = 1,
            English     = 2,
            Japanese    = 3,
        };
        
        /**
         * Mode of voice engine.
         *
         * You should set to one first.
         */
		enum GCloudVoiceMode
		{
			RealTime = 0, // realtime mode for TeamRoom or NationalRoom
			Messages,     // voice message mode
			Translation,  // speach to text mode
		};
        
        /**
         * Member's role for National Room.
         */
        enum GCloudVoiceMemberRole
        {
            Anchor = 1, // member who can open microphone and say
            Audience,   // member who can only hear anchor's voice
        };
        
        /**
         * Set the server address, just used for foreign game,such as Korea, Europe...
         *
         * @param URL: url of server
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno SetServerInfo(const char * URL) = 0;
        
        /**
         * Set your app's info such as appID/appKey.
         *
         * @param appID : your game ID from gcloud.qq.com
         * @param appKey : your game key from gcloud.qq.com
         * @param openID : player's openID from QQ or Wechat. or a unit role ID.
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno SetAppInfo(const char *appID,const char *appKey, const char *openID) = 0;

        /**
         * Init the voice engine.
         *
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno Init() = 0;
        
        /**
         * Set the notify to engine.
         *
         * @param notify: the notify
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno SetNotify(IGCloudVoiceNotify *notify) = 0;
        
        /**
         * Set the mode for engine.
         *
         * @param mode: mode to set
         *              RealTime:    realtime mode for TeamRoom or NationalRoom
         *              Messages:    voice message mode
         *              Translation: speach to text mode
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno SetMode(GCloudVoiceMode mode) = 0;
        
        /**
         * Trigger engine's callback.
         *
         * You should invoke poll on your loop. such as onUpdate() in cocos2dx
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno Poll()	= 0;
        
        /**
         * The Application's Pause.
         *
         * When your app pause such as goto backend you should invoke this
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno Pause() = 0;
        
        /**
         * The Application's Resume.
         *
         * When your app reuse such as come back from  backend you should invoke this
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno Resume() = 0;
             
        
        //Real-Time Voice API
    public:
        /**
         * Join in team room.
         * 
         * @param roomName: the room to join, should be less than 127byte, composed by alpha.
         * @param msTimeout: time for join, it is micro second. value range[5000, 60000]
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno JoinTeamRoom(const char *roomName, int msTimeout = 10000) = 0;
        

        
        /**
         * Join in a national room.
         *
         * @param roomName: the room to join, should be less than 127byte, composed by alpha.
         * @param role : a GCloudVoiceMemberRole value illustrate wheather can send voice data.
         * @param msTimeout: time for join, it is micro second. value range[5000, 60000]
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno JoinNationalRoom(const char *roomName, GCloudVoiceMemberRole role, int msTimeout = 10000) = 0;
        
        
        /**
         * Join in a FM room.
         *
         * @param roomName: the room to join, should be less than 127byte, composed by alpha.
         * @param msTimeout: time for join, it is micro second. value range[5000, 60000]
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno JoinFMRoom(const char *roomName, int msTimeout = 10000) = 0;
        
        /**
         * Quit the voice room.
         *
         * @param roomName: the room to join, should be less than 127byte, composed by alpha.
         * @param msTimeout: time for quit, it is micro second.value range[5000, 60000]
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno QuitRoom(const char *roomName, int msTimeout = 10000) = 0;
        
        /**
         * Open player's micro phone  and begin to send player's voice data.
         *
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno OpenMic() = 0;
        
        /**
         * Close players's micro phone and stop to send player's voice data.
         *
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno CloseMic() = 0;
        
        /**
         * Open player's speaker and begin recvie voice data from the net .
         *
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno OpenSpeaker() = 0;
        
        /**
         * Close player's speaker and stop to recive voice data from the net.
         *
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno CloseSpeaker() = 0;
        
        
        //Messages Voice API
    public:
        /**
         * Apply the key for voice message.
         *
         * @param msTimeout: time for apply, it is micro second.value range[5000, 60000]
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno ApplyMessageKey(int msTimeout = 10000) = 0;
        
        /**
         * Limit the max voice message's last time.
         *
         * @param msTime : message's largest time in micro second.value range[1000, 2*60*1000]
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno SetMaxMessageLength(int msTime) = 0;
        
        /**
         * Open player's microphone and record player's voice.
         *
         * @param filePath: voice data to store. file path should be such as:"your_dir/your_file_name" be sure to use "/" not "\"
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno StartRecording	(const char * filePath) = 0;
        
        /**
         * Stop player's microphone and stop record player's voice.
         *
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno StopRecording () = 0;
        
        
        /**
         * Upload player's voice message file to the net.
         *
         * @param filePath: voice data to upload. file path should be such as "your_dir/your_file_name" be sure to use "/" not "\"
         * @param msTimeout: time for upload, it is micro second.value range[5000, 60000]
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno UploadRecordedFile	(const char * filePath, int msTimeout = 60000) = 0;
        
        /**
         * Download other players' voice message.
         *
         * @param fileID : file to be download
         * @param filePath: voice data to store. file path should be such as "your_dir/your_file_name" be sure to use "/" not "\"
         * @param msTimeout: time for download, it is micro second.value range[5000, 60000]
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno DownloadRecordedFile (const char *fileID, const char * downloadFilePath, int msTimeout = 60000) = 0;
        
        /**
         * Play local voice message file.
         *
         * @param filePath: voice data to play. file path should be such as:"your_dir/your_file_name" be sure to use "/" not "\"
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno PlayRecordedFile (const char * downloadFilePath) = 0;
        
        /**
         * Stop play the file.
         *
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno StopPlayFile () = 0;
        
    public:
        /**
         * Translate voice data to text.
         *
         * @param fileID : file to be translate
         * @param msTimeout : timeout for translate
         * @param language: a GCloudLanguage indicate which language to be translate
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno SpeechToText(const char *fileID, int msTimeout = 60000,  GCloudLanguage language = China) = 0;
        
        
    public: // if needed
        virtual GCloudVoiceErrno JoinTeamRoom(const char *roomName, const char *token, int timestamp, int msTimeout = 10000) = 0;
        
        
        virtual GCloudVoiceErrno JoinNationalRoom(const char *roomName, GCloudVoiceMemberRole role, const char *token, int timestamp, int msTimeout = 10000) = 0;
        
        virtual GCloudVoiceErrno ApplyMessageKey(const char *token, int timestamp, int msTimeout = 10000) = 0;
        
        virtual GCloudVoiceErrno SpeechToText(const char *fileID, const char *token, int timestamp, int msTimeout = 60000,  GCloudLanguage language = China) = 0;

    };
    
    /**
     * Get the voice engine instance.
     *
     * @return : the voice instance on success, or NULL on failed.
     */
    extern "C" GCLOUD_VOICE_API IGCloudVoiceEngine *GetVoiceEngine();
    
} // endof namespace gcloud_voice

#endif /* gcloud_voice_GCloudVoice_h_ */
