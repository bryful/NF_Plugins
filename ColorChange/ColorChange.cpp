#include "ColorChange.h"

static PF_Err
ColChange8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	for (int i = 0; i < COLOR_MAX; i++)
	{
		if (infoP->colorTable[i].Enabled == FALSE) continue;

		if (compPix8Lv(*inP, infoP->colorTable[i].Src, infoP->lv_byte) == TRUE)
		{
			*outP = infoP->colorTable[i].Dst;
		}
	}
	return err;
}
// **********************************************************
static PF_Err
ColChange16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	for (int i = 0; i < COLOR_MAX; i++)
	{
		if (infoP->colorTable[i].Enabled == FALSE) continue;

		if (compPix16_8Lv(*inP, infoP->colorTable[i].Src, infoP->lv_byte) == TRUE)
		{
			*outP = infoP->colorTable[i].Dst16;
		}
	}
	return err;
}
// **********************************************************
static PF_Err
ColChange32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel32* inP,
	PF_Pixel32* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	for (int i = 0; i < COLOR_MAX; i++)
	{
		if (infoP->colorTable[i].Enabled == FALSE) continue;

		if (compPix32_8Lv(*inP, infoP->colorTable[i].Src, infoP->lv_byte) == TRUE)
		{
			*outP = infoP->colorTable[i].Dst32;
		}
	}
	return err;
}
// **********************************************************
PF_Err ColorChange::ParamsSetup(
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
	//def.flags = PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC("IO", ID_TOPIC_IO);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(
		"dispColorTable",
		"on",
		FALSE,
		0,
		ID_DispTable
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(
		"dispUseColor",
		"on",
		FALSE,
		0,
		ID_DispUse
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_BUTTON("ExportColors",
		"export",
		0,
		PF_ParamFlag_SUPERVISE,
		ID_BTN_SAVE);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_BUTTON("ImportColors",
		"import",
		0,
		PF_ParamFlag_SUPERVISE,
		ID_BTN_LOAD);
	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_IO_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
	PF_ADD_TOPIC("ColorTable", ID_TOPIC_CTABLE);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(
		"enabled",
		"on",
		TRUE,
		0,
		ID_ENABLED
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(
		"level",			//Name
		0,					//VALID_MIN
		100,				//VALID_MAX
		0,					//SLIDER_MIN
		10,					//SLIDER_MAX
		100,				//CURVE_TOLERANCE
		0,					//DFLT
		1,					//PREC
		0,					//DISP
		0,					//WANT_PHASE
		ID_LEVEL
	);
	//----------------------------------------------------------------
	for (int i = 0; i < COLOR_MAX; i++)
	{
		int idx = ID_CTABLE(i);
		std::string c;
		std::string idxStr = std::to_string(i);

		//----------------------------------------------------------------
		AEFX_CLR_STRUCT(def);
		c = std::string("enable") + idxStr;
		PF_ADD_CHECKBOX(
			c.c_str(),
			"on",
			FALSE,
			0,
			idx
		);
		c = std::string("src") + idxStr;
		AEFX_CLR_STRUCT(def);
		PF_ADD_COLOR(
			c.c_str(),
			0xFF,
			0xFF,
			0xFF,
			idx+1
		);
		c = std::string("dst") + idxStr;
		AEFX_CLR_STRUCT(def);
		PF_ADD_COLOR(
			c.c_str(),
			0xFF,
			0xFF,
			0xFF,
			idx + 2
		);
	}
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_CTABLE_END);
//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err	 ColorChange::UserChangedParam(
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
	if (extraP->param_index == ID_BTN_SAVE)
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
			writing_file << m_json.dump(4) << std::endl;
			writing_file.close();
		}
	}
	else if (extraP->param_index == ID_BTN_LOAD)
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
				ParamInfo info =  JsonToParams(m_json);
				ParamsSet(&info);
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
json ColorChange::ParamsToJson(ParamInfo* infoP)
{
	json ret;
	ret["level"] =infoP->level;

	json ct;

	for (int i = 0; i < COLOR_MAX; i++)
	{
		json bb;
		bb["index"] = i;
		bb["enabled"] = (bool)infoP->colorTable[i].Enabled;
		bb["src"] =  PixelToJson(infoP->colorTable[i].Src);
		bb["dst"] = PixelToJson(infoP->colorTable[i].Dst);
		ct.push_back(bb);
	}
	ret["ctable"] = ct;
	return ret;
}
// **********************************************************
ParamInfo ColorChange::JsonToParams(json jsn)
{
	ParamInfo info;
	AEFX_CLR_STRUCT(info);
	if (jsn.find("level") != jsn.end()) {
		info.level = jsn["level"].get< PF_FpLong>();
	}
	if (jsn.find("ctable") != jsn.end()) {
		json ct = jsn["ctable"];
		if (ct.is_array())
		{
			for (int i = 0; i < ct.size(); i++)
			{
				json it = ct[i];
				if (it.find("enabled") != it.end()) {
					info.colorTable[i].Enabled = it["enabled"].get<PF_Boolean>();
				}
				if (it.find("src") != it.end()) {
					info.colorTable[i].Src = JsonToPixel(it["src"]);
				}
				if (it.find("dst") != it.end()) {
					info.colorTable[i].Dst = JsonToPixel(it["dst"]);
					info.colorTable[i].Dst16 = NF_Pixel8To16(info.colorTable[i].Dst);
					info.colorTable[i].Dst32 = NF_Pixel8To32(info.colorTable[i].Dst);
				}

			}
		}
	}
	return info;
}

PF_Err ColorChange::ParamsSet(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	
	ERR(SetFLOAT(ID_LEVEL, infoP->level));
	for (int i = 0; i < COLOR_MAX; i++)
	{
		int idx = ID_CTABLE(i);
		ERR(SetCHECKBOX(idx, infoP->colorTable[i].Enabled));
		ERR(SetCOLOR(idx + 1, infoP->colorTable[i].Src));
		ERR(SetCOLOR(idx + 2, infoP->colorTable[i].Dst));
	}
	return err;
}
// **********************************************************
A_long ColorChange::ChkTableSub(ParamInfo* infoP, A_long idx)
{
	A_long ret = 0;
	if (idx >= COLOR_MAX - 1) return -1;
	if (infoP->colorTable[idx].Enabled == FALSE) return -1;
	for(int i = idx+1; i < COLOR_MAX; i++)
	{
		if (infoP->colorTable[i].Enabled == FALSE) continue;
		if (compPix8(infoP->colorTable[idx].Src, infoP->colorTable[i].Src) == TRUE)
		{
			infoP->colorTable[i].Enabled = FALSE;
		}
		if (infoP->colorTable[i].Enabled)
		{
			ret++;
		}
	}
	return ret;
}
void ColorChange::ChkParams(ParamInfo* infoP)
{
	for (int i = 0; i < COLOR_MAX; i++)
	{
		int cnt = ChkTableSub(infoP, i);
		if (cnt == 0) break;
	}

}
PF_Err ColorChange::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;

	ERR(GetCHECKBOX(ID_ENABLED, &infoP->enabled));
	ERR(GetCHECKBOX(ID_DispTable, &infoP->dispCTable));
	ERR(GetCHECKBOX(ID_DispUse, &infoP->dispUseColor));
	ERR(GetFLOAT(ID_LEVEL, &infoP->level));
	if (!err)
	{
		infoP->lv_byte = RoundByteDouble(PF_MAX_CHAN8 * infoP->level);
	}

	for (int i = 0; i < COLOR_MAX; i++)
	{
		int idx = ID_CTABLE(i);
		ERR(GetCHECKBOX(idx, &infoP->colorTable[i].Enabled));
		ERR(GetCOLOR(idx+1 , &infoP->colorTable[i].Src));
		ERR(GetCOLOR(idx+2 , &infoP->colorTable[i].Dst));
		if (!err)
		{
			infoP->colorTable[i].Dst16 = NF_Pixel8To16(infoP->colorTable[i].Dst);
			infoP->colorTable[i].Dst32 = NF_Pixel8To32(infoP->colorTable[i].Dst);
		}
		if (compPix8(infoP->colorTable[i].Src, infoP->colorTable[i].Dst) == TRUE)
		{
			infoP->colorTable[i].Enabled = FALSE;
		}

	}
	ChkParams(infoP);
	infoP->count = 0;
	for (int i = 0; i < COLOR_MAX; i++)
	{
		if(infoP->colorTable[i].Enabled) infoP->count++;
	}
	return err;
};
// **********************************************************
PF_Err ColorChange::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	NFWorld* src = new NFWorld(input, in_data, pixelFormat());
	NFWorld* dst = new NFWorld(output, in_data, pixelFormat());

	//PF_FILL(NULL, NULL, dst->world);

	dst->Copy(src);
	if ((infoP->enabled == TRUE)&&(infoP->count>0))
	{
		switch (pixelFormat())
		{
		case PF_PixelFormat_ARGB128:
			iterate32(src->world, (void*)infoP, ColChange32, dst->world);
			break;
		case PF_PixelFormat_ARGB64:
			iterate16(src->world, (void*)infoP, ColChange16, dst->world);
			break;
		case PF_PixelFormat_ARGB32:
			iterate8(src->world, (void*)infoP, ColChange8, dst->world);
			break;
		default:
			break;
		}
	}
	int sz = 50;
	PF_Pixel c = { 255,128,128,128 };
	PF_Rect rct;
	if (infoP->dispCTable==TRUE)
	{
		rct.left = sz;
		rct.top = sz;
		rct.right = rct.left + sz * 3;
		rct.bottom = rct.top + sz * COLOR_MAX;
		dst->Fill(rct, c);
		PF_Pixel con = { 255,255,255,255 };
		PF_Pixel coff = { 255,0,0,0 };
		for (int i = 0; i < COLOR_MAX; i++)
		{
			rct.left = sz+3;
			rct.right = rct.left + sz - 6;
			rct.top = sz*i + sz + 3;
			rct.bottom = rct.top + sz - 6;
			if (infoP->colorTable[i].Enabled == TRUE) {
				dst->Fill(rct, con);
			}
			else {
				dst->Fill(rct, coff);
			}
			rct.left += sz;
			rct.right += sz;
			dst->Fill(rct, infoP->colorTable[i].Src);
			rct.left += sz;
			rct.right += sz;
			dst->Fill(rct, infoP->colorTable[i].Dst);
		}
	}
	if (infoP->dispUseColor == TRUE)
	{
		std::vector<PixelCountInfo> pc = src->Histogram();
		if (pc.size() > 0)
		{
			A_long cnt = pc.size();
			if (cnt > 16 * 4) cnt = 16 * 4;

			for (int i = 0; i < cnt; i++)
			{
				rct.left = sz * (i / 16) + sz * 4;
				rct.right = rct.left + sz;
				rct.top = sz * (i % 16) + sz;
				rct.bottom = rct.top + sz;
				dst->Fill(rct, pc[i].col);

			}
		}
	}
	delete src;
	delete dst;

	return err;

};
// **********************************************************
