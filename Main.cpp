#include <windows.h>
#include <iostream>
#include <time.h>
#include "Game.h"

#include "s1.h"
#include "s2.h"

int __stdcall WinMain(HINSTANCE hInstance,
					  HINSTANCE hPrevInstance,
					  char* lpCmdLine,
					  int nCmdShow)
{
	CGame::GetGame()->Init();

	CGame::GetGame()->LoadScene("开始场景", new s1);
	CGame::GetGame()->GetScene("开始场景")->LoadUI("开始按钮", new button1);
	CGame::GetGame()->GetScene("开始场景")->LoadUI("结束按钮", new button2);

	CGame::GetGame()->LoadScene("游戏场景", new s2);
	CGame::GetGame()->GetScene("游戏场景")->LoadUI("移动键盘", new YDJP);
	CGame::GetGame()->GetScene("游戏场景")->LoadUI("编辑框", new Edit1);

	CGame::GetGame()->SetFirstScene("开始场景");

	CGame::GetGame()->Run();
	CGame::GetGame()->End();

	return 1;
}