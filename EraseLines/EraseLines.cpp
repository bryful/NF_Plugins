#include "EraseLines.h"


// **********************************************************
PF_Err EraseLines::ParamsSetup(
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
	PF_ADD_COLOR("color0",
		0x00,
		0x00,
		0x00,
		ID_LINECOLOR0
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC("colors", ID_TOPIC_COLORS);
	//----------------------------------------------------------------
	std::string cap;
	for (int i = 1; i < LINECOLR_MAX; i++)
	{
		AEFX_CLR_STRUCT(def);
		def.flags = PF_ParamFlag_SUPERVISE |
			PF_ParamFlag_CANNOT_TIME_VARY |
			PF_ParamFlag_CANNOT_INTERP;

		std::string cap = "colorEnabled" + std::to_string(i);
		PF_ADD_CHECKBOX(cap.c_str(),
			"on",
			FALSE,
			0,
			ID_LINE_COLOR_E(i)
		);
		cap = "color" + std::to_string(i);
		AEFX_CLR_STRUCT(def);
		PF_ADD_COLOR(
			cap.c_str(),
			0x00,
			0x00,
			0x00,
			ID_LINE_COLOR(i)
			);
	}
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_COLORS_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(
		"remainingPixel",
		5,	//メニューの数
		1,	//デフォルト
		"none|white|black|transparent|spotColor",
		ID_ALT_MODE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(
		"spotColor",
		0xFF,
		0xFF,
		0xFF,
		ID_ALTCOLOR
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("dotFilter", "on", TRUE, 0, ID_DOTFLT);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("prioritizeDark", "on", TRUE, 0, ID_PRIORITIZE_DARK);

//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err	 EraseLines::UserChangedParam(
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
PF_Err EraseLines::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	AEFX_CLR_STRUCT(*infoP);
	ERR(GetCOLOR(ID_LINECOLOR0, &infoP->lineColors[0]));
	infoP->lineColorsCount = 1;

	A_long idx = 1;
	for (int i = 1; i < LINECOLR_MAX; i++)
	{
		PF_Boolean ok = FALSE;
		ERR(GetCHECKBOX(ID_LINE_COLOR_E(i), &ok));
		if ((!err) && (ok == TRUE))
		{
			PF_Pixel c;
			ERR(GetCOLOR(ID_LINE_COLOR(i), &c));
			if (!err) {
				infoP->lineColors[idx] = c;
				idx++;

			}
		}
	}
	infoP->lineColorsCount = idx;
	ERR(GetPOPUP(ID_ALT_MODE, &infoP->altMode));

	ERR(GetCOLOR(ID_ALTCOLOR, &infoP->altColor));
	if (!err)
	{
		infoP->altColor16 = NF_Pixel8To16(infoP->altColor);
		infoP->altColor32 = NF_Pixel8To32(infoP->altColor);
	}
	ERR(GetCHECKBOX(ID_DOTFLT, &infoP->dotFilter));
	ERR(GetCHECKBOX(ID_PRIORITIZE_DARK, &infoP->prioritizeDark));
	return err;
};
// **********************************************************
PF_Err EraseLines::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	PF_Err err2 = PF_Err_NONE;
	NFWorld src, dst;
	src.Setup(input, in_data, pixelFormat());
	dst.Setup(output, in_data, pixelFormat());
	
	dst.Copy(&src);
	EraseLinesSub(infoP, &dst);
	return err;

};
// **********************************************************
