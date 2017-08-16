#include "CEGLProgram.h"

const GLchar* ceShader_PositionTextureColorHSL_frag =
#include "ceShader_PositionTextureColorHSL_frag.h"

const GLchar* ceShader_PositionTextureColorGrey_frag =
#include "ceShader_PositionTextureColorGrey_frag.h"

const GLchar* ceShader_PositionTextureOutline_frag =
#include "ceShader_PositionTextureOutline_frag.h"

const GLchar* ceShader_MotionBlur_frag =
#include "ceShader_Motion_Blur_frag.h"

const GLchar* ceShader_RadialBlur_frag =
#include "ceShader_Radial_Blur_frag.h"

const GLchar* ceShader_GaussBlur_frag =
#include "ceShader_Gauss_Blur_frag.h"


void CEGLProgramHSL::updateUniforms()
{
	m_uUniforms[kCEUniformHSL_H] = glGetUniformLocation(m_uProgram, "CE_HSL_H");
	m_uUniforms[kCEUniformHSL_S] = glGetUniformLocation(m_uProgram, "CE_HSL_S");
	m_uUniforms[kCEUniformHSL_L] = glGetUniformLocation(m_uProgram, "CE_HSL_L");
	CCGLProgram::updateUniforms();
}

void CEGLProgramHSL::setUniformsForBuiltins(void* data)
{
	if(!data) return;
	CCNodeRGBA* node = static_cast<CCNodeRGBA*>(data);
	const ccHSL3F hsl = node->getHSL();
	setUniformLocationWith1f(m_uUniforms[kCEUniformHSL_H], hsl.h);
	setUniformLocationWith1f(m_uUniforms[kCEUniformHSL_S], hsl.s);
	setUniformLocationWith1f(m_uUniforms[kCEUniformHSL_L], hsl.l);
	CCGLProgram::setUniformsForBuiltins();
}


void CEGLProgramGrey::updateUniforms()
{
	CCGLProgram::updateUniforms();
}

void CEGLProgramGrey::setUniformsForBuiltins(void* data)
{
	CCGLProgram::setUniformsForBuiltins();
}


void CEGLProgramOutline::updateUniforms()
{
	m_uUniforms[kCEUniformColor] = glGetUniformLocation(m_uProgram, "u_outlineColor");
	m_uUniforms[kCEUniformThreshold] = glGetUniformLocation(m_uProgram, "u_threshold");
	m_uUniforms[kCEUniformRadiusX] = glGetUniformLocation(m_uProgram, "u_radius_x");
	m_uUniforms[kCEUniformRadiusY] = glGetUniformLocation(m_uProgram, "u_radius_y");
	CCGLProgram::updateUniforms();
}

void CEGLProgramOutline::setUniformsForBuiltins(void* data /*= NULL*/)
{
	if(!data) return;
	CCSprite* node = static_cast<CCSprite*>(data);
	const ccColor4F& color = node->getOutlineColor();
	const CCSize& size = node->getContentSize();
	setUniformLocationWith3f(m_uUniforms[kCEUniformColor], color.r, color.g, color.b);
	setUniformLocationWith1f(m_uUniforms[kCEUniformThreshold], color.a);
	setUniformLocationWith1f(m_uUniforms[kCEUniformRadiusX], 1.0f / size.width);
	setUniformLocationWith1f(m_uUniforms[kCEUniformRadiusY], 1.0f / size.height);
	CCGLProgram::setUniformsForBuiltins();
}


void CEGLProgramMotionBlur::updateUniforms()
{
	m_uUniforms[kCEUniformCoefficient] = glGetUniformLocation(m_uProgram, "coefficient");
	m_uUniforms[kCEUniformOnePixelSize] = glGetUniformLocation(m_uProgram, "onePixelSize");
	CCGLProgram::updateUniforms();
}


void CEGLProgramMotionBlur::setUniformsForBuiltins(void* data /*= NULL*/)
{
	if(!data) return;
	CCSprite* node = static_cast<CCSprite*>(data);
	float coefficient = node->getMotionBlurCoefficient();
	const CCSize& size = node->getContentSize();
	setUniformLocationWith1f(m_uUniforms[kCEUniformCoefficient], coefficient);
	setUniformLocationWith2f(m_uUniforms[kCEUniformOnePixelSize], 1.0f / size.width, 1.0f / size.height);
	CCGLProgram::setUniformsForBuiltins();
}


void CEGLProgramRadialBlur::updateUniforms()
{
	m_uUniforms[kCEUniformCoefficient] = glGetUniformLocation(m_uProgram, "coefficient");
	m_uUniforms[kCEUniformOnePixelSize] = glGetUniformLocation(m_uProgram, "onePixelSize");
	m_uUniforms[kCEUniformCenter] = glGetUniformLocation(m_uProgram, "center");
	CCGLProgram::updateUniforms();
}

void CEGLProgramRadialBlur::setUniformsForBuiltins(void* data /*= NULL*/)
{
	if(!data) return;
	CCSprite* node = static_cast<CCSprite*>(data);
	const CCPoint& center = node->getRadialBlurCenter();
	float coefficient = node->getRadialBlurCoefficient();
	const CCSize& size = node->getContentSize();
	setUniformLocationWith1f(m_uUniforms[kCEUniformCoefficient], coefficient);
	setUniformLocationWith2f(m_uUniforms[kCEUniformCenter], center.x, center.y);
	setUniformLocationWith2f(m_uUniforms[kCEUniformOnePixelSize], 1.0f / size.width, 1.0f / size.height);
	CCGLProgram::setUniformsForBuiltins();
}

void CEGLProgramGaussBlur::updateUniforms()
{
    /*
	m_uUniforms[kCEUniformResolution] = glGetUniformLocation(m_uProgram, "resolution");
	m_uUniforms[kCEUniformBlurRadius] = glGetUniformLocation(m_uProgram, "blurRadius");
	m_uUniforms[kCEUniformSampleNum] = glGetUniformLocation(m_uProgram, "sampleNum");
	CCGLProgram::updateUniforms();
     */
}

void CEGLProgramGaussBlur::setUniformsForBuiltins( void* data /*= NULL*/ )
{
    /*
	if(!data) return;
	CCSprite* node = static_cast<CCSprite*>(data);
	
	const CCSize &size = node->getContentSize();
	setUniformLocationWith2f(m_uUniforms[kCEUniformResolution], size.width, size.height);
	setUniformLocationWith1f(m_uUniforms[kCEUniformBlurRadius], node->getGaussBlurBlurRadius());
	setUniformLocationWith1f(m_uUniforms[kCEUniformSampleNum], node->getGaussBlurSampleNum());
	
	CCGLProgram::setUniformsForBuiltins();
     */
}

void loadCocosEasyShaders()
{
	// HSL
	{
		CEGLProgramHSL *p = new CEGLProgramHSL();
		p->initWithVertexShaderByteArray(ccPositionTextureColor_vert ,ceShader_PositionTextureColorHSL_frag);
		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		p->link();
		p->updateUniforms();
		CHECK_GL_ERROR_DEBUG();
		CCShaderCache::sharedShaderCache()->addProgram(p, kCCShader_PositionTextureColorHSL);
		p->release();
	}

	// Gray
	{
		CEGLProgramGrey *p = new CEGLProgramGrey();
		p->initWithVertexShaderByteArray(ccPositionTextureColor_vert ,ceShader_PositionTextureColorGrey_frag);
		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		p->link();
		p->updateUniforms();
		CHECK_GL_ERROR_DEBUG();
		CCShaderCache::sharedShaderCache()->addProgram(p, kCCShader_PositionTextureColorGrey);
		p->release();
	}
/*
	// Gaussian Blur
	{
		CEGLProgramGaussBlur *p = new CEGLProgramGaussBlur();
		bool isSucceed = p->initWithVertexShaderByteArray(ccPositionTextureColor_vert ,ceShader_GaussBlur_frag);
		if(isSucceed)
		{
			p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
			p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
			p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
			if(p->link()){
				p->updateUniforms();
				CHECK_GL_ERROR_DEBUG();
				CCShaderCache::sharedShaderCache()->addProgram(p, kCCShader_GaussBlur);
			}else{
				isSucceed = false;
			}
		}
		if(!isSucceed){
			p->cleanAfterCompile();
		}	
		p->release();
	}
 */
}

void reloadCocosEasyShaders()
{
	CCShaderCache *pShaderCache = CCShaderCache::sharedShaderCache();
	// reset all programs and reload them

	// HSL
	{
		CEGLProgramHSL *p = dynamic_cast<CEGLProgramHSL*>(pShaderCache->programForKey(kCCShader_PositionTextureColorHSL));    
		p->reset();
		p->initWithVertexShaderByteArray(ccPositionTextureColor_vert ,ceShader_PositionTextureColorHSL_frag);
		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		p->link();
		p->updateUniforms();
	}

	// Gray
	{
		CEGLProgramGrey *p = dynamic_cast<CEGLProgramGrey*>(pShaderCache->programForKey(kCCShader_PositionTextureColorGrey));
		p->reset();
		p->initWithVertexShaderByteArray(ccPositionTextureColor_vert ,ceShader_PositionTextureColorGrey_frag);
		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		p->link();
		p->updateUniforms();
		CHECK_GL_ERROR_DEBUG();
	}

}