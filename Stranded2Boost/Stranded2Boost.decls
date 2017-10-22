.lib "Stranded2Boost.dll"

Stranded2BoostInit%():"_Stranded2BoostInit@0"
Stranded2BoostClear%():"_Stranded2BoostClear@0"

Stranded2BoostProcedure(hWND%):"_Stranded2BoostProcedure@4"
DirectDraw7_clear():"_DirectDraw7_clear@0"


BufOpenFile%(path$):"_BufOpenFile@4"
BufReadFile%(path$):"_BufReadFile@4"
BufWriteFile%(path$):"_BufWriteFile@4"
BufCloseFile(handle%):"_BufCloseFile@4"
BufFilePos%(handle%):"_BufFilePos@4"
BufSeekFile(handle%,offset%):"_BufSeekFile@8"

BufReadDir%(path$):"_BufReadDir@4"
BufCloseDir(dir_handle%):"_BufCloseDir@4"
BufNextFile$(dir_handle%):"_BufNextFile@4"

BufFileType%(path$):"_BufFileType@4"
BufFileSize%(path$):"_BufFileSize@4"

BufEof%(handle%):"_BufEof@4"

BufReadByte%(handle%):"_BufReadByte@4"
BufReadShort%(handle%):"_BufReadShort@4"
BufReadInt%(handle%):"_BufReadInt@4"
BufReadFloat#(handle%):"_BufReadFloat@4"
BufReadString$(handle%):"_BufReadString@4"
BufReadLine$(handle%):"_BufReadLine@4"

BufWriteByte(handle%,byte%):"_BufWriteByte@8"
BufWriteShort(handle%,short%):"_BufWriteShort@8"
BufWriteInt(handle%,int%):"_BufWriteInt@8"
BufWriteFloat(handle%,float#):"_BufWriteFloat@8"
BufWriteString(handle%,string$):"_BufWriteString@8"
BufWriteLine(handle%,string$):"_BufWriteLine@8"


GetResolutionMaxId%():"_GetResolutionMaxId@0"
GetResolutionId%(id%):"_GetResolutionId@4"
GetResolutionName$(id%):"_GetResolutionName@4"
GetResolutionX%(id%):"_GetResolutionX@4"
GetResolutionY%(id%):"_GetResolutionY@4"