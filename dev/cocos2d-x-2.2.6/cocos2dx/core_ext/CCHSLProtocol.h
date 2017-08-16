#ifndef __CC_HSL_PROTOCOL_H__
#define __CC_HSL_PROTOCOL_H__

#include "ccTypes_ext.h"

NS_CC_BEGIN

class CC_DLL CCHSLProtocol
{
public:
	CCHSLProtocol(){_hsl = cchsl3(0,0,0); }

	float hslClamp(float v, float mn, float mx){ return v < mn ? mn : (v > mx ? mx : v); }

	virtual void setHSL(const ccHSL3F& hsl)
	{
		_hsl = cchsl3(
			hslClamp(hsl.h, -180.0f, 180.f),
			hslClamp(hsl.s, -1.0f, 1.0f),
			hslClamp(hsl.l, -1.0f, 1.0f) );
	}

	virtual const ccHSL3F& getHSL(){return _hsl;}

protected:
	ccHSL3F _hsl;
};

NS_CC_END

#endif