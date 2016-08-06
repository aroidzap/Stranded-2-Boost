#include "io.h"
#include <stdio.h>

#define BUFFER_SIZE 4096

using namespace std;

//initialises buffered file system
void BufferedFileSystemInit(){
	
}

//performs cleaning
void BufferedFileSystemClear() {

}

//opens file for reading and writing
BBDECL int BBCALL BufOpenFile(const char *path) {
	FILE *f;
	fopen_s(&f, path, "r+b");
	return reinterpret_cast<int>(f);
}
//opens file for reading
BBDECL int BBCALL BufReadFile(const char *path) {
	FILE *f;
	fopen_s(&f, path, "rb");
	return reinterpret_cast<int>(f);
}
//opens file for writing
BBDECL int BBCALL BufWriteFile(const char *path) {
	FILE *f;
	fopen_s(&f, path, "wb");
	return reinterpret_cast<int>(f);
}
//closes file
BBDECL void BBCALL BufCloseFile(int handle) {
	fclose(reinterpret_cast<FILE *>(handle));
}
//returns offset from start
BBDECL int BBCALL BufFilePos(int handle) {
	return static_cast<int>(ftell(reinterpret_cast<FILE *>(handle)));
}
//sets position
BBDECL void BBCALL BufSeekFile(int handle, int offset) {
	fseek(reinterpret_cast<FILE *>(handle), offset, SEEK_SET);
}

// returns file size
BBDECL int BBCALL BufFileSize(const char *path) {
	FILE *f;
	fopen_s(&f, path, "rb");
	fseek(f, 0L, SEEK_END);
	int size = static_cast<int>(ftell(f));
	fclose(f);
	return size;
}


// BufEof%(handle%):"_BufEof@4"
BBDECL int BBCALL BufEof(int handle) {
	if (feof(reinterpret_cast<FILE *>(handle))){
		return 1;
	}
	else {
		return 0;
	}
}

// BufReadByte%(handle%):"_BufReadByte@4"
BBDECL char BBCALL BufReadByte(int handle) {
	FILE* f = reinterpret_cast<FILE *>(handle);
	char tmp;
	tmp = fgetc(f);
	return tmp;
}
// BufReadShort%(handle%):"_BufReadShort@4"
BBDECL short BBCALL BufReadShort(int handle) {
	FILE* f = reinterpret_cast<FILE *>(handle);
	short tmp;
	tmp = (fgetc(f) & 0xff);
	tmp |= (fgetc(f) & 0xff) << 8;
	return tmp;
}
// BufReadInt%(handle%):"_BufReadInt@4"
BBDECL int BBCALL BufReadInt(int handle) {
	FILE* f = reinterpret_cast<FILE *>(handle);
	int tmp;
	tmp = (fgetc(f) & 0xff);
	tmp |= (fgetc(f) & 0xff) << 8;
	tmp |= (fgetc(f) & 0xff) << 16;
	tmp |= (fgetc(f) & 0xff) << 24;
	return tmp;
}
// BufReadFloat#(handle%):"_BufReadFloat@4"
BBDECL float BBCALL BufReadFloat(int handle) {
	FILE* f = reinterpret_cast<FILE *>(handle);
	float tmp;
	int *p = reinterpret_cast<int*>(&tmp);
	*p = (fgetc(f) & 0xff);
	*p |= (fgetc(f) & 0xff) << 8;
	*p |= (fgetc(f) & 0xff) << 16;
	*p |= (fgetc(f) & 0xff) << 24;
	return tmp;
}
// BufReadString$(handle%):"_BufReadString@4"
BBDECL char* BBCALL BufReadString(int handle) {
	FILE *f = reinterpret_cast<FILE *>(handle);
	static char buf[BUFFER_SIZE];
	char *p = buf;
	unsigned int count = fgetc(f) & 0xff;
	count |= (fgetc(f) & 0xff) << 8;
	count |= (fgetc(f) & 0xff) << 16;
	count |= (fgetc(f) & 0xff) << 24;
	while (count) {
		*p = static_cast<char>(fgetc(f));
		p++; count--;
	}
	*p = 0;
	return buf;
}
// BufReadLine$(handle%):"_BufReadLine@4"
BBDECL char* BBCALL BufReadLine(int handle) {
	FILE *f = reinterpret_cast<FILE *>(handle);
	static char buf[BUFFER_SIZE];
	char *p = buf;
	int c;
	while ((c = fgetc(f)) != EOF) {
		if (c == 0x0a) { break; }
		if (c == 0x0d) { 
			if (!(fgetc(f) == 0x0a)) {
				fseek(f, -1, SEEK_CUR);
			}
			break;
		}
		*p = static_cast<char>(c);
		p++;
	}
	*p = 0;
	return buf;
}

// BufWriteByte(handle%,byte%):"_BufWriteByte@8"
BBDECL void BBCALL BufWriteByte(int handle, int val) {
	FILE* f = reinterpret_cast<FILE *>(handle);
	putc(static_cast<char>(val), f);
}
// BufWriteShort(handle%,short%):"_BufWriteShort@8"
BBDECL void BBCALL BufWriteShort(int handle, int val) {
	FILE* f = reinterpret_cast<FILE *>(handle);
	putc(static_cast<char>(val), f);
	putc(static_cast<char>(val >> 8), f);
}
// BufWriteInt(handle%,int%):"_BufWriteInt@8"
BBDECL void BBCALL BufWriteInt(int handle, int val) {
	FILE* f = reinterpret_cast<FILE *>(handle);
	putc(static_cast<char>(val), f);
	putc(static_cast<char>(val >> 8), f);
	putc(static_cast<char>(val >> 16), f);
	putc(static_cast<char>(val >> 24), f);
}
// BufWriteFloat(handle%,float#):"_BufWriteFloat@8"
BBDECL void BBCALL BufWriteFloat(int handle, float var) {
	FILE* f = reinterpret_cast<FILE *>(handle);
	int *p = reinterpret_cast<int*>(&var);
	putc(static_cast<char>(*p), f);
	putc(static_cast<char>(*p >> 8), f);
	putc(static_cast<char>(*p >> 16), f);
	putc(static_cast<char>(*p >> 24), f);
}
// BufWriteString(handle%,string$):"_BufWriteString@8"
BBDECL void BBCALL BufWriteString(int handle, const char *val) {
	FILE* f = reinterpret_cast<FILE *>(handle);
	char *p = const_cast<char*>(val);
	unsigned int count = 0;
	while (*p != 0) { p++; count++; }
	p = const_cast<char*>(val);
	putc(static_cast<char>(count), f);
	putc(static_cast<char>(count >> 8), f);
	putc(static_cast<char>(count >> 16), f);
	putc(static_cast<char>(count >> 24), f);
	while (*p != 0) {
		putc(*p, f);
		p++;
	}
}
// BufWriteLine(handle%,string$):"_BufWriteLine@8"
BBDECL void BBCALL BufWriteLine(int handle, const char *val) {
	FILE* f = reinterpret_cast<FILE *>(handle);
	char *p = const_cast<char*>(val);
	while (*p != 0) {
		putc(*p, f);
		p++;
	}
	putc(0x0d, f);
	putc(0x0a, f);
}


//opens directory
BBDECL int BBCALL BufReadDir(const char *path) {
	//TODO
	return -1;
}

//closes directory
BBDECL void BBCALL BufCloseDir(int dir_handle) {
	//TODO
}

//gets next filename (including directories)
BBDECL char* BBCALL BufNextFile(int dir_handle) {
	//TODO
	return nullptr;
}

// returns type of file
BBDECL int BBCALL BufFileType(const char *path) {
	// 1 = The filename exists
	// 0 = The filename doesn't exist
	// 2 = The filename is not a file - but a directory
	//TODO
	return -1;
}