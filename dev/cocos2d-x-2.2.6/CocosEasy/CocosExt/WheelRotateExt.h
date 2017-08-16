#ifndef __CE_WHEEL_ROTATE_EXT_H__
#define __CE_WHEEL_ROTATE_EXT_H__

#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN

class CEWheelRotateExt : public CCNode 
{  
public:  
	static CEWheelRotateExt *create(ImageView *imageView,float degreeSpeed);
	
	virtual ~CEWheelRotateExt();

	void startWithSpeed(int fIntevalSpeed);
	
	void startToAngel(int fIntevalSpeed , int fAngelAcc , int targetAngel);

	void stop();

	void registerScriptHandler(int stopHandler,int musicHandler);

	virtual void update(float delta);
	
	virtual void onEnter();

	virtual void onExit();

private:
	float		m_accSpeed;
	int			m_fAngelAcc;
	int			m_targetAngel;
	float		m_cunAngel;
	bool		m_isToTarget;
	int			m_stopScriptHandler;
	int			m_musicScripHandler;
	float       m_degreeSpeed;
	int			m_curClock; 
	ImageView	*m_pMainSpr;
	int			m_arrIndex;
	CEWheelRotateExt(ImageView *pSpr , float facN);
	void updateSpeed(float delta);
	void updateToTarget(float delta);
	void doStopScriptEvent();
	void doMusicScriptEvent();
};

NS_CC_EASY_END

#endif // __CE_WHEEL_ROTATE_EXT_H__