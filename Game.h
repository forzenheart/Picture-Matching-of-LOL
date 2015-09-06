#ifndef _GAME_H_
#define _GAME_H_

#include "CPGameInput.h" //输入
#include "GameOutput.h" //输出
#include "AudioManager.h" //音频
#include "MemoryPool.h" //内存
#include "ReadFile.h" //读文件
#include "WriteFile.h" //写文件
#include "Scene.h" //场景
#include "UI.h" //UI

#include <windows.h>
#include <string>
#include <map>

class CGame
{
	HINSTANCE m_hInstance; //应用程序实例句柄
	WNDCLASS m_WndClass; //窗口类别
	char m_WndClassName[32]; //窗口类别名
	int m_ClientW, m_ClientH; //客户区宽高
	int m_SleepTime; //休息时间
	HWND m_hWnd; //窗口
	BOOL m_Active; //程序激活标志
	POINT m_PreCurPos; //前一帧光标位置
	bool m_DoubleByte; //是否是双字节字符
	int m_DoubleByteChar; //双字节字符

	CCPGameInput* m_GI; //输入
	CGameOutput* m_GO; //输入
	CAudioManager* m_AM; //音频
	CMemoryPool* m_MP; //内存池
	CReadFile* m_RF; //读文件
	CWriteFile* m_WF; //写文件

	std::map<std::string, CScene*> m_Scene; //场景映射
	CScene* m_CurScene; //当前场景

	//指向自己的一个静态指针
	static CGame* m_pGame;

	//构造设置为私有
	CGame();
	~CGame();

	//私有的窗口消息处理函数
	static LRESULT __stdcall WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	static CGame* GetGame();
	
	bool Init(int ClientW = 640, int ClientH = 480, int SleepTime = 33, const char* Title = 0);
	void Run();
	void End();

	//场景的管理
	bool LoadScene(const char* id, CScene* pScene); //加载场景
	bool UnloadScene(const char* id); //卸载场景
	bool SetFirstScene(const char* id); //设置最初场景
	bool ChangeScene(const char* id); //切换场景
	CScene* GetScene(const char* id); //获取场景

	//其它函数
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