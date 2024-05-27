#pragma once
#ifndef NoiseHorizon_H
#define NoiseHorizon_H

#include "../_NFLib/AE_SDK.h"
#include "../_NFLib/AEInfo.h"
#include "../_NFLib/NFWorld.h"
#include "../_NFLib/NFLibVersion.h"
#include "../_NFLib/NFBlend.h"

#include <string>

#include "NF_Target.h"

typedef struct RGBShiftInfo {
	A_long		seed;
	A_long		frame;
	A_long		RShift;
	A_long		GShift;
	A_long		BShift;
	NFWorld* nfworld;
} RGBShift, * RGBShiftP, ** RGBShiftH;
typedef struct RShiftInfo {
	A_long		seed;
	A_long		frame;
	A_long		value;
	A_long		XShift;
} RShiftInfo, * RShiftInfoP, ** RShiftInfoH;
typedef struct RandomLineInfo {
	A_long		seed;
	A_long		frame;
	A_long		value;
	A_long		value2;
	PF_FpLong	Opacity;
} RandomLineInfo, * RandomLineInfoP, ** RandomLineInfoH;

//UIのパラメータ
typedef struct ParamInfo {
	PF_Boolean	moving;
	A_long		frame;
	RGBShiftInfo	rgbs;
	RShiftInfo		rs;
	A_long		swapline_seed;
	A_long		swapValue;
	A_long		swapHeight;

	A_long		noise_seed;
	PF_FpLong	noisevalue;
	A_long		noiseLength;
	PF_FpLong	noiseStrong;
	RandomLineInfo	rl;
	NFWorld*	nfworld;


} ParamInfo, * ParamInfoP, ** ParamInfoH;

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer
	ID_MOVING,
	ID_GLOBAL_VALUE,

	ID_TOPIC_RGBS,
	ID_RGBS_SEED,
	ID_RGBS_VALUE,
	ID_RGBS_R,
	ID_RGBS_G,
	ID_RGBS_B,
	ID_TOPIC_RGBS_END,

	ID_TOPIC_RS,
	ID_RS_SEED,
	ID_RS_VALUE,
	ID_RS_SHIFT,
	ID_TOPIC_RS_END,


	ID_TOPIC_SWAPLINE,
	ID_SWAPLINE_SEED,
	ID_SWAPVALUE,
	ID_SWAPHEIGHT,
	ID_TOPIC_SWAPLINE_END,

	ID_TOPIC_NOISE,
	ID_NOISE_SEED,
	ID_NOISEVALUE,
	ID_NOISELENGTH,
	ID_NOISESTRONG,
	ID_TOPIC_NOISE_END,

	ID_TOPIC_RL,
	ID_RL_SEED,
	ID_RL_VALUE,
	ID_RL_VALUE2,
	ID_RL_OPACITY,

	ID_TOPIC_RL_END,



	ID_NUM_PARAMS
};

// 関数定義
PF_Err NoiseHor8(ParamInfo* infoP);
PF_Err SwapLine8(ParamInfo* infoP);
//-------------------------------------------------------
class NoiseHorizon : public AEInfo
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
	//PF_Err TargetExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst);
	//PF_Err BlendExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst);
	PF_Err RGBShiftExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst);
	PF_Err RandomShiftExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst);
	PF_Err RandomLineExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst);

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
	PF_Err QueryDynamicFlags(
		PF_InData* in_dataP,
		PF_OutData* out_dataP,
		PF_ParamDef* paramsP[],
		PF_LayerDef* outputP,
		PF_UserChangedParamExtra* extraP,
		A_long pc
	)override;

};

#endif // NoiseHorizon_H
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
