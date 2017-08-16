#ifndef __CE_UP_LOAD_MANAGER_H__
#define __CE_UP_LOAD_MANAGER_H__
#include "Common/CocosEasyDef.h"
NS_CC_EASY_BEGIN;
class CEUpLoadManager
{
public:
	static CEUpLoadManager *getInstance();
	static void end();

	~CEUpLoadManager();
	/*
	* token : 
	* key:
	* fileName: the full path of the file
	* return: true-success
	*/
	bool uploadFile(const char *token , const char *key , const char *localFile);

private:
	typedef struct _UpLoad_Io_form {
		struct curl_httppost* formpost;
		struct curl_httppost* lastptr;
	}UpLoad_Io_form;

	static CEUpLoadManager *ince;

	CEUpLoadManager();

	void ioFormInit(UpLoad_Io_form &ioForm , const string &uptoken , const string &key);

	void ioCall(struct curl_httppost * formpost);


};
NS_CC_EASY_END;
#endif //__CE_UP_LOAD_MANAGER_H__