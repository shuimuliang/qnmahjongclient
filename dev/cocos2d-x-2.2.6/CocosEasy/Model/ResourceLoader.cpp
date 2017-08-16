#include "ResourceLoader.h"
#include "SimpleAudioEngine.h"
#include "CocosExt/ArmatureExt.h"


NS_CC_EASY_BEGIN

CResArmature::CResArmature( const char* json, const char* name /*= NULL*/,bool onlyConfig) : CResBase(ARMATURE)
{
	if(!json){astBack;}
	m_onlyConfig = onlyConfig;
	m_ResName = json;
	if(name)
		m_ArmName = name;
	else
	{
		size_t pos = m_ResName.find_last_of(".");
		if (pos != std::string::npos)
		{
			m_ArmName = m_ResName.substr(0, pos);
		}
	}

	// correct armature name
	size_t pos = m_ArmName.find_last_of("/");
	if (pos == std::string::npos) pos = m_ArmName.find_last_of("\\");
	if (pos == std::string::npos) return;
	m_ArmName = m_ArmName.substr(pos+1, strlen(m_ArmName.c_str()));
}


CResLoaderList::~CResLoaderList()
{
	m_ArmatureList.clear();
	m_TextureList.clear();
	m_MiscList.clear();
	m_EffectList.clear();
}


void CResourceLoader::st_Cache::serialize()
{
	resList.clear();

	{// texture
		const std::vector<CResTexture>& list = resloader.getTextureList();
		for (unsigned int i=0; i< list.size(); ++i){resList.push_back((const CResBase*)&list[i]);}
	}
	{// armature
		const std::vector<CResArmature>& list = resloader.getArmatureList();
		for (unsigned int i=0; i< list.size(); ++i){resList.push_back((const CResBase*)&list[i]);}
	}
	{// music
		const std::vector<CResMusic>& list = resloader.getMiscList();
		for (unsigned int i=0; i< list.size(); ++i){resList.push_back((const CResBase*)&list[i]);}
	}
	{// effect
		const std::vector<CResEffect>& list = resloader.getEffectList();
		for (unsigned int i=0; i< list.size(); ++i){resList.push_back((const CResBase*)&list[i]);}
	}
	{// ui json
		const std::vector<CResUIJson>& list = resloader.getUIJsonList();
		for (unsigned int i=0; i< list.size(); ++i){resList.push_back((const CResBase*)&list[i]);}
	}
}





CResourceLoader* CResourceLoader::m_Instance = NULL;

CResourceLoader::CResourceLoader()
: m_State(SLEEP)
, m_MaxLoadStepTick(16)
, m_debugOnOff(false)
{
	clearCache();
	m_CacheList.reserve(100);
}


CResourceLoader::~CResourceLoader()
{
	m_ResContainer.clear();
	m_CacheList.clear();
	m_ListenerList.clear();
}


void CResourceLoader::clearCache()
{
	m_CacheList.clear();

	m_CurCacheIndex = 0;
	m_CurListIndex = 0;

	m_ResFinishedCount = 0;
	m_ResSumCount = 0;

	m_AsyncImageCount = 0;
	m_AsyncArmatureCount = 0;

	m_LastSumAsyncCount = 0;

	m_ListenerList.clear();
}



CResourceLoader* CResourceLoader::getInstance()
{
	if(m_Instance) return m_Instance;
	m_Instance = new CResourceLoader;
	return m_Instance;
}

void CResourceLoader::start(const char** pathList)
{
	setSearchPath(pathList);
}

void CResourceLoader::start( const std::vector<std::string>& list )
{
	setSearchPath(list);
}


void CResourceLoader::end()
{
	CC_SAFE_DELETE(m_Instance);
	m_Instance = NULL;
}

void CResourceLoader::setSearchPath( const char** pathList )
{
	// set resource path
	std::vector<std::string> resFindPath;
	const char** pPath = &pathList[0];
	while (*pPath)
	{
		resFindPath.push_back(*pPath);
		++pPath;
	}
	setSearchPath(resFindPath);
}

void CResourceLoader::setSearchPath( const std::vector<std::string>& list )
{
	CCFileUtils::sharedFileUtils()->setSearchPaths(list);
}


void CResourceLoader::addResourceLoaderList(const char* key,CResLoaderList& list )
{
	if(!key){assert(false); return;}
	if(findResourceLoaderList(key)){assert(false); return;}
	m_ResContainer[key] = list;
}

const CResLoaderList* CResourceLoader::findResourceLoaderList( const char* name )
{
	if(!name){assert(false); return NULL;}
	HashMap<std::string,CResLoaderList>::iterator it = m_ResContainer.find((std::string)name);
	if(it == m_ResContainer.end()){return NULL;}
	return &it->second;
	return NULL;
}

float CResourceLoader::getPercent()
{
	if(!isRunning()) return 100.0f;
	if(!m_ResSumCount){astReturn(100.0f);}
	float sum = m_ResSumCount + m_AsyncArmatureCount + m_AsyncImageCount;
	return (float)m_ResFinishedCount / (float)sum * 100.0f;
}


void CResourceLoader::loadArmature( const char* file )
{
	if(!file || isRunning()) {astBack};
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(file);
	bool hasConfig = CCArmatureDataManager::sharedArmatureDataManager()->hasLoaded(fullPath.c_str());
	if(!hasConfig)
		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(fullPath.c_str());
	else
		CCArmatureDataManager::sharedArmatureDataManager()->loadResource(fullPath.c_str());
}

void CResourceLoader::unloadArmature( const char* file )
{
	if(!file || isRunning()) {astBack};
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(file);
	CCArmatureDataManager::sharedArmatureDataManager()->unloadResource(fullPath.c_str());
}

void CResourceLoader::loadEffect( const char* file )
{
	if(!file || isRunning()) {astBack};
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(file);
	if ( m_GlobalResource.findEffect(fullPath.c_str()) ) return;
	m_GlobalResource.addEffect(fullPath.c_str());
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(fullPath.c_str());
}

unsigned int CResourceLoader::playEffect( const char* file )
{
	if(!file) {astReturn(-1)};
	const CResEffect* pRes = m_GlobalResource.findEffect(file);
	if(!pRes) {astReturn(-1);}
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pRes->getResName());
	return CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(fullPath.c_str());
}


void CResourceLoader::unloadEffect( const char* file )
{
	if(!file || isRunning()) {astBack};
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(file);
	if (! m_GlobalResource.findEffect(fullPath.c_str()) ) return;
	m_GlobalResource.delEffect(fullPath.c_str());
	CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect(fullPath.c_str());
}


bool CResourceLoader::run( const char* key ,CCObject* listener,SEL_ResStepLoad selector,bool noticeStep)
{
	HashMap<std::string,CResLoaderList>::iterator it = m_ResContainer.find(key);
	if(it == m_ResContainer.end()) return false;
	run(it->second,listener,selector,noticeStep);
	return true;
}



void CResourceLoader::run( const CResLoaderList& resloader,CCObject* listener /*= NULL*/,SEL_ResStepLoad selector /*= NULL*/, bool noticeStep /*= false*/ )
{
	//CC_SAFE_RETAIN(listener);
	st_Cache cache;
	cache.resloader = resloader;
	
	st_loadListener loadListener;
	loadListener.m_pStepListener = listener;
	loadListener.m_pStepSelector = selector;
	loadListener.m_noticeStep = noticeStep;

	run(cache,loadListener);
}

bool CResourceLoader::run( const char* key,int handler,bool noticeStep /*= false*/ )
{
	HashMap<std::string,CResLoaderList>::iterator it = m_ResContainer.find(key);
	if(it == m_ResContainer.end()) return false;
	run(it->second,handler,noticeStep);
	return true;
}


void CResourceLoader::run( const CResLoaderList& resloader,int handler,bool noticeStep /*= false*/ )
{
	st_Cache cache;
	cache.resloader = resloader;

	st_loadListener loadListener;
	loadListener.m_StepScriptHandler = handler;
	loadListener.m_noticeStep = noticeStep;
	run(cache,loadListener);
}

void CResourceLoader::run(st_Cache& cache ,const st_loadListener& listener)
{
	if(m_CacheList.size() + 1 >= m_CacheList.capacity()) return;
	// check listener
	if( (listener.m_pStepSelector && listener.m_pStepListener) ||
		listener.m_StepScriptHandler)
	{
		m_ListenerList.push_back(listener);
	}

	// add cache
	m_CacheList.push_back(cache);
	m_CacheList[m_CacheList.size()-1].serialize();
	m_ResSumCount += cache.resloader.getCount();

	if (!isRunning())
	{
		CCScheduler* pScheduler = CCDirector::sharedDirector()->getScheduler();
		pScheduler->scheduleSelector(schedule_selector(CResourceLoader::loop),this,0,false);
	}

	m_State = RUNNING;
}

void CResourceLoader::unloadAllResource()
{
	if(isRunning()){astBack;}

	// unload armature
	CCArmatureDataManager::sharedArmatureDataManager()->unloadAllResource();

	// unload effects
	TResEffectMap& list = const_cast<TResEffectMap&> (m_GlobalResource.getEffectList());
	TResEffectMap::iterator it = list.begin();
	for (it; it!=list.end(); ++it)
	{
		const CResEffect& item = it->second;
		std::string fullPathName = CCFileUtils::sharedFileUtils()->fullPathForFilename(item.getResName());
		CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect(fullPathName.c_str());
	}
	m_GlobalResource.clearEffect();

	// unload texture
    //CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	
	// debug
	if (m_debugOnOff)
	{
		CCLog("============== remove all resource end ==============");
		CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
	}
}


void CResourceLoader::loop( float dt )
{
	// check step finished
	if(m_CurCacheIndex >= m_CacheList.size())
	{
		// check async finished
		if(!m_AsyncImageCount && !m_AsyncArmatureCount)
		{
			// reset state
			CCScheduler* pScheduler = CCDirector::sharedDirector()->getScheduler();
			pScheduler->unscheduleSelector(schedule_selector(CResourceLoader::loop),this);
			m_State = SLEEP;
			
			// send finished event
			sendEvent();
			clearCache();
			return;
		}

		unsigned int curSumAsyncCount = m_AsyncImageCount + m_AsyncArmatureCount;
		if(curSumAsyncCount != m_LastSumAsyncCount)
		{
			sendEvent();
			m_LastSumAsyncCount = curSumAsyncCount;
		}
		return;
	}

	unsigned int startTick = getTickCount();
	const st_Cache& cache = m_CacheList[m_CurCacheIndex];
	while (m_CurListIndex < cache.resList.size())
	{
		// load resource step
		const CResBase* pRes = cache.resList[m_CurListIndex];
		loadStep(pRes);
		++m_CurListIndex;
		
		// check time out
		unsigned int runTick = getTickCount() - startTick;
		if(runTick >= m_MaxLoadStepTick) 
			return;
	}
	++m_CurCacheIndex;
	m_CurListIndex = 0;
}

void CResourceLoader::loadStep( const CResBase* pResItem)
{
	if(!pResItem) return;
	CResBase::eResType type = pResItem->getType();
	const char* name = pResItem->getResName();
	if (strlen(name) == 0){assert(false);}

	std::string resPath = name;
	std::string fullPathName = CCFileUtils::sharedFileUtils()->fullPathForFilename(resPath.c_str());
	name = fullPathName.c_str();
	if(!CCFileUtils::sharedFileUtils()->isAbsolutePath(resPath) &&
		fullPathName == resPath)
	{
		m_ResFinishedCount += pResItem->getCount();
		CCLog("[CResourceLoader::loadStep] file %s can not find",resPath.c_str());
		return;
	}

	bool isAsync = false;
	switch (type)
	{
	case cocoseasy::CResBase::TEXTURE:
		isAsync = true;
		++m_AsyncImageCount;
		CCTextureCache::sharedTextureCache()->addImageAsync(name,this,
			callfuncO_selector(CResourceLoader::addImageCallBack));
		break;
	
	case cocoseasy::CResBase::ARMATURE:
		{
			const CResArmature* pResArm = static_cast<const CResArmature*>(pResItem);
			bool autoloadRes = !pResArm->onlyLoadConfig();
			bool hasLoad = CCArmatureDataManager::sharedArmatureDataManager()->hasLoaded(name);
			if(hasLoad && autoloadRes)
			{
				CCArmatureDataManager::sharedArmatureDataManager()->loadResource(name);
			}
			else
			{
				isAsync = true;
				++m_AsyncArmatureCount;
				CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync(name,this,
					schedule_selector(CResourceLoader::addArmatureCallBack), autoloadRes);
			}
		}break;
		
	case cocoseasy::CResBase::MUSIC:
        CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(name);
		break;

	case cocoseasy::CResBase::EFFECT:
		{
			if (m_GlobalResource.findEffect(name)) break;
			m_GlobalResource.addEffect(name);
			CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(name);
		}break;
	
	case cocoseasy::CResBase::UI_JSON:
		{
			//GUIReader::shareReader()->readJsonFile(name);
			GUIReader::shareReader()->readJsonFileToStru(name);//terry lu modify
		}
		break;

	default: assert(false); break;
	}
	m_ResFinishedCount += pResItem->getCount();

	if(!isAsync) sendEvent();
}


void CResourceLoader::sendEvent( )
{
	float percent = getPercent();
	bool hasFinished = isSleep();

	if(hasFinished && m_debugOnOff)
	{
		CCLog("============== load all resource end ==============");
		CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
	}

	for (unsigned int i=0; i<m_ListenerList.size(); ++i)
	{
		const st_loadListener& listener = m_ListenerList[i];
		if(!listener.m_noticeStep && !hasFinished) continue;

		// send c event
		if(listener.m_pStepListener && listener.m_pStepSelector)
		{
			(listener.m_pStepListener->*listener.m_pStepSelector)(percent,hasFinished);
		}

		// send script event
		if(listener.m_StepScriptHandler)
		{
			CCArray* pArrayArgs = CCArray::createWithCapacity(2);
			pArrayArgs->addObject(CCFloat::create(percent));
			pArrayArgs->addObject(CCBool::create(hasFinished));
			CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(listener.m_StepScriptHandler,pArrayArgs);
		}
	}	
}


void CResourceLoader::addImageCallBack( CCObject* pObj )
{
	--m_AsyncImageCount;
	sendEvent();
}

void CResourceLoader::addArmatureCallBack( float percent )
{
	--m_AsyncArmatureCount;
	sendEvent();
}


// ================= CResourceGlobal ================= //


CResourceGlobal::~CResourceGlobal()
{
	m_EffectList.clear();
}

void CResourceGlobal::addEffect( const char* name)
{
	if(!name){astBack;}
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(name);
	if (findEffect(fullPath.c_str())) return;
	CResEffect item(fullPath.c_str());
	m_EffectList.insert( std::pair<std::string, CResEffect>(fullPath, item) );
}


void CResourceGlobal::delEffect( const char* name )
{
	if(!name){astBack;}
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(name);
	TResEffectMap::iterator it = m_EffectList.find(name);
	if(it == m_EffectList.end()) return;
	m_EffectList.erase(it);
}


const CResEffect* CResourceGlobal::findEffect( const char* name )
{
	if (!name) return false;
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(name);
	TResEffectMap::iterator it = m_EffectList.find(fullPath);
	if (it == m_EffectList.end()) return NULL;
	return &it->second;
}



NS_CC_EASY_END