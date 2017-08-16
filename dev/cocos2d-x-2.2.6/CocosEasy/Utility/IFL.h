#ifndef __CE_IFL_H__
#define __CE_IFL_H__

#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN

class CIFL
{
public:
	CIFL();

	// pause function
public:
	virtual bool isPause(){return m_pause;}
	virtual void pause();
	virtual void resume();
protected:
	virtual void pauseProccess() = 0;
	virtual void resumeProccess() = 0;
protected:
	bool m_pause;
};


NS_CC_EASY_END

#endif