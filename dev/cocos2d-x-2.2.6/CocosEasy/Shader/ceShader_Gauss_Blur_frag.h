// Shader taken from: http://webglsamples.googlecode.com/hg/electricflower/electricflower.html

#ifndef __CE_SHADER_GAUSS_BLUR_FRAG_H__
#define __CE_SHADER_GAUSS_BLUR_FRAG_H__

"																		\n\
#ifdef GL_ES															\n\
precision mediump float;												\n\
#endif																	\n\
																		\n\
varying vec4 v_fragmentColor;											\n\
varying vec2 v_texCoord;												\n\
uniform sampler2D CC_Texture0;                     						\n\
uniform vec2 resolution;												\n\
uniform float blurRadius;												\n\
uniform float sampleNum;												\n\
vec4 blur(vec2);														\n\
																		\n\
void main(void)															\n\
{																		\n\
	gl_FragColor = blur(v_texCoord) * v_fragmentColor;						\n\
}																		\n\
																		\n\
vec4 blur(vec2 p)														\n\
{																		\n\
	if (blurRadius > 0.0 && sampleNum > 1.0)							\n\
	{																	\n\
		vec4 col = vec4(0);												\n\
		vec2 unit = 1.0 / resolution.xy;								\n\
																		\n\
		float r = blurRadius;											\n\
                                                                        \n\
																		\n\
		float count = 0.0;												\n\
																		\n\
		for(float x = -r; x < r; x += sampleNum)						\n\
		{																						\n\
			for(float y = -r; y < r; y += sampleNum)											\n\
			{																					\n\
				col += texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y));\n\
				++ count;										\n\
			}															\n\
		}																\n\
																		\n\
		return col / count;												\n\
	}																	\n\
																		\n\
	return texture2D(CC_Texture0, p);								\n\
}																		\n\
";																		
#endif //__CE_SHADER_GAUSS_BLUR_FRAG_H__