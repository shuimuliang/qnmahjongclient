#include "Analysis.h"

CAnalysis* CAnalysis::m_Instance = NULL;

CAnalysis::CAnalysis()
:m_ServerID(0)
{

}

CAnalysis* CAnalysis::getInstance()
{
	if(!m_Instance)
	{
		m_Instance = new CAnalysis();
	}
	return m_Instance;
}

void CAnalysis::end()
{
	CC_SAFE_DELETE(m_Instance);
	m_Instance = NULL;
}