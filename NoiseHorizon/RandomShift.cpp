#include "NoiseHorizon.h"

PF_Err NoiseHorizon::RandomShiftExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	int cnt = infoP->rs.value;
	int xs = infoP->rs.XShift;
	if ((cnt <= 0)||(xs<=0)) return err;

	if (infoP->moving == TRUE)
	{
		F_SRAND(infoP->rs.seed + infoP->frame);
	}
	else {
		F_SRAND(infoP->rs.seed);
	}
	double ds = ParamDownScaleX();
	//PF_COPY
	PF_EffectWorld bufW = NewEffectWorld((int)(128*ds), (int)(16*ds), pixelFormat());
	if (ErrResult() != PF_Err_NONE) return ErrResult();
	NFWorld* buf = new NFWorld(&bufW, in_data, pixelFormat());

	int w = dst->width();
	int h= dst->height();

	int minV = (int)(8 * ds);
	for (int i = 0; i < cnt; i++)
	{
		A_long x = F_RANDR(-minV, w - minV);
		A_long y = F_RANDR(-minV, h - minV);

		A_long ww = F_RANDR(buf->width()/8, buf->width());
		A_long hh = F_RANDR(buf->height()/4, buf->height());

		A_long xxs = F_RANDR(-xs, xs);
		if (xxs ==0 )xxs = F_RANDR(-xs, xs);
		A_long x2 = x + xxs;
		PF_Rect bufR;
		bufR.left = 0;
		bufR.top = 0;
		bufR.right = bufR.left + ww;
		bufR.bottom = bufR.top + hh;
		PF_Rect srcR;
		srcR.left = x;
		srcR.top = y;
		srcR.right = srcR.left + ww;
		srcR.bottom = srcR.top + hh;

		PF_Rect dstR;
		dstR.left = srcR.left + xxs;
		dstR.top = srcR.top;
		dstR.right = dstR.left + ww;
		dstR.bottom = dstR.top + hh;

		PF_COPY(dst->world, &bufW, &srcR, &bufR);
		PF_COPY(&bufW,dst->world, &bufR,&dstR);

	}

	delete buf;
	DisposeEffectWorld(bufW);

	return err;
}