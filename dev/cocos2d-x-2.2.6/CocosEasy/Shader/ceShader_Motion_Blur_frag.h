#ifndef __CE_SHADER_MOTION_BLUR_FRAG_H__
#define __CE_SHADER_MOTION_BLUR_FRAG_H__

//algorithm ref to: http://zh.wikipedia.org/wiki/HSL
//other link: http://baike.baidu.com/subview/541362/8445478.htm?fr=aladdin

"															 \n\
#ifdef GL_ES                                            	 \n\
precision mediump float;                                	 \n\
#endif                                                  	 \n\
															 \n\
varying vec4 v_fragmentColor;                           	 \n\
varying vec2 v_texCoord;                                	 \n\
															 \n\
uniform sampler2D CC_Texture0;                          	 \n\
uniform float coefficient;  		                              	 \n\
uniform vec2 onePixelSize;                     \n\
void main() {												 \n\
	float sinF = 0.5;										\n\
	float cosF = 0.86;														\n\
	if(coefficient > 0.0) {                                  \n\
		vec4 sum = vec4(0.0);                                 \n\
		vec2 offset;                                           \n\
		float weight;                                            \n\
		float squareX;											\n\
		float sumWeight = 1.0;										\n\
		                                                           \n\
		for(float dx = 0.0; dx <= coefficient; dx += 1.0) {\n\
			weight = (coefficient - dx)*(coefficient - dx);\n\
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