#include "NoiseHorizon.h"


static PF_Err
RGBShift8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err			err = PF_Err_NONE;
	RGBShiftInfo* infoP = reinterpret_cast<RGBShiftInfo*>(refcon);

	PF_Pixel rp = { 0,0,0,0 };
	PF_Pixel gp = { 0,0,0,0 };
	PF_Pixel bp = { 0,0,0,0 };

	A_u_char r = 0;
	A_u_char g = 0;
	A_u_char b = 0;
	PF_FpLong a = 0;
	rp = infoP->nfworld->GetPix8(xL - infoP->RShift, yL);
	r = rp.red;
	a += rp.alpha / 3;
	gp = infoP->nfworld->GetPix8(xL - infoP->GShift, yL);
	g = gp.green;
	a += gp.alpha / 3;
	bp = infoP->nfworld->GetPix8(xL - infoP->BShift, yL);
	b = bp.blue;
	a += bp.alpha / 3;

	outP->red = r;
	outP->green = g;
	outP->blue = b;
	outP->alpha = RoundByteFpLong(a);

	return err;
}
//*************************************************************************************
static PF_Err
RGBShift16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err			err = PF_Err_NONE;
	RGBShiftInfo* infoP = reinterpret_cast<RGBShiftInfo*>(refcon);

	PF_Pixel16 rp = { 0,0,0,0 };
	PF_Pixel16 gp = { 0,0,0,0 };
	PF_Pixel16 bp = { 0,0,0,0 };

	A_u_short r = 0;
	A_u_short g = 0;
	A_u_short b = 0;
	PF_FpLong a = 0;
	rp = infoP->nfworld->GetPix16(xL - infoP->RShift, yL);
	r = rp.red;
	a += rp.alpha / 3;
	gp = infoP->nfworld->GetPix16(xL - infoP->GShift, yL);
	g = gp.green;
	a += gp.alpha / 3;
	bp = infoP->nfworld->GetPix16(xL - infoP->BShift, yL);
	b = bp.blue;
	a += bp.alpha / 3;

	outP->red = r;
	outP->green = g;
	outP->blue = b;
	outP->alpha = RoundShortFpLong(a);

	return err;
}
//*************************************************************************************
static PF_Err
RGBShift32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat* inP,
	PF_PixelFloat* outP)
{
	PF_Err			err = PF_Err_NONE;
	RGBShiftInfo* infoP = reinterpret_cast<RGBShiftInfo*>(refcon);

	PF_PixelFloat rp = { 0,0,0,0 };
	PF_PixelFloat gp = { 0,0,0,0 };
	PF_PixelFloat bp = { 0,0,0,0 };

	PF_FpShort r = 0;
	PF_FpShort g = 0;
	PF_FpShort b = 0;
	PF_FpLong a = 0;
	rp = infoP->nfworld->GetPix32(xL - infoP->RShift, yL);
	r = rp.red;
	a += rp.alpha / 3;
	gp = infoP->nfworld->GetPix32(xL - infoP->GShift, yL);
	g = gp.green;
	a += gp.alpha / 3;
	bp = infoP->nfworld->GetPix32(xL - infoP->BShift, yL);
	b = bp.blue;
	a += bp.alpha / 3;

	outP->red = r;
	outP->green = g;
	outP->blue = b;
	outP->alpha = RoundFpShortDouble(a);

	return err;
}
//*************************************************************************************

PF_Err NoiseHorizon::RGBShiftExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	RGBShiftInfo rgbsInfo;
	rgbsInfo.seed = infoP->rgbs.seed;

	rgbsInfo.RShift = F_RANDR(-infoP->rgbs.RShift, infoP->rgbs.RShift);
	rgbsInfo.GShift = F_RANDR(-infoP->rgbs.GShift, infoP->rgbs.GShift);
	rgbsInfo.BShift = F_RANDR(-infoP->rgbs.BShift, infoP->rgbs.BShift);

	if ((rgbsInfo.RShift == 0) && (rgbsInfo.GShift == 0) && (rgbsInfo.BShift == 0)) return err;
	rgbsInfo.nfworld = infoP->rgbs.nfworld;
	if (infoP->moving == TRUE)
	{
		F_SRAND(infoP->rgbs.seed + infoP->frame);
	}
	else {
		F_SRAND(infoP->rgbs.seed);
	}
	switch (pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		iterate32(src->world, (void*)&rgbsInfo, RGBShift32, dst->world);
		break;
	case PF_PixelFormat_ARGB64:
		iterate16(src->world, (void*)&rgbsInfo, RGBShift16, dst->world);
		break;
	case PF_PixelFormat_ARGB32:
		iterate8(src->world, (void*)&rgbsInfo, RGBShift8, dst->world);
		break;
	default:
		break;
	}
	return err;
}