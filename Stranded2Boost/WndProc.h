#pragma once
#include "Blitz3D.h"
#include <ddraw.h>

extern IDirectDraw7 *DirectDraw7;

void WndProcInit();
void WndProcClear();

// Stranded2BoostProcedure(hWND%):"_Stranded2BoostProcedure@4"
BBDECL void BBCALL Stranded2BoostProcedure(int hWND);

// DirectDraw7_clear():"_DirectDraw7_clear@0"
BBDECL void BBCALL DirectDraw7_clear();