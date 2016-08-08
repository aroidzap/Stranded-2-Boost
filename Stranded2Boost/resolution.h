#pragma once
#include "Blitz3D.h"

void ResolutionsInit();

// GetResolutionMaxId%():"_GetResolutionMaxId@0"
BBDECL int BBCALL GetResolutionMaxId();
// GetResolutionId%(id%):"_GetResolutionId@4"
BBDECL int BBCALL GetResolutionId(int width);
// GetResolutionName$(id%):"_GetResolutionName@4"
BBDECL const char* BBCALL GetResolutionName(int id);
// GetResolutionX%(id%):"_GetResolutionX@4"
BBDECL int BBCALL GetResolutionX(int id);
// GetResolutionY%(id%):"_GetResolutionY@4"
BBDECL int BBCALL GetResolutionY(int id);