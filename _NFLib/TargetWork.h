#pragma once
#ifndef TARGETWORK_H
#define TARGETWORK_H

#include "AEInfo.h"
#include "NFWorld.h"

typedef struct TargetWorkInfo {
	A_long		targetMode;
	PF_Pixel	targetColor;
	A_long		blendMode;
	PF_Pixel	blendColor;
	PF_FpLong	blendOpacity;
	PF_FixedPoint	pointValue;

} TargetWorkInfo, * TargetWorkInfoP, ** TargetWorkInfoH;

PF_Err AlphaTh(TargetWorkInfo* infoP, NFWorld* dst);




#endif





