#pragma once
#ifndef ColorSelect_H
#define ColorSelect_H

#include "../_NFLib/AE_SDK.h"
#include "../_NFLib/AEInfo.h"
#include "../_NFLib/NFWorld.h"
#include "../_NFLib/NFLibVersion.h"
#include "../_NFLib/NFBlend.h"
#include "../_NFLib/NFJson.h"

#include "../_NFLib/tinyfiledialogs.h"
#include <string>
#include <vector>

#include "NF_Target.h"

//UIのパラメータ
typedef struct ColorSelectInfo {
	PF_Boolean	Enabled;
	PF_Pixel	Src;
} ColorSelectInfo, * ColorSelectInfoP, ** ColorSelectInfoH;

#define COLOR_MAX 16
typedef struct ParamInfo {
	PF_Boolean	enabled;
	PF_Boolean	dispCTable;
	PF_Boolean	dispUseColor;
	PF_FpLong	level;
	A_u_char	lv_byte;
	PF_Boolean	rev;
	A_long		count;
	ColorSelectInfo colorTable[COLOR_MAX];
} ParamInfo, * ParamInfoP, ** ParamInfoH;

//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer
	ID_TOPIC_IO,
	ID_DispTable,
	ID_DispUse,
	ID_BTN_SAVE,
	ID_BTN_LOAD,
	ID_TOPIC_IO_END,
	ID_TOPIC_CTABLE,
	ID_ENABLED,
	ID_LEVEL,
	ID_REV,
	ID_CTABLE_ST //8
};
#define ID_CTABLE(idx) (ID_CTABLE_ST + idx*2 )
#define ID_TOPIC_CTABLE_END (ID_CTABLE_ST + COLOR_MAX*2)
#define ID_NUM_PARAMS  (ID_TOPIC_CTABLE_END+1)

// 関数定義
static PF_Err
ColSelect8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP);
static PF_Err
ColSelect16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP);
static PF_Err
ColSelect32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel32* inP,
	PF_Pixel32* outP);

static std::string directoryPath;
//-------------------------------------------------------
class ColorSelect : public AEInfo
{
public:
	// ******************************************************

	// ******************************************************
	A_long ChkTableSub(ParamInfo* infoP, A_long idx);
	void ChkParams(ParamInfo* infoP);
	PF_Err GetParams(ParamInfo *infoP);
	ParamInfo JsonToParams(json jsn);
	json ParamsToJson(ParamInfo* infoP);
	PF_Err ParamsSet(ParamInfo* infoP);
	PF_Err Exec(ParamInfo* infoP);
	PF_Err ParamsSetup(
		PF_InData* in_dataP,
		PF_OutData* out_dataP,
		PF_ParamDef* paramsP[],
		PF_LayerDef* outputP) override;
	//PF_Err TargetExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst);
	//PF_Err BlendExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst);
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
	std::string OpenJsonFileDialog(std::string title,std::string defp )
	{
		const char* filterPatterns[] = { "*,csj" };
		const char* selectedFile = tinyfd_openFileDialog(
			title.c_str(),                      // ダイアログのタイトル
			defp.c_str(),                       // 初期ディレクトリ
			1,                          // フィルタパターンの数
			filterPatterns,             // フィルタパターン
			"Csj files", // フィルタの説明
			0                           // マルチセレクトの可否 (0 = No, 1 = Yes)
		);
		std::string ret;
		if (selectedFile)
		{
			ret = std::string(selectedFile);
		}

		return ret;
	}
	std::string SaveJsonFileDialog(std::string title, std::string defp)
	{
		const char* filterPatterns[] = { "*,csj" };
		const char* selectedFile = tinyfd_saveFileDialog(
			title.c_str(),                      // ダイアログのタイトル
			defp.c_str(),                       // 初期ディレクトリ
			1,                          // フィルタパターンの数
			filterPatterns,             // フィルタパターン
			"Csj files" // フィルタの説明
		);
		std::string ret;
		if (selectedFile)
		{
			ret = std::string(selectedFile);
		}

		return ret;
	}
};

#endif // ColorSelect_H
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
