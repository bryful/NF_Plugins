// NF-Target.h
#pragma once
#ifndef NF_TARGET_H
#define NF_TARGET_H
//-----------------------------------------------------------------------------------
//プラグインの識別に使われる名前
#define NF_NAME			"NF-ShiftPer"
#define NF_MATCHNAME	"NF-ShiftPer"

//-----------------------------------------------------------------------------------
//プラグインの説明に使われる文字
#define NF_DESCRIPTION	"Shift the screen by a specified percentage of the width."

//-----------------------------------------------------------------------------------
//プラグインが表示されるメニュー名
#define NF_CATEGORY "NF-Plugins(Beta)"

//-----------------------------------------------------------------------------------
// バージョン
//バージョンを買えたAlphaFixPiPL.rのAE_Effect_Versionも変えること
#define	MAJOR_VERSION		1
#define	MINOR_VERSION		0
#define	BUG_VERSION			0
#define	STAGE_VERSION		PF_Stage_DEVELOP
//#define	STAGE_VERSION		PF_Stage_ALPHA
//#define	STAGE_VERSION		PF_Stage_BETA
//#define	STAGE_VERSION		PF_Stage_RELEASE
#define	BUILD_VERSION		0

//上の値を計算した結果
#define NF_VERSION	524288
//-----------------------------------------------------------------------------------
//out_flags
/*
out_data->out_flags
	PF_OutFlag_PIX_INDEPENDENT		1024
	PF_OutFlag_NON_PARAM_VARY		4
	PF_OutFlag_DEEP_COLOR_AWARE		33554432
	PF_OutFlag_USE_OUTPUT_EXTENT	64
	PF_OutFlag_I_EXPAND_BUFFER		512
	PF_OutFlag_I_DO_DIALOG			32
*/

//#define NF_OUT_FLAGS	33556032	//通常はこちら
//#define NF_OUT_FLAGS	33556036	//こっちを有効にすると毎フレームごとに描画する。NON_PARAM_VARYを動作中に切り替えるときもこちらに
#define NF_OUT_FLAGS	1600		//8bitのみ

//-----------------------------------------------------------------------------------
//out_flags2
/*
	out_data->out_flags2
	PF_OutFlag2_FLOAT_COLOR_AWARE
	PF_OutFlag2_PARAM_GROUP_START_COLLAPSED_FLAG
	PF_OutFlag2_SUPPORTS_SMART_RENDER
	PF_OutFlag2_SUPPORTS_QUERY_DYNAMIC_FLAGS
	PF_OutFlag2_DOESNT_NEED_EMPTY_PIXELS;
*/
//#define NF_OUT_FLAGS2 134222921
#define NF_OUT_FLAGS2 0

//-----------------------------------------------------------------------------------
#endif