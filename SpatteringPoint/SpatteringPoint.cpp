#include "SpatteringPoint.h"


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
	PF_ADD_CHECKBOX(NF_CATEGORY,NF_NAME,FALSE,0,ID_VERSION);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC("texture", ID_TOPIC_TEX);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_LAYER("texLayer",
		PF_LayerDefault_NONE,
		ID_LAYER);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(
		"bufferSize",
		5,	//メニューの数
		1,	//デフォルト
		"16|32|48|64|128",
		ID_TEXSIZE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"opacity",				//Name
		1,						//VALID_MIN
		100,					//VALID_MAX
		10,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_OPACITY
	);	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"opacityRandom",		//Name
		0,						//VALID_MIN
		1000,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_OPACITYRANDOM
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"scale",				//Name
		1,						//VALID_MIN
		100,					//VALID_MAX
		10,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_SCALE
	);		
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"scaleRandom",		//Name
		0,						//VALID_MIN
		1000,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_SCALERANDOM
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_TEX_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC("objects", ID_TOPIC_OBJ);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(
		"Pos",			//"New Center"
		50,	// X
		50,	// Y
		0,	// Flag
		ID_POS
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"radius",		//Name
		0,						//VALID_MIN
		10000,					//VALID_MAX
		0,						//SLIDER_MIN
		1000,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		200,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_RADIUS
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"randomRadius",		//Name
		0,						//VALID_MIN
		200,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		50,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_RADIUS_RANDOM
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"radiusScale",		//Name
		0,						//VALID_MIN
		10000,					//VALID_MAX
		0,						//SLIDER_MIN
		1000,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_RADOUS_SCALE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"globalValue",		//Name
		0,						//VALID_MIN
		500,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_GLOBAL_VALUE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"valueMain",			//パラメータの名前
		0, 						//数値入力する場合の最小値
		5000,				//数値入力する場合の最大値
		0,				//スライダーの最小値
		200,			//スライダーの最大値
		10,				//デフォルトの値
		ID_VALUE_MAIN
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"valueSub",			//パラメータの名前
		0, 						//数値入力する場合の最小値
		1000,				//数値入力する場合の最大値
		0,				//スライダーの最小値
		200,			//スライダーの最大値
		20,				//デフォルトの値
		ID_VALUE_SUB
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"radiusSub",		//Name
		0,						//VALID_MIN
		1000,					//VALID_MAX
		0,						//SLIDER_MIN
		200,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_RADIUS_SUB
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"radiusSubRandom",		//Name
		0,						//VALID_MIN
		200,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		50,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_RADIUS_SUB_RANDOM
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_OBJ_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC("dir", ID_TOPIC_DIR);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(
		"directionKind",
		2,	//メニューの数
		1,	//デフォルト
		"round|directinal",
		ID_DIRECTION_KIND
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(
		"direction",
		0,
		ID_DIRECTION);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"directionSpread",		//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		20,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_DIRECTION_SPREAD
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(
		"anchorDir",
		180,
		ID_ANCHOR_DIR);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"anchorLength",		//Name
		-10000,						//VALID_MIN
		10000,					//VALID_MAX
		-300,						//SLIDER_MIN
		300,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_ANCHOR_LENGTH
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_DIR_END);

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
	def.flags = PF_ParamFlag_SUPERVISE |
		PF_ParamFlag_CANNOT_TIME_VARY |
		PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(
		"moving",
		"on",
		TRUE,
		0,
		ID_MOVING
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(
		"Origin",
		"on",
		FALSE,
		0,
		ID_COPYTOORIGIN
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
PF_Err SpatteringPoint::QueryDynamicFlags(
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
		if ((in_data != NULL) && (out_data != NULL)) {
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
			ERR(PF_CHECKIN_PARAM(in_dataP, &def));
		}
	}
	catch (PF_Err& errP) {
		err = errP;
	}
	return err;
}
// **********************************************************
PF_Err SpatteringPoint::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	A_long v;
	ERR(GetPOPUP(ID_TEXSIZE, &v));
	if (!err)
	{
		infoP->textSize = 16 * v;
	}
	else {
		infoP->textSize = 16;
	}
	ERR(GetFLOAT(ID_OPACITY, &infoP->opacity));
	if (!err)
	{
		infoP->opacity /= 100;
	}
	ERR(GetFLOAT(ID_OPACITYRANDOM, &infoP->opacityRandom));
	if (!err)
	{
		infoP->opacityRandom /= 100;
	}
	ERR(GetFLOAT(ID_SCALE, &infoP->scale));
	if (!err)
	{
		infoP->scale /= 100;
	}
	ERR(GetFLOAT(ID_SCALERANDOM, &infoP->scaleRandom));
	if (!err)
	{
		infoP->scaleRandom /= 100;
	}
	ERR(GetPOINT_DOUBLE(ID_POS, &infoP->pos));
	ERR(GetFLOAT(ID_RADIUS, &infoP->radius));
	ERR(GetFLOAT(ID_RADIUS_RANDOM, &infoP->radiusRandom));
	if (!err) { infoP->radiusRandom /= 100; }else{ infoP->radiusRandom = 0; }
	ERR(GetFLOAT(ID_RADOUS_SCALE, &infoP->radiusScale));
	if (!err) { infoP->radiusScale /= 100; }else{ infoP->radiusScale = 1; }

	PF_FpLong gv = 1;
	ERR(GetFLOAT(ID_GLOBAL_VALUE, &gv));
	if (!err) { gv /= 100; }else{ gv = 1; }
	
	ERR(GetADD(ID_VALUE_MAIN, &v));
	if (!err)
	{
		infoP->valueMain = (A_long)((double)v * gv + 0.5);
		if ((infoP->valueMain == 0) && (v > 0))
		{
			infoP->valueMain = 1;
		}
	}
	ERR(GetADD(ID_VALUE_SUB, &v));
	if (!err)
	{
		infoP->valueSub = (A_long)((double)v * gv + 0.5);
		if ((infoP->valueSub == 0) && (v > 0))
		{
			infoP->valueSub = 1;
		}
	}
	ERR(GetFLOAT(ID_RADIUS_SUB, &infoP->radiusSub));
	ERR(GetFLOAT(ID_RADIUS_SUB_RANDOM, &infoP->radiusSubRandom));
	if (!err) { infoP->radiusSubRandom /= 100; }
	else { infoP->radiusSubRandom = 1; }

	ERR(GetPOPUP(ID_DIRECTION_KIND, &infoP->DirectionKind));
	ERR(GetANGLE_DOUBLE(ID_DIRECTION, &infoP->direction));
	ERR(GetFLOAT(ID_DIRECTION_SPREAD, &infoP->directionSpread));
	if (!err)
	{
		infoP->directionSpread /= 100;
	}
	ERR(GetANGLE_DOUBLE(ID_ANCHOR_DIR, &infoP->anchorDir));
	ERR(GetFLOAT(ID_ANCHOR_LENGTH, &infoP->anchorLength));


	ERR(GetCHECKBOX(ID_MOVING, &infoP->moving));
	ERR(GetCHECKBOX(ID_COPYTOORIGIN, &infoP->isCopyOrigin));

	return err;
};

// **********************************************************
PF_Err SpatteringPoint::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;



	if (infoP->isCopyOrigin == TRUE)
	{
		Copy(input, output);
	}
	else {
		PF_FILL(NULL, NULL, output);
	}
	if ((infoP->valueMain > 0)&&((infoP->valueSub > 0)) && (infoP->opacity > 0) && (infoP->scale>0))
	{
		NFWorld dst;
		dst.Setup(output, in_data, pixelFormat());

		GetSpatFromLayer(ID_LAYER, infoP->textSize, &infoP->spat);
		if (infoP->spat.count == 0) return err;
		PF_EffectWorld buf = NewEffectWorld(infoP->spat.width, infoP->spat.height, pixelFormat());
		NFWorld bufNF;
		bufNF.Setup(&buf, in_data, pixelFormat());
		NFWorld nf;
		A_long seed = infoP->seed;
		if (infoP->moving == TRUE)
		{
			seed += frame();
		}
		PF_FpLong al = infoP->anchorLength * ParamDownScaleX();
		A_FloatPoint anchor = ROT_LENGTH(in_data, infoP->anchorDir, al, infoP->pos);

		for (int i = 0; i < infoP->valueMain; i++)
		{
			PF_FpLong rt = 0;
			F_SRAND(seed + i * 8);
			if (infoP->DirectionKind == 2)
			{
				PF_FpLong bb = 360 * infoP->directionSpread/2;
				rt = infoP->direction + (-bb + 2*bb * F_RAND_D());
			}
			else {
				rt = 360 * F_RAND_D();
			}
			PF_FpLong ln = infoP->radius * ParamDownScaleX();
			ln = ln + (1-2*F_RAND_D() )* ln * infoP->radiusRandom;
			if (infoP->radiusRandom != 0) {
				if ((i % 12) == 0) ln *= 1.3;
				else if ((i % 13) == 0) ln *= 0.7;
			}

			A_FloatPoint ps = ROT_LENGTH(in_data,rt, ln, infoP->pos);

			ps = POS_SCALE(ps, anchor, infoP->radiusScale);

			for (int j = 0; j < infoP->valueSub; j++)
			{
				PF_FpLong ln2 = infoP->radiusSub * ParamDownScaleX();
				ln2 = ln2 + (1 - 2 * F_RAND_D()) * ln2 * infoP->radiusSubRandom;
				if (infoP->radiusSubRandom != 0) {
					if ((j % 6) == 0) ln2 *= 1.5;
					else if ((j % 7) == 0) ln2 *= 0.5;
				}

				A_FloatPoint ps2 = ROT_LENGTH(in_data, 360* F_RAND_D(), ln2, ps);
				ps2 = POS_SCALE(ps2, anchor, infoP->radiusScale);

				PF_FpLong sc = RandomD(infoP->scale, infoP->scaleRandom);
				PF_FpLong op = RandomD(infoP->opacity, infoP->opacityRandom);
				int idx = F_RANDR(0, infoP->spat.count - 1);
				nf.Setup(&infoP->spat.worlds[idx], in_data, pixelFormat());
				bufNF.CopyCenter(&nf, sc);

				if (op < 1)
				{
					bufNF.AlphaMul(op);
				}


				dst.CopyEX(&bufNF, (A_long)(ps2.x +0.5), (A_long)(ps2.y + 0.5));

			}
		}
		DeleteSpat(&infoP->spat);
		DisposeEffectWorld(buf);

	}

	return err;

};
// **********************************************************
