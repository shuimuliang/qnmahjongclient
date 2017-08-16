#include "UploadManager.h"
#include "Log.h"
using namespace cocoseasy;
CEUpLoadManager * CEUpLoadManager::ince = NULL;
const char* QINIU_UP_HOST = "http://upload.qiniu.com";
CEUpLoadManager *CEUpLoadManager::getInstance()
{
	if(!ince){
		ince = new CEUpLoadManager;
	}
	return ince;
}

void cocoseasy::CEUpLoadManager::end()
{
	CC_SAFE_DELETE(ince);
}

cocoseasy::CEUpLoadManager::CEUpLoadManager()
{

}

cocoseasy::CEUpLoadManager::~CEUpLoadManager()
{

}

void cocoseasy::CEUpLoadManager::ioFormInit( UpLoad_Io_form &ioForm , const string &uptoken , const string &key )
{
	//add token
	curl_formadd(&ioForm.formpost,&ioForm.lastptr,CURLFORM_COPYNAME , "token" , CURLFORM_COPYCONTENTS , uptoken.c_str(),CURLFORM_END);

	//add key
	if(!key.empty())
	{
		curl_formadd(&ioForm.formpost,&ioForm.lastptr,CURLFORM_COPYNAME , "key" , CURLFORM_COPYCONTENTS , key.c_str(),CURLFORM_END);
	}

}

void cocoseasy::CEUpLoadManager::ioCall( struct curl_httppost * formpost )
{
	//init curl
	CURL *curl = curl_easy_init();
	if(!curl){
		CEError("init curl error");
	}

	//head
	curl_slist *headers = curl_slist_append(NULL , "Expect:");

	//setopt
	curl_easy_setopt(curl, CURLOPT_URL, QINIU_UP_HOST);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


}


bool CEUpLoadManager::uploadFile( const char *token , const char *key , const char *localFile )
{
	UpLoad_Io_form form;
	ioFormInit(form , token , key);

	curl_formadd(
		&form.formpost, &form.lastptr, CURLFORM_COPYNAME, "file", CURLFORM_FILE, localFile, CURLFORM_END);

	return true;
}




