#pragma once
#ifndef Skelton_H
#define Skelton_H

#include "../_NFLib/AE_SDK.h"
#include "../_NFLib/AEInfo.h"
#include "../_NFLib/NFWorld.h"
#include "../_NFLib/NFLibVersion.h"


#include "NF_Target.h"



//UIのパラメータ
typedef struct ParamInfo {
	NFWorld*	inWld;
	NFWorld*	outWld;
	PF_FpLong	shiftX;
	PF_FpLong	shiftY;
	A_long		shiftXPx;
	A_long		shiftYPx;

	PF_Pixel	color;
	A_long		intValue;
	A_Fixed		fixedValue;
	PF_FpLong	floatvalue;
	PF_Boolean	check;
	A_Fixed		rotValue;
	A_long		popupValue;
	PF_FixedPoint	pointValue;
	Point3D		point3DValue;
	
} ParamInfo, * ParamInfoP, ** ParamInfoH;

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer
	ID_SHIFTX,
	ID_SHIFTY,
	ID_TOPIC,
	ID_COLOR,
	ID_INT,
	ID_FIXED,
	ID_FLOAT,
	ID_CHECK,
	ID_ANGLE,
	ID_POPUP,
	ID_POINT,
	ID_POINT3D,
	ID_TOPICEND,
	ID_NUM_PARAMS
};


//-------------------------------------------------------
class Skelton : public AEInfo
{
public:
	// ******************************************************
	PF_Err GetParams(ParamInfo *infoP);
	PF_Err Exec(ParamInfo* infoP);
	PF_Err ParamsSetup(
		PF_InData* in_dataP,
		PF_OutData* out_dataP,
		PF_ParamDef* paramsP[],
		PF_LayerDef* outputP) override;
	// ******************************************************
	PF_Err	About(
		PF_InData* in_dataP,
		PF_OutData* out_dataP,
		PF_ParamDef* paramsP[],
		PF_LayerDef* outputP) override
	{
		PF_Err err = PF_Err_NONE;
		Init();
		m_cmd = PF_Cmd_ABOUT;
		in_data = in_dataP;
		out_data = out_dataP;

		GetFrame(in_dataP);
		GetSuites(in_dataP);

		A_char MJV[64] = { '\0' };
		A_char MNV[64] = { '\0' };
		PF_SPRINTF(MJV, "%d", MAJOR_VERSION);
		PF_SPRINTF(MNV, "%d", MINOR_VERSION);

		ERR(AboutBox(
			NF_NAME,
			MJV,
			MNV,
			NF_DESCRIPTION));

		return err;

	};
	// ******************************************************
	PF_Err	GlobalSetup(
		PF_InData* in_dataP,
		PF_OutData* out_dataP,
		PF_ParamDef* paramsP[],
		PF_LayerDef* outputP) override
	{
		PF_Err	err = PF_Err_NONE;
		Init();
		m_cmd = PF_Cmd_GLOBAL_SETUP;
		in_data = in_dataP;
		out_data = out_dataP;

		suitesP = new AEGP_SuiteHandler(in_dataP->pica_basicP);

		//Fs_Target.hで定義
		out_dataP->my_version = NF_VERSION;
		out_dataP->out_flags = NF_OUT_FLAGS;
		out_dataP->out_flags2 = NF_OUT_FLAGS2;
		/*
		プラグインIDを獲得して、グローバルにアクセスできるように保存
		*/
		ae_plugin_idH = suitesP->HandleSuite1()->host_new_handle(sizeof(ae_global_data));

		if (ae_plugin_idH) {
			ae_plugin_idP = reinterpret_cast<ae_global_dataP>(suitesP->HandleSuite1()->host_lock_handle(ae_plugin_idH));
			if (ae_plugin_idP) {
				ae_plugin_idP->initializedB = TRUE;

				if (in_data->appl_id != 'PrMr') {
					ERR(suitesP->UtilitySuite3()->AEGP_RegisterWithAEGP(NULL, NF_NAME, &ae_plugin_idP->my_id));
				}
				if (!err) {
					out_data->global_data = ae_plugin_idH;
				}
			}
			suitesP->HandleSuite1()->host_unlock_handle(ae_plugin_idH);
		}
		else {
			err = PF_Err_INTERNAL_STRUCT_DAMAGED;
		}

		return err;
	}
	// ******************************************************
	PF_Err GlobalSetdown(PF_InData* in_dataP) override
	{
		PF_Err	err = PF_Err_NONE;
		Init();
		m_cmd = PF_Cmd_GLOBAL_SETDOWN;
		suitesP = new AEGP_SuiteHandler(in_dataP->pica_basicP);
		in_data = in_dataP;

		//ae_plugin_idH
		if (in_dataP->global_data) {
			suitesP->HandleSuite1()->host_dispose_handle(in_dataP->global_data);
		}

		return err;
	}
	// ******************************************************
	

};

#endif // Skelton_H
#ifndef EFFECT_MAIN_H
#define EFFECT_MAIN_H
//-----------------------------------------------------------------------------------
extern "C" {
	DllExport
		PF_Err PluginDataEntryFunction2(
			PF_PluginDataPtr inPtr,
			PF_PluginDataCB2 inPluginDataCallBackPtr,
			SPBasicSuite* inSPBasicSuitePtr,
			const char* inHostName,
			const char* inHostVersion)
	{
		PF_Err result = PF_Err_INVALID_CALLBACK;

		result = PF_REGISTER_EFFECT_EXT2(
			inPtr,
			inPluginDataCallBackPtr,
			NF_NAME, // Name
			NF_MATCHNAME, // Match Name
			NF_CATEGORY, // Category
			AE_RESERVED_INFO, // Reserved Info
			"EffectMain",	// Entry point
			"https://github.com/bryful");	// support URL

		return result;
	}
	DllExport
		PF_Err
		EffectMain(
			PF_Cmd			cmd,
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_ParamDef* paramsP[],
			PF_LayerDef* outputP,
			void* extraP)

	{
		PF_Err err = PF_Err_NONE;
		PF_Err err2 = PF_Err_NONE;

		try
		{

			Skelton ae;
			ae.in_data = in_dataP;

			switch (cmd) {
			case PF_Cmd_ABOUT:
				err = ae.About(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_GLOBAL_SETUP:
				err = ae.GlobalSetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_GLOBAL_SETDOWN:
				err = ae.GlobalSetdown(in_dataP);
				break;
			case PF_Cmd_PARAMS_SETUP:
				err = ae.ParamsSetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SEQUENCE_SETUP:
				err = ae.SequenceSetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SEQUENCE_SETDOWN:
				err = ae.SequenceSetdown(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SEQUENCE_RESETUP:
				err = ae.SequenceResetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_RENDER:
				err = ae.Render(in_dataP, out_dataP, paramsP, outputP, ID_NUM_PARAMS);
				if (!err)
				{
					ParamInfo info;
					ERR(ae.GetParams(&info));
					ERR(ae.Exec(&info));
				}
				break;
			case PF_Cmd_SMART_PRE_RENDER:
				err = ae.PreRender(in_dataP, out_dataP, reinterpret_cast<PF_PreRenderExtra*>(extraP), ID_NUM_PARAMS, sizeof(ParamInfo));
				if (!err)
				{
					ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
					ERR(ae.SetHostPreRenderData());
					ERR(ae.GetParams(infoP));
					ERR(ae.UnSetPreRenderData());
					ERR(ae.UnlockPreRenderData());

				}
				break;
			case PF_Cmd_SMART_RENDER:
				err = ae.SmartRender(in_dataP, out_dataP, reinterpret_cast<PF_SmartRenderExtra*>(extraP), ID_NUM_PARAMS);
				if (!err) {
					ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
					if (infoP) {
						ERR(ae.Exec(infoP));
						ERR2(ae.UnsetSmartRender());
						ae.UnlockPreRenderData();
					}
					else {
						err = PF_Err_OUT_OF_MEMORY;
					}
				}
				break;
			case PF_Cmd_COMPLETELY_GENERAL:
				err = ae.RespondtoAEGP(in_dataP, out_dataP, paramsP, outputP, extraP);
				break;
			case PF_Cmd_DO_DIALOG:
				//err = PopDialog(in_data,out_data,params,output);
				break;
			case PF_Cmd_USER_CHANGED_PARAM:
				err = ae.HandleChangedParam(in_dataP,
					out_dataP,
					paramsP,
					outputP,
					reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
			case PF_Cmd_QUERY_DYNAMIC_FLAGS:
				err = ae.QueryDynamicFlags(in_dataP,
					out_dataP,
					paramsP,
					reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
			}
		}
		catch (PF_Err& thrown_err) {
			err = thrown_err;
		}
		return err;
	}
}
#endif
