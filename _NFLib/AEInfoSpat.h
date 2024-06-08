#pragma once
#ifndef AEINFOSPAT_H
#define AEINFOSPAT_H
#include "AE_SDK.h"
#include "AEInfo.h"
#include "NFWorld.h"
#include "NFUtils.h"
#include "NFLibVersion.h"
#include "Tex.h"


#define SPAT_BUF_MAX 256
typedef struct SpatInfo {
	PF_EffectWorld worlds[SPAT_BUF_MAX];
	A_long		count;
	A_long		width;
	A_long		height;
} SpatInfo, * SpatInfoP, ** SpatInfoH;


class AEInfoSpat : public AEInfo
{
public:

	PF_Err GetSpatFromLayer(A_long idx, A_long txsize,SpatInfo * infoP);
	PF_Err DeleteSpat(SpatInfo* infoP);
	PF_FpLong RandomD(PF_FpLong mx, PF_FpLong rnd)
	{
		PF_FpLong ret = 1;
		if (rnd <= 0) {
			xorShiftDouble();
			return mx;
		}
		PF_FpLong v = 1;

		if (mx >= 1)
		{
			v = xorShiftDouble() * rnd;
			ret = mx - v;
		}
		else {
			v = ( - 1 + 2 * xorShiftDouble()) * rnd;
			ret = mx + v;
		}
		if (ret < 0) ret = 0;
		else if (ret > 1) ret = 1;

		return ret;
	}
	// ******************************************************

};
#endif