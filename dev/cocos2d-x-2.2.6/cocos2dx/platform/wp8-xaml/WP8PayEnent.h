#ifndef __WP8_PAY_EVENT__
#define __WP8_PAY_EVENT__

#include <agile.h>
#include "InputEvent.h"

ref class Cocos2dRenderer;
namespace PhoneDirect3DXamlAppComponent
{
class WP8PayEvent: public InputEvent
{
public:
	WP8PayEvent(bool state,Platform::String^ code);
	WP8PayEvent(bool state);
	virtual void execute(Cocos2dRenderer^ render);
private:
	Platform::Agile<Platform::String> m_code;
	bool m_state;
};

}




#endif //__WP8_PAY_EVENT__