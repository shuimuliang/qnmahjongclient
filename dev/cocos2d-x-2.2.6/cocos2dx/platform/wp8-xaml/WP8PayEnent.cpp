#include "WP8PayEnent.h"
//#include "SDKEasy/SDKManager.h"
#include "Cocos2dRenderer.h"

namespace PhoneDirect3DXamlAppComponent
{
	WP8PayEvent::WP8PayEvent(bool state,Platform::String^ code)
		:m_state(state),m_code(code)
	{

	}
	void WP8PayEvent::execute(Cocos2dRenderer ^ renderer)
	{
		renderer->PayComplete(m_code.Get(),m_state);
	}
}