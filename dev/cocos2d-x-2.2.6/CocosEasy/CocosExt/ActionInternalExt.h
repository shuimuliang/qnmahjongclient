#ifndef __CE_ACTION_INTERNAL_EXT_H__
#define __CE_ACTION_INTERNAL_EXT_H__

#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN

class CEShake : public cocos2d::CCActionInterval  
{  
public:  
	CEShake();  
	// Create the action with a time and a strength (same in x and y)  
	static CEShake *create(float d, float strength );  

	// Create the action with a time and strengths (different in x and y)  
	static CEShake *create(float d, float strength_x, float strength_y );  

	bool initWithDuration(float d, float strength_x, float strength_y );  

protected:  

	void startWithTarget(cocos2d::CCNode *pTarget);  
	void update(float time);  
	void stop(void);  

	CCPoint m_StartPosition;  

	// Strength of the action  
	float m_strength_x, m_strength_y;  
};

NS_CC_EASY_END

#endif