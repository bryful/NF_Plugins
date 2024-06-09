#pragma once
#ifndef TEX_H
#define TEX_H
#include "AE_SDK.h"

#define SPT_COUNT 60
#define SPT_WIDTH 16
#define SPT_VALUE(IDX,X,Y) (SPT[(IDX)][(X) + ((Y) * SPT_WIDTH)])

extern A_u_char SPT[SPT_COUNT][SPT_WIDTH * SPT_WIDTH];


#endif