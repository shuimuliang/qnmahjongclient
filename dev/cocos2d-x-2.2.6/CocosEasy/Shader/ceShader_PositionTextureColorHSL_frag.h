#ifndef __CE_SHADER_POSITION_TEXTURE_COLOR_HSL_FRAG_H__
#define __CE_SHADER_POSITION_TEXTURE_COLOR_HSL_FRAG_H__

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
uniform float CE_HSL_H;                                 	 \n\
uniform float CE_HSL_S;                                 	 \n\
uniform float CE_HSL_L;                                 	 \n\
															 \n\
void main() {                                           	 \n\
	vec4 texColor = texture2D(CC_Texture0, v_texCoord); 	 \n\
	float r = texColor.r;                               	 \n\
	float g = texColor.g;                               	 \n\
	float b = texColor.b;                               	 \n\
	float a = texColor.a*v_fragmentColor.a;             	 \n\
															 \n\
	float h,s,l;                                        	 \n\
	{                                                   	 \n\
		float max = max(max(r,g),b);                    	 \n\
		float min = min(min(r,g),b);                    	 \n\
															 \n\
		if(max == min){                                 	 \n\
			h = 0.0;                                    	 \n\
		}else if(max == r && g>= b){                    	 \n\
			h = 60.0*(g-b)/(max-min)+0.0;               	 \n\
		}else if(max == r && g<b){                      	 \n\
			h = 60.0*(g-b)/(max-min)+360.0;             	 \n\
		}else if(max == g){                             	 \n\
			h = 60.0*(b-r)/(max-min)+120.0;             	 \n\
		}else if(max == b){                             	 \n\
			h = 60.0*(r-g)/(max-min)+240.0;             	 \n\
		}                                               	 \n\
															 \n\
															 \n\
		l = 0.5*(max+min);                                   \n\
		if(l == 0.0||max == min){                       	 \n\
			s = 0.0;                                    	 \n\
		}else if(0.0 <= l && l <= 0.5){                 	 \n\
			s = (max-min)/(2.0*l);                      	 \n\
		}else if(l>0.5){                                	 \n\
			s = (max-min)/(2.0-2.0*l);                  	 \n\
		}                                               	 \n\
	}                                                   	 \n\
															 \n\
	h = h+CE_HSL_H;                                     	 \n\
	s = min(1.0,max(0.0,s+CE_HSL_S));                   	 \n\
	l = l; 													 \n\
															 \n\
	vec4 finalColor;                                    	 \n\
	{                                                   	 \n\
		float q;                                        	 \n\
		if(l<0.5){                                      	 \n\
			q = l*(1.0+s);                              	 \n\
		}else if(l>= 0.5){                              	 \n\
			q = l+s-l*s;                                	 \n\
		}                                               	 \n\
		float p = 2.0*l-q;                              	 \n\
		float hk = h/360.0;                             	 \n\
		float t[3];                                     	 \n\
		t[0] = hk+1.0/3.0;t[1] = hk;t[2] = hk-1.0/3.0;  	 \n\
		for(int i = 0;i<3;i++){                         	 \n\
			if(t[i]<0.0) t[i] += 1.0;                    	 \n\
			if(t[i]>1.0) t[i] -= 1.0;                    	 \n\
		}												     \n\
		float c[3];                                     	 \n\
		for(int i = 0;i<3;i++){                         	 \n\
			if(t[i]<1.0/6.0){                           	 \n\
				c[i] = p+((q-p)*6.0*t[i]);              	 \n\
			}else if(1.0/6.0 <= t[i] && t[i]<0.5){      	 \n\
				c[i] = q;                               	 \n\
			}else if(0.5 <= t[i] && t[i]<2.0/3.0){      	 \n\
				c[i] = p+((q-p)*6.0*(2.0/3.0-t[i]));    	 \n\
			}else{                                      	 \n\
				c[i] = p;                               	 \n\
			}                                           	 \n\
		}                                               	 \n\
		finalColor = vec4(c[0],c[1],c[2],a);            	 \n\
	}                                                   	 \n\
															 \n\
	if(a>0.0){												 \n\
	finalColor += vec4(CE_HSL_L*a,CE_HSL_L*a,CE_HSL_L*a,0.0);\n\
	}														 \n\
	gl_FragColor = finalColor;                          	 \n\
}															 \n\
";

#endif