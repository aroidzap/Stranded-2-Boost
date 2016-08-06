#include "resolution.h"
#include <windows.h>

// returns available resolution count
BBDECL int BBCALL GetResolutionCount() {
	DEVMODE dm = { 0 };
	dm.dmSize = sizeof(dm);
	int iModeNum;
	for (iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dm) != 0; iModeNum++);
	return iModeNum;
}

//returns width (component==1) or height (component==0) of resolution with specific id
BBDECL int BBCALL GetResolutionComponent(int component, int id) {
	DEVMODE dm = { 0 };
	dm.dmSize = sizeof(dm);
	int iModeNum;
	for (iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dm) != 0; iModeNum++) {
		if (iModeNum = id) {
			if (component) {
				return dm.dmPelsWidth;
			}
			else {
				return dm.dmPelsHeight;
			}
		}
	}
	return -1;
}