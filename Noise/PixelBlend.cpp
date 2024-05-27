#include "Noise.h"

static PF_Err
Blend8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	if (outP->alpha > 0)
	{
		PF_Pixel col = infoP->blendColor;
		col.alpha = RoundByteFpLong(PF_MAX_CHAN8 * infoP->blendOpacity);

		switch (infoP->blendMode)
		{
		case 5:
			*outP = PixelBlendOverlay8(*inP, col);
			break;
		case 4:
			*outP = PixelBlendScreen8(*inP, col);
			break;
		case 3:
			*outP = PixelBlendMultiply8(*inP, col);
			break;
		case 2:
			*outP = PixelBlendAdd8(*inP, col);
			break;
		case 1:
		default:
			*outP = PixelBlend8(*inP, col);
			break;
		}

	}
	else {
		*outP = *inP;
	}

	return err;
}
static PF_Err
Blend16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);
	if (outP->alpha > 0)
	{
		PF_Pixel16 col = NF_Pixel8To16( infoP->blendColor);
		col.alpha = RoundShortFpLong(PF_MAX_CHAN16 * infoP->blendOpacity);

		switch (infoP->blendMode)
		{
		case 5:
			*outP = PixelBlendOverlay16(*inP, col);
			break;
		case 4:
			*outP = PixelBlendScreen16(*inP, col);
			break;
		case 3:
			*outP = PixelBlendMultiply16(*inP, col);
			break;
		case 2:
			*outP = PixelBlendAdd16(*inP, col);
			break;
		case 1:
		default:
			*outP = PixelBlend16(*inP, col);
			break;
		}
	}
	else {
		*outP = *inP;
	}
	return err;
}
static PF_Err
Blend32(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel32* inP,
	PF_Pixel32* outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	if (outP->alpha > 0)
	{
		PF_Pixel32 col = NF_Pixel8To32(infoP->blendColor);
		col.alpha = RoundFpShortDouble(infoP->blendOpacity);

		switch (infoP->blendMode)
		{
		case 5:
			*outP = PixelBlendOverlay32(*inP, col);
			break;
		case 4:
			*outP = PixelBlendScreen32(*inP, col);
			break;
		case 3:
			*outP = PixelBlendMultiply32(*inP, col);
			break;
		case 2:
			*outP = PixelBlendAdd32(*inP, col);
			break;
		case 1:
		default:
			*outP = PixelBlend32(*inP, col);
			break;
		}

	}
	else {
		*outP = *inP;
	}
	return err;
}
PF_Err Noise::BlendExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err err = PF_Err_NONE;
	switch (pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		iterate32(src->world, (void*)infoP, Blend32, dst->world);
		break;
	case PF_PixelFormat_ARGB64:
		iterate16(src->world, (void*)infoP, Blend16, dst->world);
		break;
	case PF_PixelFormat_ARGB32:
		iterate8(src->world, (void*)infoP, Blend8, dst->world);
		break;
	default:
		break;
	}
	return err;
}