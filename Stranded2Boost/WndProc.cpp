#include "WndProc.h"
#include <windows.h>
#include <commctrl.h>
#include <ddraw.h>
#include "init.h"
#include "ddraw_hook.h"


static LRESULT CALLBACK S2B_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
static void _stdcall DirectDrawCreateEx_HookFunction(GUID *lpGUID, LPVOID *lplpDD, const IID &iid, IUnknown *pUnkOuter);


static HWND handle;
static bool callback_registred = false;


static IDirectDraw7 **ddraw; //TODO: not working properly
static bool hooked = false;


static void *_old_a, *_old_b, *_cont;


void WndProcInit() {
	ddraw_hook_safe_call();
	if (!DirectDrawCreateEx_CreateHook(DirectDrawCreateEx, DirectDrawCreateEx_HookFunction, VirtualProtect, &_cont, &_old_a, &_old_b)){
		hooked = true;
	}
}

void WndProcClear() {
	if (callback_registred) {
		RemoveWindowSubclass(handle, S2B_WndProc, 0);
		callback_registred = false;
	}
	ddraw_hook_safe_call();
	if (DirectDrawCreateEx_RemoveHook(DirectDrawCreateEx, VirtualProtect, _old_a, _old_b)) {
		hooked = false;
	}
}

static void _stdcall DirectDrawCreateEx_HookFunction(GUID *lpGUID, LPVOID *lplpDD, const IID &iid, IUnknown *pUnkOuter)
{
	ddraw = (IDirectDraw7 **)lplpDD; //TODO: not working properly

	ddraw_hook_safe_call();
	__asm {
		push ebp;
		push _cont;
		call DirectDrawCreateEx_HookContinue;
	}
}

BBDECL void BBCALL Stranded2BoostProcedure(int hWND) {
	/*long style = GetWindowLong((HWND)hWND, GWL_STYLE);
	style &= ~WS_MAXIMIZE;
	SetWindowLong((HWND)hWND, GWL_STYLE, style);*/
	SetWindowSubclass((HWND)hWND, S2B_WndProc, 0, 0);
	handle = (HWND)hWND;
	callback_registred = true;
}

BBDECL inline void BBCALL ResetDisplayMode() {
	ChangeDisplaySettings(NULL, 0);
	if (hooked) {
		/*LPDIRECTDRAWSURFACE7 surf;
		(*ddraw)->GetGDISurface(&surf); //TODO: not working properly
		surf->Flip(0, DDFLIP_DONOTWAIT | DDFLIP_NOVSYNC);*/
	}
}

static LRESULT CALLBACK S2B_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		if (!wParam) {
			//ShowWindow(hwnd, SW_SHOWMINNOACTIVE);
			ResetDisplayMode();
		}
		else {
			//ShowWindow(hwnd, SW_SHOW);
			//UpdateWindow(hwnd);
		}
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		Stranded2BoostClear();
		break;
	}

	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}