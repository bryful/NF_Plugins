#pragma once
#ifndef AAA_H
#define AAA_H

#include "../_NFLib/AE_SDK.h"
#include "../_NFLib/AEInfo.h"
#include "../_NFLib/NFWorld.h"
#include "../_NFLib/NFLibVersion.h"
#include "../_NFLib/NFBlend.h"
#include "NF_Target.h"


//UIのパラメータ
typedef struct ParamInfo {
	A_long		AddValue;
	PF_Pixel	Color;
} ParamInfo, * ParamInfoP, ** ParamInfoH;

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer
	ID_BTN1,
	ID_BTN2,
	ID_ADD,
	ID_COLOR,
	ID_NUM_PARAMS
};

// 関数定義
//-------------------------------------------------------
class AAA : public AEInfo
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
	
	PF_Err TargetExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst);
	PF_Err BlendExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst);

	// ******************************************************
	PF_Err UpdateParamsUI(
		PF_InData* in_dataP,
		PF_OutData* out_dataP,
		PF_ParamDef* paramsP[],
		PF_LayerDef* outputP)
	{
		PF_Err err = PF_Err_NONE;
		PF_InData* in_data = in_dataP;
		GetSuites(in_dataP);
		// ボタンが押されたかどうかをチェック
		if (paramsP[ID_BTN1]->u.button_d.value) {
			// スライダーパラメータを変更
			PF_ParamDef def;
			AEFX_CLR_STRUCT(def);
			def.u.fs_d.value = 75.0; // 新しい値に設定

			// パラメータを設定

			suitesP->ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, ID_ADD, &def);

			// ボタンの状態をリセット
			AEFX_CLR_STRUCT(def);
			def.u.button_d.value = 0; // リセット
			suitesP->ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, ID_BTN1, &def);
		}
		return err;
	}
	// ******************************************************
	PF_Err
		UserChangedParam(
			PF_InData* in_dataP,
			PF_OutData* out_dataP,
			PF_ParamDef* paramsP[],
			PF_LayerDef* outputP,
			PF_UserChangedParamExtra* extraP)
	{
		PF_Err err = PF_Err_NONE;
		Init();
		m_cmd = PF_Cmd_USER_CHANGED_PARAM;
		in_data = in_dataP;
		out_data = out_dataP;
		output = outputP;
		GetFrame(in_dataP);
		GetSuites(in_dataP);

		if (extraP->param_index == ID_BTN1)
		{
			out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE;
			
			paramsP[ID_ADD]->u.sd.value = 75;
			paramsP[ID_ADD]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
		}
		else if (extraP->param_index == ID_BTN2)
		{

			paramsP[ID_COLOR]->u.cd.value.red = 75;
			paramsP[ID_COLOR]->u.cd.value.green = 128;
			paramsP[ID_COLOR]->u.cd.value.blue = 36;
			paramsP[ID_COLOR]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;

		}
		return err;
	}
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

};

#endif // AAA_H
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
