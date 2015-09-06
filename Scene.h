#ifndef _SCENE_H_
#define _SCENE_H_

#include <string>
#include <map>

class CUI;

class CScene
{
	friend class CGame;

protected:
	std::map<std::string, CUI*> m_UI; //UI映射
	CUI* m_KeyUI; //拥有键盘焦点的UI

public:

	//UI相关操作
	bool LoadUI(const char* id, CUI* pUI); //加载UI
	bool ReleaseUI(const char* id); //卸载UI
	CUI* GetUI(const char* id); //获取UI
	bool SetKeyUI(const char* id = 0); //设置键盘焦点UI

	CScene();
	virtual ~CScene();

	virtual void Init();
	virtual void Enter();
	virtual void Run();
	virtual void Quit();
	virtual void End();
};

//class CChangeScene : public CScene
//{
//	int darkstate; //明暗状态
//	int dark; //明暗值
//
//public:
//
//	void Enter()
//	{
//		darkstate = 1;
//		dark = 255;
//		_Enter();
//	}
//
//	void Quit()
//	{
//		darkstate = -1;
//		dark = 0;
//		_Quit();
//	}
//
//	virtual void _Enter(){}
//	virtual void _Quit(){}
//
//	virtual void _Render(){}
//	virtual void _Logic(){}
//	virtual void _Input(){}
//
//	void Run()
//	{
//		_Render();
//		if (darkstate == 0)
//		{
//			_Logic();
//			_Input();
//		}
//		if (darkstate == 1)
//		{
//			CGame::GetGame()->GetGO()->SetDark(dark);
//			dark -= 5;
//			if (dark <= 0)
//			{
//				dark = 0;
//				darkstate = 0;
//			}
//		}
//		else if (darkstate == -1)
//		{
//			CGame::GetGame()->GetGO()->SetDark(dark);
//			dark += 5;
//			if (dark >= 255)
//			{
//				dark = 255;
//				darkstate = 0;
//			}
//		}
//	}
//};

#endif