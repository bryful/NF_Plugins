#include "AEInfoSpat.h"


PF_Err AEInfoSpat::GetSpatFromLayer(A_long layerIndex, A_long txsize,SpatInfo* infoP)
{
	PF_Err err = PF_Err_NONE;

	//NFWorld nf;
	PF_ParamDef			pdef;
	infoP->count = 0;

	A_long cnt = frameCount();
	if (cnt > SPAT_BUF_MAX) cnt = SPAT_BUF_MAX;

	int errCount = 0;
	int idx = 0;
	A_long bx = (A_long)((double)txsize * ParamDownScaleX());
	if (bx < 4) bx = 4;
	for (int i = 0; i < cnt; i++)
	{
		ERR(PF_CHECKOUT_PARAM(in_data,
			layerIndex,
			(in_data->time_step * i),
			(in_data->time_step),
			in_data->time_scale,
			&pdef)
		);
		if (pdef.u.ld.data) {
			A_long w = pdef.u.ld.width;
			A_long h = pdef.u.ld.height;
			A_long sz = w;
			if (w > h) sz = h;
			PF_Rect srcR;
			srcR.left = (w - sz) / 2;
			srcR.top = (h - sz) / 2;
			srcR.right = srcR.left + sz;
			srcR.bottom = srcR.top + sz;
			PF_Rect dstR;
			dstR.left = dstR.top = 0;
			dstR.right = dstR.left + bx;
			dstR.bottom = dstR.top + bx;
			infoP->worlds[idx] = NewEffectWorld(bx, bx, pixelFormat());
			if (!ErrResult())
			{
				ERR(PF_COPY(&pdef.u.ld,
					&infoP->worlds[idx],
					&srcR,
					&dstR));
				idx++;
			}
		}
		else {
			errCount++;
			if (errCount > 2) break;
		}
		ERR(PF_CHECKIN_PARAM(in_data, &pdef));		// ALWAYS check in,
		infoP->count = idx;
		infoP->width = bx;
		infoP->height = bx;
	}
	if (infoP->count <= 0)
	{
		infoP->width = SPT_WIDTH;
		infoP->height = SPT_WIDTH;

		NFWorld nf;
		for (int i = 0; i < SPT_COUNT; i++)
		{
			infoP->worlds[i] = NewEffectWorld(SPT_WIDTH, SPT_WIDTH, pixelFormat());
			PF_Pixel8 p8 = { PF_MAX_CHAN8,PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8, };

			nf.Setup(&infoP->worlds[i], in_data, pixelFormat());
			nf.SetChar(SPT[i], SPT_WIDTH, SPT_WIDTH, p8);
		}
		infoP->count = SPT_COUNT;
	}
	return err;
}
PF_Err AEInfoSpat::DeleteSpat(SpatInfo* infoP)
{
	PF_Err err = PF_Err_NONE;

	if (infoP->count <= 0) return err;

	for (int i = infoP->count - 1; i >= 0; i--)
	{
		DisposeEffectWorldP(&infoP->worlds[i]);
	}
	infoP->count = 0;
	infoP->height = 0;
	infoP->width = 0;

	return err;
}

