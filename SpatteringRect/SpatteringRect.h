#pragma once
#ifndef SpatteringRect_H
#define SpatteringRect_H

#include "../_NFLib/AE_SDK.h"
#include "../_NFLib/AEInfo.h"
#include "../_NFLib/NFWorld.h"
#include "../_NFLib/NFLibVersion.h"
#include "../_NFLib/NFUtils.h"
#include <string>
#include <vector>

#include "../_NFLib/AEInfoSpat.h"

#include "NF_Target.h"

//UIのパラメータ


typedef struct ParamInfo {
	A_long		seed; 	
	A_long		value;
	PF_FpLong	opacity;
	PF_FpLong	opacityRandom;
	PF_FpLong	scale;
	PF_FpLong	scaleRandom;
	PF_Rect		rect;
	A_long		textSize;
	PF_Boolean	isCopyOrigin;
	PF_Boolean	moving;
	SpatInfo	spat;

} ParamInfo, * ParamInfoP, ** ParamInfoH;

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer
	ID_VERSION,
	ID_TOPIC_TEX,
	ID_LAYER,
	ID_TEXSIZE,
	ID_OPACITY,
	ID_OPACITYRANDOM,
	ID_SCALE,
	ID_SCALERANDOM,
	ID_TOPIC_TEX_END,

	ID_VALUE,
	ID_TOPLEFT,
	ID_BOTTOMRIGHT,
	ID_SEED,
	ID_MOVING,
	ID_COPYTOORIGIN,
	ID_NUM_PARAMS
};

// 関数定義


//-------------------------------------------------------
class SpatteringPoint : public AEInfoSpat
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
	PF_Err QueryDynamicFlags(
		PF_InData* in_dataP,
		PF_OutData* out_dataP,
		PF_ParamDef* paramsP[],
		PF_LayerDef* outputP,
		PF_UserChangedParamExtra* extraP,
		A_long pc
	) override;

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
	PF_Err	UserChangedParam(
		PF_InData* in_dataP,
		PF_OutData* out_dataP,
		PF_ParamDef* paramsP[],
		PF_LayerDef* outputP,
		PF_UserChangedParamExtra* extraP,
		A_long pc)override;
	
};

#endif // SpatteringRect_H
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
			const char* inHostVersion);

	DllExport
		PF_Err
		EffectMain(
			PF_Cmd			cmd,
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_ParamDef* paramsP[],
			PF_LayerDef* outputP,
			void* extraP);

}
#endif
