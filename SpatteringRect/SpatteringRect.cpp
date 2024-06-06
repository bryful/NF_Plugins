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
	//def.flags = PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC("textue", ID_TOPIC_TEX);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_LAYER("texLayer",
		PF_LayerDefault_NONE,
		ID_LAYER);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(
		"texSize",
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
		100,					//VALID_MAX
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
		"scaleRandom",		//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
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
	PF_ADD_SLIDER(
		"value",			//パラメータの名前
		0, 		//数値入力する場合の最小値
		15000,			//数値入力する場合の最大値
		0,			//スライダーの最小値
		1000,			//スライダーの最大値
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
		"BottomRight",			//"New Center"
		75,	// X
		75,	// Y
		0,	// Flag
		ID_BOTTOMRIGHT
	);
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
		"CopyToOrigin",
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
	ERR(GetFLOAT(ID_SCALERANDOM, &infoP->scaleRandom));
	if (!err)
	{
		infoP->scaleRandom /= 100;
	}

	ERR(GetADD(ID_VALUE, &infoP->value));

	PF_Point p1;
	p1.h = 0;
	p1.v = 0;
	ERR(GetPOINT_INT(ID_TOPLEFT, &p1));
	PF_Point p2;
	p2.h = 0;
	p2.v = 0;
	ERR(GetPOINT_INT(ID_BOTTOMRIGHT, &p2));
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
	ERR(GetADD(ID_SEED, &infoP->seed));
	ERR(GetCHECKBOX(ID_MOVING, &infoP->moving));
	ERR(GetCHECKBOX(ID_COPYTOORIGIN, &infoP->isCopyOrigin));

	return err;
};
// **********************************************************
PF_Err  SpatteringPoint::CreateBuf(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;

	infoP->buf.count = 0;
	NFWorld* nf = new NFWorld();


	PF_ParamDef			buf;

	A_long cnt = frameCount();
	if (cnt > BUF_MAX) cnt = BUF_MAX;

	int errCount = 0;
	int idx = 0;
	A_long bx = (A_long)((double)infoP->textSize * ParamDownScaleX());
	if (bx < 4) bx = 4;
	for (int i = 0; i < cnt; i++)
	{
		ERR(PF_CHECKOUT_PARAM(in_data,
			ID_LAYER,
			(in_data->time_step*i),
			(in_data->time_step),
			in_data->time_scale,
			&buf)
		);
		if (buf.u.ld.data) {
			A_long w = buf.u.ld.width;
			A_long h = buf.u.ld.height;
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
			infoP->buf.worlds[idx] = NewEffectWorld(bx, bx, pixelFormat());
			if (!ErrResult())
			{
				ERR(PF_COPY(&buf.u.ld,
					&infoP->buf.worlds[idx],
					&srcR,
					&dstR));
				idx++;
			}
		}	else {
			errCount++;
			if (errCount > 2) break;
		}
		ERR(PF_CHECKIN_PARAM(in_data, &buf));		// ALWAYS check in,
		infoP->buf.count = idx;
		infoP->buf.width = bx;
		infoP->buf.height = bx;
	}
	/*
	if (infoP->buf.count <= 0)
	{
		infoP->buf.width = SPT_WIDTH;
		infoP->buf.height = SPT_WIDTH;

		for (int i = 0; i < SPT_COUNT; i++)
		{
			PF_EffectWorld wld = NewEffectWorld(SPT_WIDTH, SPT_WIDTH, pixelFormat());
			infoP->buf.worlds[i] = wld;
			PF_Pixel8 p8 = { PF_MAX_CHAN8,PF_HALF_CHAN8, PF_HALF_CHAN8, PF_MAX_CHAN8, };

			nf->Setup(&wld, in_data, pixelFormat());
			nf->SetChar(SPT[i], SPT_WIDTH, SPT_WIDTH, p8);
		}
		infoP->buf.count = SPT_COUNT;
	}
	*/
	delete nf;
	return err;

}
// **********************************************************
PF_Err SpatteringPoint::DeleteBuf(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;

	if (infoP->buf.count <= 0) return err;

	for (int i = infoP->buf.count - 1; i >=0 ; i--)
	{
		DisposeEffectWorldP(&infoP->buf.worlds[i]);
	}
	infoP->buf.count = 0;
	

	return err;
}
// **********************************************************
PF_Err SpatteringPoint::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;



	if (infoP->isCopyOrigin == TRUE)
	{
		Copy(input, output);
	}

	if (infoP->value >= 0)
	{

		NFWorld* src = new NFWorld(input, in_data, pixelFormat());
		NFWorld* dst = new NFWorld(output, in_data, pixelFormat());

		CreateBuf(infoP);
		PF_EffectWorld buf = NewEffectWorld(infoP->buf.width, infoP->buf.height, pixelFormat());
		NFWorld* bufNF = new NFWorld(&buf, in_data, pixelFormat());
		if (infoP->buf.count > 0)
		{
			if (infoP->moving == TRUE)
			{
				F_SRAND(infoP->seed + frame());
			}
			else {
				F_SRAND(infoP->seed);
			}

			int w = infoP->rect.right - infoP->rect.left;
			int h = infoP->rect.bottom - infoP->rect.top;

			NFWorld* nf = new NFWorld();
			for (int i = 0; i < infoP->value; i++)
			{
				//位置を決める
				int x = infoP->rect.left + F_RANDR(0, w);
				int y = infoP->rect.top + F_RANDR(0, h);

				//拡大率を求める
				PF_FpLong sc = 1 - infoP->scaleRandom;
				if (sc != 1) 
				{
					sc = sc + (1.1 - sc) * F_RAND_D();
					if (sc > 1)sc = 1;
				}
				else {
					F_RAND_D();
				}

				PF_FpLong op = infoP->opacity;
				if (infoP->opacityRandom != 0)
				{
					PF_FpLong op2 = op - op * infoP->opacityRandom;
					op = op2 + (op-op2) * F_RAND_D();
				}
				else {
					F_RAND_D();
				}
				//TexIndexを得る
				int ggIdx = F_RANDR(0, infoP->buf.count -1);
				nf->Setup(&infoP->buf.worlds[ggIdx],in_data,pixelFormat());

				bufNF->CopyCenter(nf, sc);
				
				if (op < 1)
				{
					bufNF->AlphaMul(op);
				}
				

				dst->CopyEX(bufNF, x, y);
			}
			delete nf;
		}

		DeleteBuf(infoP);
		DisposeEffectWorld(buf);
		delete bufNF;
		delete src;
		delete dst;
	}
	return err;

};
// **********************************************************
