#include "CPGameInput.h"
#include <windows.h>

bool CCPGameInput::GetCursorPosition(int* pX, int* pY)
{
	//得到当前线程中的被激活的窗口
	HWND hWnd = GetActiveWindow();
	if (hWnd)
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(hWnd, &p);
		*pX = p.x;
		*pY = p.y;

		RECT r;
		GetClientRect(hWnd, &r);
		return TRUE == PtInRect(&r, p);
	}
	else
		return false;
}