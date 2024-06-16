#include "Skelton.h"


// **********************************************************
PF_Err Skelton::ParamsSetup(
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
	/*
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
	*/
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(
		"TargetMode",
		3,	//メニューの数
		2,	//デフォルト
		"FullScreen|AlphaOn|TargetColor|",
		ID_TARGET_MODE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR("TargetColor",
		0xFF,
		0xFF,
		0xFF,
		ID_TARGET_COLOR
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(
		"TargetMode",
		5,	//メニューの数
		1,	//デフォルト
		"Normal|Add|Multiply|Screen|Overlay",
		ID_BLEND_MODE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR("BlendColor",
		0xFF,
		0x80,
		0x00,
		ID_BLEND_COLOR
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"BlendOpacity",			//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		100,					//SLIDER_MAX
		100,					//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_BLEND_OPACITY
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags |= PF_ParamFlag_SUPERVISE;
	PF_ADD_BUTTON("ColorSet",
		"Red",
		0,
		PF_ParamFlag_SUPERVISE,
		ID_BTN1);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags |= PF_ParamFlag_SUPERVISE;
	PF_ADD_BUTTON("OpacitySet",
		"50%",
		0,
		PF_ParamFlag_SUPERVISE,
		ID_BTN2);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC("Topic", ID_TOPIC);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR("Color",
		0xFF,
		0xFF,
		0xFF,
		ID_COLOR
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(
		"Int",			//パラメータの名前
		-30000, 		//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		0,				//スライダーの最小値
		300,			//スライダーの最大値
		10,				//デフォルトの値
		ID_INT
	);		
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_FIXED(
		"Fiexed",		//パラメータの名前
		0, 				//数値入力する場合の最小値
		100,			//数値入力する場合の最大値
		0,				//スライダーの最小値
		100,			//スライダーの最大値
		100,			//デフォルトの値
		1,				//数値表示に関するフラグ
		0,
		0,
		ID_FIXED
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"float",				//Name
		0,						//VALID_MIN
		400,					//VALID_MAX
		0,						//SLIDER_MIN
		200,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_FLOAT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(
		"Check",
		"on",
		FALSE,
		0,
		ID_CHECK
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_INTERP;	//キーフレームの自動保管を停止する
	PF_ADD_ANGLE(
		"Angle", 
		45, 
		ID_ANGLE);
	//----------------------------------------------------------------
	//ポップアップメニュー
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_INTERP;	//キーフレームの自動保管を停止する
	PF_ADD_POPUP(
		"Popup",
		5,	//メニューの数
		1,	//デフォルト
		"AAA|BBB|CCC|DDD|EEE",
		ID_POPUP
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(
		"Point",			//"New Center"
		50,	// X
		50,	// Y
		0,	// Flag
		ID_POINT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT_3D(
		"Point3D",			//"New Center"
		50,	// X
		50,	// Y
		50, // Z
		ID_POINT3D
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPICEND);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags |= PF_ParamFlag_SUPERVISE;
	PF_ADD_BUTTON("ColorP",
		"save",
		0,
		PF_ParamFlag_SUPERVISE,
		ID_BTNSV);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags |= PF_ParamFlag_SUPERVISE;
	PF_ADD_BUTTON("ColorP",
		"Load",
		0,
		PF_ParamFlag_SUPERVISE,
		ID_BTNLD);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_LAYER("layer",
		PF_LayerDefault_NONE,
		ID_LAYER);
//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err	 Skelton::UserChangedParam(
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
	if (extraP->param_index == ID_BTN2)
	{
		if (params[ID_BLEND_OPACITY]->u.fs_d.value != 100) {
			SetFLOAT(ID_BLEND_OPACITY, 100);
		}
		else {
			SetFLOAT(ID_BLEND_OPACITY, 50);
		}
	}
	else if (extraP->param_index == ID_BTN1)
	{
		if (params[ID_BLEND_COLOR]->u.cd.value.red != 255)
		{
			PF_Pixel c = { 255,255,0,0 };
			SetCOLOR(ID_BLEND_COLOR, c);
		}
		else if (params[ID_BLEND_COLOR]->u.cd.value.green != 255)
		{
			PF_Pixel c2 = { 255,0,255,0 };
			SetCOLOR(ID_BLEND_COLOR, c2);
		}
		else if (params[ID_BLEND_COLOR]->u.cd.value.blue != 255)
		{
			PF_Pixel c3 = { 255,0,255,255 };
			SetCOLOR(ID_BLEND_COLOR, c3);
		}
		else {
			PF_Pixel c4 = { 255,0,0,0 };
			SetCOLOR(ID_BLEND_COLOR, c4);

		}

	}
	else if (extraP->param_index == ID_BTNSV)
	{
		std::string p = SaveJsonFileDialog(std::string("Json Save"), directoryPath);
		if (p.empty() == FALSE)
		{
			directoryPath = p;
			ParamInfo info;
			GetParams(&info);
			json m_json = ParamsToJson(&info);


			std::ofstream writing_file;
			writing_file.open(directoryPath, std::ios::out);
			writing_file << m_json.dump() << std::endl;
			writing_file.close();
		}
	}
	else if (extraP->param_index == ID_BTNLD)
	{
		std::string p = OpenJsonFileDialog(std::string("Json Save"), directoryPath);
		if (p.empty() == FALSE)
		{
			directoryPath = p;
			std::ifstream ifs(directoryPath);
			if (ifs.good())
			{
				json m_json;
				ifs >> m_json;
				JsonLoad(m_json);
			}
			else
			{
				err = PF_Err_BAD_CALLBACK_PARAM;
			}
		}

	}
	return err;
}
// **********************************************************
json Skelton::ParamsToJson(ParamInfo* infoP)
{
	json m_json;
	m_json["blendColor"] = PixelToJson(infoP->blendColor);
	m_json["blemdOpacity"] =infoP->blendOpacity*100;
	return m_json;
}
// **********************************************************
PF_Err Skelton::JsonLoad(json jsn)
{
	PF_Err err = PF_Err_NONE;
	if (jsn.find("blendColor") != jsn.end()) {
		PF_Pixel bc = JsonToPixel(jsn["blendColor"]);
		ERR(SetCOLOR(ID_BLEND_COLOR, bc));
	}
	if (jsn.find("blemdOpacity") != jsn.end()) {
		PF_FpLong bo = jsn["blemdOpacity"].get< PF_FpLong>();
		ERR(SetFLOAT(ID_BLEND_OPACITY, bo));
	}
	return err;
}
// **********************************************************
PF_Err Skelton::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	ERR(GetPOPUP(ID_TARGET_MODE, &infoP->targetMode));
	ERR(GetCOLOR(ID_TARGET_COLOR, &infoP->targetColor));
	ERR(GetPOPUP(ID_BLEND_MODE, &infoP->blendMode));
	ERR(GetCOLOR(ID_BLEND_COLOR, &infoP->blendColor));
	ERR(GetFLOAT(ID_BLEND_OPACITY, &infoP->blendOpacity));
	if (!err) infoP->blendOpacity /= 100;

	ERR(GetPOINT_FIXED(ID_POINT, &infoP->pointValue));

	/*
	ERR(GetCOLOR(ID_COLOR, &infoP->color));
	ERR(GetADD(ID_INT, &infoP->intValue));
	ERR(GetFIXED(ID_FIXED, &infoP->fixedValue));
	ERR(GetFLOAT(ID_FLOAT, &infoP->floatvalue));
	ERR(GetFLOAT(ID_FLOAT, &infoP->floatvalue));
	ERR(GetCHECKBOX(ID_CHECK, &infoP->check));
	ERR(GetPOPUP(ID_POPUP, &infoP->popupValue));
	ERR(GetPOINT3D(ID_POINT, &infoP->point3DValue));
	*/
	return err;
};
// **********************************************************
PF_Err Skelton::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	PF_Err err2 = PF_Err_NONE;
	NFWorld* src = new NFWorld(input, in_data, pixelFormat());
	NFWorld* dst = new NFWorld(output, in_data, pixelFormat());
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

	delete src;
	delete dst;

	return err;

};
// **********************************************************
