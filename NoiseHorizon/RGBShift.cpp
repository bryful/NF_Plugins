#include "NoiseHorizon.h"

//*************************************************************************************
PF_Err  NoiseHorizon::RGBSHiftSub8(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	int w = dst->width();
	int h = dst->height();
	A_long xr = F_RANDR(-infoP->rgbs.RShift, infoP->rgbs.RShift);
	A_long xg = F_RANDR(-infoP->rgbs.GShift, infoP->rgbs.GShift);
	A_long xb = F_RANDR(-infoP->rgbs.BShift, infoP->rgbs.BShift);
	if ((xr == 0) && (xg == 0) && (xb == 0)) return err;


	PF_EffectWorld bufW = NewEffectWorld(w, 8, pixelFormat());
	if (ErrResult() != PF_Err_NONE) return err;
	NFWorld* buf = new NFWorld(&bufW, in_data, pixelFormat());



	PF_Rect rct;
	rct.left = 0;
	rct.right = w;
	PF_Rect rctB;
	rctB.left = 0;
	rctB.right = w;
	rctB.top = 0;
	rctB.bottom = 2;

	PF_Pixel8* data = dst->data8();

	for (int y = 0; y < h; y++)
	{
		rct.top = y;
		rct.bottom = y+2;
		PF_COPY(dst->world, &bufW, &rct, &rctB);
		PF_Pixel8* adr = data + dst->yAdr(y);
		for (int x = 0; x < w; x++)
		{
			PF_Pixel tp = { 0,0,0,0 };
			PF_Pixel pp = { 0,0,0,0 };

			PF_FpLong a = 0;
			tp = buf->GetPix8(x - xr, 0);
			pp.red = tp.red;
			a += (PF_FpLong)tp.alpha / 3;
			tp = buf->GetPix8(x - xg, 0);
			pp.green = tp.green;
			a += (PF_FpLong)tp.alpha / 3;
			tp = buf->GetPix8(x - xb, 0);
			pp.blue = tp.blue;
			a += (PF_FpLong)tp.alpha / 3;
			pp.alpha = RoundByteDouble(a);
			*adr = pp;
			adr++;
		}
	}
	delete buf;
	DisposeEffectWorld(bufW);
	return err;
}
PF_Err  NoiseHorizon::RGBSHiftSub16(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	double ds = ParamDownScaleX();
	int w = dst->width();
	int h = dst->height();
	A_long xr = F_RANDR(-infoP->rgbs.RShift, infoP->rgbs.RShift);
	A_long xg = F_RANDR(-infoP->rgbs.GShift, infoP->rgbs.GShift);
	A_long xb = F_RANDR(-infoP->rgbs.BShift, infoP->rgbs.BShift);
	if ((xr == 0) && (xg == 0) && (xb == 0)) return err;

	
	PF_EffectWorld bufW = NewEffectWorld(w, 8, pixelFormat());
	if (ErrResult() != PF_Err_NONE) return err;
	NFWorld* buf = new NFWorld(&bufW, in_data, pixelFormat());


	PF_Rect rct;
	rct.left = 0;
	rct.right = w;
	PF_Rect rctB;
	rctB.left = 0;
	rctB.right = w;
	rctB.top = 0;
	rctB.bottom = 1;

	PF_Pixel16* data = dst->data16();

	for (int y = 0; y < h; y++)
	{
		rct.top = y;
		rct.bottom = y + 1;
		PF_COPY(dst->world, &bufW, &rct, &rctB);
		PF_Pixel16* adr = data + dst->yAdr(y);
		for (int x = 0; x < w; x++)
		{
			PF_Pixel16 tp = { 0,0,0,0 };
			PF_Pixel16 pp = { 0,0,0,0 };

			PF_FpLong a = 0;
			tp = buf->GetPix16(x - xr, 0);
			pp.red = tp.red;
			a += (PF_FpLong)tp.alpha / 3;
			tp = buf->GetPix16(x - xg, 0);
			pp.green = tp.green;
			a += (PF_FpLong)tp.alpha / 3;
			tp = buf->GetPix16(x - xb, 0);
			pp.blue = tp.blue;
			a += (PF_FpLong)tp.alpha / 3;
			pp.alpha = RoundShortFpLong(a);
			*adr = pp;
			adr++;
		}
	}
	delete buf;
	DisposeEffectWorld(bufW);
	return err;
}

PF_Err  NoiseHorizon::RGBSHiftSub32(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	double ds = ParamDownScaleX();
	int w = dst->width();
	int h = dst->height();

	A_long xr = F_RANDR(-infoP->rgbs.RShift, infoP->rgbs.RShift);
	A_long xg = F_RANDR(-infoP->rgbs.GShift, infoP->rgbs.GShift);
	A_long xb = F_RANDR(-infoP->rgbs.BShift, infoP->rgbs.BShift);
	if ((xr == 0) && (xg == 0) && (xb == 0)) return err;

	PF_EffectWorld bufW = NewEffectWorld(w, 8, pixelFormat());
	if (ErrResult() != PF_Err_NONE) return err;
	NFWorld* buf = new NFWorld(&bufW, in_data, pixelFormat());

	PF_Rect rct;
	rct.left = 0;
	rct.right = w;
	PF_Rect rctB;
	rctB.left = 0;
	rctB.right = w;
	rctB.top = 0;
	rctB.bottom = 1;

	PF_PixelFloat* data = dst->data32();

	for (int y = 0; y < h; y++)
	{
		rct.top = y;
		rct.bottom = y + 1;
		PF_COPY(dst->world, &bufW, &rct, &rctB);
		PF_PixelFloat* adr = data + dst->yAdr(y);
		for (int x = 0; x < w; x++)
		{
			PF_PixelFloat tp = { 0,0,0,0 };
			PF_PixelFloat pp = { 0,0,0,0 };

			PF_FpLong a = 0;
			tp = buf->GetPix32(x - xr, 0);
			pp.red = tp.red;
			a += (PF_FpLong)tp.alpha / 3;
			tp = buf->GetPix32(x - xg, 0);
			pp.green = tp.green;
			a += (PF_FpLong)tp.alpha / 3;
			tp = buf->GetPix32(x - xb, 0);
			pp.blue = tp.blue;
			a += (PF_FpLong)tp.alpha / 3;
			pp.alpha = RoundFpShortDouble(a);
			*adr = pp;
			adr++;
		}
	}
	delete buf;
	DisposeEffectWorld(bufW);
	return err;
}
//*************************************************************************************
PF_Err NoiseHorizon::RGBShiftExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	RGBShiftInfo rgbsInfo;
	rgbsInfo.seed = infoP->rgbs.seed;
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
		RGBSHiftSub32(infoP, src, dst);
		break;
	case PF_PixelFormat_ARGB64:
		RGBSHiftSub16(infoP, src, dst);
		break;
	case PF_PixelFormat_ARGB32:
		RGBSHiftSub8(infoP, src, dst);
		break;
	default:
		break;
	}
	return err;
}