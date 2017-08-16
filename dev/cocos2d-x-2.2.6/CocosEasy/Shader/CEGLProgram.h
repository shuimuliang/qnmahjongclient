#ifndef __CE_GL_PROGRAM_H__
#define __CE_GL_PROGRAM_H__

#include "cocos2d.h"
#include "shaders/CCGLProgram.h"
using namespace cocos2d;

class CEGLProgramHSL : public CCGLProgram
{
public:
	enum {kCEUniformBase = kCCUniformBaseCount, kCEUniformHSL_H, kCEUniformHSL_S, kCEUniformHSL_L};
	virtual void updateUniforms();
	virtual void setUniformsForBuiltins(void* data = NULL);	// data must be CCNodeRGBA
};

class CEGLProgramGrey : public CCGLProgram
{
public:
	virtual void updateUniforms();
	virtual void setUniformsForBuiltins(void* data = NULL);	// data must be CCNodeRGBA
};

class CEGLProgramOutline : public CCGLProgram
{
public:
	enum {kCEUniformBase = kCCUniformBaseCount, kCEUniformColor, kCEUniformRadiusX, kCEUniformRadiusY, kCEUniformThreshold};
	virtual void updateUniforms();
	virtual void setUniformsForBuiltins(void* data = NULL);	// data must be CCSprite
};

class CEGLProgramMotionBlur : public CCGLProgram
{
public:
	enum {kCEUniformBase = kCCUniformBaseCount, kCEUniformCoefficient, kCEUniformOnePixelSize};
	virtual void updateUniforms();
	virtual void setUniformsForBuiltins(void* data = NULL);	// data must be CCSprite
};

class CEGLProgramRadialBlur : public CCGLProgram
{
public:
	enum {kCEUniformBase = kCCUniformBaseCount, kCEUniformCoefficient, kCEUniformOnePixelSize, kCEUniformCenter};
	virtual void updateUniforms();
	virtual void setUniformsForBuiltins(void* data = NULL);	// data must be CCSprite
};


class CEGLProgramGaussBlur : public CCGLProgram
{
public:
	enum {kCEUniformBase = kCCUniformBaseCount, kCEUniformResolution, kCEUniformBlurRadius, kCEUniformSampleNum};
	virtual void updateUniforms();
	virtual void setUniformsForBuiltins(void* data = NULL);	// data must be CCSprite
};

void loadCocosEasyShaders();
void reloadCocosEasyShaders();


#endif