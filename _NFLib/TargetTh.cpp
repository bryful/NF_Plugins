#include "TargetWork.h"


PF_Err AlphaThSub8(TargetWorkInfo* infoP, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;

	for (A_long y = 0; y < dst->height(); y++)
	{
		for (A_long x = 0; x < dst->width(); x++)
		{
			PF_Pixel col = { PF_MAX_CHAN8,PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8 };
			if (dst->GetPix8(x, y).alpha < PF_HALF_CHAN8)
			{
				col.red = 0;
				col.green = 0;
				col.blue = 0;
				col.alpha = 0;
			}
			dst->SetPix8(x, y, col);
		}
	}
	return err;
}
PF_Err AlphaThSub16(TargetWorkInfo* infoP, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;

	for (A_long y = 0; y < dst->height(); y++)
	{
		for (A_long x = 0; x < dst->width(); x++)
		{
			PF_Pixel16 col = { PF_MAX_CHAN16,PF_MAX_CHAN16, PF_MAX_CHAN16, PF_MAX_CHAN16 };
			if (dst->GetPix16(x, y).alpha < PF_HALF_CHAN16)
			{
				col.red = 0;
				col.green = 0;
				col.blue = 0;
				col.alpha = 0;
			}
			dst->SetPix16(x, y, col);
		}
	}
	return err;
}
PF_Err AlphaThSub32(TargetWorkInfo* infoP, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;

	for (A_long y = 0; y < dst->height(); y++)
	{
		for (A_long x = 0; x < dst->width(); x++)
		{
			PF_Pixel32 col = { 1,1, 1, 1 };
			if (dst->GetPix32(x, y).alpha < 0.5)
			{
				col.red = 0;
				col.green = 0;
				col.blue = 0;
				col.alpha = 0;
			}
			dst->SetPix32(x, y, col);
		}
	}
	return err;
}
PF_Err AlphaTh(TargetWorkInfo* infoP, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	switch (dst->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(AlphaThSub32(infoP, dst));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(AlphaThSub16(infoP, dst));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(AlphaThSub8(infoP, dst));
		break;
	default:
		break;
	}
	return err;

}