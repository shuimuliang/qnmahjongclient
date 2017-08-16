#ifndef __CE_TOKEN_DATA_H__
#define __CE_TOKEN_DATA_H__

#include "Common/CocosEasyDef.h"
#ifdef DEBUG
#define ANALYSIS_SERVER_IP "thlog.dev.chaoshenxy.cn"
#define ANALYSIS_SERVER_PORT 7777
#else
#define ANALYSIS_SERVER_IP "thlog.dev.chaoshenxy.cn"
#define ANALYSIS_SERVER_PORT 7777
#endif
class CAnalysis : public CCObject
{
public:
	CAnalysis();
	~CAnalysis(){}
	static CAnalysis* getInstance();
    
	void end();
    
    void logRegister(const char* parmas);
    void logLogin(const char* parmas);
    void logPurchase(const char* parmas);
    
    const char* getAdID();

//    void logPurchase(const char* orderId, const char* iapId, double currencyAmount, const char* currencyType, double virtualCurrencyAmount, const char* paymentType);
    void logEvent(const char *eventID,const char *eventTag);
	void pageviewStart(const char *pageName);
	void pageviewEnd(const char *pageName);

	// implement interface for different platforms
public:
	void initAnalysis();

private:
	static CAnalysis* m_Instance;
    CC_SYNTHESIZE(std::string, m_PlayerID, PlayerID);
    CC_SYNTHESIZE(int, m_ServerID, ServerID);
};

#endif

