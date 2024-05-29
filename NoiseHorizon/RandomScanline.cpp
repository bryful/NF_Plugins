#include "NoiseHorizon.h"

PF_Err NoiseHorizon::RandomScanlineExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;

	int xs = infoP->sl.XShift;
	if ((infoP->sl.value <= 0) || (xs <= 0)) return err;

	if (infoP->moving == TRUE)
	{
		F_SRAND(infoP->rs.seed + infoP->frame);
	}
	else {
		F_SRAND(infoP->rs.seed);
	}
	double ds = ParamDownScaleX();
	int w = dst->width();
	int h = dst->height();

	int h2 = (int)(4 * ds + 0.5);
	if (h2 <= 0) h2 = 1;
	//PF_COPY
	int h3 = h2;
	if (h3 < 8) h3 = 8;
	PF_EffectWorld bufW = NewEffectWorld(w,h3, pixelFormat());
	if (ErrResult() != PF_Err_NONE) return err;
	NFWorld* buf = new NFWorld(&bufW, in_data, pixelFormat());


	int yCnt = h / h2;
	int border = (int)(F_RAND_MAX * infoP->sl.value);
	for (int i = 0; i < yCnt; i++)
	{
		A_long xxs = F_RANDR(-xs, xs);
		if (xxs == 0) continue;
		if (border < F_RAND()) continue;

		PF_Rect bufR;
		bufR.left = 0;
		bufR.top = 0;
		bufR.right = bufR.left + w;
		bufR.bottom = bufR.top + h2;

		PF_Rect srcR;
		srcR.left = 0;
		srcR.top = i * h2;
		srcR.right = srcR.left + w;
		srcR.bottom = srcR.top + h2;

		PF_Rect dstR;
		dstR.left = srcR.left + xxs;
		dstR.top = srcR.top;
		dstR.right = dstR.left + w;
		dstR.bottom = dstR.top + h2;

		PF_COPY(dst->world, &bufW, &srcR, &bufR);
		PF_COPY(&bufW, dst->world, &bufR, &dstR);

	}

	delete buf;
	DisposeEffectWorld(bufW);

	return err;
}