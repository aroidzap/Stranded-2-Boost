#include "WndProc.h"
#include <windows.h>
#include "init.h"

static LRESULT CALLBACK S2B_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

static WNDPROC B3D_WndProc;
static HWND handle;
static bool callback_registred = false;

void WndProcClear() {
	if (callback_registred) {
		SetWindowLongPtr(handle, GWL_WNDPROC, (LONG_PTR)&B3D_WndProc);
		callback_registred = false;
	}
}

BBDECL void BBCALL Stranded2BoostProcedure(int hWND) {
	/*long style = GetWindowLong((HWND)hWND, GWL_STYLE);
	style &= ~WS_MAXIMIZE;
	SetWindowLong((HWND)hWND, GWL_STYLE, style);*/
	B3D_WndProc = (WNDPROC)SetWindowLongPtr((HWND)hWND, GWL_WNDPROC, (LONG_PTR)&S2B_WndProc);
	handle = (HWND)hWND;
	callback_registred = true;
}

static LRESULT CALLBACK S2B_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		if (!wParam) {
			ShowWindow(hwnd, SW_SHOWMINNOACTIVE);
			ChangeDisplaySettings(NULL, 0);
		}
		else {
			ShowWindow(hwnd, SW_SHOW);
		}
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		Stranded2BoostClear();
		break;
	}

	return CallWindowProc(B3D_WndProc, hwnd, uMsg, wParam, lParam);
}