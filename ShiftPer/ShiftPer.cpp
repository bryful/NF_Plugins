#include "ShiftPer.h"
// **********************************************************

static PF_Err
Shift8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	A_long w = infoP->inWld->width();
	A_long h = infoP->inWld->height();
	A_long nx = (xL - infoP->shiftXPx) % w;
	if (nx < 0) nx += w;
	A_long ny = (yL - infoP->shiftYPx) % h;
	if (ny < 0) ny += h;
	*outP = infoP->inWld->GetPix8(nx, ny);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
Shift16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	A_long w = infoP->inWld->width();
	A_long h = infoP->inWld->height();
	A_long nx = (xL - infoP->shiftXPx) % w;
	if (nx < 0) nx += w;
	A_long ny = (yL - infoP->shiftYPx) % h;
	if (ny < 0) ny += h;
	*outP = infoP->inWld->GetPix16(nx, ny);

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
Shift32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat* inP,
	PF_PixelFloat* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	A_long w = infoP->inWld->width();
	A_long h = infoP->inWld->height();
	A_long nx = (xL - infoP->shiftXPx) % w;
	if (nx < 0) nx += w;
	A_long ny = (yL - infoP->shiftYPx) % h;
	if (ny < 0) ny += h;
	*outP = infoP->inWld->GetPix32(nx, ny);
	return err;
}
// **********************************************************
PF_Err ShiftPer::ParamsSetup(
	PF_InData* in_dataP,
	PF_OutData* out_dataP,
	PF_ParamDef* paramsP[],
	PF_LayerDef* outputP)
{
	PF_Err err = PF_Err_NONE;
	Init();
	m_cmd = PF_Cmd_PARAMS_SETUP;
	in_data = in_dataP;
	out_data = out_dataP;
	PF_ParamDef		def;
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"shiftX(%)",				//Name
		-30000,					//VALID_MIN
		30000,					//VALID_MAX
		-200,						//SLIDER_MIN
		200,				//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_SHIFTX
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"shiftY(%)",				//Name
		-30000,					//VALID_MIN
		30000,					//VALID_MAX
		-200,						//SLIDER_MIN
		200,				//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_SHIFTY
	);
//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err ShiftPer::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	ERR(GetFLOAT(ID_SHIFTX, &infoP->shiftX));
	if (!err){infoP->shiftX /= 100;}
	ERR(GetFLOAT(ID_SHIFTY, &infoP->shiftY));
	if (!err) { infoP->shiftY /= 100; }
	return err;
};
// **********************************************************
PF_Err ShiftPer::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	NFWorld* src = new NFWorld(input, in_data, pixelFormat());
	NFWorld* dst = new NFWorld(output, in_data, pixelFormat());
	dst->Copy(src);
	if ((infoP->shiftX != 0)|| (infoP->shiftY != 0)) {
		//init_xorShift(frame()); // —”‚Ì‰Šú‰»
		//ŽÀÛ‚É“®‚©‚·ƒsƒNƒZƒ‹”‚ðŒvŽZ
		infoP->shiftXPx = (A_long)(infoP->shiftX * (PF_FpLong)(src->width()) + 0.5);
		infoP->shiftYPx = (A_long)(infoP->shiftY * (PF_FpLong)(src->height()) + 0.5);
		//‰æ‘œ‚ð“o˜^
		infoP->inWld = src;
		infoP->outWld = dst;

		switch (pixelFormat())
		{
		case PF_PixelFormat_ARGB128:
			iterate32(src->world, (void*)infoP, Shift32, dst->world);
			break;
		case PF_PixelFormat_ARGB64:
			iterate16(src->world, (void*)infoP, Shift16, dst->world);
			break;
		case PF_PixelFormat_ARGB32:
			iterate8(src->world, (void*)infoP, Shift8, dst->world);
			break;
		default:
			break;
		}
	}


	delete src;
	delete dst;

	return err;

};
// **********************************************************
