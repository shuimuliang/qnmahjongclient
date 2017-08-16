#ifndef __CE_SHADER_POSITION_TEXTURE_OUTLINE_FRAG_H__
#define __CE_SHADER_POSITION_TEXTURE_OUTLINE_FRAG_H__

/*
http://www.idevgames.com/forums/thread-3010.html
*/

"															 											\n\
#ifdef GL_ES                                            	 											\n\
precision mediump float;                                	 											\n\
#endif                                                  	 											\n\
															 											\n\
varying vec2 v_texCoord;																				\n\
varying vec4 v_fragmentColor;																			\n\
																										\n\
uniform vec3 u_outlineColor;																			\n\
uniform float u_threshold;																				\n\
uniform float u_radius_x;																				\n\
uniform float u_radius_y;																				\n\
																										\n\
uniform sampler2D CC_Texture0;                          												\n\
																										\n\
void main()																								\n\
{																										\n\
	vec4 accum = vec4(0.0);																				\n\
	vec4 normal = texture2D(CC_Texture0, v_texCoord);													\n\
																										\n\
	accum += texture2D(CC_Texture0, vec2(v_texCoord.x - u_radius_x, v_texCoord.y - u_radius_y));		\n\
	accum += texture2D(CC_Texture0, vec2(v_texCoord.x + u_radius_x, v_texCoord.y - u_radius_y));		\n\
	accum += texture2D(CC_Texture0, vec2(v_texCoord.x + u_radius_x, v_texCoord.y + u_radius_y));		\n\
	accum += texture2D(CC_Texture0, vec2(v_texCoord.x - u_radius_x, v_texCoord.y + u_radius_y));		\n\
																										\n\
	accum *= u_threshold;																				\n\
	accum.rgb =  u_outlineColor * accum.a;																\n\
																										\n\
	normal = ( accum * (1.0 - normal.a)) + (normal * normal.a);											\n\
																										\n\
	gl_FragColor = v_fragmentColor * normal;															\n\
}																										\n\
";	
#endif