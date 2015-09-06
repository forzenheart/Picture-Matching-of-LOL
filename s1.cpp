#include "s1.h"
#include "s2.h"

button1::button1()
:
CUI(100, 100, 250, 180, true, true),
in(false)
{}

void button1::OnMouseMessage(_MOUSE_MESSAGE mm, int x, int y)
{
	if (mm == CUI::_M_L_UP_IN)
	{
		((s1*)scene)->change = true;
	}
	else if (mm == CUI::_M_ENTER)
		in = true;
	else if (mm == CUI::_M_QUIT)
		in = false;
}

void button1::ActiveRender()
{
	if (in)
	{
		CGame::GetGame()->GetGO()->DrawBmp(
			"pic\\1\\��ť.bmp",
			x1,
			y1,
			x2 - x1,
			y2 - y1,
			0,
			0);
	}
	else
	{
		CGame::GetGame()->GetGO()->DrawBmp(
			"pic\\1\\��ť.bmp",
			x1,
			y1,
			x2 - x1,
			y2 - y1,
			0,
			80);
	}
}

void button1::UnactiveRender()
{
	CGame::GetGame()->GetGO()->DrawRectangle(
		x1, y1, x2, y2, RGB(0, 0, 0), RGB(127, 127, 127));
}

button2::button2()
:
CUI(100, 300, 250, 380, true, true),
in(false)
{}

void button2::OnMouseMessage(_MOUSE_MESSAGE mm, int x, int y)
{
	if (mm == CUI::_M_L_UP_IN)
		CGame::GetGame()->Exit();
	else if (mm == CUI::_M_ENTER)
		in = true;
	else if (mm == CUI::_M_QUIT)
		in = false;
}

void button2::ActiveRender()
{
	if (in)
	{
		CGame::GetGame()->GetGO()->DrawBmp(
			"pic\\1\\��ť.bmp",
			x1,
			y1,
			x2 - x1,
			y2 - y1,
			0,
			160);
	}
	else
	{
		CGame::GetGame()->GetGO()->DrawBmp(
			"pic\\1\\��ť.bmp",
			x1,
			y1,
			x2 - x1,
			y2 - y1,
			0,
			240);
	}
}

void button2::UnactiveRender()
{
	CGame::GetGame()->GetGO()->DrawRectangle(
		x1, y1, x2, y2, RGB(0, 0, 0), RGB(127, 127, 127));
}

void s1::Init()
{
	p = CGame::GetGame();

	//����ͼƬ
	p->GetGO()->LoadBmpDirectory("pic");

	//��������
	p->GetAM()->SetOnceVolume(255);
	p->GetAM()->SetLoopVolume(10);
	p->GetAM()->PushLoopAudio("��ʼ������������", "music\\7.wav");
	p->GetAM()->PushOnceAudio("��ʼ��ť��Ч", "music\\BOMB2.WAV");
	p->GetAM()->PushOnceAudio("�浵��Ч", "music\\SND045.WAV");
	p->GetAM()->PushLoopAudio("��Ϸ������������", "music\\starsng.wav");
}
void s1::Enter()
{
	p->GetAM()->PlayLoopAudio("��ʼ������������");
	dark = 0;
	change = false;
}
void s1::Run()
{
	if (change)
	{
		dark += 8;
		CGame::GetGame()->GetGO()->SetDark(dark);
		if (dark > 255)
			CGame::GetGame()->ChangeScene("��Ϸ����");
	}

	int x, y;
	p->GetGI()->GetCursorPosition(&x, &y);

	CGame::GetGame()->GetGO()->DrawBmp(
		"pic\\1\\��ʼ����ͼ.bmp",
		0,
		0,
		640,
		480,
		0,
		0);
}
void s1::Quit()
{
	p->GetAM()->StopLoopAudio("��ʼ������������");
}