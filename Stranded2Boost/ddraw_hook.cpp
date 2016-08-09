// Useful link: http://jbremer.org/x86-api-hooking-demystified/

#include "ddraw_hook.h"
#include <Windows.h>

static bool _safe_call = false;

void ddraw_hook_safe_call() {
	_safe_call = true;
}

#pragma optimize( "", off )
int DirectDrawCreateEx_CreateHook (	void *original /*8*/, void* hook /*12*/, void* VirtualProtect_func  /*16*/, 
									void **continue_address  /*20*/, void **oldcontent_a  /*24*/, void **oldcontent_b /*28*/)
{
	if (_safe_call) {
		__asm {
			push eax;
			push ebx;
			push ecx;
			push edx;

#ifdef _DEBUG
			// === find _DirectDrawCreateEx@16 ===
			mov eax, [ebp + 8]; // void* original
			mov ebx, eax;
			inc eax;
			mov eax, [eax];
			add eax, ebx;
			add eax, 7;
			mov eax, [eax];
			mov edx, [eax]; //EDX = _DirectDrawCreateEx@16 location
#else
			// === find _DirectDrawCreateEx@16 ===
			mov edx, [ebp + 8]; // void* original
#endif

			;// === save old content ===
			mov eax, [edx];
			mov ecx, [ebp + 24]
			mov [ecx], eax; //save oldcontent_a
			mov eax, [edx + 4];
			mov ecx, [ebp + 28]
			mov[ecx], eax; //save oldcontent_b

			;// === turn off protection ===

			push edx;
			mov eax, [ebp + 16]; //VirtualProtect_func
			lea ebx, [ebp + 8]; // "original" as temporary variable
			push ebx; 
			push 0x40; //PAGE_EXECUTE_READWRITE
			push 5;
			push edx;
			call eax;
			pop edx;

			;// === add hook ===

			mov eax, edx;
			mov cl, 0xE9;
			mov [eax], cl; // jmp rel32
			inc eax;
			mov ecx, [ebp + 12]; // hook
			sub ecx, edx;
			sub ecx, 5;
			mov [eax], ecx;

			;// === turn on protection ===

			push edx;
			mov eax, [ebp + 16]; //VirtualProtect_func
			lea ebx, [ebp + 8]; // "original" as temporary variable
			push ebx;
			mov ebx, [ebp + 8]; //previous protection value
			push ebx; //previous protection
			push 5;
			push edx;
			call eax;
			pop edx;

			;// === save continue address
			mov eax, edx;
			add eax, 5;
			mov ebx, [ebp + 20];
			mov[ebx], eax;

			pop edx;
			pop ecx;
			pop ebx;
			pop eax;
		}
		_safe_call = false;
		return 0;
	}
	else {
		OutputDebugString("ERROR: Calling \"DirectDrawCreateEx_CreateHook\" is not safe!!! You must know, what you are doing!!!");
		throw; return 1;
	}
}

void DirectDrawCreateEx_HookContinue(void *continue_address, int EBP)
{
	if (_safe_call) {
		_safe_call = false;
		__asm {
			mov eax, [ebp + 8]
			mov esp, [ebp + 12]
			mov ebp, esp

			jmp eax
		}
	}
	else {
		OutputDebugString("ERROR: Calling \"DirectDrawCreateEx_HookContinue\" is not safe!!! You must know, what you are doing!!!");
		throw;
	}
}

int DirectDrawCreateEx_RemoveHook(	void *original /*8*/, void* VirtualProtect_func /*12*/,
									void *oldcontent_a /*16*/, void *oldcontent_b /*20*/)
{
	if (_safe_call) {
		__asm {
			push eax;
			push ebx;
			push ecx;
			push edx;

#ifdef _DEBUG
			// === find _DirectDrawCreateEx@16 ===
			mov eax, [ebp + 8]; // void* original
			mov ebx, eax;
			inc eax;
			mov eax, [eax];
			add eax, ebx;
			add eax, 7;
			mov eax, [eax];
			mov edx, [eax]; //EDX = _DirectDrawCreateEx@16 location
#else
			// === find _DirectDrawCreateEx@16 ===
			mov edx, [ebp + 8];
#endif

			;// === turn off protection ===

			push edx;
			mov eax, [ebp + 12]; //VirtualProtect_func
			lea ebx, [ebp + 8]; // "original" as temporary variable
			push ebx;
			push 0x40; //PAGE_EXECUTE_READWRITE
			push 5;
			push edx;
			call eax;
			pop edx;

			;// === remove hook ===

			mov eax, edx;
			mov ebx, [ebp + 16];
			mov[eax], ebx;  //restore oldcontent_a
			add eax, 4;
			mov bl, [ebp + 20];
			mov [eax], bl; //restore oldcontent_b

			;// === turn on protection ===

			push edx;
			mov eax, [ebp + 12]; //VirtualProtect_func
			lea ebx, [ebp + 8]; // "original" as temporary variable
			push ebx;
			mov ebx, [ebp + 8]; //previous protection value
			push ebx; //previous protection
			push 5;
			push edx;
			call eax;
			pop edx;

			pop edx;
			pop ecx;
			pop ebx;
			pop eax;
		}
		_safe_call = false;
		return 0;
	}
	else {
		OutputDebugString("ERROR: Calling \"DirectDrawCreateEx_RemoveHook\" is not safe!!! You must know, what you are doing!!!");
		throw; return 1;
	}
}
#pragma optimize( "", on )