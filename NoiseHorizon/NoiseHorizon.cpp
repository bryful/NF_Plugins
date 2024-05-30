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
	);	
	//----------------------------------------------------------------
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
		"rgbs_seed",	//�p�����[�^�̖��O
		-30000, 		//���l���͂���ꍇ�̍ŏ��l
		30000,			//���l���͂���ꍇ�̍ő�l
		-200,			//�X���C�_�[�̍ŏ��l
		200,			//�X���C�_�[�̍ő�l
		0,				//�f�t�H���g�̒l
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
		"RedShift",	//�p�����[�^�̖��O
		0, 		//���l���͂���ꍇ�̍ŏ��l
		30000,			//���l���͂���ꍇ�̍ő�l
		0,			//�X���C�_�[�̍ŏ��l
		200,			//�X���C�_�[�̍ő�l
		2,				//�f�t�H���g�̒l
		ID_RGBS_R
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"GreenShift",	//�p�����[�^�̖��O
		0, 		//���l���͂���ꍇ�̍ŏ��l
		30000,			//���l���͂���ꍇ�̍ő�l
		0,				//�X���C�_�[�̍ŏ��l
		200,			//�X���C�_�[�̍ő�l
		2,				//�f�t�H���g�̒l
		ID_RGBS_G
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"BlueShift",	//�p�����[�^�̖��O
		-0, 		//���l���͂���ꍇ�̍ŏ��l
		30000,			//���l���͂���ꍇ�̍ő�l
		-0,			//�X���C�_�[�̍ŏ��l
		200,			//�X���C�_�[�̍ő�l
		2,				//�f�t�H���g�̒l
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
		"rs_seed",	//�p�����[�^�̖��O
		-30000, 		//���l���͂���ꍇ�̍ŏ��l
		30000,			//���l���͂���ꍇ�̍ő�l
		-200,			//�X���C�_�[�̍ŏ��l
		200,			//�X���C�_�[�̍ő�l
		0,				//�f�t�H���g�̒l
		ID_RS_SEED
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"rsValue",	//�p�����[�^�̖��O
		0, 			//���l���͂���ꍇ�̍ŏ��l
		2000,		//���l���͂���ꍇ�̍ő�l
		0,			//�X���C�_�[�̍ŏ��l
		100,		//�X���C�_�[�̍ő�l
		10,			//�f�t�H���g�̒l
		ID_RS_VALUE
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"rsSize",	//�p�����[�^�̖��O
		16, 			//���l���͂���ꍇ�̍ŏ��l
		256,		//���l���͂���ꍇ�̍ő�l
		16,			//�X���C�_�[�̍ŏ��l
		128,		//�X���C�_�[�̍ő�l
		64,			//�f�t�H���g�̒l
		ID_RS_SIZE
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"xShift",	//�p�����[�^�̖��O
		1, 			//���l���͂���ꍇ�̍ŏ��l
		1000,		//���l���͂���ꍇ�̍ő�l
		1,			//�X���C�_�[�̍ŏ��l
		200,		//�X���C�_�[�̍ő�l
		100,			//�f�t�H���g�̒l
		ID_RS_SHIFT
	);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_RS_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC("Scanline", ID_TOPIC_SL);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"sl_seed",	//�p�����[�^�̖��O
		-30000, 		//���l���͂���ꍇ�̍ŏ��l
		30000,			//���l���͂���ꍇ�̍ő�l
		-200,			//�X���C�_�[�̍ŏ��l
		200,			//�X���C�_�[�̍ő�l
		0,				//�f�t�H���g�̒l
		ID_SL_SEED
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"slVaue",			//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		100,					//CURVE_TOLERANCE
		10,					//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_SL_VALUE
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"slShift",		//�p�����[�^�̖��O
		0, 				//���l���͂���ꍇ�̍ŏ��l
		3000,			//���l���͂���ꍇ�̍ő�l
		0,				//�X���C�_�[�̍ŏ��l
		100,			//�X���C�_�[�̍ő�l
		20,				//�f�t�H���g�̒l
		ID_SL_SHIFT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_SL_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC("swapLine", ID_TOPIC_SWAPLINE);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"sw_seed",	//�p�����[�^�̖��O
		-30000, 		//���l���͂���ꍇ�̍ŏ��l
		30000,			//���l���͂���ꍇ�̍ő�l
		-200,			//�X���C�_�[�̍ŏ��l
		200,			//�X���C�_�[�̍ő�l
		0,				//�f�t�H���g�̒l
		ID_SWAPLINE_SEED
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"swapValue",	//�p�����[�^�̖��O
		0, 		//���l���͂���ꍇ�̍ŏ��l
		1000,			//���l���͂���ꍇ�̍ő�l
		0,			//�X���C�_�[�̍ŏ��l
		100,			//�X���C�_�[�̍ő�l
		3,				//�f�t�H���g�̒l
		ID_SWAPVALUE
	);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"swapHeight",	//�p�����[�^�̖��O
		1, 				//���l���͂���ꍇ�̍ŏ��l
		1000,			//���l���͂���ꍇ�̍ő�l
		1,				//�X���C�_�[�̍ŏ��l
		200,			//�X���C�_�[�̍ő�l
		35,				//�f�t�H���g�̒l
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
		"n_seed",	//�p�����[�^�̖��O
		-30000, 		//���l���͂���ꍇ�̍ŏ��l
		30000,			//���l���͂���ꍇ�̍ő�l
		-200,			//�X���C�_�[�̍ŏ��l
		200,			//�X���C�_�[�̍ő�l
		0,				//�f�t�H���g�̒l
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
		"noiseLength",	//�p�����[�^�̖��O
		1, 				//���l���͂���ꍇ�̍ŏ��l
		300,			//���l���͂���ꍇ�̍ő�l
		1,				//�X���C�_�[�̍ŏ��l
		100,			//�X���C�_�[�̍ő�l
		30,				//�f�t�H���g�̒l
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
		20,						//DFLT
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
		"rl_seed",	//�p�����[�^�̖��O
		-30000, 		//���l���͂���ꍇ�̍ŏ��l
		30000,			//���l���͂���ꍇ�̍ő�l
		-200,			//�X���C�_�[�̍ŏ��l
		200,			//�X���C�_�[�̍ő�l
		0,				//�f�t�H���g�̒l
		ID_RL_SEED
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"rlValue",	//�p�����[�^�̖��O
		0, 				//���l���͂���ꍇ�̍ŏ��l
		100,			//���l���͂���ꍇ�̍ő�l
		0,				//�X���C�_�[�̍ŏ��l
		10,			//�X���C�_�[�̍ő�l
		2,				//�f�t�H���g�̒l
		ID_RL_VALUE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"rlValue2",	//�p�����[�^�̖��O
		1, 				//���l���͂���ꍇ�̍ŏ��l
		100,			//���l���͂���ꍇ�̍ő�l
		1,				//�X���C�_�[�̍ŏ��l
		20,			//�X���C�_�[�̍ő�l
		2,				//�f�t�H���g�̒l
		ID_RL_VALUE2
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

	// **************
	ERR(GetCHECKBOX(ID_MOVING, &infoP->moving));

	ERR(GetFLOAT(ID_GLOBAL_VALUE, &gv));
	if (!err) gv /= 100;

	// **************
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

	ERR(GetADD(ID_SL_SEED, &infoP->sl.seed));
	ERR(GetFLOAT(ID_SL_VALUE, &infoP->sl.value));
	if (!err)
	{
		infoP->sl.value /= 100;
	}
	ERR(GetADD(ID_SL_SHIFT, &infoP->sl.XShift));
	if (!err)
	{
		infoP->sl.XShift = (A_long)((double)infoP->sl.XShift * gv + 0.5);
	}

	ERR(GetADD(ID_RS_SEED, &v));
	if (!err)
	{
		infoP->rs.seed = v;
	}
	ERR(GetADD(ID_RS_SIZE, &infoP->rs.size));
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
	if (!err)
	{
		infoP->swapHeight = (A_long)((double)infoP->swapHeight * gv + 0.5);
	}

	ERR(GetADD(ID_NOISE_SEED, &infoP->noise_seed));
	ERR(GetFLOAT(ID_NOISEVALUE, &infoP->noisevalue));
	if (!err)
	{
		infoP->noisevalue /= 100;
		infoP->noisevalue *= gv;
	}
	ERR(GetADD(ID_NOISELENGTH, &infoP->noiseLength));
	if (!err)
	{
		infoP->noiseLength = (A_long)((double)infoP->noiseLength * gv + 0.5);
	}
	ERR(GetFLOAT(ID_NOISESTRONG, &infoP->noiseStrong));
	if (!err) infoP->noiseStrong /= 100;
	
	ERR(GetADD(ID_RL_SEED, &infoP->rl.seed));
	ERR(GetADD(ID_RL_VALUE, &infoP->rl.value));
	if (!err)
	{
		infoP->rl.value = (A_long)((double)infoP->rl.value * gv + 0.5);
	}
	ERR(GetADD(ID_RL_VALUE2, &infoP->rl.value2));

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
	infoP->frame = frame();
	infoP->rgbs.frame = frame();
	double ds = ParamDownScaleX();

	//�𑜓x�̕␳
	infoP->noiseLength = (A_long)((double)infoP->noiseLength * ds);
	infoP->swapHeight = (A_long)((double)infoP->swapHeight * ds);
	infoP->rgbs.RShift = (A_long)((double)infoP->rgbs.RShift * ds);
	infoP->rgbs.GShift = (A_long)((double)infoP->rgbs.GShift * ds);
	infoP->rgbs.BShift = (A_long)((double)infoP->rgbs.BShift * ds);
	infoP->rs.XShift = (A_long)((double)infoP->rs.XShift * ds);
	infoP->rs.size = (A_long)((double)infoP->rs.size * ds);
	infoP->rl.value = (A_long)((double)infoP->rl.value * ds);
	infoP->sl.XShift = (A_long)((double)infoP->sl.XShift * ds);

	RandomShiftExec(infoP, src, dst);
	RandomScanlineExec(infoP, src, dst);
	SwapLineExec(infoP, src, dst);
	NoiseHorExec(infoP, src, dst);
	RGBShiftExec(infoP, src, dst);
	RandomLineExec(infoP, src, dst);

	delete src;
	delete dst;

	return err;

};
// **********************************************************