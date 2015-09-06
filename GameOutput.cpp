#include "GameOutput.h"
#include <io.h>

#pragma comment(lib, "Msimg32.lib")

CGameOutput::CGameOutput(HWND hWnd)
:
m_hWnd(hWnd),
m_Dark(0)
{
	//得到客户区宽高
	RECT r;
	GetClientRect(m_hWnd, &r);
	m_ClientW = r.right - r.left;
	m_ClientH = r.bottom - r.top;

	//得到主设备
	m_MainDC = GetDC(m_hWnd);

	HBITMAP hbmp;

	//得到后备设备
	m_BackDC = CreateCompatibleDC(m_MainDC);
	hbmp = CreateCompatibleBitmap(m_MainDC, m_ClientW, m_ClientH);
	DeleteObject(SelectObject(m_BackDC, hbmp));
	DeleteObject(hbmp);

	//设置后备设备的文字背景为透明
	SetBkMode(m_BackDC, TRANSPARENT);

	//得到镜像设备
	m_MirrorDC = CreateCompatibleDC(m_MainDC);
	hbmp = CreateCompatibleBitmap(m_MainDC, m_ClientW, m_ClientH);
	DeleteObject(SelectObject(m_MirrorDC, hbmp));
	DeleteObject(hbmp);

	//得到暗度设备
	m_DarkDC = CreateCompatibleDC(m_MainDC);
	hbmp = CreateCompatibleBitmap(m_MainDC, m_ClientW, m_ClientH);
	DeleteObject(SelectObject(m_DarkDC, hbmp));
	DeleteObject(hbmp);

	//设置暗度设备内容为全黑
	BitBlt(m_DarkDC, 0, 0, m_ClientW, m_ClientH, 0, 0, 0, BLACKNESS);
}

CGameOutput::~CGameOutput()
{
	//释放所有字体
	std::map<std::string, HFONT>::iterator i1;
	for (i1 = m_Font.begin(); i1 != m_Font.end(); ++i1)
		DeleteObject(i1->second);

	//释放所有位图
	std::map<std::string, HDC>::iterator i2;
	for (i2 = m_Bmp.begin(); i2 != m_Bmp.end(); ++i2)
		DeleteDC(i2->second);

	//释放相关设备
	DeleteDC(m_MirrorDC);
	DeleteDC(m_BackDC);
	ReleaseDC(m_hWnd, m_MainDC);
}

void CGameOutput::SetDark(unsigned char Dark)
{
	m_Dark = Dark;
}

unsigned char CGameOutput::GetDark()
{
	return m_Dark;
}

void CGameOutput::Begin()
{
	BitBlt(m_BackDC, 0, 0, m_ClientW, m_ClientH, 0, 0, 0, WHITENESS);
}

void CGameOutput::End()
{
	if (m_Dark > 0)
	{
		BLENDFUNCTION bf = {0, 0, m_Dark, 0};
		AlphaBlend(m_BackDC, 0, 0, m_ClientW, m_ClientH, m_DarkDC, 0, 0, m_ClientW, m_ClientH, bf);
	}
	BitBlt(m_MainDC, 0, 0, m_ClientW, m_ClientH, m_BackDC, 0, 0, SRCCOPY);
}

int CGameOutput::LoadBmpDirectory(const char* d)
{
	int r = 0;

	char path[MAX_PATH];
	strcpy_s(path, MAX_PATH, d);
	strcat_s(path, MAX_PATH, "\\*.*");

	_finddata_t fd;

	__w64 int i = _findfirst(path, &fd);

	if (-1 != i)
	{
		do
		{
			//若是子目录则向下递归去遍历子目录
			if (fd.attrib & _A_SUBDIR)
			{
				if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0)
				{
					std::string f = d;
					f += "\\";
					f += fd.name;
					r += LoadBmpDirectory(f.c_str());
				}
			}
			else
			{
				//找到一个字符串中最后一次出现的字符的地址
				char* s = strrchr(fd.name, '.');
				if (s)
				{
					char hz[MAX_PATH];
					strcpy_s(hz, MAX_PATH, s + 1);

					//将后缀名归为大写
					for (int i = 0; hz[i]; ++i)
					{
						if (hz[i] >= 'a' && hz[i] <= 'z')
							hz[i] -= 'a' - 'A';
					}

					//是位图的情况下加载位图
					if (0 == strcmp(hz, "BMP"))
					{
						std::string f = d;
						f += "\\";
						f += fd.name;

						if (LoadBmp(f.c_str(), f.c_str()))
							r += 1;
					}
				}
			}
		} while (0 == _findnext(i, &fd));
	}

	return r;
}

bool CGameOutput::LoadBmp(const char* id, const char* f)
{
	//判断当前位图映射中有无该id的位图
	std::string s = id;
	if (m_Bmp.end() != m_Bmp.find(s))
		return false;

	//加载
	HBITMAP hbmp = (HBITMAP)LoadImage(
		0, f, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (0 == hbmp)
		return false;

	//创建设备
	HDC hdc = CreateCompatibleDC(m_MainDC);
	DeleteObject(SelectObject(hdc, hbmp));
	DeleteObject(hbmp);

	//放入映射
	m_Bmp.insert(std::pair<std::string, HDC>(std::string(id), hdc));

	return true;
}

bool CGameOutput::UnloadBmp(const char* id)
{
	std::string s = id;
	std::map<std::string, HDC>::iterator i = m_Bmp.find(s);
	if (m_Bmp.end() == i)
		return false;
	else
	{
		DeleteDC(i->second);
		m_Bmp.erase(i);
		return true;
	}
}

bool CGameOutput::LoadFont(const char* id,
						   int w,
						   int h,
						   int t,
						   bool i,
						   bool u,
						   const char* f)
{
	std::string s = id;
	if (m_Font.end() != m_Font.find(s))
		return false;

	HFONT hfont = CreateFont(
		h,
		w,
		0,
		0,
		t,
		i,
		u,
		0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH |FF_DONTCARE,
		f);

	m_Font.insert(std::pair<std::string, HFONT>(s, hfont));

	return true;
}

bool CGameOutput::UnloadFont(const char* id)
{
	std::string s = id;
	std::map<std::string, HFONT>::iterator i = m_Font.find(s);
	if (m_Font.end() == i)
		return false;
	else
	{
		DeleteObject(i->second);
		m_Font.erase(i);
		return true;
	}
}

bool CGameOutput::DrawDot(int x,
						  int y,
						  unsigned long c)
{
	return 1 != SetPixel(m_BackDC, x, y, c);
}

bool CGameOutput::DrawLine(int x1,
						   int y1,
						   int x2,
						   int y2,
						   unsigned long c)
{
	HPEN newpen = CreatePen(PS_SOLID, 1, c);
	HPEN oldpen = (HPEN)SelectObject(m_BackDC, newpen);
	MoveToEx(m_BackDC, x1, y1, 0);
	bool r = (0 != LineTo(m_BackDC, x2, y2));
	SelectObject(m_BackDC, oldpen);
	DeleteObject(newpen);
	return r;
}

bool CGameOutput::DrawRectangle(int x1,
								int y1,
								int x2,
								int y2,
								unsigned long pc,
								unsigned long bc,
								bool f)
{
	HPEN newpen = CreatePen(PS_SOLID, 1, pc);
	HPEN oldpen = (HPEN)SelectObject(m_BackDC, newpen);

	bool r;
	if (f)
	{
		HBRUSH newbrush = CreateSolidBrush(bc);
		HBRUSH oldbrush = (HBRUSH)SelectObject(m_BackDC, newbrush);
		r = (0 != Rectangle(m_BackDC, x1, y1, x2, y2));
		SelectObject(m_BackDC, oldbrush);
		DeleteObject(newbrush);
	}
	else
	{
		POINT p[]
		=
		{
			x1, y1,
			x2, y1,
			x2, y2,
			x1, y2
		};
		for (int i = 0; i < 4; ++i)
		{
			MoveToEx(m_BackDC, p[i].x, p[i].y, 0);
			LineTo(m_BackDC, p[(i + 1) % 4].x, p[(i + 1) % 4].y);
		}
	}

	SelectObject(m_BackDC, oldpen);
	DeleteObject(newpen);

	return r;
}

bool CGameOutput::DrawEllipse(int x1,
							  int y1,
							  int x2,
							  int y2,
							  unsigned long pc,
							  unsigned long bc)
{
	HPEN newpen = CreatePen(PS_SOLID, 1, pc);
	HPEN oldpen = (HPEN)SelectObject(m_BackDC, newpen);
	HBRUSH newbrush = CreateSolidBrush(bc);
	HBRUSH oldbrush = (HBRUSH)SelectObject(m_BackDC, newbrush);

	bool r = (0 != Ellipse(m_BackDC, x1, y1, x2, y2));

	SelectObject(m_BackDC, oldbrush);
	DeleteObject(newbrush);
	SelectObject(m_BackDC, oldpen);
	DeleteObject(newpen);

	return r;
}

bool CGameOutput::DrawBmp(const char* id,
						  int dx,
						  int dy,
						  int dw,
						  int dh,
						  int sx,
						  int sy,
						  int m)
{
	if (m < 0 || m > 3)
		return false;

	//判断当前位图映射中有无该id的位图
	std::string s = id;
	std::map<std::string, HDC>::iterator i = m_Bmp.find(s);
	if (m_Bmp.end() == i)
		return false;
	else
	{
		bool r;
		switch (m)
		{
		case 0: r = (0 != BitBlt    (m_BackDC,          dx,          dy,  dw,  dh, i->second, sx, sy, SRCCOPY)); break;
		case 1: r = (0 != StretchBlt(m_BackDC,          dx, dy + dh - 1,  dw, -dh, i->second, sx, sy, dw, dh, SRCCOPY)); break;
		case 2: r = (0 != StretchBlt(m_BackDC, dx + dw - 1,          dy, -dw,  dh, i->second, sx, sy, dw, dh, SRCCOPY)); break;
		case 3: r = (0 != StretchBlt(m_BackDC, dx + dw - 1, dy + dh - 1, -dw, -dh, i->second, sx, sy, dw, dh, SRCCOPY)); break;
		}
		return r;
	}
}

bool CGameOutput::DrawBmpT(const char* id,
						   int dx,
						   int dy,
						   int dw,
						   int dh,
						   int sx,
						   int sy,
						   unsigned long tc,
						   int m)
{
	if (m < 0 || m > 3)
		return false;

	//判断当前位图映射中有无该id的位图
	std::string s = id;
	std::map<std::string, HDC>::iterator i = m_Bmp.find(s);
	if (m_Bmp.end() == i)
		return false;
	else
	{
		switch (m)
		{
		case 0: return FALSE != TransparentBlt(m_BackDC, dx, dy, dw, dh, i->second, sx, sy, dw, dh, tc);
		case 1:
			{
				StretchBlt(m_MirrorDC, 0, 0 + dh - 1, dw, -dh, i->second, sx, sy, dw, dh, SRCCOPY);
				break;
			}
		case 2:
			{
				StretchBlt(m_MirrorDC, 0 + dw - 1, 0, -dw, dh, i->second, sx, sy, dw, dh, SRCCOPY);
				break;
			}
		case 3:
			{
				StretchBlt(m_MirrorDC, 0 + dw - 1, 0 + dh - 1, -dw, -dh, i->second, sx, sy, dw, dh, SRCCOPY);
				break;
			}
		}
		return FALSE != TransparentBlt(m_BackDC, dx, dy, dw, dh, m_MirrorDC, 0, 0, dw, dh, tc);
	}
}

bool CGameOutput::DrawTextL(const char* id,
							int x,
							int y,
							const char* t,
							int l,
							unsigned long c)
{
	std::string s = id;
	std::map<std::string, HFONT>::iterator i = m_Font.find(s);
	if (m_Font.end() == i)
		return false;

	HFONT oldfont = (HFONT)SelectObject(m_BackDC, i->second);
	COLORREF oldcolor = SetTextColor(m_BackDC, c);

	TextOutA(m_BackDC, x, y, t, l);

	SelectObject(m_BackDC, oldfont);
	SetTextColor(m_BackDC, oldcolor);

	return true;
}

bool CGameOutput::DrawTextR(const char* id,
							int x1,
							int y1,
							int x2,
							int y2,
							const char* t,
							int l,
							unsigned long c)
{
	std::string s = id;
	std::map<std::string, HFONT>::iterator i = m_Font.find(s);
	if (m_Font.end() == i)
		return false;

	HFONT oldfont = (HFONT)SelectObject(m_BackDC, i->second);
	COLORREF oldcolor = SetTextColor(m_BackDC, c);

	RECT r = {x1, y1, x2, y2};
	DrawTextA(m_BackDC, t, l, &r, DT_WORDBREAK);

	SelectObject(m_BackDC, oldfont);
	SetTextColor(m_BackDC, oldcolor);

	return true;
}