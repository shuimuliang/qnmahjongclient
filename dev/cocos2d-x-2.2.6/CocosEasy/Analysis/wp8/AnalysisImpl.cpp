#include "../Analysis.h"


namespace PhoneDirect3DXamlAppComponent 
{
	public delegate void InitAnalysisDelegate();
	public delegate void LogEventDelegate(Platform::String^ eventID,Platform::String^ eventTag);
	public delegate void PageviewStartDelegate(Platform::String^ pagename);
	public delegate void PageviewEndDelegate(Platform::String^ pagename);
	public ref class AnalysisDelegate sealed
	{
	public:
		AnalysisDelegate(void){}
		void SetInitAnalysisDelegate(InitAnalysisDelegate^ del)
		{
			m_InitAnalysisDelegate = del;
		}
		void SetLogEventDelegate(LogEventDelegate^ del)
		{
			m_LogEventDelegate = del;
		}
		void SetPageviewStartDelegate(PageviewStartDelegate^ del)
		{
			m_PageviewStartDelegate = del;
		}
		void SetPageviewEndDelegate(PageviewEndDelegate^ del)
		{
			m_PageviewEndDelegate = del;
		}

		void DInitAnalysis()
		{            
			if(m_InitAnalysisDelegate)
			{
				m_InitAnalysisDelegate->Invoke();
			}
		}
		void DLogEvent(Platform::String^ eventID,Platform::String^ eventTag)
		{
			if(m_LogEventDelegate)
			{
				m_LogEventDelegate->Invoke(eventID,eventTag);
			}
		}
		void DPageviewStart(Platform::String^ pagename)
		{
			if(m_PageviewStartDelegate)
			{
				m_PageviewStartDelegate->Invoke(pagename);
			}
		}
		void DPageviewEnd(Platform::String^ pagename)
		{
			if(m_PageviewEndDelegate)
			{
				m_PageviewEndDelegate->Invoke(pagename);
			}
		}
	private:
		property static InitAnalysisDelegate^ m_InitAnalysisDelegate;
		property static LogEventDelegate^ m_LogEventDelegate;
		property static PageviewStartDelegate^ m_PageviewStartDelegate;
		property static PageviewEndDelegate^ m_PageviewEndDelegate;
	};

}
PhoneDirect3DXamlAppComponent::AnalysisDelegate^ manager;

PhoneDirect3DXamlAppComponent::AnalysisDelegate^ GetDelegate()
{
	if(!manager)
		manager = ref new PhoneDirect3DXamlAppComponent::AnalysisDelegate();
	return manager;
}

Platform::String^ ToPlatformString(const char* charpoint){
	std::string str = charpoint;
	std::wstring ws;
	ws.assign(str.begin(), str.end());
	return ref new Platform::String(ws.c_str());
}

void CAnalysis::initAnalysis()
{
	GetDelegate()->DInitAnalysis();
	return;
}
void CAnalysis::logEvent(const char *eventID,const char *eventTag){
    GetDelegate()->DLogEvent(ToPlatformString(eventID),ToPlatformString(eventTag));
}
void CAnalysis::logPurchase(const char* parmas){
}
void CAnalysis::logRegister(const char* parmas){
}
void CAnalysis::logLogin(const char* parmas){
}

void CAnalysis::pageviewStart(const char *pageName){
    GetDelegate()->DPageviewStart(ToPlatformString(pageName));
}
void CAnalysis::pageviewEnd(const char *pageName){
    GetDelegate()->DPageviewEnd(ToPlatformString(pageName));
}
const char*  CAnalysis::getAdID(){
    return "";
}