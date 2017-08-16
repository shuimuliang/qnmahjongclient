#ifndef __CE_SCENE_TRANSIT_H__
#define __CE_SCENE_TRANSIT_H__

#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN

class CLoadingTransitLayer : public CCLayer
{
public:
	CLoadingTransitLayer(){m_pNextScene = NULL; m_LoadStepScriptHandler = 0;}
	static CLoadingTransitLayer* create();

public:
	virtual CCScene* getNextScene(){return m_pNextScene;}
	virtual void enterNextScene();
	virtual void registerLoadStepScriptHandler(int handler){m_LoadStepScriptHandler = handler;}
	virtual void onEnter();
	virtual void startResourceLoad();

protected:
	virtual void setNextScene(CCScene* pScene);
	virtual void setLoadResName(const char* name){m_ResName = name;}
	virtual void loadStep(float percent,bool hasFinished);

private:
	void resourceLoadCallBack(float percent,bool hasFinished);

private:
	CCScene* m_pNextScene;
	std::string m_ResName;
	int m_LoadStepScriptHandler;
	friend class CSceneTransitMgr;
};


class CSceneTransitMgr : public CCObject
{
public:
	static CSceneTransitMgr* getInstance();
	void end();
	bool isAutoTransit(){return m_AutoTransit;}
	void setLoadingLayer(CLoadingTransitLayer* pLayer);
	void runWithScene(CCScene* pScene);
	void replaceScene(CCScene* pScene, const char* resName);


protected:
	CSceneTransitMgr();

protected:
	bool m_AutoTransit;
	CLoadingTransitLayer* m_pLoadingLayer;

private:
	static CSceneTransitMgr* m_Instance;
};


NS_CC_EASY_END



#endif