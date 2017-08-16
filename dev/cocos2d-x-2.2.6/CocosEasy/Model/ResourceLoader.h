#ifndef __CE_RESOURCE_LOADER_H__
#define __CE_RESOURCE_LOADER_H__

#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN

class CArmatureExt;

class CResBase
{
public:
	enum eResType{UNKNOWN=-1,TEXTURE,ARMATURE,MUSIC,EFFECT, UI_JSON, COUNT,};
	CResBase(eResType t){m_Type = t; m_Count = 1;}
	void setCount(unsigned int count){m_Count = count;}
	unsigned int getCount() const {return m_Count;}
	eResType getType() const {return m_Type;}
	virtual const char* getResName() const {return m_ResName.c_str();}

protected:
	eResType m_Type;
	unsigned int m_Count;
	std::string m_ResName;
};

class CResTexture : public CResBase
{
public:
	CResTexture(const char* name) : CResBase(TEXTURE){assert(name); m_ResName = name;}
};

class CResArmature : public CResBase
{
public:
	CResArmature(const char* json, const char* name = NULL, bool onlyConfig = false);
	const char* getArmName() const {return m_ArmName.c_str();}
	bool onlyLoadConfig() const {return m_onlyConfig;}
protected:
	std::string m_ArmName;
	bool m_onlyConfig;
};

class CResMusic : public CResBase
{
public:
	CResMusic(const char* misc) : CResBase(MUSIC) {m_ResName = misc;}
};

class CResEffect : public CResBase
{
public:
	CResEffect(const char* effect) : CResBase(EFFECT) {m_ResName = effect;}
};

class CResUIJson : public CResBase
{
public:
	CResUIJson(const char* json) : CResBase(UI_JSON) {m_ResName = json;}
};

class CResLoaderList
{
public:
	CResLoaderList(){clear();}
	~CResLoaderList();
	void clear(){m_Count = 0; m_EffectList.clear(); m_ArmatureList.clear(); m_TextureList.clear(); m_MiscList.clear();}
	void addImage(const char* png, const char* plist = NULL){if(png){addTexture(png);}}
	void addTexture(const char* name){CResTexture res(name); m_TextureList.push_back(res); m_Count += res.getCount();}
	void addArmature(const char* json, const char* name = NULL, bool onlyconfig = false){CResArmature res(json,name,onlyconfig); m_ArmatureList.push_back(res); m_Count += res.getCount();}
	void addMusic(const char* name){CResMusic res(name); m_MiscList.push_back(res); m_Count += res.getCount();}
	void addEffect(const char* name){CResEffect res(name); m_EffectList.push_back(res); m_Count += res.getCount();}
	void addUIJson(const char* name){CResUIJson res(name); m_UIJsonList.push_back(res); m_Count += res.getCount();}
	unsigned int getCount() const {return m_Count;}
	const std::vector<CResArmature>& getArmatureList() const {return m_ArmatureList;}
	const std::vector<CResTexture>& getTextureList() const {return m_TextureList;}
	const std::vector<CResMusic>& getMiscList() const {return m_MiscList;}
	const std::vector<CResEffect>& getEffectList() const {return m_EffectList;}
	const std::vector<CResUIJson>& getUIJsonList() const {return m_UIJsonList;}
protected:
	std::vector<CResArmature> m_ArmatureList;
	std::vector<CResTexture> m_TextureList;
	std::vector<CResMusic> m_MiscList;
	std::vector<CResEffect> m_EffectList;
	std::vector<CResUIJson> m_UIJsonList;
	unsigned int m_Count;
};


typedef std::map<std::string, CResEffect> TResEffectMap;
class CResourceGlobal
{
public:
	~CResourceGlobal();
	void addEffect(const char* name);
	void delEffect(const char* name);
	const CResEffect* findEffect(const char* name);
	void clearEffect(){m_EffectList.clear();}

protected:
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(TResEffectMap, m_EffectList, EffectList);
};


typedef void (CCObject::*SEL_ResStepLoad)(float,bool);
#define resStepLoad_selector(_SELECTOR) (SEL_ResStepLoad)(&_SELECTOR)

class CResourceLoader : public CCObject
{
public:
	enum STATE{SLEEP=0,RUNNING};
	struct st_Cache
	{
		void serialize();
		CResLoaderList resloader;
		std::vector<const CResBase*> resList;
	};

	struct st_loadListener
	{
		st_loadListener(){memset(this,0,sizeof(st_loadListener));}
		CCObject* m_pStepListener;
		SEL_ResStepLoad m_pStepSelector;
		int m_StepScriptHandler;
		bool m_noticeStep;
	};
	
	~CResourceLoader();

	static CResourceLoader* getInstance();
	void start(const char** pathList);
	void start(const std::vector<std::string>& list);
	void end();
	void setSearchPath(const char** pathList);
	void setSearchPath(const std::vector<std::string>& list);
	void addResourceLoaderList(const char* key,CResLoaderList& list);
	const CResLoaderList* findResourceLoaderList(const char* name);
	bool isRunning(){return m_State == RUNNING;}
	bool isSleep()
	{
		if (m_State != 0 && m_State != 1)
		{
			assert(false);
		}
		return m_State == SLEEP;
	}
	float getPercent();

	void loadArmature(const char* file);
	void unloadArmature(const char* file);

	void loadEffect(const char* file);
	unsigned int playEffect(const char* file);
	void unloadEffect(const char* file);
	
	
	// only permit loading scene to call
	void unloadAllResource();

public:
	bool run(const char* key,CCObject* listener = NULL,SEL_ResStepLoad selector = NULL, bool noticeStep = false);
	void run(const CResLoaderList& resloader,CCObject* listener = NULL,SEL_ResStepLoad selector = NULL, bool noticeStep = false);
	bool run(const char* key,int handler,bool noticeStep = false);
	void run(const CResLoaderList& resloader,int handler,bool noticeStep = false);
	void setMaxLoadStepTime(float s){m_MaxLoadStepTick = (unsigned int)(s * 1000);}

protected:
	CResourceLoader();
	void clearCache();
	void run(st_Cache& cache, const st_loadListener& listener);
	void loop(float dt);
	void loadStep(const CResBase* pResItem);
	void sendEvent();
	void addImageCallBack(CCObject* pObj);
	void addArmatureCallBack(float percent);

	// property
private:
	STATE m_State;
	HashMap<std::string,CResLoaderList> m_ResContainer;
	unsigned int m_MaxLoadStepTick;

private:
	// load cache
	std::vector<st_Cache> m_CacheList;

	unsigned int m_CurCacheIndex;
	unsigned int m_CurListIndex;

	unsigned int m_ResFinishedCount;
	unsigned int m_ResSumCount;

	unsigned int m_AsyncImageCount;
	unsigned int m_AsyncArmatureCount;

	unsigned int m_LastSumAsyncCount;

	std::vector<st_loadListener> m_ListenerList;

	CResourceGlobal m_GlobalResource;

private:
	static CResourceLoader* m_Instance;
	bool m_debugOnOff;
};



NS_CC_EASY_END


#endif