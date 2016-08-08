#include "init.h"
#include "io.h"
#include "resolution.h"

//initialisation of Stranded2Boost
BBDECL int BBCALL Stranded2BoostInit() {
	BufferedFileSystemInit();
	ResolutionsInit();
	return 0;
}

//Stranded2Boost cleaning
BBDECL int BBCALL Stranded2BoostClear() {
	BufferedFileSystemClear();
	return 0;
}