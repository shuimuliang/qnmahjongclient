#include "IFL.h"

NS_CC_EASY_BEGIN

CIFL::CIFL()
: m_pause(false)
{

}

void CIFL::pause()
{
	if(m_pause) return;
	m_pause = true;
	pauseProccess();
}

void CIFL::resume()
{
	if(!m_pause) return;
	m_pause = false;
	resumeProccess();
}

NS_CC_EASY_END