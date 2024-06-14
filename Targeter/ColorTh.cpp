#include "TargetWork.h"


PF_Err ColorTh(TargetWorkInfo* infoP, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	switch (dst->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(AlphaThSub32(infoP, dst));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(AlphaThSub16(infoP, dst));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(AlphaThSub8(infoP, dst));
		break;
	default:
		break;
	}
	return err;

}