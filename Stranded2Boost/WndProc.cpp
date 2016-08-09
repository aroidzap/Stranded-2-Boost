#include "WndProc.h"
#include <windows.h>
#include <commctrl.h>
#include "init.h"

static LRESULT CALLBACK S2B_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

static HWND handle;
static bool callback_registred = false;

void WndProcClear() {
	if (callback_registred) {
		RemoveWindowSubclass(handle, S2B_WndProc, 0);
		callback_registred = false;
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
}

static LRESULT CALLBACK S2B_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		if (!wParam) {
			ShowWindow(hwnd, SW_SHOWMINNOACTIVE);
			ResetDisplayMode();
		}
		else {
			ShowWindow(hwnd, SW_SHOW);
			UpdateWindow(hwnd);
		}
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		Stranded2BoostClear();
		break;
	}

	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}