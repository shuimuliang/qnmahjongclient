#ifndef __CE_SHADER_RADIAL_BLUR_FRAG_H__
#define __CE_SHADER_RADIAL_BLUR_FRAG_H__


"															\n\
#ifdef GL_ES                                            	\n\
precision mediump float;                                	\n\
#endif                                                  	\n\
															\n\
varying vec4 v_fragmentColor;                           	\n\
varying vec2 v_texCoord;                                	\n\
															\n\
uniform sampler2D CC_Texture0;                          	\n\
uniform vec2 onePixelSize;									\n\
uniform float coefficient;									\n\
uniform vec2 center;										\n\
void main() {												\n\
	vec2 curPos = center-v_texCoord;						\n\
	float dis = length(curPos);								\n\
	float coefficientDis = coefficient*dis;                 \n\
	float sinF = curPos.y/dis;								\n\
	float cosF = curPos.x/dis;								\n\
	if(coefficientDis > 0.0) {                              \n\
		vec4 sum = vec4(0.0);                                 \n\
		vec2 offset;                                           \n\
		float weight;                                            \n\
		float squareX;											\n\
		float sumWeight;										\n\
		                                                           \n\
		for(float dx = 0.0; dx <= coefficientDis; dx += 1.0) {\n\
			weight = (coefficientDis - dx)*(coefficientDis - dx);\n\
			\n\
			offset.x = -dx * cosF * onePixelSize.x;\n\
			offset.y = -dx * sinF *onePixelSize.x;\n\
			sum += texture2D(CC_Texture0, v_texCoord + offset) * weight;\n\
			sumWeight += weight;\n\
			\n\
			offset.x = dx * cosF * onePixelSize.x;\n\
			offset.y = dx * sinF *onePixelSize.x;\n\
			sum += texture2D(CC_Texture0, v_texCoord + offset) * weight;\n\
			sumWeight += weight;\n\
			\n\
		}\n\
		gl_FragColor = sum/sumWeight * v_fragmentColor;\n\
	}\n\
	else {\n\
		gl_FragColor = texture2D(CC_Texture0, v_texCoord) * v_fragmentColor;\n\
	}\n\
}															 \n\
";

#endif