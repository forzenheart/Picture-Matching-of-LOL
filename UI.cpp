#include "UI.h"

CUI::CUI(int X1, int Y1, int X2, int Y2, bool s, bool a)
:
x1(X1),
y1(Y1),
x2(X2),
y2(Y2),
show(s),
active(a)
{}

CUI::~CUI()
{}

void CUI::SetPosition(int X1, int Y1, int X2, int Y2)
{
	x1 = X1;
	y1 = Y1;
	x2 = X2;
	y2 = Y2;
}	

void CUI::GetPosition(int* X1, int* Y1, int* X2, int* Y2)
{
	*X1 = x1;
	*Y1 = y1;
	*X2 = x2;
	*Y2 = y2;
}

void CUI::SetShow(bool s)
{
	show = s;
}

bool CUI::GetShow()
{
	return show;
}

void CUI::SetActive(bool a)
{
	active = a;
}

bool CUI::GetActive()
{
	return active;
}

void CUI::OnMouseMessage(_MOUSE_MESSAGE mm, int x, int y)
{}

void CUI::OnKeyMessage(bool Down, int Key)
{}

void CUI::OnCharMessage(int Char, bool db, bool print, bool ctrl)
{}

void CUI::ActiveRender()
{}

void CUI::UnactiveRender()
{}

void CUI::Init()
{}

void CUI::End()
{}