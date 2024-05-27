#include "NoiseHorizon.h"

PF_Err NoiseHor8(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	if ((infoP->noisevalue <= 0) || (infoP->noiseStrong <= 0) || (infoP->noiseLength <= 0)) return err;

	int w = infoP->nfworld->width();
	int h = infoP->nfworld->height();
	if (infoP->moving == TRUE)
	{
		F_SRAND(infoP->swapline_seed + infoP->frame);
	}
	else {
		F_SRAND(infoP->swapline_seed);
	}

	int cnt = w * h;
	cnt = (int)((double)cnt * infoP->noisevalue + 0.5);
	cnt = (int)( cnt * F_RAND_D() + 0.5);
	if (cnt <= 0) return err;


	PF_Pixel* data = infoP->nfworld->data8();
	for (int i = 0; i < cnt; i++)
	{
		int nl = F_RANDR(1, infoP->noiseLength);

		int x = (w - nl) * F_RAND() / F_RAND_MAX;
		int y = (h - 1) * F_RAND() / F_RAND_MAX;

		double ad = PF_MAX_CHAN8 * infoP->noiseStrong;
		ad = F_RAND_D2(-ad, ad);
		if (ad != 0) {
			PF_Pixel* adr = data + infoP->nfworld->yAdr(y) + x;
			for (int x = 0; x < nl; x++)
			{
				PF_Pixel p = *adr;
				if (p.alpha != PF_MAX_CHAN8)
				{
					p.alpha = RoundByteDouble(ABS((double)p.alpha + ad));
				}
				PF_FpLong a = 1;
				if (p.alpha != 0)
				{
					a = PF_MAX_CHAN8 / (double)p.alpha;
				}
				PF_FpLong r = ABS((double)p.red + ad) * a;
				PF_FpLong g = ABS((double)p.green + ad) * a;
				PF_FpLong b = ABS((double)p.blue + ad) * a;


				p.red = RoundByteDouble(r);
				p.green = RoundByteDouble(g);
				p.blue = RoundByteDouble(b);
				*adr = p;
				adr++;
			}
		}
	}
	return err;
}
