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

			//�е�ǰ����
			if (s)
			{
				std::map<std::string, CUI*>::iterator i;
				for (i = s->m_UI.begin(); i != s->m_UI.end(); ++i)
				{
					CUI* pUI = i->second;

					if (pUI->show && pUI->active)
					{
						RECT r = {pUI->x1, pUI->y1, pUI->x2, pUI->y2};

						//�õ�ǰһ�κ���һ�ι���Ƿ���UI��
						BOOL pre = PtInRect(&r, m_pGame->m_PreCurPos);
						BOOL cur = PtInRect(&r, p);

						if (pre && cur)
							i->second->OnMouseMessage(CUI::_M_MOVE_IN, p.x - pUI->x1, p.y - pUI->y1); //��̬
						else if (pre && !cur)
							i->second->OnMouseMessage(CUI::_M_QUIT, p.x - pUI->x1, p.y - pUI->y1); //��̬
						else if (!pre && cur)
							i->second->OnMouseMessage(CUI::_M_ENTER, p.x - pUI->x1, p.y - pUI->y1); //��̬
						else
							i->second->OnMouseMessage(CUI::_M_MOVE_OUT, p.x - pUI->x1, p.y - pUI->y1); //��̬

						//�������Ķ�̬�����иı��˵�ǰ����,������ѭ����
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

			//�е�ǰ����
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

						//�������Ķ�̬�����иı��˵�ǰ����,������ѭ����
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
				//�ȴ�˫�ֽ����ֵ���һ���ֽ�
				if (m_pGame->m_DoubleByte)
				{
					m_pGame->m_DoubleByte = false;
					m_pGame->m_DoubleByteChar |= (unsigned char)(wParam) << 8;
					m_pGame->m_CurScene->m_KeyUI->OnCharMessage(m_pGame->m_DoubleByteChar, true, true, false);
				}
				else
				{
					//˫�ֽ��ַ�
					if (IsDBCSLeadByte((unsigned char)(wParam)))
					{
						m_pGame->m_DoubleByte = true;
						m_pGame->m_DoubleByteChar = 0;
						m_pGame->m_DoubleByteChar = (unsigned char)(wParam);
					}
					//���ֽ��ַ�
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
	//�õ�Ӧ�ó���ʵ�����
	m_hInstance = GetModuleHandle(0);

	//��䴰�����
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

	//ע�ᴰ�����
	RegisterClass(&m_WndClass);

	//�õ��ͻ�������
	m_ClientW = ClientW;
	m_ClientH = ClientH;

	//�õ�ÿ����Ϸѭ������Ϣʱ��
	m_SleepTime = SleepTime;

	//���㴰�ڵĴ�С
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

	//��������
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

	//��ʼ��ǰһ֡���λ��
	GetCursorPos(&m_PreCurPos);
	ScreenToClient(m_hWnd, &m_PreCurPos);

	//��ʼ�����й���
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
	//������ʼ��
	std::map<std::string, CScene*>::iterator i;
	for (i = m_Scene.begin(); i != m_Scene.end(); ++i)
	{
		i->second->Init(); //��̬

		//��ǰ�����е�UI��ʼ��
		std::map<std::string, CUI*>::iterator j;
		for (j = i->second->m_UI.begin(); j != i->second->m_UI.end(); ++j)
			j->second->Init(); //��̬
	}

	//�������������Enter
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

			//��������
			m_GI->Run();

			//�����ʼ
			m_GO->Begin();

			//��ǰ��������
			if (m_CurScene)
			{
				//�õ���ǰ����
				CScene* p1 = m_CurScene;
				m_CurScene->Run(); //��̬

				//�����ڳ������޸�,���ٵ���һ���³���������
				if (p1 != m_CurScene)
					m_CurScene->Run(); //��̬

				std::map<std::string, CUI*>::iterator i;
				for (i = m_CurScene->m_UI.begin(); i != m_CurScene->m_UI.end(); ++i)
				{
					if (i->second->GetShow())
					{
						if (i->second->GetActive())
							i->second->ActiveRender(); //��̬
						else
							i->second->UnactiveRender(); //��̬
					}
				}
			}

			//�������
			m_GO->End();

			int at = GetTickCount() - bt;
			Sleep(m_SleepTime > at ? m_SleepTime - at : 1);
		}
		else
			WaitMessage();
	}

	//��������
	for (i = m_Scene.begin(); i != m_Scene.end(); ++i)
	{
		//��ǰ�����е�UI����
		std::map<std::string, CUI*>::iterator j;
		for (j = i->second->m_UI.begin(); j != i->second->m_UI.end(); ++j)
			j->second->End(); //��̬

		i->second->End(); //��̬
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

	//���ó�����End
	i->second->End(); //��̬

	//ɾ������
	delete i->second;

	//�ӳ���ӳ����ɾ��
	m_Scene.erase(i);

	return true;
}

bool CGame::SetFirstScene(const char* id)
{
	std::string s = id;

	std::map<std::string, CScene*>::iterator i = m_Scene.find(s);
	if (m_Scene.end() == i)
		return false;

	//����ĳ���ֻ��������һ��
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

	m_CurScene->Quit(); //��̬

	m_CurScene = i->second;

	m_CurScene->Enter(); //��̬

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