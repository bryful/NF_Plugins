#include "SpatteringRect.h"


// **********************************************************
PF_Err SpatteringPoint::ParamsSetup(
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
	PF_ADD_SLIDER(
		"seed",			//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		-200,			//スライダーの最小値
		200,			//スライダーの最大値
		0,				//デフォルトの値
		ID_SEED
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"value",			//パラメータの名前
		0, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		1000,			//スライダーの最小値
		0,			//スライダーの最大値
		0,				//デフォルトの値
		ID_VALUE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(
		"TopLeft",			//"New Center"
		25,	// X
		25,	// Y
		0,	// Flag
		ID_TOPLEFT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(
		"TopLeft",			//"New Center"
		75,	// X
		75,	// Y
		0,	// Flag
		ID_BOTTOMRIGHT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(
		"CopyToOrigin",
		"on",
		FALSE,
		0,
		ID_COPYTORIGIN
	);
//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
}
// **********************************************************
PF_Err	 SpatteringPoint::UserChangedParam(
	PF_InData* in_dataP,
	PF_OutData* out_dataP,
	PF_ParamDef* paramsP[],
	PF_LayerDef* outputP,
	PF_UserChangedParamExtra* extraP,
	A_long pc)
{
	PF_Err err = PF_Err_NONE;
	Init(in_dataP, out_dataP, paramsP, outputP,ID_NUM_PARAMS);
	m_cmd = PF_Cmd_USER_CHANGED_PARAM;

	return err;
}
// **********************************************************
PF_Err SpatteringPoint::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	ERR(GetADD(ID_SEED, &infoP->seed));
	PF_Point p1;
	p1.h = 0;
	p1.v = 0;
	PF_Point p2;
	p2.h = 0;
	p2.v = 0;
	ERR(GetPOINT_INT(ID_TOPLEFT, &p1));
	ERR(GetPOINT_INT(ID_TOPLEFT, &p2));
	if (p1.h <= p2.h)
	{
		infoP->rect.left = p1.h;
		infoP->rect.right = p2.h;
	}
	else {
		infoP->rect.left = p2.h;
		infoP->rect.right = p1.h;
	}
	if (p1.v <= p2.v)
	{
		infoP->rect.top = p1.v;
		infoP->rect.bottom = p2.v;
	} else {
		infoP->rect.top = p2.v;
		infoP->rect.bottom = p1.v;
	}
	ERR(GetCHECKBOX(ID_COPYTORIGIN, &infoP->isCopyOrigin));

	return err;
};
// **********************************************************
PF_Err SpatteringPoint::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	PF_Err err2 = PF_Err_NONE;
	NFWorld* src = new NFWorld(input, in_data, pixelFormat());
	NFWorld* dst = new NFWorld(output, in_data, pixelFormat());

	if (infoP->isCopyOrigin == TRUE)
	{
		dst->Copy(src);
	}

	/*
	if (infoP->blendOpacity != 0) {
		TargetExec(infoP, src, dst);
		BlendExec(infoP, src, dst);
	}
	else {
		dst->Copy(src);
	}
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
