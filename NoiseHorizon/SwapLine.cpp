#include "NoiseHorizon.h"

//*****************************************************************************************
PF_Err SwapLine8(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;

	int cnt = infoP->swapValue;

	PF_Pixel* data = infoP->nfworld->data8();
	int w = infoP->nfworld->width();
	int h = infoP->nfworld->height();
	int he = F_RANDR(1, infoP->swapHeight);
	for (int i = 0; i < cnt; i++)
	{

		A_long y0 = (h - he) * F_RAND() / F_RAND_MAX;
		A_long y1 = (h - he) * F_RAND() / F_RAND_MAX;
		if (y0 == y1) continue;
		for (int y = 0; y < he; y++)
		{
			PF_Pixel* adrY0 = data + infoP->nfworld->yAdr(y + y0);
			PF_Pixel* adrY1 = data + infoP->nfworld->yAdr(y + y1);
			for (int x = 0; x < w; x++)
			{
				*adrY0 = *adrY1;
				adrY0++;
				adrY1++;
			}

		}

	}
	return err;
}
//*****************************************************************************************
PF_Err SwapLine16(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;

	int cnt = infoP->swapValue;

	PF_Pixel16* data = infoP->nfworld->data16();
	int w = infoP->nfworld->width();
	int h = infoP->nfworld->height();
	int he = F_RANDR(1, infoP->swapHeight);
	for (int i = 0; i < cnt; i++)
	{

		A_long y0 = (h - he) * F_RAND() / F_RAND_MAX;
		A_long y1 = (h - he) * F_RAND() / F_RAND_MAX;
		if (y0 == y1) continue;
		for (int y = 0; y < he; y++)
		{
			PF_Pixel16* adrY0 = data + infoP->nfworld->yAdr(y + y0);
			PF_Pixel16* adrY1 = data + infoP->nfworld->yAdr(y + y1);
			for (int x = 0; x < w; x++)
			{
				*adrY0 = *adrY1;
				adrY0++;
				adrY1++;
			}

		}

	}
	return err;
}
//*****************************************************************************************
PF_Err SwapLine32(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;

	int cnt = infoP->swapValue;

	PF_Pixel32* data = infoP->nfworld->data32();
	int w = infoP->nfworld->width();
	int h = infoP->nfworld->height();
	int he = F_RANDR(1, infoP->swapHeight);
	for (int i = 0; i < cnt; i++)
	{

		A_long y0 = (h - he) * F_RAND() / F_RAND_MAX;
		A_long y1 = (h - he) * F_RAND() / F_RAND_MAX;
		if (y0 == y1) continue;
		for (int y = 0; y < he; y++)
		{
			PF_Pixel32* adrY0 = data + infoP->nfworld->yAdr(y + y0);
			PF_Pixel32* adrY1 = data + infoP->nfworld->yAdr(y + y1);
			for (int x = 0; x < w; x++)
			{
				*adrY0 = *adrY1;
				adrY0++;
				adrY1++;
			}

		}

	}
	return err;
}
//*****************************************************************************************
PF_Err NoiseHorizon::SwapLineExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;

	if (infoP->swapValue <= 0) return err;

	if (infoP->moving == TRUE)
	{
		F_SRAND(infoP->swapline_seed + infoP->frame);
	}
	else {
		F_SRAND(infoP->swapline_seed);
	}

	infoP->nfworld = dst;

	switch (pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		SwapLine32(infoP);
		break;
	case PF_PixelFormat_ARGB64:
		SwapLine16(infoP);
		break;
	case PF_PixelFormat_ARGB32:
		SwapLine8(infoP);
		break;
	default:
		break;
	}
	return err;
}