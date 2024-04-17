
#pragma once
#ifndef Skelton_step1_H
#define Skelton_step1_H

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

#define DESCRIPTION	"NF-Skelton_step1 by bry-ful"

#define NAME			"NF-Skelton_step1"
#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	0

enum {
	PARAM_INPUT = 0,
	PARAM_SLIDER,		// default input layer 
	PARAM_NUM_PARAMS
};

enum {
	SLIDER_DISK_ID = 1
};


#define FILTER_NOISE_MIN	0
#define FILTER_NOISE_MAX	1000
#define FILTER_NOISE_DFLT	10
#define SLIDER_MIN			0
#define	SLIDER_MAX			100

#define RESTRICT_BOUNDS			0
#define SLIDER_PRECISION		1
#define DISPLAY_FLAGS			PF_ValueDisplayFlag_PERCENT


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

typedef struct ParamInfo{
	PF_FpLong	valF;
} ParamInfo, *ParamInfoP, **ParamInfoH;

typedef struct {
	A_u_char	blue, green, red, alpha;
} PF_Pixel_BGRA_8u;

typedef struct {
	PF_FpShort	blue, green, red, alpha;
} PF_Pixel_BGRA_32f;


#endif // Skelton_step1_H