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

	CGame::GetGame()->LoadScene("��ʼ����", new s1);
	CGame::GetGame()->GetScene("��ʼ����")->LoadUI("��ʼ��ť", new button1);
	CGame::GetGame()->GetScene("��ʼ����")->LoadUI("������ť", new button2);

	CGame::GetGame()->LoadScene("��Ϸ����", new s2);
	CGame::GetGame()->GetScene("��Ϸ����")->LoadUI("�ƶ�����", new YDJP);
	CGame::GetGame()->GetScene("��Ϸ����")->LoadUI("�༭��", new Edit1);

	CGame::GetGame()->SetFirstScene("��ʼ����");

	CGame::GetGame()->Run();
	CGame::GetGame()->End();

	return 1;
}