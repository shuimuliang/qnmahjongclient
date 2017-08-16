#ifndef __CC_TYPES_EXT_H__
#define __CC_TYPES_EXT_H__

#include "ccTypes.h"

typedef struct _ccHSL3F
{
	float h;
	float s;
	float l;
} ccHSL3F;

//! helper macro that creates an ccHSL3F type
static inline ccHSL3F
	cchsl3(const float h, const float s, const float l)
{
	ccHSL3F hsl = {h, s, l};
	return hsl;
}


#define DEF_OPT_FLOAT_FUNC(c_type,accuracy) \
	c_type() : _i(0), _f(0), _s(0) {} \
	c_type(float other)  \
	{ \
		_i = (unsigned int)other; \
		_f = (unsigned int)( (other - _i) * (float)accuracy ); \
		_s = other < 0 ? 1 : 0; \
	} \
	c_type& operator = (float other) \
	{ \
		_i = (unsigned int)other; \
		_f = (unsigned int)( (other - _i) * (float)accuracy ); \
		_s = other < 0 ? 1 : 0; \
		return *this; \
	} \
	c_type& operator = (int other) \
	{ \
		_i = (int)other; \
		_s = other < 0 ? 1 : 0; \
		return *this; \
	} \
	c_type& operator = (double other) \
	{ \
		*this = (float)other; \
		return *this; \
	} \
	c_type& operator = (const c_type &right) \
	{ \
		_i = right._i; \
		_f = right._f; \
		_s = right._s; \
		return *this; \
	} \
	float operator+(float right) const {return tofloat() + right;} \
	float operator-(float right) const {return tofloat() - right;} \
	float operator*(float right) const {return tofloat() * right;} \
	float operator/(float right) const {return tofloat() / right;} \
 \
	c_type operator+(const c_type &right) const {return c_type(tofloat() + right.tofloat());} \
	c_type operator-(const c_type &right) const {return c_type(tofloat() - right.tofloat());} \
	c_type operator*(const c_type &right) const {return c_type(tofloat() * right.tofloat());} \
	c_type operator/(const c_type &right) const {return c_type(tofloat() / right.tofloat());} \
 \
	c_type operator+=(float right) {*this = *this + right; return *this;} \
	c_type operator-=(float right) {*this = *this - right; return *this;} \
	c_type operator*=(float right) {*this = *this * right; return *this;} \
	c_type operator/=(float right) {*this = *this / right; return *this;} \
 \
	c_type operator+=(const c_type &right) {*this = *this + right; return *this;} \
	c_type operator-=(const c_type &right) {*this = *this - right; return *this;} \
	c_type operator*=(const c_type &right) {*this = *this * right; return *this;} \
	c_type operator/=(const c_type &right) {*this = *this / right; return *this;} \
 \
	float tofloat() const \
	{ \
		float r = (float)_i + ((float)_f) / (float)accuracy; \
		r = _s == 1 ? -1.0f * r : r; \
		return r; \
	} 

class float21
{
public:
	DEF_OPT_FLOAT_FUNC(float21,10);
	
protected:
	unsigned short _i : 11;
	unsigned short _f : 4;
	unsigned short _s : 1;
};


// +guyi for optimize all graph size of data
#define  OPTIMIZE_TYPE_DATA
#ifdef OPTIMIZE_TYPE_DATA
typedef short opt_int;
typedef short opt_float;
typedef unsigned char opt_uint1;
typedef unsigned short opt_uint2;
typedef float21 opt_float21;
#define _ofx2f(opt) opt.tofloat()
#else
typedef int opt_int;
typedef float opt_float;
typedef unsigned int opt_uint1;
typedef unsigned int opt_uint2;
typedef float opt_float21;
#define _ofx2f(opt) opt
#endif



#endif