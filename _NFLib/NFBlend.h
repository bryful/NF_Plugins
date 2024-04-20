#pragma once
#ifndef NFBLEND_H
#define NFBLEND_H

#include "AE_SDK.h"
#include "NFUtils.h"


inline PF_Pixel PixelBlend8(PF_Pixel bm, PF_Pixel tp)
{
	if (tp.alpha == 0)
	{
		return bm;
	}
	else if (tp.alpha >= PF_MAX_CHAN8)
	{
		return tp;
	}
	bm = ToPremultiplied8(bm);
	tp = ToPremultiplied8(tp);

	PF_Pixel ret = { 0,0,0,0 };
	
	double d1 = (double)tp.alpha / PF_MAX_CHAN8;
	double d2 = 1.0 - d1;

	ret.red = RoundByteFpLong((double)bm.red * d2 + (double)tp.red * d1);
	ret.green= RoundByteFpLong((double)bm.green * d2 + (double)tp.green * d1);
	ret.blue = RoundByteFpLong((double)bm.blue * d2 + (double)tp.blue * d1);
	ret.alpha = RoundByteFpLong((double)bm.alpha + (double)tp.alpha - (double)bm.alpha *d1);

	ret = ToStraight8(ret);
	return ret;
}
inline PF_Pixel16 PixelBlend16(PF_Pixel16 bm, PF_Pixel16 tp)
{
	if (tp.alpha == 0)
	{
		return bm;
	}
	else if (tp.alpha >= PF_MAX_CHAN16)
	{
		return tp;
	}
	bm = ToPremultiplied16(bm);
	tp = ToPremultiplied16(tp);

	PF_Pixel16 ret = { 0,0,0,0 };

	double d1 = (double)tp.alpha / PF_MAX_CHAN16;
	double d2 = 1 - d1;

	ret.red = RoundShortFpLong((double)bm.red * d2 + (double)tp.red * d1);
	ret.green = RoundShortFpLong((double)bm.green * d2 + (double)tp.green * d1);
	ret.blue = RoundShortFpLong((double)bm.blue * d2 + (double)tp.blue * d1);
	ret.alpha = RoundShortFpLong((double)bm.alpha + (double)tp.alpha - (double)bm.alpha * d1);

	ret = ToStraight16(ret);
	return ret;
}
inline PF_Pixel32 PixelBlend32(PF_Pixel32 bm, PF_Pixel32 tp)
{
	if (tp.alpha <= 0)
	{
		return bm;
	}
	else if (tp.alpha >= 1.0)
	{
		tp.alpha = 1.0;
		return tp;
	}
	bm = ToPremultiplied32(bm);
	tp = ToPremultiplied32(tp);

	PF_Pixel32 ret = { 0,0,0,0 };

	double d1 = (double)tp.alpha;
	double d2 = 1 - d1;

	ret.red = RoundFpShortDouble((double)bm.red * d2 + (double)tp.red * d1);
	ret.green = RoundFpShortDouble((double)bm.green * d2 + (double)tp.green * d1);
	ret.blue = RoundFpShortDouble((double)bm.blue * d2 + (double)tp.blue * d1);
	ret.alpha = RoundFpShortDouble((double)bm.alpha + (double)tp.alpha - (double)bm.alpha * (double)tp.alpha);

	ret = ToStraight32(ret);
	return ret;
}

inline PF_Pixel PixelBlendAdd8(PF_Pixel bm, PF_Pixel tp)
{
	if (tp.alpha == 0)
	{
		return bm;
	}
	bm = ToPremultiplied8(bm);
	tp = ToPremultiplied8(tp);

	PF_Pixel ret = { 0,0,0,0 };


	ret.red = RoundByteFpLong((double)bm.red  + (double)tp.red );
	ret.green = RoundByteFpLong((double)bm.green + (double)tp.green);
	ret.blue = RoundByteFpLong((double)bm.blue + (double)tp.blue);
	ret.alpha = RoundByteFpLong((double)bm.alpha + (double)tp.alpha - (double)bm.alpha * (double)tp.alpha/PF_MAX_CHAN8);

	ret = ToStraight8(ret);
	return ret;
}
inline PF_Pixel16 PixelBlendAdd16(PF_Pixel16 bm, PF_Pixel16 tp)
{
	if (tp.alpha == 0)
	{
		return bm;
	}
	bm = ToPremultiplied16(bm);
	tp = ToPremultiplied16(tp);

	PF_Pixel16 ret = { 0,0,0,0 };


	ret.red = RoundShortFpLong((double)bm.red + (double)tp.red);
	ret.green = RoundShortFpLong((double)bm.green + (double)tp.green);
	ret.blue = RoundShortFpLong((double)bm.blue + (double)tp.blue);
	ret.alpha = RoundShortFpLong((double)bm.alpha + (double)tp.alpha - (double)bm.alpha * (double)tp.alpha / PF_MAX_CHAN16);

	ret = ToStraight16(ret);
	return ret;
}
inline PF_Pixel32 PixelBlendAdd32(PF_Pixel32 bm, PF_Pixel32 tp)
{
	if (tp.alpha == 0)
	{
		return bm;
	}
	bm = ToPremultiplied32(bm);
	tp = ToPremultiplied32(tp);

	PF_Pixel32 ret = { 0,0,0,0 };

	ret.red = RoundFpShortDouble((double)bm.red + (double)tp.red);
	ret.green = RoundFpShortDouble((double)bm.green + (double)tp.green);
	ret.blue = RoundFpShortDouble((double)bm.blue + (double)tp.blue);
	ret.alpha = RoundFpShortDouble((double)bm.alpha + (double)tp.alpha - (double)bm.alpha * (double)tp.alpha);

	ret = ToStraight32(ret);
	return ret;
}

inline PF_Pixel PixelBlendMultiply8(PF_Pixel bm, PF_Pixel tp)
{

	if (tp.alpha == 0)
	{
		return bm;
	}
	bm = ToPremultipliedW8(bm);
	tp = ToPremultipliedW8(tp);

	PF_Pixel ret = { 0,0,0,0 };
		 

	double r1 = (double)bm.red / PF_MAX_CHAN8;
	double g1 = (double)bm.green / PF_MAX_CHAN8;
	double b1 = (double)bm.blue / PF_MAX_CHAN8;
	double a1 = (double)bm.alpha / PF_MAX_CHAN8;
	double r2 = (double)tp.red / PF_MAX_CHAN8;
	double g2 = (double)tp.green / PF_MAX_CHAN8;
	double b2 = (double)tp.blue / PF_MAX_CHAN8;
	double a2 = (double)tp.alpha / PF_MAX_CHAN8;

	ret.red = RoundByteFpLong(r1 * r2 * PF_MAX_CHAN8);
	ret.green = RoundByteFpLong(g1 * g2 * PF_MAX_CHAN8);
	ret.blue = RoundByteFpLong(b1 * b2 * PF_MAX_CHAN8);
	ret.alpha = RoundByteFpLong(  (a1 + a2 -a1*a2) * PF_MAX_CHAN8);

	ret = ToStraightW8(ret);
	return ret;
}
inline PF_Pixel16 PixelBlendMultiply16(PF_Pixel16 bm, PF_Pixel16 tp)
{
	if (tp.alpha == 0)
	{
		return bm;
	}
	bm = ToPremultipliedW16(bm);
	tp = ToPremultipliedW16(tp);

	PF_Pixel16 ret = { 0,0,0,0 };

	double r1 = (double)bm.red / PF_MAX_CHAN16;
	double g1 = (double)bm.green / PF_MAX_CHAN16;
	double b1 = (double)bm.blue / PF_MAX_CHAN16;
	double a1 = (double)bm.alpha / PF_MAX_CHAN16;
	double r2 = (double)tp.red / PF_MAX_CHAN16;
	double g2 = (double)tp.green / PF_MAX_CHAN16;
	double b2 = (double)tp.blue / PF_MAX_CHAN16;
	double a2 = (double)tp.alpha / PF_MAX_CHAN16;

	ret.red = RoundShortFpLong(r1 * r2 * PF_MAX_CHAN16);
	ret.green = RoundShortFpLong(g1 * g2 * PF_MAX_CHAN16);
	ret.blue = RoundShortFpLong(b1 * b2 * PF_MAX_CHAN16);
	ret.alpha = RoundShortFpLong((a1 + a2 - a1 * a2) * PF_MAX_CHAN16);
	ret = ToStraightW16(ret);
	return ret;
}
inline PF_Pixel32 PixelBlendMultiply32(PF_Pixel32 bm, PF_Pixel32 tp)
{
	if (tp.alpha == 0)
	{
		return bm;
	}
	bm = ToPremultipliedW32(bm);
	tp = ToPremultipliedW32(tp);

	PF_Pixel32 ret = { 0,0,0,0 };

	double r1 = (double)bm.red;
	double g1 = (double)bm.green;
	double b1 = (double)bm.blue;
	double a1 = (double)bm.alpha;
	if (a1 > 1) a1 = 1;
	double r2 = (double)tp.red;
	double g2 = (double)tp.green;
	double b2 = (double)tp.blue;
	double a2 = (double)tp.alpha;
	if (a2 > 1) a2 = 1;

	ret.red = RoundFpShortDouble(r1 * r2);
	ret.green = RoundFpShortDouble(g1 * g2);
	ret.blue = RoundFpShortDouble(b1 * b2);
	ret.alpha = RoundFpShortDouble(a1 + a2 - a1 * a2);
	ret = ToStraightW32(ret);
	return ret;
}


inline PF_Pixel PixelBlendScreen8(PF_Pixel bm, PF_Pixel tp)
{

	if (tp.alpha == 0)
	{
		return bm;
	}
	bm = ToPremultiplied8(bm);
	tp = ToPremultiplied8(tp);

	PF_Pixel ret = { 0,0,0,0 };


	double r1 = (double)bm.red / PF_MAX_CHAN8;
	double g1 = (double)bm.green / PF_MAX_CHAN8;
	double b1 = (double)bm.blue / PF_MAX_CHAN8;
	double a1 = (double)bm.alpha / PF_MAX_CHAN8;
	double r2 = (double)tp.red / PF_MAX_CHAN8;
	double g2 = (double)tp.green / PF_MAX_CHAN8;
	double b2 = (double)tp.blue / PF_MAX_CHAN8;
	double a2 = (double)tp.alpha / PF_MAX_CHAN8;

	ret.red = RoundByteFpLong  ((r1+r2 - r1 * r2) * PF_MAX_CHAN8);
	ret.green = RoundByteFpLong((g1+g2 - g1 * g2) * PF_MAX_CHAN8);
	ret.blue = RoundByteFpLong (( b1 + b2 -b1 * b2) * PF_MAX_CHAN8);
	ret.alpha = RoundByteFpLong((a1 + a2 - a1 * a2) * PF_MAX_CHAN8);

	ret = ToStraight8(ret);
	return ret;
}
inline PF_Pixel16 PixelBlendScreen16(PF_Pixel16 bm, PF_Pixel16 tp)
{

	if (tp.alpha == 0)
	{
		return bm;
	}
	bm = ToPremultiplied16(bm);
	tp = ToPremultiplied16(tp);

	PF_Pixel16 ret = { 0,0,0,0 };


	double r1 = (double)bm.red / PF_MAX_CHAN16;
	double g1 = (double)bm.green / PF_MAX_CHAN16;
	double b1 = (double)bm.blue / PF_MAX_CHAN16;
	double a1 = (double)bm.alpha / PF_MAX_CHAN16;
	double r2 = (double)tp.red / PF_MAX_CHAN16;
	double g2 = (double)tp.green / PF_MAX_CHAN16;
	double b2 = (double)tp.blue / PF_MAX_CHAN16;
	double a2 = (double)tp.alpha / PF_MAX_CHAN16;

	ret.red = RoundShortFpLong((r1 + r2 - r1 * r2) * PF_MAX_CHAN16);
	ret.green = RoundShortFpLong((g1 + g2 - g1 * g2) * PF_MAX_CHAN16);
	ret.blue = RoundShortFpLong((b1 + b2 - b1 * b2) * PF_MAX_CHAN16);
	ret.alpha = RoundShortFpLong((a1 + a2 - a1 * a2) * PF_MAX_CHAN16);

	ret = ToStraight16(ret);
	return ret;
}
inline PF_Pixel32 PixelBlendScreen32(PF_Pixel32 bm, PF_Pixel32 tp)
{

	if (tp.alpha == 0)
	{
		return bm;
	}
	bm = ToPremultiplied32(bm);
	tp = ToPremultiplied32(tp);

	PF_Pixel32 ret = { 0,0,0,0 };


	double r1 = (double)bm.red;
	double g1 = (double)bm.green;
	double b1 = (double)bm.blue;
	double a1 = (double)bm.alpha;
	double r2 = (double)tp.red;
	double g2 = (double)tp.green;
	double b2 = (double)tp.blue;
	double a2 = (double)tp.alpha;

	ret.red = RoundFpShortDouble((r1 + r2 - r1 * r2));
	ret.green = RoundFpShortDouble((g1 + g2 - g1 * g2));
	ret.blue = RoundFpShortDouble((b1 + b2 - b1 * b2));
	ret.alpha = RoundFpShortDouble((a1 + a2 - a1 * a2));

	ret = ToStraight32(ret);
	return ret;
}

inline PF_Pixel PixelBlendOverlay8(PF_Pixel bm, PF_Pixel tp)
{
	if (tp.alpha == 0)
	{
		return bm;
	}
	PF_FpLong a = (PF_FpLong)bm.alpha / PF_MAX_CHAN8;
	double r = ((PF_FpLong)bm.red   / PF_MAX_CHAN8) * a;
	double g = ((PF_FpLong)bm.green / PF_MAX_CHAN8) * a;
	double b = ((PF_FpLong)bm.blue  / PF_MAX_CHAN8) * a;
	double v = (0.29891 * r + 0.58661 * g + 0.11448 * b);

	if (v == 0.5)
	{
		return bm;
	}
	else if (v < 0.5) {
		return PixelBlendMultiply8(bm, tp);
	}
	else {
		return PixelBlendScreen8(bm, tp);
	}
}
inline PF_Pixel16 PixelBlendOverlay16(PF_Pixel16 bm, PF_Pixel16 tp)
{
	if (tp.alpha == 0)
	{
		return bm;
	}
	PF_FpLong a = (PF_FpLong)bm.alpha / PF_MAX_CHAN16;
	double r = ((PF_FpLong)bm.red / PF_MAX_CHAN16) * a;
	double g = ((PF_FpLong)bm.green / PF_MAX_CHAN16) * a;
	double b = ((PF_FpLong)bm.blue / PF_MAX_CHAN16) * a;
	double v = (0.29891 * r + 0.58661 * g + 0.11448 * b);

	if (v == 0.5)
	{
		return bm;
	}
	else if (v < 0.5) {
		return PixelBlendMultiply16(bm, tp);
	}
	else {
		return PixelBlendScreen16(bm, tp);
	}
}
inline PF_Pixel32 PixelBlendOverlay32(PF_Pixel32 bm, PF_Pixel32 tp)
{
	if (tp.alpha == 0)
	{
		return bm;
	}
	PF_FpLong a = (PF_FpLong)bm.alpha;
	double r = ((PF_FpLong)bm.red) * a;
	double g = ((PF_FpLong)bm.green) * a;
	double b = ((PF_FpLong)bm.blue) * a;
	double v = (0.29891 * r + 0.58661 * g + 0.11448 * b);

	if (v == 0.5)
	{
		return bm;
	}
	else if (v < 0.5) {
		return PixelBlendMultiply32(bm, tp);
	}
	else {
		return PixelBlendScreen32(bm, tp);
	}
}


#endif