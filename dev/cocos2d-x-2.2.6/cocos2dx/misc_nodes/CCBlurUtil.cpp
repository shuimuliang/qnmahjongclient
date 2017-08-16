/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2009      Jason Booth

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCBlurUtil.h"

/// Stackblur algorithm body
void stackblurJob(unsigned char* src,				///< input image data
                  unsigned int w,					///< image width
                  unsigned int h,					///< image height
                  unsigned int radius,				///< blur intensity (should be in 2..254 range)
                  int cores,						///< total number of working threads
                  int core,							///< current thread number
                  int step,							///< step of processing (1,2)
                  unsigned char* stack				///< stack buffer
)
{
    unsigned int x, y, xp, yp, i;
    unsigned int sp;
    unsigned int stack_start;
    unsigned char* stack_ptr;
    
    unsigned char* src_ptr;
    unsigned char* dst_ptr;
    
    unsigned long sum_r;
    unsigned long sum_g;
    unsigned long sum_b;
    unsigned long sum_a;
    unsigned long sum_in_r;
    unsigned long sum_in_g;
    unsigned long sum_in_b;
    unsigned long sum_in_a;
    unsigned long sum_out_r;
    unsigned long sum_out_g;
    unsigned long sum_out_b;
    unsigned long sum_out_a;
    
    unsigned int wm = w - 1;
    unsigned int hm = h - 1;
    unsigned int w4 = w * 4;
    unsigned int div = (radius * 2) + 1;
    unsigned int mul_sum = stackblur_mul[radius];
    unsigned char shr_sum = stackblur_shr[radius];
    
    
    if (step == 1)
    {
        int minY = core * h / cores;
        int maxY = (core + 1) * h / cores;
        
        for(y = minY; y < maxY; y++)
        {
            sum_r = sum_g = sum_b = sum_a =
            sum_in_r = sum_in_g = sum_in_b = sum_in_a =
            sum_out_r = sum_out_g = sum_out_b = sum_out_a = 0;
            
            src_ptr = src + w4 * y; // start of line (0,y)
            
            for(i = 0; i <= radius; i++)
            {
                stack_ptr    = &stack[ 4 * i ];
                stack_ptr[0] = src_ptr[0];
                stack_ptr[1] = src_ptr[1];
                stack_ptr[2] = src_ptr[2];
                stack_ptr[3] = src_ptr[3];
                sum_r += src_ptr[0] * (i + 1);
                sum_g += src_ptr[1] * (i + 1);
                sum_b += src_ptr[2] * (i + 1);
                sum_a += src_ptr[3] * (i + 1);
                sum_out_r += src_ptr[0];
                sum_out_g += src_ptr[1];
                sum_out_b += src_ptr[2];
                sum_out_a += src_ptr[3];
            }
            
            
            for(i = 1; i <= radius; i++)
            {
                if (i <= wm) src_ptr += 4;
                stack_ptr = &stack[ 4 * (i + radius) ];
                stack_ptr[0] = src_ptr[0];
                stack_ptr[1] = src_ptr[1];
                stack_ptr[2] = src_ptr[2];
                stack_ptr[3] = src_ptr[3];
                sum_r += src_ptr[0] * (radius + 1 - i);
                sum_g += src_ptr[1] * (radius + 1 - i);
                sum_b += src_ptr[2] * (radius + 1 - i);
                sum_a += src_ptr[3] * (radius + 1 - i);
                sum_in_r += src_ptr[0];
                sum_in_g += src_ptr[1];
                sum_in_b += src_ptr[2];
                sum_in_a += src_ptr[3];
            }
            
            
            sp = radius;
            xp = radius;
            if (xp > wm) xp = wm;
            src_ptr = src + 4 * (xp + y * w); //   img.pix_ptr(xp, y);
            dst_ptr = src + y * w4; // img.pix_ptr(0, y);
            for(x = 0; x < w; x++)
            {
                dst_ptr[0] = (sum_r * mul_sum) >> shr_sum;
                dst_ptr[1] = (sum_g * mul_sum) >> shr_sum;
                dst_ptr[2] = (sum_b * mul_sum) >> shr_sum;
                dst_ptr[3] = (sum_a * mul_sum) >> shr_sum;
                dst_ptr += 4;
                
                sum_r -= sum_out_r;
                sum_g -= sum_out_g;
                sum_b -= sum_out_b;
                sum_a -= sum_out_a;
                
                stack_start = sp + div - radius;
                if (stack_start >= div) stack_start -= div;
                stack_ptr = &stack[4 * stack_start];
                
                sum_out_r -= stack_ptr[0];
                sum_out_g -= stack_ptr[1];
                sum_out_b -= stack_ptr[2];
                sum_out_a -= stack_ptr[3];
                
                if(xp < wm)
                {
                    src_ptr += 4;
                    ++xp;
                }
                
                stack_ptr[0] = src_ptr[0];
                stack_ptr[1] = src_ptr[1];
                stack_ptr[2] = src_ptr[2];
                stack_ptr[3] = src_ptr[3];
                
                sum_in_r += src_ptr[0];
                sum_in_g += src_ptr[1];
                sum_in_b += src_ptr[2];
                sum_in_a += src_ptr[3];
                sum_r    += sum_in_r;
                sum_g    += sum_in_g;
                sum_b    += sum_in_b;
                sum_a    += sum_in_a;
                
                ++sp;
                if (sp >= div) sp = 0;
                stack_ptr = &stack[sp*4];
                
                sum_out_r += stack_ptr[0];
                sum_out_g += stack_ptr[1];
                sum_out_b += stack_ptr[2];
                sum_out_a += stack_ptr[3];
                sum_in_r  -= stack_ptr[0];
                sum_in_g  -= stack_ptr[1];
                sum_in_b  -= stack_ptr[2];
                sum_in_a  -= stack_ptr[3];
                
                
            }
            
        }
    }
    
    // step 2
    if (step == 2)
    {
        int minX = core * w / cores;
        int maxX = (core + 1) * w / cores;
        
        for(x = minX; x < maxX; x++)
        {
            sum_r =	sum_g =	sum_b =	sum_a =
            sum_in_r = sum_in_g = sum_in_b = sum_in_a =
            sum_out_r = sum_out_g = sum_out_b = sum_out_a = 0;
            
            src_ptr = src + 4 * x; // x,0
            for(i = 0; i <= radius; i++)
            {
                stack_ptr    = &stack[i * 4];
                stack_ptr[0] = src_ptr[0];
                stack_ptr[1] = src_ptr[1];
                stack_ptr[2] = src_ptr[2];
                stack_ptr[3] = src_ptr[3];
                sum_r           += src_ptr[0] * (i + 1);
                sum_g           += src_ptr[1] * (i + 1);
                sum_b           += src_ptr[2] * (i + 1);
                sum_a           += src_ptr[3] * (i + 1);
                sum_out_r       += src_ptr[0];
                sum_out_g       += src_ptr[1];
                sum_out_b       += src_ptr[2];
                sum_out_a       += src_ptr[3];
            }
            for(i = 1; i <= radius; i++)
            {
                if(i <= hm) src_ptr += w4; // +stride
                
                stack_ptr = &stack[4 * (i + radius)];
                stack_ptr[0] = src_ptr[0];
                stack_ptr[1] = src_ptr[1];
                stack_ptr[2] = src_ptr[2];
                stack_ptr[3] = src_ptr[3];
                sum_r += src_ptr[0] * (radius + 1 - i);
                sum_g += src_ptr[1] * (radius + 1 - i);
                sum_b += src_ptr[2] * (radius + 1 - i);
                sum_a += src_ptr[3] * (radius + 1 - i);
                sum_in_r += src_ptr[0];
                sum_in_g += src_ptr[1];
                sum_in_b += src_ptr[2];
                sum_in_a += src_ptr[3];
            }
            
            sp = radius;
            yp = radius;
            if (yp > hm) yp = hm;
            src_ptr = src + 4 * (x + yp * w); // img.pix_ptr(x, yp);
            dst_ptr = src + 4 * x; 			  // img.pix_ptr(x, 0);
            for(y = 0; y < h; y++)
            {
                dst_ptr[0] = (sum_r * mul_sum) >> shr_sum;
                dst_ptr[1] = (sum_g * mul_sum) >> shr_sum;
                dst_ptr[2] = (sum_b * mul_sum) >> shr_sum;
                dst_ptr[3] = (sum_a * mul_sum) >> shr_sum;
                dst_ptr += w4;
                
                sum_r -= sum_out_r;
                sum_g -= sum_out_g;
                sum_b -= sum_out_b;
                sum_a -= sum_out_a;
                
                stack_start = sp + div - radius;
                if(stack_start >= div) stack_start -= div;
                stack_ptr = &stack[4 * stack_start];
                
                sum_out_r -= stack_ptr[0];
                sum_out_g -= stack_ptr[1];
                sum_out_b -= stack_ptr[2];
                sum_out_a -= stack_ptr[3];
                
                if(yp < hm)
                {
                    src_ptr += w4; // stride
                    ++yp;
                }
                
                stack_ptr[0] = src_ptr[0];
                stack_ptr[1] = src_ptr[1];
                stack_ptr[2] = src_ptr[2];
                stack_ptr[3] = src_ptr[3];
                
                sum_in_r += src_ptr[0];
                sum_in_g += src_ptr[1];
                sum_in_b += src_ptr[2];
                sum_in_a += src_ptr[3];
                sum_r    += sum_in_r;
                sum_g    += sum_in_g;
                sum_b    += sum_in_b;
                sum_a    += sum_in_a;
                
                ++sp;
                if (sp >= div) sp = 0;
                stack_ptr = &stack[sp*4];
                
                sum_out_r += stack_ptr[0];
                sum_out_g += stack_ptr[1];
                sum_out_b += stack_ptr[2];
                sum_out_a += stack_ptr[3];
                sum_in_r  -= stack_ptr[0];
                sum_in_g  -= stack_ptr[1];
                sum_in_b  -= stack_ptr[2];
                sum_in_a  -= stack_ptr[3];
            }
        }
    }
    
}



//********************************************* 快速压缩算法 **********************************//
//快速压缩访问一个点的函数可以写为：
inline TARGB32& Pixels(const TPicRegion& pic,const long x,const long y)
{
    return ( (TARGB32*)((TUInt8*)pic.pdata+pic.byte_width*y) )[x];
}
void PicZoom3(const TPicRegion& Dst,const TPicRegion& Src)
{
    if (  (0==Dst.width)||(0==Dst.height)
        ||(0==Src.width)||(0==Src.height)) return;
    unsigned long xrIntFloat_16=(Src.width<<16)/Dst.width;
    unsigned long yrIntFloat_16=(Src.height<<16)/Dst.height;
    unsigned long dst_width=Dst.width;
    TARGB32* pDstLine=Dst.pdata;
    unsigned long srcy_16=0;
    for (unsigned long y=0;y<Dst.height;++y)
    {
        TARGB32* pSrcLine=((TARGB32*)((TUInt8*)Src.pdata+Src.byte_width*(srcy_16>>16)));
        unsigned long srcx_16=0;
        for (unsigned long x=0;x<dst_width;++x)
        {
            pDstLine[x]=pSrcLine[srcx_16>>16];
            srcx_16+=xrIntFloat_16;
        }
        srcy_16+=yrIntFloat_16;
        ((TUInt8*&)pDstLine)+=Dst.byte_width;
    }
}
