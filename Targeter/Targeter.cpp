#include "Targeter.h"


// **********************************************************
PF_Err Targeter::ParamsSetup(
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
	PF_ADD_CHECKBOX(NF_CATEGORY, NF_NAME, FALSE, 0, ID_VERSION);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(
		"TargetMode",
		3,	//メニューの数
		1,	//デフォルト
		"1:Alpha|2:Color|3:Diff",
		ID_TARGET_MODE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(
		"IsEdge",
		"on",
		FALSE,
		0,
		ID_ISEDGE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(
		"EdgeMode",
		9,	//メニューの数
		5,	//デフォルト
		"1:TopLeft|2:Top|3:TopRight|4:Left|5:ALL|6:Right|7:BottomLeft|8:Bottom|9:BottomRight",
		ID_EDGEMODE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR("TargetColor",
		0xFF,
		0xFF,
		0xFF,
		ID_TARGET_COLOR0
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC("TargetColors", ID_TOPIC_TARGETCOLOR);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("ColorEnabled1", "on", FALSE, 0, ID_TARGET_COLOR_ENABLED1);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR("color1",
		0xFF,
		0x00,
		0x00,
		ID_TARGET_COLOR1
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("ColorEnabled2", "on", FALSE, 0, ID_TARGET_COLOR_ENABLED2);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR("color2",
		0x00,
		0xFF,
		0x00,
		ID_TARGET_COLOR2
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("ColorEnabled3", "on", FALSE, 0, ID_TARGET_COLOR_ENABLED3);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR("color3",
		0x00,
		0xFF,
		0x00,
		ID_TARGET_COLOR3
	);



	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_TARGETCOLOR_END);

	
//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err	 Targeter::UserChangedParam(
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
	/*
	setParamCount(pc);
	if (paramsP != NULL) {
		input = &paramsP[0]->u.ld;
		if (pc > 0) {
			for (A_long i = 0; i < pc; i++) params[i] = paramsP[i];
		}
	}
	in_data = in_dataP;
	out_data = out_dataP;
	output = outputP;
	GetFrame(in_dataP);
	GetSuites(in_dataP);
	*/

	return err;
}
// **********************************************************

// **********************************************************
PF_Err Targeter::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	ERR(GetPOPUP(ID_TARGET_MODE, &infoP->targetMode));
	ERR(GetCHECKBOX(ID_ISEDGE, &infoP->isEdge));
	ERR(GetPOPUP(ID_EDGEMODE, &infoP->edgeMode));

	PF_Pixel c;
	ERR(GetCOLOR(ID_TARGET_COLOR0, &c));
	if (!err)
	{
		infoP->targetColors[0] = c;
		infoP->targetColorsCount = 1;
	}
	for (int i = 0; i < 3; i++)
	{
		int idx = ID_TARGET_COLOR_ENABLED1 + i * 2;
		PF_Boolean b = FALSE;
		ERR(GetCHECKBOX(idx, &b));
		if ((!err) && (b == TRUE))
		{
			ERR(GetCOLOR(idx+1, &c));
			if (!err)
			{
				infoP->targetColors[infoP->targetColorsCount] = c;
				infoP->targetColorsCount++;
			}
		}
	}

	return err;
};
// **********************************************************
PF_Err Targeter::TargetPix(ParamInfo* infoP,  NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	TargetWorkInfo tinfo;

	switch (infoP->targetMode)
	{
	case 1:
		ERR(AlphaTh(&tinfo, dst));
		break;
	case 2:
		break;
	case 3:
		break;
	}
	return err;
}
// **********************************************************
PF_Err Targeter::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	PF_Err err2 = PF_Err_NONE;
	NFWorld src, dst,sub8;
	src.Setup(input, in_data, pixelFormat());
	dst.Setup(output, in_data, pixelFormat());
	PF_EffectWorld subBuf8 = NewEffectWorld(input->width, input->height, PF_PixelFormat_ARGB32);
	sub8.Setup(&subBuf8, in_data, PF_PixelFormat_ARGB32);
	
	//PF_COPY(input, &subBuf8, NULL, NULL);
	sub8.Copyto8Bit(&src);

	TargetPix(infoP, &sub8);
	dst.Copyto_From8Bit(&sub8);
	//PF_COPY(&subBuf8, output,NULL,NULL);

	DisposeEffectWorld(subBuf8);
	return err;

};
// **********************************************************
