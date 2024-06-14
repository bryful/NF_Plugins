#pragma once
#ifndef TARGETWORK_H
#define TARGETWORK_H

#include "AEInfo.h"
#include "NFWorld.h"

typedef struct TargetWorkInfo {
	PF_Pixel	colTbl[6];
	A_long		colTblCount;

} TargetWorkInfo, * TargetWorkInfoP, ** TargetWorkInfoH;

PF_Err AlphaTh(TargetWorkInfo* infoP, NFWorld* dst);
PF_Err ColorTh(TargetWorkInfo* infoP, NFWorld* dst);




#endif





