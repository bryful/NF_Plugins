#include "TargetWork.h"
PF_Err ColorThSub8(TargetWorkInfo* infoP, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	if (infoP->colTblCount <= 0) return err;

	for (A_long y = 0; y < dst->height(); y++)
	{
		for (A_long x = 0; x < dst->width(); x++)
		{
			PF_Pixel c = dst->GetPix8(x, y);

			PF_Boolean MC = FALSE;
			for (A_long i = 0; i < infoP->colTblCount; i++)
			{
				if (compPix8(c, infoP->colTbl[i]) == TRUE)
				{
					MC = TRUE;
					break;
				}
			}
			if (MC == TRUE)
			{
				c = { PF_MAX_CHAN8,PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8 };
			}
			else {
				c = { 0,0, 0, 0 };
			}
			dst->SetPix8(x, y, c);
		}
	}
	return err;
}


PF_Err ColorTh(TargetWorkInfo* infoP, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	switch (dst->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		//ERR(AlphaThSub32(infoP, dst));
		break;
	case PF_PixelFormat_ARGB64:
		//ERR(AlphaThSub16(infoP, dst));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ColorThSub8(infoP, dst));
		break;
	default:
		break;
	}
	return err;

}