#ifndef __CE_SHADER_POSITION_TEXTURE_COLOR_GRAY_FRAG_H__
#define __CE_SHADER_POSITION_TEXTURE_COLOR_GRAY_FRAG_H__

"																		\n\
#ifdef GL_ES															\n\
precision lowp float;													\n\
#endif																	\n\
																		\n\
varying vec4 v_fragmentColor;											\n\
varying vec2 v_texCoord;												\n\
uniform sampler2D CC_Texture0;											\n\
																		\n\
void main()																\n\
{																		\n\
	vec4 col = texture2D(CC_Texture0, v_texCoord) * v_fragmentColor;	\n\
	float grey = dot(col.xyz, vec3(0.299, 0.587, 0.114));				\n\
	gl_FragColor = vec4(grey, grey, grey, col.a);						\n\
}																		\n\
";
#endif