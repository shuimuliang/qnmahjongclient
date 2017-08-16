#include "SpriteExt.h"

NS_CC_EASY_BEGIN

CCSprite*  createMaskedSprite(CCSprite* src, const char* maskFile) 
{ 
	CCSprite * mask = CCSprite::create(maskFile); 
	assert(src && mask);
	if(!src || !mask) return NULL;

	CCSize srcContent = src->getContentSize(); 
	CCSize maskContent = mask->getContentSize(); 

	CCRenderTexture * rt = CCRenderTexture::create(srcContent.width, srcContent.height, kTexture2DPixelFormat_RGBA8888); 

	float ratiow = srcContent.width / maskContent.width; 
	float ratioh = srcContent.height / maskContent.height; 
	mask->setScaleX(ratiow); 
	mask->setScaleY(ratioh); 

	mask->setPosition(ccp(srcContent.width / 2, srcContent.height / 2)); 
	src->setPosition(ccp(srcContent.width / 2, srcContent.height / 2)); 

	ccBlendFunc blendFunc2 = { GL_ONE, GL_ZERO }; 
	mask->setBlendFunc(blendFunc2); 
	ccBlendFunc blendFunc3 = { GL_DST_ALPHA, GL_ZERO }; 
	src->setBlendFunc(blendFunc3); 

	rt->begin(); 
	mask->visit(); 
	src->visit(); 
	rt->end(); 

	CCSprite * retval = CCSprite::createWithTexture(rt->getSprite()->getTexture()); 
	retval->setFlipY(true); 
	return retval; 
} 

NS_CC_EASY_END