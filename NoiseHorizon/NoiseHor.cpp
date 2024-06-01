#include "NoiseHorizon.h"

//********************************************************************************
PF_Err NoiseHor8(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	int w = infoP->nfworld->width();
	int h = infoP->nfworld->height();
	
	int cnt = w * h / 10240;
	cnt = (int)((double)cnt * infoP->noisevalue + 0.5);
	if (cnt > 0) {

		for (int i = 0; i < cnt; i++)
		{

			int x = F_RANDR(0, w - 1);
			int y = F_RANDR(0, h - 1);

			PF_Pixel p = infoP->nfworld->GetPix8(x, y);

			double ad = PF_MAX_CHAN8 * infoP->noiseStrong;
			ad = F_RAND_D2(-ad, ad);
			if (ad == 0) F_RAND_D2(-ad, ad);
			int xx = F_RANDR(2, 2 + infoP->noiseLength);
			int yy = F_RANDR(2, 2 + infoP->noiseLength/2 );
			if (ad != 0)
			{
				PF_Rect rct;
				rct.left = x - xx / 2;
				rct.top = y - yy / 2;
				rct.right = rct.left + xx;
				rct.bottom = rct.top + yy;
				if (p.alpha == 0)
				{
					p.alpha = PF_HALF_CHAN8;
					p.red = PF_HALF_CHAN8;
					p.green = PF_HALF_CHAN8;
					p.blue = PF_HALF_CHAN8;
				}
				if (p.alpha != PF_MAX_CHAN8)
				{
					p.alpha = RoundByteFpLong((double)p.alpha + ad);
				}
				p.red = RoundByteFpLong((double)p.red + ad);
				p.green = RoundByteFpLong((double)p.green + ad);
				p.blue = RoundByteFpLong((double)p.blue + ad);

				infoP->nfworld->Fill8(rct, p);
			}
		}
	}


	cnt = w * h/8;
	cnt = (int)((double)cnt * infoP->noisevalue + 0.5);
	cnt = F_RANDR(cnt/2,cnt);
	if (cnt <= 0) return err;


	PF_Pixel* data = infoP->nfworld->data8();
	for (int i = 0; i < cnt; i++)
	{
		int nl = F_RANDR(1, infoP->noiseLength);

		int x = F_RANDR(0,w-1);
		int y = F_RANDR(0, h-1);

		if (x < 0)
		{
			nl += x;
			x = 0;
		}
		else if((x+nl)>=w){
			nl -=  (x-(w-1));
		}
		if (nl <= 0) continue;

		double ad = PF_MAX_CHAN8 * infoP->noiseStrong;
		ad = F_RAND_D2(-ad, ad);
		if (ad != 0) {
			PF_Pixel* adr = data + infoP->nfworld->yAdr(y) + x;
			for (int j = 0; j < nl; j++)
			{
				PF_Pixel p = *adr;
				if (p.alpha != PF_MAX_CHAN8)
				{
					p.alpha = RoundByteDouble(ABS((double)p.alpha + ad));
				}
				PF_FpLong a = 1;
				if ((p.alpha >0)&&(p.alpha<PF_MAX_CHAN8))
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
//********************************************************************************
PF_Err NoiseHor16(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;

	int w = infoP->nfworld->width();
	int h = infoP->nfworld->height();

	int cnt = w * h / 10240;
	cnt = (int)((double)cnt * infoP->noisevalue + 0.5);
	if (cnt > 0) {

		for (int i = 0; i < cnt; i++)
		{

			int x = F_RANDR(0, w - 1);
			int y = F_RANDR(0, h - 1);

			PF_Pixel16 p = infoP->nfworld->GetPix16(x, y);

			double ad = PF_MAX_CHAN16 * infoP->noiseStrong;
			ad = F_RAND_D2(-ad, ad);
			if (ad == 0) F_RAND_D2(-ad, ad);
			int xx = F_RANDR(2, 2 + infoP->noiseLength);
			int yy = F_RANDR(2, 2 + infoP->noiseLength);
			if (ad != 0)
			{
				PF_Rect rct;
				rct.left = x - xx / 2;
				rct.top = y - yy / 2;
				rct.right = rct.left + xx;
				rct.bottom = rct.top + yy;
				if (p.alpha == 0)
				{
					p.alpha = PF_HALF_CHAN16;
					p.red = PF_HALF_CHAN16;
					p.green = PF_HALF_CHAN16;
					p.blue = PF_HALF_CHAN16;
				}
				if (p.alpha != PF_MAX_CHAN16)
				{
					p.alpha = RoundShortFpLong((double)p.alpha + ad);
				}
				p.red = RoundShortFpLong((double)p.red + ad);
				p.green = RoundShortFpLong((double)p.green + ad);
				p.blue = RoundShortFpLong((double)p.blue + ad);

				infoP->nfworld->Fill16(rct, p);
			}
		}
	}



	cnt = w * h/4;
	cnt = (int)((double)cnt * infoP->noisevalue + 0.5);
	cnt = F_RANDR(cnt / 2, cnt);
	if (cnt <= 0) return err;


	PF_Pixel16* data = infoP->nfworld->data16();
	for (int i = 0; i < cnt; i++)
	{
		int nl = F_RANDR(1, infoP->noiseLength);

		int x = F_RANDR(0, w - 1);
		int y = F_RANDR(0, h - 1);

		if (x < 0)
		{
			nl += x;
			x = 0;
		}
		else if ((x + nl) >= w) {
			nl -= (x - (w - 1));
		}
		if (nl <= 0) continue;

		double ad = PF_MAX_CHAN16 * infoP->noiseStrong;
		ad = F_RAND_D2(-ad, ad);
		if (ad != 0) {
			PF_Pixel16* adr = data + infoP->nfworld->yAdr(y) + x;
			for (int j = 0; j < nl; j++)
			{
				PF_Pixel16 p = *adr;
				if (p.alpha != PF_MAX_CHAN16)
				{
					p.alpha = RoundShortFpLong(ABS((double)p.alpha + ad));
				}
				PF_FpLong a = 1;
				if ((p.alpha > 0) && (p.alpha < PF_MAX_CHAN16))
				{
					a = PF_MAX_CHAN16 / (double)p.alpha;
				}
				PF_FpLong r = ABS((double)p.red + ad) * a;
				PF_FpLong g = ABS((double)p.green + ad) * a;
				PF_FpLong b = ABS((double)p.blue + ad) * a;


				p.red = RoundShortFpLong(r);
				p.green = RoundShortFpLong(g);
				p.blue = RoundShortFpLong(b);
				*adr = p;
				adr++;
			}
		}
	}
	return err;
}
//********************************************************************************
PF_Err NoiseHor32(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;


	int w = infoP->nfworld->width();
	int h = infoP->nfworld->height();

	int cnt = w * h / 10240;
	cnt = (int)((double)cnt * infoP->noisevalue + 0.5);
	if (cnt > 0) {

		for (int i = 0; i < cnt; i++)
		{

			int x = F_RANDR(0, w - 1);
			int y = F_RANDR(0, h - 1);

			PF_PixelFloat p = infoP->nfworld->GetPix32(x, y);

			double ad = 1 * infoP->noiseStrong;
			ad = F_RAND_D2(-ad, ad);
			if (ad == 0) F_RAND_D2(-ad, ad);
			int xx = F_RANDR(2, 2 + infoP->noiseLength);
			int yy = F_RANDR(2, 2 + infoP->noiseLength);
			if (ad != 0)
			{
				PF_Rect rct;
				rct.left = x - xx / 2;
				rct.top = y - yy / 2;
				rct.right = rct.left + xx;
				rct.bottom = rct.top + yy;
				if (p.alpha == 0)
				{
					p.alpha = 0.5;
					p.red = 0.5;
					p.green = 0.5;
					p.blue = 0.5;
				}
				if (p.alpha != 1)
				{
					p.alpha = RoundFpShortDouble((double)p.alpha + ad);
				}
				p.red = RoundFpShortDouble((double)p.red + ad);
				p.green = RoundFpShortDouble((double)p.green + ad);
				p.blue = RoundFpShortDouble((double)p.blue + ad);

				infoP->nfworld->Fill32(rct, p);
			}
		}
	}




	cnt = w * h/4;
	cnt = (int)((double)cnt * infoP->noisevalue + 0.5);
	cnt = F_RANDR(cnt / 2, cnt);
	if (cnt <= 0) return err;


	PF_PixelFloat* data = infoP->nfworld->data32();
	for (int i = 0; i < cnt; i++)
	{
		int nl = F_RANDR(1, infoP->noiseLength);

		int x = F_RANDR(0, w - 1);
		int y = F_RANDR(0, h - 1);

		if (x < 0)
		{
			nl += x;
			x = 0;
		}
		else if ((x + nl) >= w) {
			nl -= (x - (w - 1));
		}
		if (nl <= 0) continue;

		double ad = 1 * infoP->noiseStrong;
		ad = F_RAND_D2(-ad, ad);
		if (ad != 0) {
			PF_PixelFloat* adr = data + infoP->nfworld->yAdr(y) + x;
			for (int j = 0; j < nl; j++)
			{
				PF_PixelFloat p = *adr;
				if (p.alpha != 1)
				{
					p.alpha = RoundFpShortDouble(ABS((double)p.alpha + ad));
				}
				PF_FpLong a = 1;
				if ((p.alpha > 0) && (p.alpha < 1))
				{
					a = 1 / (double)p.alpha;
				}
				PF_FpLong r = ABS((double)p.red + ad) * a;
				PF_FpLong g = ABS((double)p.green + ad) * a;
				PF_FpLong b = ABS((double)p.blue + ad) * a;


				p.red = RoundFpShortDouble(r);
				p.green = RoundFpShortDouble(g);
				p.blue = RoundFpShortDouble(b);
				*adr = p;
				adr++;
			}
		}
	}
	return err;
}
//********************************************************************************
PF_Err NoiseHorizon::NoiseHorExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	if ((infoP->noisevalue <= 0) || (infoP->noiseStrong <= 0) || (infoP->noiseLength <= 0)) return err;

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
		NoiseHor32(infoP);
		break;
	case PF_PixelFormat_ARGB64:
		NoiseHor16(infoP);
		break;
	case PF_PixelFormat_ARGB32:
		NoiseHor8(infoP);
		break;
	default:
		break;
	}
	return err;
}