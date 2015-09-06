#ifndef _GAME_H_
#define _GAME_H_

#include "CPGameInput.h" //����
#include "GameOutput.h" //���
#include "AudioManager.h" //��Ƶ
#include "MemoryPool.h" //�ڴ�
#include "ReadFile.h" //���ļ�
#include "WriteFile.h" //д�ļ�
#include "Scene.h" //����
#include "UI.h" //UI

#include <windows.h>
#include <string>
#include <map>

class CGame
{
	HINSTANCE m_hInstance; //Ӧ�ó���ʵ�����
	WNDCLASS m_WndClass; //�������
	char m_WndClassName[32]; //���������
	int m_ClientW, m_ClientH; //�ͻ������
	int m_SleepTime; //��Ϣʱ��
	HWND m_hWnd; //����
	BOOL m_Active; //���򼤻��־
	POINT m_PreCurPos; //ǰһ֡���λ��
	bool m_DoubleByte; //�Ƿ���˫�ֽ��ַ�
	int m_DoubleByteChar; //˫�ֽ��ַ�

	CCPGameInput* m_GI; //����
	CGameOutput* m_GO; //����
	CAudioManager* m_AM; //��Ƶ
	CMemoryPool* m_MP; //�ڴ��
	CReadFile* m_RF; //���ļ�
	CWriteFile* m_WF; //д�ļ�

	std::map<std::string, CScene*> m_Scene; //����ӳ��
	CScene* m_CurScene; //��ǰ����

	//ָ���Լ���һ����ָ̬��
	static CGame* m_pGame;

	//��������Ϊ˽��
	CGame();
	~CGame();

	//˽�еĴ�����Ϣ������
	static LRESULT __stdcall WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	static CGame* GetGame();
	
	bool Init(int ClientW = 640, int ClientH = 480, int SleepTime = 33, const char* Title = 0);
	void Run();
	void End();

	//�����Ĺ���
	bool LoadScene(const char* id, CScene* pScene); //���س���
	bool UnloadScene(const char* id); //ж�س���
	bool SetFirstScene(const char* id); //�����������
	bool ChangeScene(const char* id); //�л�����
	CScene* GetScene(const char* id); //��ȡ����

	//��������
	CCPGameInput* GetGI();
	CGameOutput* GetGO();
	CAudioManager* GetAM();
	CMemoryPool* GetMP();
	CReadFile* GetRF();
	CWriteFile* GetWF();
	void SetWindowTitle(const char* s);
	void Exit();
};

#endif