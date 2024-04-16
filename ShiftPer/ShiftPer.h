
#pragma once
#ifndef ShiftPer_H
#define ShiftPer_H

#include "NF_Target.h"


#include "AEConfig.h"
#include "entry.h"
#include "AEFX_SuiteHelper.h"
#include "PrSDKAESupport.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_EffectCBSuites.h"
#include "AE_Macros.h"
#include "AEGP_SuiteHandler.h"
#include "String_Utils.h"
#include "Param_Utils.h"
#include "Smart_Utils.h"

#ifdef AE_OS_WIN
	#include <Windows.h>
#endif


enum {
	SFTP_INPUT = 0,
	SFTP_X,
	SFTP_Y,
	SFTP_NUM_PARAMS
};

typedef struct SFTPInfo {
	PF_FpLong	shiftX;
	PF_FpLong	shiftY;
} SFTPInfo, * SFTPInfoP, ** SFTPInfoH;



extern "C" {

	DllExport 
	PF_Err
	EffectMain (	
		PF_Cmd			cmd,
		PF_InData		*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output,
		void			*extra);

}


#endif // ShiftPer_H