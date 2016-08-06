#pragma once
#include "Blitz3D.h"

void BufferedFileSystemInit();
void BufferedFileSystemClear();

// BufOpenFile%(path$):"_BufOpenFile@4"
BBDECL int BBCALL BufOpenFile(const char *path);
// BufReadFile%(path$):"_BufReadFile@4"
BBDECL int BBCALL BufReadFile(const char *path);
// BufWriteFile%(path$):"_BufWriteFile@4"
BBDECL int BBCALL BufWriteFile(const char *path);
// BufCloseFile(handle%):"_BufCloseFile@4"
BBDECL void BBCALL BufCloseFile(int handle);
// BufFilePos%(handle%):"_BufFilePos@4"
BBDECL int BBCALL BufFilePos(int handle);
// BufSeekFile(handle%,offset%):"_BufSeekFile@8"
BBDECL void BBCALL BufSeekFile(int handle, int offset);

// BufReadDir%(path$):"_BufReadDir@4"
BBDECL int BBCALL BufReadDir(const char *path);
// BufCloseDir(dir_handle%):"_BufCloseDir@4"
BBDECL void BBCALL BufCloseDir(int dir_handle);
// BufNextFile$(dir_handle%):"_BufNextFile@4"
BBDECL char* BBCALL BufNextFile(int dir_handle);

// BufFileType%(path$):"_BufFileType@4"
BBDECL int BBCALL BufFileType(const char *path);
// BufFileSize%(path$):"_BufFileSize@4"
BBDECL int BBCALL BufFileSize(const char *path);


// BufEof%(handle%):"_BufEof@4"
BBDECL int BBCALL BufEof(int handle);

// BufReadByte%(handle%):"_BufReadByte@4"
BBDECL char BBCALL BufReadByte(int handle);
// BufReadShort%(handle%):"_BufReadShort@4"
BBDECL short BBCALL BufReadShort(int handle);
// BufReadInt%(handle%):"_BufReadInt@4"
BBDECL int BBCALL BufReadInt(int handle);
// BufReadFloat#(handle%):"_BufReadFloat@4"
BBDECL float BBCALL BufReadFloat(int handle);
// BufReadString$(handle%):"_BufReadString@4"
BBDECL char* BBCALL BufReadString(int handle);
// BufReadLine$(handle%):"_BufReadLine@4"
BBDECL char* BBCALL BufReadLine(int handle);

// BufWriteByte(handle%,byte%):"_BufWriteByte@8"
BBDECL void BBCALL BufWriteByte(int handle, int val);
// BufWriteShort(handle%,short%):"_BufWriteShort@8"
BBDECL void BBCALL BufWriteShort(int handle, int val);
// BufWriteInt(handle%,int%):"_BufWriteInt@8"
BBDECL void BBCALL BufWriteInt(int handle, int val);
// BufWriteFloat(handle%,float#):"_BufWriteFloat@8"
BBDECL void BBCALL BufWriteFloat(int handle, float val);
// BufWriteString(handle%,string$):"_BufWriteString@8"
BBDECL void BBCALL BufWriteString(int handle, const char *val);
// BufWriteLine(handle%,string$):"_BufWriteLine@8"
BBDECL void BBCALL BufWriteLine(int handle, const char *val);