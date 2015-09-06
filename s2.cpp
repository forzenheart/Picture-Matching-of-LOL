#include "s2.h"

YDJP::YDJP()
:
CUI(100, 100, 250, 250, true, true),
in(false),
qu(0)
{}

void YDJP::OnMouseMessage(_MOUSE_MESSAGE mm, int x, int y)
{
	//scene��CUI�еı�����Ա,��ָ��UI�����ĳ�����ָ��
	s2* ps = (s2*)scene;

	if (mm == CUI::_M_L_UP_IN)
	{
		if (x <= 75 && y <= 75)
		{
			ps->x -= 10;
			ps->y -= 10;
		}
		else if (x > 75 && y <= 75)
		{
			ps->x += 10;
			ps->y -= 10;
		}
		else if (x <= 75 && y > 75)
		{
			ps->x -= 10;
			ps->y += 10;
		}
		else
		{
			ps->x += 10;
			ps->y += 10;
		}
	}
	else if (mm == CUI::_M_MOVE_IN)
	{
		if (x <= 75 && y <= 75)
		{
			qu = 1;
		}
		else if (x > 75 && y <= 75)
		{
			qu = 2;
		}
		else if (x <= 75 && y > 75)
		{
			qu = 3;
		}
		else
		{
			qu = 4;
		}
	}
	else if (mm == CUI::_M_ENTER)
		in = true;
	else if (mm == CUI::_M_QUIT)
		in = false;
}
void YDJP::ActiveRender()
{
	if (in)
	{
		switch (qu)
		{
		case 1: CGame::GetGame()->GetGO()->DrawBmpT("pic\\2\\�������1.bmp",x1,y1,x2-x1,y2-y1,0,0,RGB(0, 255, 0)); break;
		case 2: CGame::GetGame()->GetGO()->DrawBmpT("pic\\2\\�������2.bmp",x1,y1,x2-x1,y2-y1,0,0,RGB(0, 255, 0)); break;
		case 3: CGame::GetGame()->GetGO()->DrawBmpT("pic\\2\\�������3.bmp",x1,y1,x2-x1,y2-y1,0,0,RGB(0, 255, 0)); break;
		case 4: CGame::GetGame()->GetGO()->DrawBmpT("pic\\2\\�������4.bmp",x1,y1,x2-x1,y2-y1,0,0,RGB(0, 255, 0)); break;
		}
	}
	else
	{
		CGame::GetGame()->GetGO()->DrawBmpT(
			"pic\\2\\�������0.bmp",
			x1,
			y1,
			x2 - x1,
			y2 - y1,
			0,
			0,
			RGB(0, 255, 0));
	}
}
void YDJP::UnactiveRender()
{
	CGame::GetGame()->GetGO()->DrawRectangle(
		x1, y1, x2, y2);
}

Edit1::Edit1()
:
CUI(200, 300, 400, 350, true, true),
len(0),
input(false)
{
	str[len] = 0;
}

void Edit1::Init()
{
	//���ر��༭�������
	CGame::GetGame()->GetGO()->LoadFont(
		"�༭������",
		(x2 - x1) / max,
		y2 - y1);
}

void Edit1::OnMouseMessage(_MOUSE_MESSAGE mm, int x, int y)
{
	if (mm == _M_L_DOWN_IN)
	{
		input = true;
		scene->SetKeyUI("�༭��");
	}
	else if (mm == _M_L_DOWN_OUT)
	{
		input = false;
		scene->SetKeyUI(0);
	}
}

void Edit1::OnCharMessage(int Char, bool db, bool print, bool ctrl)
{
	if (db)
	{
		if (len  + 2 > 16)
			return;

		*((int*)(str + len)) = Char;
		len += 2;
		str[len] = 0;
	}
	else
	{
		//����
		if (Char == 8 && len > 0)
		{
			bool db;
			for (int i = 0; i < len; ++i)
			{
				if (IsDBCSLeadByte(str[i]))
				{
					db = true;
					i += 1;
				}
				else
					db = false;
			}
			str[len -= (db ? 2 : 1)] = 0;
		}
		//�������ַ�
		else
		{
			if (len  + 1 > 16)
				return;

			if (print && !ctrl)
			{
				*((int*)(str + len)) = Char;
				len += 1;
				str[len] = 0;
			}
		}
	}
}

void Edit1::ActiveRender()
{
	if (input)
	{
		CGame::GetGame()->GetGO()->DrawRectangle(
			x1, y1, x2, y2);
	}
	else
	{
		CGame::GetGame()->GetGO()->DrawRectangle(
			x1, y1, x2, y2, RGB(0, 0, 0), RGB(127, 127, 127));
	}
	CGame::GetGame()->GetGO()->DrawTextL(
		"�༭������",
		x1, y1, str, len, RGB(255, 0, 0));
}

void Edit1::UnactiveRender()
{
	
}

void Edit1::SetText(const char* s)
{
	if (strlen(s) > max)
		return;

	strcpy_s(str, 32, s);
}

char* Edit1::GetText()
{
	return str;
}

void s2::Init()
{
	p = CGame::GetGame();
	x = y = 0;
}
void s2::Enter()
{
	p->GetAM()->PlayLoopAudio("��Ϸ������������");
	dark = 255;
}
void s2::Run()
{
	if (dark > 0)
	{
		dark -= 8;
		if (dark <= 0)
			dark = 0;
		CGame::GetGame()->GetGO()->SetDark(dark);
	}
	else
	{
		if (CGame::GetGame()->GetGI()->Get(_GI_M_R) == _KS_UC)
		{
			Edit1* pEdit = (Edit1*)GetUI("�༭��");
			CGame::GetGame()->SetWindowTitle(pEdit->GetText());
		}
		if (p->GetGI()->Get(_GI_K_UP) == _KS_DH)
			y -= 5;
		if (p->GetGI()->Get(_GI_K_DOWN) == _KS_DH)
			y += 5;
		if (p->GetGI()->Get(_GI_K_LEFT) == _KS_DH)
			x -= 5;
		if (p->GetGI()->Get(_GI_K_RIGHT) == _KS_DH)
			x += 5;
		if (p->GetGI()->Get(_GI_K_RETURN) == _KS_UC)
		{
			p->GetAM()->PlayOnceAudio("�浵��Ч");
			p->GetWF()->Create();
			p->GetWF()->WriteType<int>(x);
			p->GetWF()->WriteType<int>(y);
			p->GetWF()->Write("save");
		}
		if (p->GetGI()->Get(_GI_K_ESC) == _KS_UC)
		{
			p->ChangeScene("��ʼ����");
		}
	}

	CGame::GetGame()->GetGO()->DrawBmp(
		"pic\\2\\��ͼ.bmp",
		0,
		0,
		640,
		480,
		0,
		0);

	CGame::GetGame()->GetGO()->DrawBmpT(
		"pic\\2\\Ӣ��.bmp",
		x,
		y,
		50,
		80,
		0,
		0,
		RGB(0, 255, 0));
}
void s2::Quit()
{
	p->GetAM()->StopLoopAudio("��Ϸ������������");
}