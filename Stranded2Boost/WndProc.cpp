#include "WndProc.h"
#include <windows.h>
#include <commctrl.h>
#include <ddraw.h>
#include "init.h"
#include "ddraw_hook.h"

IDirectDraw7 *DirectDraw7;

static LRESULT CALLBACK S2B_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
static void _stdcall DirectDrawCreateEx_HookFunction(GUID *lpGUID, LPVOID *lplpDD, const IID &iid, IUnknown *pUnkOuter);
static void ClearDisplayMode();

static HWND handle;
static bool callback_registred = false;

static volatile bool dx7_cleared = false;
static volatile bool hooked = false;

static void *_cont, *_orig_ret;
static m32 _old_a, _old_b;

#pragma optimize( "", off )
BBDECL void BBCALL Stranded2BoostProcedure(int hWND) {
	SetWindowSubclass((HWND)hWND, S2B_WndProc, 0, 0);
	handle = (HWND)hWND;
	callback_registred = true;
}
#pragma optimize( "", on )

#pragma optimize( "", off )
BBDECL void BBCALL DirectDraw7_clear() {
	dx7_cleared = true;
}
#pragma optimize( "", on )

void WndProcInit() {
	if (hooked == false) {
		ddraw_hook_safe_call();
		if (!DirectDrawCreateEx_CreateHook(DirectDrawCreateEx, DirectDrawCreateEx_HookFunction, VirtualProtect, &_cont, &_old_a, &_old_b)) {
			hooked = true;
		}
	}
}

void WndProcClear() {
	if (callback_registred) {
		RemoveWindowSubclass(handle, S2B_WndProc, 0);
		callback_registred = false;
	}
	if (hooked == true) {
		ddraw_hook_safe_call();
		if (DirectDrawCreateEx_RemoveHook(DirectDrawCreateEx, VirtualProtect, _old_a, _old_b)) {
			hooked = false;
		}
	}
}

#pragma optimize( "", off )
static _declspec(naked) void _stdcall DirectDrawCreateEx_HookFunction(GUID *lpGUID, LPVOID *lplpDD, const IID &iid, IUnknown *pUnkOuter)
{
	// Enter stack frame
	__asm{
		push ebp
		mov ebp, esp
	}
	// Call original function
	__asm {
		// substitute return address
		push eax
		mov eax, [ebp + 4]
		mov _orig_ret, eax
		mov eax, __hook_return
		mov [ebp + 4], eax
		pop eax
		// "call" original function
		jmp _cont
	__hook_return:
		// restore stack
		sub esp, 20 
		push ebp
		mov ebp, esp
		//save back original return address
		mov eax, _orig_ret
		mov [ebp + 4], eax
	} 

	// Do needed things
	DirectDraw7 = (IDirectDraw7 *)*lplpDD;

	// Exit stack frame
	__asm{
		mov esp, ebp
		pop ebp
		ret 16
	}
}
#pragma optimize( "", on )


static void ClearDisplayMode() {
	ChangeDisplaySettings(NULL, 0);
	if (hooked && !dx7_cleared) {
		IDirectDrawSurface7 *surf;
		if (DirectDraw7->GetGDISurface(&surf) == DD_OK && surf != nullptr) {
			surf->Flip(0, DDFLIP_DONOTWAIT | DDFLIP_NOVSYNC);
		}
	}
}

static LRESULT CALLBACK S2B_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		if (!wParam) {
			ClearDisplayMode();
			ShowWindow(hwnd, SW_SHOWMINNOACTIVE);
		}
		else {
			ShowWindow(hwnd, SW_SHOW);
			UpdateWindow(hwnd);
		}
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		DirectDraw7_clear();
		ClearDisplayMode();
		Stranded2BoostClear();
		break;
	}

	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}