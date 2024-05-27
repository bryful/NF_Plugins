#include "NoiseHorizon.h"

PF_Err DrawLine8(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	if ((infoP->rl.value <= 0)||(infoP->rl.Opacity<=0)) return err;
	A_long cnt =  F_RANDR(1, infoP->rl.value);
	A_long cnt2 = F_RANDR(1, infoP->rl.value2);

	int w = infoP->nfworld->width();
	int h = infoP->nfworld->height();
	PF_FpLong opa = infoP->rl.Opacity;
	PF_Pixel* data = infoP->nfworld->data8();
	for (int i = 0; i < cnt; i++)
	{
		A_long y = F_RANDR(0, h - 1);
		for (int j = 0; j < cnt2; j++)
		{
			A_long y2 = y +F_RANDR(-h/10,h/10);
			if (y2 < 0)y2 = 0;
			else if(y2 >= h) y2 = h - 1;


			PF_Pixel col = { 0,0,0,0 };

			A_long v = PF_MAX_CHAN8 * F_RAND() / F_RAND_MAX;
			if (v >= PF_HALF_CHAN8)
			{
				v = v*3/2;
				if (v > PF_MAX_CHAN8)v = PF_MAX_CHAN8;
			}
			else {
				v /= 2;
			}
			col.red = (A_u_char)v;
			col.green = (A_u_char)v;
			col.blue = (A_u_char)v;
			col.alpha  =  RoundByteFpLong (PF_MAX_CHAN8 * F_RAND_D() * opa);

			PF_Pixel* adr = data + infoP->nfworld->yAdr(y2);
			for (int x = 0; x < w; x++)
			{
				*adr = PixelBlend8(*adr, col);
				adr++;
			}
		}
	}


	return err;
}
PF_Err NoiseHorizon::RandomLineExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	if (infoP->rl.value <= 0) return err;

	if (infoP->moving == TRUE)
	{
		F_SRAND(infoP->rl.seed + infoP->frame);
	}
	else {
		F_SRAND(infoP->rl.seed);
	}
	infoP->nfworld = dst;
	switch (pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		//iterate32(src->world, (void*)infoP, Blend32, dst->world);
		break;
	case PF_PixelFormat_ARGB64:
		//iterate16(src->world, (void*)infoP, Blend16, dst->world);
		break;
	case PF_PixelFormat_ARGB32:
		//iterate8(src->world, (void*)infoP, NoiseHorizon8, dst->world);
		DrawLine8(infoP);
		break;
	default:
		break;
	}
	return err;
}