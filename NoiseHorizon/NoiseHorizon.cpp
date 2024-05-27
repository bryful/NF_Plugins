#include "NoiseHorizon.h"



// **********************************************************
PF_Err NoiseHorizon::ParamsSetup(
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
	def.flags = PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(
		"moving",
		"on",
		TRUE,
		0,
		ID_MOVING
	);	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"globalValue",			//Name
		0,						//VALID_MIN
		1000,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		100,					//CURVE_TOLERANCE
		100,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_GLOBAL_VALUE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC("RGBShift", ID_TOPIC_RGBS);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"rgbs_seed",	//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-200,			//スライダーの最小値
		200,			//スライダーの最大値
		0,				//デフォルトの値
		ID_RGBS_SEED
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"shiftVaue",			//Name
		0,						//VALID_MIN
		1000,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		100,					//CURVE_TOLERANCE
		100,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_RGBS_VALUE
	);	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"RedShift",	//パラメータの名前
		0, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		0,			//スライダーの最小値
		200,			//スライダーの最大値
		0,				//デフォルトの値
		ID_RGBS_R
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"GreenShift",	//パラメータの名前
		0, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-200,			//スライダーの最小値
		200,			//スライダーの最大値
		0,				//デフォルトの値
		ID_RGBS_G
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"BlueShift",	//パラメータの名前
		-0, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-200,			//スライダーの最小値
		200,			//スライダーの最大値
		0,				//デフォルトの値
		ID_RGBS_B
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_RGBS_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC("RandomShift", ID_TOPIC_RS);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"rs_seed",	//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-200,			//スライダーの最小値
		200,			//スライダーの最大値
		0,				//デフォルトの値
		ID_RS_SEED
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"rsValue",	//パラメータの名前
		0, 			//数値入力する場合の最小値
		2000,		//数値入力する場合の最大値
		0,			//スライダーの最小値
		100,		//スライダーの最大値
		0,			//デフォルトの値
		ID_RS_VALUE
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"xShift",	//パラメータの名前
		1, 			//数値入力する場合の最小値
		1000,		//数値入力する場合の最大値
		1,			//スライダーの最小値
		200,		//スライダーの最大値
		20,			//デフォルトの値
		ID_RS_SHIFT
	);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_RS_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC("swapLine", ID_TOPIC_SWAPLINE);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"sw_seed",	//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-200,			//スライダーの最小値
		200,			//スライダーの最大値
		0,				//デフォルトの値
		ID_SWAPLINE_SEED
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"swapValue",	//パラメータの名前
		0, 		//数値入力する場合の最小値
		1000,			//数値入力する場合の最大値
		0,			//スライダーの最小値
		100,			//スライダーの最大値
		3,				//デフォルトの値
		ID_SWAPVALUE
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"swapHeight",	//パラメータの名前
		1, 				//数値入力する場合の最小値
		200,			//数値入力する場合の最大値
		1,				//スライダーの最小値
		100,			//スライダーの最大値
		30,				//デフォルトの値
		ID_SWAPVALUE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_SWAPLINE_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC("noise", ID_TOPIC_NOISE);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"n_seed",	//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-200,			//スライダーの最小値
		200,			//スライダーの最大値
		0,				//デフォルトの値
		ID_NOISE_SEED
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"noiseValue",			//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		50,					//SLIDER_MAX
		100,					//CURVE_TOLERANCE
		10,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_NOISEVALUE
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"noiseLength",	//パラメータの名前
		1, 				//数値入力する場合の最小値
		300,			//数値入力する場合の最大値
		1,				//スライダーの最小値
		100,			//スライダーの最大値
		30,				//デフォルトの値
		ID_NOISELENGTH
	);	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"noiseStrong",			//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		100,					//CURVE_TOLERANCE
		10,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_NOISESTRONG
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_NOISE_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC("drawLine", ID_TOPIC_RL);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"rl_seed",	//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-200,			//スライダーの最小値
		200,			//スライダーの最大値
		0,				//デフォルトの値
		ID_RL_SEED
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"rlValue",	//パラメータの名前
		0, 				//数値入力する場合の最小値
		300,			//数値入力する場合の最大値
		0,				//スライダーの最小値
		100,			//スライダーの最大値
		3,				//デフォルトの値
		ID_RL_VALUE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"rlValue2",	//パラメータの名前
		1, 				//数値入力する場合の最小値
		300,			//数値入力する場合の最大値
		1,				//スライダーの最小値
		50,			//スライダーの最大値
		3,				//デフォルトの値
		ID_RL_VALUE2
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"rlOpacity",			//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		100,					//CURVE_TOLERANCE
		10,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_RL_OPACITY
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_RL_END);

//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err	 NoiseHorizon::UserChangedParam(
	PF_InData* in_dataP,
	PF_OutData* out_dataP,
	PF_ParamDef* paramsP[],
	PF_LayerDef* outputP,
	PF_UserChangedParamExtra* extraP,
	A_long pc)
{
	PF_Err err = PF_Err_NONE;
	try {
		Init(in_dataP, out_dataP, paramsP, outputP,ID_NUM_PARAMS);
		m_cmd = PF_Cmd_USER_CHANGED_PARAM;
		if (!err) {
			
		}
	}
	catch (PF_Err& errP) {
		err = errP;
	}
	return err;
}
PF_Err NoiseHorizon::QueryDynamicFlags(
	PF_InData* in_dataP,
	PF_OutData* out_dataP,
	PF_ParamDef* paramsP[],
	PF_LayerDef* outputP,
	PF_UserChangedParamExtra* extraP,
	A_long pc
)
{
	PF_Err err = PF_Err_NONE;
	try {
		Init(in_dataP, out_dataP, paramsP, outputP, ID_NUM_PARAMS);
		m_cmd = PF_Cmd_QUERY_DYNAMIC_FLAGS;
		if ((in_data!=NULL)&&(out_data!=NULL)) {
			PF_ParamDef def;
			AEFX_CLR_STRUCT(def);
			
			ERR(PF_CHECKOUT_PARAM(
				in_dataP,
				ID_MOVING,
				in_dataP->current_time, in_dataP->time_step, in_dataP->time_scale,
				&def));
			PF_Boolean b = (PF_Boolean)def.u.bd.value;
			if (out_dataP->out_flags != PF_OutFlag_NONE) {
				if (b == TRUE) {
					out_dataP->out_flags |= PF_OutFlag_NON_PARAM_VARY;
				}
				else {
					out_dataP->out_flags &= ~PF_OutFlag_NON_PARAM_VARY;
				}
			}
			ERR(PF_CHECKIN_PARAM(in_dataP,&def));
		}
	}
	catch (PF_Err& errP) {
		err = errP;
	}
	return err;
}
// **********************************************************
// **********************************************************
PF_Err NoiseHorizon::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	PF_FpLong gv=1;
	ERR(GetCHECKBOX(ID_MOVING, &infoP->moving));

	ERR(GetFLOAT(ID_GLOBAL_VALUE, &gv));
	if (!err) gv /= 100;

	ERR(GetADD(ID_RGBS_SEED, &infoP->rgbs.seed));
	PF_FpLong rv = 1;
	ERR(GetFLOAT(ID_RGBS_VALUE, &rv));
	if (!err) rv /= 100;
	ERR(GetADD(ID_RGBS_R, &infoP->rgbs.RShift));
	if (!err)
	{
		infoP->rgbs.RShift = (A_long)((double)infoP->rgbs.RShift * gv*rv + 0.5);
	}
	ERR(GetADD(ID_RGBS_G, &infoP->rgbs.GShift));
	if (!err)
	{
		infoP->rgbs.GShift = (A_long)((double)infoP->rgbs.GShift * gv*rv + 0.5);
	}
	ERR(GetADD(ID_RGBS_B, &infoP->rgbs.BShift));
	if (!err)
	{
		infoP->rgbs.BShift = (A_long)((double)infoP->rgbs.BShift * gv*rv + 0.5);
	}
	A_long v=0;
	ERR(GetADD(ID_RS_SEED, &v));
	if (!err)
	{
		infoP->rs.seed = v;
	}
	ERR(GetADD(ID_RS_VALUE, &v));
	if (!err)
	{
		infoP->rs.value = (A_long)((double)v * gv + 0.5);
	}
	ERR(GetADD(ID_RS_SHIFT, &v));
	if (!err)
	{
		infoP->rs.XShift = v;
	}



	ERR(GetADD(ID_SWAPLINE_SEED, &infoP->swapline_seed));
	ERR(GetADD(ID_SWAPVALUE, &infoP->swapValue));
	if (!err)
	{
		infoP->swapValue = (A_long)((double)infoP->swapValue * gv + 0.5);
	}
	ERR(GetADD(ID_SWAPHEIGHT, &infoP->swapHeight));


	ERR(GetADD(ID_NOISE_SEED, &infoP->noise_seed));
	ERR(GetFLOAT(ID_NOISEVALUE, &infoP->noisevalue));
	if (!err)
	{
		infoP->noisevalue /= 100;
		infoP->noisevalue *= gv;
	}
	ERR(GetADD(ID_NOISELENGTH, &infoP->noiseLength));
	ERR(GetFLOAT(ID_NOISESTRONG, &infoP->noiseStrong));
	if (!err) infoP->noiseStrong /= 100;
	ERR(GetADD(ID_RL_SEED, &infoP->rl.seed));
	ERR(GetADD(ID_RL_VALUE, &infoP->rl.value));
	if (!err)
	{
		infoP->rl.value = (A_long)((double)infoP->rl.value * gv + 0.5);
	}
	ERR(GetADD(ID_RL_VALUE2, &infoP->rl.value2));
	ERR(GetFLOAT(ID_RL_OPACITY, &infoP->rl.Opacity));
	if (!err)
	{
		infoP->rl.Opacity /= 100;
	}

	return err;
};
// **********************************************************
PF_Err NoiseHorizon::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	PF_Err err2 = PF_Err_NONE;
	NFWorld* src = new NFWorld(input, in_data, pixelFormat());
	NFWorld* dst = new NFWorld(output, in_data, pixelFormat());
	dst->Copy(src);
	infoP->nfworld = dst;
	infoP->rgbs.nfworld = src;
	infoP->frame = frame();
	infoP->rgbs.frame = frame();
	double ds = ParamDownScaleX();

	infoP->noiseLength = (A_long)((double)infoP->noiseLength * ds);
	infoP->swapHeight = (A_long)((double)infoP->swapHeight * ds);
	infoP->rgbs.RShift = (A_long)((double)infoP->rgbs.RShift * ds);
	infoP->rgbs.GShift = (A_long)((double)infoP->rgbs.GShift * ds);
	infoP->rgbs.BShift = (A_long)((double)infoP->rgbs.BShift * ds);
	infoP->rs.XShift = (A_long)((double)infoP->rs.XShift * ds);
	infoP->rl.value = (A_long)((double)infoP->rl.value * ds);

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
		RGBShiftExec(infoP, src, dst);
		RandomShiftExec(infoP, src, dst);
		SwapLine8(infoP);
		NoiseHor8(infoP);
		RandomLineExec(infoP, src, dst);
		break;
	default:
		break;
	}
	/*
	PF_ParamDef			checkout;
	PF_ChannelDesc		desc;
	PF_ChannelRef		ref;
	PF_ChannelChunk		chunk;
	PF_Boolean			found_depthPB;
	int32_t				num_channelsL = 0;
	PF_Rect				rect = { 0,0,100,100 };
	PF_Rect				rect2 = { 0,0,100,100 };
	
	ERR(PF_CHECKOUT_PARAM(in_data,
		ID_LAYER,
		(in_data->current_time),
		in_data->time_step,
		in_data->time_scale,
		&checkout));
	if (checkout.u.ld.data) {
		rect.left = 0;
		rect.top = 0;
		rect.right = rect.left + checkout.u.ld.width;
		rect.bottom = rect.top + checkout.u.ld.height;

		rect2.left = infoP->pointValue.x / 66636;
		rect2.top = infoP->pointValue.y / 66636;
		rect2.right = rect2.left + checkout.u.ld.width;
		rect2.bottom= rect2.top + checkout.u.ld.height;
		ERR(PF_COPY(&checkout.u.ld,
			dst->world,
			&rect,
			&rect2));
	}
	ERR2(PF_CHECKIN_PARAM(in_data, &checkout));		// ALWAYS check in,
	*/
	delete src;
	delete dst;

	return err;

};
// **********************************************************
