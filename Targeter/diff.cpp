#include "Targeter.h"

inline A_u_char VValue8(PF_Pixel c)
{
	PF_FpLong a = (PF_FpLong)c.alpha / PF_MAX_CHAN8;
	PF_FpLong v = (((PF_FpLong)c.red * a) + ((PF_FpLong)c.green * a) + ((PF_FpLong)c.blue * a)) / 3;
	return RoundByteFpLong(v);

}

PF_Err diff8(NFWorld* src, NFWorld* dst8)
{
	PF_Err err = PF_Err_NONE;

	A_long adrS = 0;
	A_long adrD = 0;
	for (int y = 0; y < src->height(); y++)
	{
		for (int x = 0; x < src->width(); x++)
		{
			A_u_char v = 0;
			A_u_char v1 = 0;
			A_long xx = x - 1;
			A_long yy = y - 1;

			A_u_char vb = VValue8(src->GetPix8(x, y));


			if (yy >= 0)
			{
				xx = x - 1;
				if (xx >= 0)
				{
					v1 = VValue8(src->GetPix8(xx, yy));
					v1 = ABS(v1 - vb);
					if (v < v1) v = v1;
				}
				xx = x;
				v1 = VValue8(src->GetPix8(xx, yy));
				v1 = ABS(v1 - vb);
				if (v < v1) v = v1;
				xx = x + 1;
				if (xx < src->width())
				{
					v1 = VValue8(src->GetPix8(xx, yy));
					v1 = ABS(v1 - vb);
					if (v < v1) v = v1;
				}
			}
			yy = y;
			xx = x - 1;
			if (xx >= 0)
			{
				v1 = VValue8(src->GetPix8(xx, yy));
				v1 = ABS(v1 - vb);
				if (v < v1) v = v1;
			}
			xx = x + 1;
			if (xx < src->width())
			{
				v1 = VValue8(src->GetPix8(xx, yy));
				v1 = ABS(v1 - vb);
				if (v < v1) v = v1;
			}
			yy = y + 1;
			if (yy < src->height())
			{
				xx = x - 1;
				if (xx >= 0)
				{
					v1 = VValue8(src->GetPix8(xx, yy));
					v1 = ABS(v1 - vb);
					if (v < v1) v = v1;
				}
				xx = x;
				v1 = VValue8(src->GetPix8(xx, yy));
				v1 = ABS(v1 - vb);
				if (v < v1) v = v1;
				xx = x + 1;
				if (xx < src->width())
				{
					v1 = VValue8(src->GetPix8(xx, yy));
					v1 = ABS(v1 - vb);
					if (v < v1) v = v1;
				}
			}
			PF_Pixel c = { PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8 };
			c.alpha = v;
			dst8->SetPix8(x, y, c);
		}
	}
	return err;
}



PF_Err diff(NFWorld* src, NFWorld* dst8)
{
	PF_Err err = PF_Err_NONE;
	switch (src->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		//ERR(AlphaThSub32(infoP, dst));
		break;
	case PF_PixelFormat_ARGB64:
		//ERR(AlphaThSub16(infoP, dst));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(diff8(src, dst8));
		break;
	default:
		break;
	}
	return err;
}