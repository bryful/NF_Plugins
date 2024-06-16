#include "EraseLines.h"

typedef struct P3Info8 {
	PF_Pixel	org;
	PF_Pixel	dst;
	PF_Boolean	target;
	PF_FpLong	level;
	PF_Boolean	canUse;
} P3Info8, * P3Info8P, ** P3Info8H;

PF_Err AltCol8(ParamInfo* infoP, NFWorld* src)
{
	PF_Err err = PF_Err_NONE;
	for (int y = 0; y < src->height(); y++)
	{
		for (int x = 0; x < src->width(); x++)
		{
			PF_Pixel c = src->GetPix8(x, y);
			PF_Boolean OK = FALSE;
			for (int j = 0; j < infoP->lineColorsCount; j++)
			{
				if (compPix8(c, infoP->lineColors[j]) == TRUE)
				{
					OK = TRUE;
					break;
				}
			}
			if (OK == TRUE)
			{
				PF_Pixel nc = { PF_MAX_CHAN8 ,PF_MAX_CHAN8 ,PF_MAX_CHAN8 ,PF_MAX_CHAN8 };
				switch (infoP->altMode)
				{
				case 5:
					src->SetPix8(x, y, infoP->altColor); 
					break;
				case 4:
					nc.alpha = nc.red = nc.green = nc.blue = 0;
					src->SetPix8(x, y, nc); 
					break;
				case 3:
					nc.red = nc.green = nc.blue = 0;
					src->SetPix8(x, y, nc); 
					break;
				case 2:
					src->SetPix8(x, y, nc);
					break;
				case 1:
				default:
					break;
				}

			}
		}

	}
	return err;
}
PF_Err CheckDot8(ParamInfo* infoP,NFWorld* src)
{
	PF_Err err = PF_Err_NONE;
	PF_Pixel w = { PF_MAX_CHAN8,PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8 };
	for (int y = 0; y < src->height(); y++)
	{
		//012
		//345
		//678
		for (int x = 0; x < src->width(); x++)
		{
			PF_Pixel c4 = src->GetPix8Z(x, y);
			if (c4.alpha == 0) continue;
			for (int j = 0; j < infoP->lineColorsCount; j++)
			{
				if (compPix8(c4, infoP->lineColors[j]) == TRUE)
				{
					continue;
				}
			}

			PF_Pixel c1 = src->GetPix8Z(x, y-1);
			PF_Pixel c7 = src->GetPix8Z(x, y + 1);
			if ((compPix8(c4, c1) == TRUE)&& (compPix8(c4, c7) == TRUE))continue;

			PF_Pixel c3 = src->GetPix8Z(x-1, y);
			PF_Pixel c5 = src->GetPix8Z(x + 1, y);
			if ((compPix8(c4, c3) == TRUE) && (compPix8(c4, c5) == TRUE))continue;

			if (compPix8(c3, c5) == TRUE)
			{
				if ((compPix8(c3, w) == FALSE) && (c3.alpha != 0))
				{
					src->SetPix8(x, y, c3);
					continue;
				}
			}
			if (compPix8(c1, c7) == TRUE)
			{
				if ((compPix8(c1, w) == FALSE) && (c1.alpha != 0))
				{
					src->SetPix8(x, y, c1);
					continue;
				}
			}
			PF_Pixel c0 = src->GetPix8Z(x-1, y - 1);
			PF_Pixel c8 = src->GetPix8Z(x + 1, y + 1);
			if ((compPix8(c0, c8) == TRUE))
			{
				if ((compPix8(c0, w) == FALSE) && (c0.alpha != 0))
				{
					src->SetPix8(x, y, c0);
					continue;
				}

			}
			PF_Pixel c6 = src->GetPix8Z(x - 1, y + 1);
			PF_Pixel c2 = src->GetPix8Z(x + 1, y - 1);
			if ((compPix8(c6, c2) == TRUE))
			{
				if ((compPix8(c6, w) == FALSE) && (c6.alpha != 0))
				{
					src->SetPix8(x, y, c6);
					continue;
				}

			}
		}
	}
	return err;

}


A_long EraseLinesScanline8(ParamInfo* infoP,P3Info8* tbl, A_long cnt)
{
	A_long ret = 0;

	for (int i = 0; i < cnt; i++)
	{
		PF_Pixel c = tbl[i].org;
		tbl[i].dst = c;
		tbl[i].target = FALSE;
		tbl[i].canUse = FALSE;
		tbl[i].level = 0;
		PF_Boolean OK = FALSE;
		if (c.alpha == 0)
		{
			tbl[i].level = 0;
			tbl[i].org = tbl[i].dst = { 0,0,0,0 };
		}
		else if ((c.red == PF_MAX_CHAN8) && (c.green == PF_MAX_CHAN8) && (c.blue == PF_MAX_CHAN8))
		{
			tbl[i].level = PF_MAX_CHAN8;
		}
		else {

			for (int j = 0; j < infoP->lineColorsCount; j++)
			{
				if (compPix8(c, infoP->lineColors[j]) == TRUE)
				{
					OK = TRUE;
					break;
				}
			}
			if (OK == TRUE)
			{
				tbl[i].target = TRUE;
				
			}
			else
			{
				tbl[i].canUse = TRUE;
				tbl[i].level = (((double)c.red + (double)c.green + (double)c.blue) / 3);
				
			}
		}
	}




	if (tbl[0].target == TRUE)
	{
		if (tbl[1].canUse == TRUE)
		{
			tbl[0].dst = tbl[1].org;
		}
		else {
			ret++;
		}
	}
	for (int i = 1; i < cnt-1; i++)
	{
		if(tbl[i].target==FALSE) continue;
		int idx0 = -1;
		int idx1 = -1;
		if ((tbl[i - 1].canUse == FALSE) && (tbl[i + 1].canUse == FALSE))
		{
			ret++;
			continue;
		}
		else if ((tbl[i - 1].canUse == TRUE) && (tbl[i + 1].canUse == TRUE))
		{
			if (tbl[i - 1].level < tbl[i + 1].level)
			{
				//tbl[i].dst = tbl[i - 1].org;
				idx0 = i - 1;
			}
			else {
				//tbl[i].dst = tbl[i + 1].org;
				idx1 = i + 1;
			}
		}
		else if ((tbl[i - 1].canUse == TRUE) && (tbl[i + 1].canUse == FALSE))
		{
			//tbl[i].dst = tbl[i - 1].org;
			idx0 = i - 1;
			for (int t = 2; t < 20; t++)
			{
				int tt = i + t;
				if (tt >= cnt) break;
				if (tbl[tt].canUse == TRUE)
				{
					idx1 = tt;
					break;
				}
			}
		}
		else if ((tbl[i - 1].canUse == FALSE) && (tbl[i + 1].canUse == TRUE))
		{
			for (int t = 2; t < 20; t++)
			{
				int tt = i - t;
				if (tt <  0) break;
				if (tbl[tt].canUse == TRUE)
				{
					idx0 = tt;
					break;
				}
			}
			idx1 = i + 1;
			//tbl[i].dst = tbl[i + 1].org;
		}
		else {
			ret++;
		}
		if ((idx0 >= 0) && (idx1 >= 0))
		{
			if (infoP->prioritizeDark == TRUE)
			{
				if (tbl[idx0].level <= tbl[idx1].level)
				{
					tbl[i].dst = tbl[idx0].org;
				}
				else {
					tbl[i].dst = tbl[idx1].org;
				}
			}
			else 
			{
				if (tbl[idx0].level >= tbl[idx1].level)
				{
					tbl[i].dst = tbl[idx0].org;
				}
				else {
					tbl[i].dst = tbl[idx1].org;
				}
			}
		}
		else if ((idx0 >= 0) && (idx1 < 0))
		{
			tbl[i].dst = tbl[idx0].org;
		}
		else if ((idx0 < 0) && (idx1 >= 0))
		{
			tbl[i].dst = tbl[idx1].org;
		}
		else {
			ret++;
		}

	}
	if (tbl[cnt-1].target == TRUE)
	{
		if (tbl[cnt-2].canUse == TRUE)
		{
			tbl[cnt-1].dst = tbl[cnt - 1].org;
		}
		else {
			ret++;
		}
	}
	return ret;
}

A_long EraseLinesSub8(P3Info8* subBuf, ParamInfo* infoP, NFWorld* src)
{
	A_long ret =0;

	//P3Info8* subBuf = (P3Info8*)sub->data8();

	//êÖïΩï˚å¸ÇÃÉXÉLÉÉÉì
	for (int y = 0; y < src->height(); y++)
	{
		A_long cnt = src->width();
		for (int x = 0; x < cnt; x++)
		{
			subBuf[x].org = src->GetPix8(x, y);
		}
		ret += EraseLinesScanline8(infoP,subBuf, cnt);
		for (int x = 0; x < cnt; x++)
		{
			src->SetPix8(x, y, subBuf[x].dst);
		}

	}
	if (ret > 0) {
		ret = 0;
		for (int x = 0; x < src->width(); x++)
		{
			A_long cnt = src->height();
			for (int y = 0; y < cnt; y++)
			{
				subBuf[y].org = src->GetPix8(x, y);
			}
			ret += EraseLinesScanline8(infoP, subBuf, cnt);
			for (int y = 0; y < cnt; y++)
			{
				src->SetPix8(x, y, subBuf[y].dst);
			}

		}
	}
	
	return ret;
}
PF_Err EraseLines::EraseLinesSub( ParamInfo* infoP, NFWorld* src)
{
	PF_Err err = PF_Err_NONE;

	int w = src->width();
	if (w < src->height()) w = src->height();

//	PF_EffectWorld sub = NewEffectWorld(w, 8, src->pixelFormat());
//	NFWorld subNF;
//	subNF.Setup(&sub, src->in_data, src->pixelFormat());

	A_long mi = 0;
	P3Info8* scanline8;
	PF_Handle sH;
	switch (src->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		//iterate32(src->world, (void*)infoP, ColChange32, dst->world);
		break;
	case PF_PixelFormat_ARGB64:
		//iterate16(src->world, (void*)infoP, ColChange16, dst->world);
		break;
	case PF_PixelFormat_ARGB32:
		sH = PF_NEW_HANDLE(w * sizeof(P3Info8) * 2);
		if (!sH) return PF_Err_OUT_OF_MEMORY;
		scanline8 = *(P3Info8**)sH;

	//	for (int i = 0; i < 30; i++)
	//	{
			/*A_long mi2 =*/ EraseLinesSub8(scanline8, infoP, src); EraseLinesSub8(scanline8, infoP, src);
		/*	if (mi == mi2)break;
			mi = mi2;
			if (mi2 == 0)break;
			{
				
			}
		}*/
		//if (mi > 0)
		{
			//AltCol8(infoP, src);
		}
		
		PF_DISPOSE_HANDLE(sH);
		break;
	default:
		break;
	}
	return err;
}
