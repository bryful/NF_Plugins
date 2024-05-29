#include "NoiseHorizon.h"

PF_Err NoiseHorizon::RandomLineExec(ParamInfo* infoP, NFWorld* src, NFWorld* dst)
{
	PF_Err	err = PF_Err_NONE;
	if (infoP->rl.value <= 0) return err;
	if (infoP->moving == TRUE)
	{
		F_SRAND(infoP->rl.seed + infoP->frame);
	}
	else {
		F_SRAND(infoP->rl.seed);
	}
	A_long cnt = F_RANDR(1, infoP->rl.value+1);
	A_long cnt2 = F_RANDR(1, infoP->rl.value2+1);

	int w = dst->width()-1;
	int h = dst->height()-1;
	int h2 = h / 4;
	for (int i = 0; i < cnt; i++)
	{
		int y0 = F_RANDR(0, h);
		for (int j = 0; j < cnt2; j++)
		{
			int y1 = F_RANDR(-h2, h2) + y0;
			if (y1 < 0) y1 = 0;
			else if (y1 > h) y1 = h;

			int xx = F_RANDR(0, w);
			PF_Pixel col = dst->GetPix(xx, y1);
			A_long r = 0;
			A_long g = 0;
			A_long b = 0;
			if (col.alpha == 0)
			{
				r = g= b = F_RANDR(0, PF_MAX_CHAN8);
			}else{
				A_long ad = F_RANDR(-32, 32);
				r = ((A_long)col.red * (A_long)col.alpha / PF_MAX_CHAN8) + ad;
				g = ((A_long)col.green * (A_long)col.alpha / PF_MAX_CHAN8) + ad;
				b = ((A_long)col.blue * (A_long)col.alpha / PF_MAX_CHAN8) + ad;

			}
			col.alpha = PF_MAX_CHAN8;
			col.red = RoundByteLong(r);
			col.green = RoundByteLong(g);
			col.blue = RoundByteLong(b);


			PF_Rect rct;
			rct.left = 0;
			rct.right = w;
			rct.top = y1;
			rct.bottom = y1 + 1;
			
			PF_FILL(&col, &rct, dst->world);

		}
	}
	return err;

}


