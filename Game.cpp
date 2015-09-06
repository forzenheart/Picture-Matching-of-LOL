#include "Game.h"
#include <time.h>

CGame* CGame::m_pGame = 0;

CGame::CGame()
:
m_hInstance(0),
m_ClientW(0),
m_ClientH(0),
m_hWnd(0),
m_Active(TRUE),
m_GI(0),
m_GO(0),
m_AM(0),
m_MP(0),
m_CurScene(0),
m_DoubleByte(false)
{
	srand((unsigned int)time(0));
}

CGame* CGame::GetGame()
{
	if (0 == m_pGame)
		m_pGame = new CGame;
	return m_pGame;
}

CGame::~CGame()
{
	std::map<std::string, CScene*>::iterator i;
	for (i = m_Scene.begin(); i != m_Scene.end(); ++i)
		delete i->second;

	if (m_WF)
		delete m_WF;
	if (m_RF)
		delete m_RF;
	if (m_MP)
		delete m_MP;
	if (m_AM)
		delete m_AM;
	if (m_GO)
		delete m_GO;
	if (m_GI)
		delete m_GI;
}

LRESULT __stdcall CGame::WndProc(HWND hwnd,
								 UINT uMsg,
								 WPARAM wParam,
								 LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_ACTIVATEAPP:
		{
			m_pGame->m_Active = (BOOL)wParam;
			return 0;
		}
	case WM_MOUSEMOVE:
		{
			POINT p = {LOWORD(lParam), HIWORD(lParam)};

			CScene* s = m_pGame->m_CurScene;

			//有当前场景
			if (s)
			{
				std::map<std::string, CUI*>::iterator i;
				for (i = s->m_UI.begin(); i != s->m_UI.end(); ++i)
				{
					CUI* pUI = i->second;

					if (pUI->show && pUI->active)
					{
						RECT r = {pUI->x1, pUI->y1, pUI->x2, pUI->y2};

						//得到前一次和这一次光标是否在UI中
						BOOL pre = PtInRect(&r, m_pGame->m_PreCurPos);
						BOOL cur = PtInRect(&r, p);

						if (pre && cur)
							i->second->OnMouseMessage(CUI::_M_MOVE_IN, p.x - pUI->x1, p.y - pUI->y1); //多态
						else if (pre && !cur)
							i->second->OnMouseMessage(CUI::_M_QUIT, p.x - pUI->x1, p.y - pUI->y1); //多态
						else if (!pre && cur)
							i->second->OnMouseMessage(CUI::_M_ENTER, p.x - pUI->x1, p.y - pUI->y1); //多态
						else
							i->second->OnMouseMessage(CUI::_M_MOVE_OUT, p.x - pUI->x1, p.y - pUI->y1); //多态

						//如果上面的多态处理中改变了当前场景,则不能再循环了
						if (s != m_pGame->m_CurScene)
						{
							m_pGame->m_PreCurPos = p;
							return 0;
						}
					}
				}
			}

			m_pGame->m_PreCurPos = p;

			return 0;
		}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		{
			POINT p = {LOWORD(lParam), HIWORD(lParam)};

			CScene* s = m_pGame->m_CurScene;

			//有当前场景
			if (s)
			{
				std::map<std::string, CUI*>::iterator i;
				for (i = s->m_UI.begin(); i != s->m_UI.end(); ++i)
				{
					CUI* pUI = i->second;

					if (pUI->show && pUI->active)
					{
						RECT r = {pUI->x1, pUI->y1, pUI->x2, pUI->y2};

						if (PtInRect(&r, p))
						{
							switch (uMsg)
							{
							case WM_LBUTTONDOWN: i->second->OnMouseMessage(CUI::_M_L_DOWN_IN, p.x - pUI->x1, p.y - pUI->y1); break;
							case WM_LBUTTONUP: i->second->OnMouseMessage(CUI::_M_L_UP_IN, p.x - pUI->x1, p.y - pUI->y1); break;
							case WM_MBUTTONDOWN: i->second->OnMouseMessage(CUI::_M_M_DOWN_IN, p.x - pUI->x1, p.y - pUI->y1); break;
							case WM_MBUTTONUP: i->second->OnMouseMessage(CUI::_M_M_UP_IN, p.x - pUI->x1, p.y - pUI->y1); break;
							case WM_RBUTTONDOWN: i->second->OnMouseMessage(CUI::_M_R_DOWN_IN, p.x - pUI->x1, p.y - pUI->y1); break;
							case WM_RBUTTONUP: i->second->OnMouseMessage(CUI::_M_R_UP_IN, p.x - pUI->x1, p.y - pUI->y1); break;
							}
						}
						else
						{
							switch (uMsg)
							{
							case WM_LBUTTONDOWN: i->second->OnMouseMessage(CUI::_M_L_DOWN_OUT, p.x - pUI->x1, p.y - pUI->y1); break;
							case WM_LBUTTONUP: i->second->OnMouseMessage(CUI::_M_L_UP_OUT, p.x - pUI->x1, p.y - pUI->y1); break;
							case WM_MBUTTONDOWN: i->second->OnMouseMessage(CUI::_M_M_DOWN_OUT, p.x - pUI->x1, p.y - pUI->y1); break;
							case WM_MBUTTONUP: i->second->OnMouseMessage(CUI::_M_M_UP_OUT, p.x - pUI->x1, p.y - pUI->y1); break;
							case WM_RBUTTONDOWN: i->second->OnMouseMessage(CUI::_M_R_DOWN_OUT, p.x - pUI->x1, p.y - pUI->y1); break;
							case WM_RBUTTONUP: i->second->OnMouseMessage(CUI::_M_R_UP_OUT, p.x - pUI->x1, p.y - pUI->y1); break;
							}
						}

						//如果上面的多态处理中改变了当前场景,则不能再循环了
						if (s != m_pGame->m_CurScene)
							return 0;
					}
				}
			}

			return 0;
		}
	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			if (m_pGame->m_CurScene &&
				m_pGame->m_CurScene->m_KeyUI &&
				m_pGame->m_CurScene->m_KeyUI->GetShow() &&
				m_pGame->m_CurScene->m_KeyUI->GetActive())
			{
				m_pGame->m_CurScene->m_KeyUI->OnKeyMessage(uMsg == WM_KEYDOWN, (int)wParam);
			}

			return 0;
		}
	case WM_CHAR:
		{
			if (m_pGame->m_CurScene &&
				m_pGame->m_CurScene->m_KeyUI &&
				m_pGame->m_CurScene->m_KeyUI->GetShow() &&
				m_pGame->m_CurScene->m_KeyUI->GetActive())
			{
				//等待双字节文字的下一个字节
				if (m_pGame->m_DoubleByte)
				{
					m_pGame->m_DoubleByte = false;
					m_pGame->m_DoubleByteChar |= (unsigned char)(wParam) << 8;
					m_pGame->m_CurScene->m_KeyUI->OnCharMessage(m_pGame->m_DoubleByteChar, true, true, false);
				}
				else
				{
					//双字节字符
					if (IsDBCSLeadByte((unsigned char)(wParam)))
					{
						m_pGame->m_DoubleByte = true;
						m_pGame->m_DoubleByteChar = 0;
						m_pGame->m_DoubleByteChar = (unsigned char)(wParam);
					}
					//单字节字符
					else
					{
						m_pGame->m_CurScene->m_KeyUI->OnCharMessage((int)wParam, false, 0 != isprint((int)wParam), 0 != iscntrl((int)wParam));
					}
				}
			}

			return 0;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool CGame::Init(int ClientW, int ClientH, int SleepTime, const char* Title)
{
	//得到应用程序实例句柄
	m_hInstance = GetModuleHandle(0);

	//填充窗口类别
	m_WndClass.style = CS_HREDRAW | CS_VREDRAW;
	m_WndClass.lpfnWndProc = WndProc;
	m_WndClass.cbClsExtra = 0;
	m_WndClass.cbWndExtra = 0;
	m_WndClass.hInstance = m_hInstance;
	m_WndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	m_WndClass.hCursor = LoadCursor(0, IDC_ARROW);
	m_WndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	m_WndClass.lpszMenuName = 0;
	m_WndClassName[31] = 0;
	for (int i = 0; i < 31; ++i)
		m_WndClassName[i] = rand() % 26 + 'a';
	m_WndClass.lpszClassName = m_WndClassName;

	//注册窗口类别
	RegisterClass(&m_WndClass);

	//得到客户区宽、高
	m_ClientW = ClientW;
	m_ClientH = ClientH;

	//得到每次游戏循环的休息时间
	m_SleepTime = SleepTime;

	//计算窗口的大小
	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);
	RECT r =
	{
		(sw - m_ClientW) / 2,
		(sh - m_ClientH) / 2,
		(sw - m_ClientW) / 2 + m_ClientW,
		(sh - m_ClientH) / 2 + m_ClientH
	};
	AdjustWindowRect(&r, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

	//创建窗口
	m_hWnd = CreateWindow(
		m_WndClass.lpszClassName,
		Title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		r.left,
		r.top,
		r.right - r.left,
		r.bottom - r.top,
		HWND_DESKTOP,
		0,
		m_hInstance,
		0);

	//初始化前一帧光标位置
	GetCursorPos(&m_PreCurPos);
	ScreenToClient(m_hWnd, &m_PreCurPos);

	//初始化所有工具
	m_GI = new CCPGameInput;
	m_GO = new CGameOutput(m_hWnd);
	m_AM = new CAudioManager;
	m_MP = new CMemoryPool;
	m_RF = new CReadFile;
	m_WF = new CWriteFile;

	return true;
}

void CGame::Run()
{
	//场景初始化
	std::map<std::string, CScene*>::iterator i;
	for (i = m_Scene.begin(); i != m_Scene.end(); ++i)
	{
		i->second->Init(); //多态

		//当前场景中的UI初始化
		std::map<std::string, CUI*>::iterator j;
		for (j = i->second->m_UI.begin(); j != i->second->m_UI.end(); ++j)
			j->second->Init(); //多态
	}

	//调用最初场景的Enter
	if (m_CurScene)
		m_CurScene->Enter();

	UpdateWindow(m_hWnd);
	ShowWindow(m_hWnd, SW_SHOW);

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (m_Active)
		{
			int bt = GetTickCount();

			//输入运行
			m_GI->Run();

			//输出开始
			m_GO->Begin();

			//当前场景运行
			if (m_CurScene)
			{
				//得到当前场景
				CScene* p1 = m_CurScene;
				m_CurScene->Run(); //多态

				//若当期场景被修改,则再调用一次新场景的运行
				if (p1 != m_CurScene)
					m_CurScene->Run(); //多态

				std::map<std::string, CUI*>::iterator i;
				for (i = m_CurScene->m_UI.begin(); i != m_CurScene->m_UI.end(); ++i)
				{
					if (i->second->GetShow())
					{
						if (i->second->GetActive())
							i->second->ActiveRender(); //多态
						else
							i->second->UnactiveRender(); //多态
					}
				}
			}

			//输出结束
			m_GO->End();

			int at = GetTickCount() - bt;
			Sleep(m_SleepTime > at ? m_SleepTime - at : 1);
		}
		else
			WaitMessage();
	}

	//场景结束
	for (i = m_Scene.begin(); i != m_Scene.end(); ++i)
	{
		//当前场景中的UI结束
		std::map<std::string, CUI*>::iterator j;
		for (j = i->second->m_UI.begin(); j != i->second->m_UI.end(); ++j)
			j->second->End(); //多态

		i->second->End(); //多态
	}
}

void CGame::End()
{
	if (m_pGame)
		delete m_pGame;
}

bool CGame::LoadScene(const char* id, CScene* pScene)
{
	std::string s = id;

	if (m_Scene.end() != m_Scene.find(s))
		return false;

	m_Scene.insert(std::pair<std::string, CScene*>(s, pScene));

	return true;
}

bool CGame::UnloadScene(const char* id)
{
	std::string s = id;

	std::map<std::string, CScene*>::iterator i = m_Scene.find(s);
	if (m_Scene.end() == i)
		return false;

	//调用场景的End
	i->second->End(); //多态

	//删除场景
	delete i->second;

	//从场景映射中删除
	m_Scene.erase(i);

	return true;
}

bool CGame::SetFirstScene(const char* id)
{
	std::string s = id;

	std::map<std::string, CScene*>::iterator i = m_Scene.find(s);
	if (m_Scene.end() == i)
		return false;

	//最初的场景只能是设置一次
	if (0 == m_CurScene)
	{
		m_CurScene = i->second;
		return true;
	}
	else
		return false;
}

bool CGame::ChangeScene(const char* id)
{
	if (0 == m_CurScene)
		return false;

	std::string s = id;
	std::map<std::string, CScene*>::iterator i = m_Scene.find(s);
	if (m_Scene.end() == i)
		return false;

	m_CurScene->Quit(); //多态

	m_CurScene = i->second;

	m_CurScene->Enter(); //多态

	return true;
}

CScene* CGame::GetScene(const char* id)
{
	std::string s = id;

	std::map<std::string, CScene*>::iterator i = m_Scene.find(s);

	return m_Scene.end() == i ? 0 : i->second;
}

CCPGameInput* CGame::GetGI()
{
	return m_GI;
}

CGameOutput* CGame::GetGO()
{
	return m_GO;
}

CAudioManager* CGame::GetAM()
{
	return m_AM;
}

CMemoryPool* CGame::GetMP()
{
	return m_MP;
}

CReadFile* CGame::GetRF()
{
	return m_RF;
}

CWriteFile* CGame::GetWF()
{
	return m_WF;
}

void CGame::SetWindowTitle(const char* s)
{
	SetWindowText(m_hWnd, s);
}

void CGame::Exit()
{
	DestroyWindow(m_hWnd);
}