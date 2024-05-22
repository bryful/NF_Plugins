#include "AAA.h"


// **********************************************************
PF_Err AAA::ParamsSetup(
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

	def.flags |= PF_ParamFlag_SUPERVISE;
	PF_ADD_BUTTON("Button1",
		"Button1-1",
		0,
		PF_ParamFlag_SUPERVISE,
		ID_BTN1);

	AEFX_CLR_STRUCT(def);
	def.flags |= PF_ParamFlag_SUPERVISE;
	PF_ADD_BUTTON("Button2",
		"Button2-1",
		0,
		PF_ParamFlag_SUPERVISE,
		ID_BTN2);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER("add",
		0,
		100,
		0,
		100,
		28,
		ID_ADD);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags |= PF_ParamFlag_EXCLUDE_FROM_HAVE_INPUTS_CHANGED;
	PF_ADD_COLOR("TargetColor",
		0xFF,
		0xFF,
		0xFF,
		ID_COLOR
	);
					
//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err AAA::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	ERR(GetADD(ID_ADD, &infoP->AddValue));
	ERR(GetCOLOR(ID_COLOR, &infoP->Color));
	return err;
};
// **********************************************************
PF_Err AAA::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	NFWorld* src = new NFWorld(input, in_data, pixelFormat());
	NFWorld* dst = new NFWorld(output, in_data, pixelFormat());

	dst->Copy(src);



	delete src;
	delete dst;

	return err;

};
// **********************************************************
