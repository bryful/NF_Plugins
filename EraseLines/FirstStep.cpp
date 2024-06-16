#include "EraseLines.h"


PF_Err FirstStep8(ParamInfo *infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;

	PF_Pixel c = { 0,0,0,0 };
	PF_Pixel* srdD = src->data8();
	PF_Pixel* dstD = src->data8();
	for (int y = 0; y < src->height(); y++)
	{
		PF_Pixel* srcD = src->data8() + src->yAdr(y);
		PF_Pixel* dstD = dst->data8() + dst->yAdr(y);
		for (int x = 0; x < src->width(); x++)
		{
			PF_Boolean mc = FALSE;
			for (int i = 0; i < infoP->lineColorsCount; i++)
			{
				if (compPix8(*srcD, infoP->lineColors[i]))
				{
					mc = TRUE;
					break;
				}

			}
			if (mc == TRUE)
			{

				*dstD = { 0,0,0,0 };
			}
			else {
				*dstD = *srcD;
				dstD->alpha = PF_MAX_CHAN8;
			}
			srcD++;
			dstD++;
		}
	}

	return err;
}
PF_Err FirstStep(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	switch (src->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		//iterate32(src->world, (void*)infoP, ColChange32, dst->world);
		break;
	case PF_PixelFormat_ARGB64:
		//iterate16(src->world, (void*)infoP, ColChange16, dst->world);
		break;
	case PF_PixelFormat_ARGB32:
		ERR(FirstStep8(infoP, src, dst));
		break;
	default:
		break;
	}
	return err;
}
