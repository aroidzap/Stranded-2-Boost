#include "init.h"
#include "io.h"
#include "resolution.h"
#include "WndProc.h"

//initialisation of Stranded2Boost
BBDECL int BBCALL Stranded2BoostInit() {
	BufferedFileSystemInit();
	ResolutionsInit();
	return 0;
}

//Stranded2Boost cleaning
BBDECL int BBCALL Stranded2BoostClear() {
	BufferedFileSystemClear();
	WndProcClear();
	return 0;
}