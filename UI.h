#ifndef _UI_H_
#define _UI_H_

//UI(User Interface)
//用户接口,一般是指除了游戏主体元素之外的界面元素,主要
//负责与用户直接进行交换,常见的有按钮、编辑框、滚动条

class CScene;

class CUI
{
	friend class CScene;
	friend class CGame;

protected:
	CScene* scene; //所属场景
	int x1, y1, x2, y2; //位置
	bool show; //是否可见(决定是否绘制出来)
	bool active; //是否激活(决定是否接收消息)

public:
	CUI(int X1, int Y1, int X2, int Y2, bool s, bool a);
	virtual ~CUI();

	//消息种类
	enum _MOUSE_MESSAGE
	{
		//鼠标
		_M_ENTER, //进入
		_M_QUIT, //退出
		_M_MOVE_IN, //在UI内部移动
		_M_MOVE_OUT, //在UI外部移动

		_M_L_DOWN_IN, //左键内部按下
		_M_L_DOWN_OUT, //左键外部按下
		_M_L_UP_IN, //左键内部抬起
		_M_L_UP_OUT, //左键外部抬起

		_M_M_DOWN_IN, //中键内部按下
		_M_M_DOWN_OUT, //中键外部按下
		_M_M_UP_IN, //中键内部抬起
		_M_M_UP_OUT, //中键外部抬起

		_M_R_DOWN_IN, //右键内部按下
		_M_R_DOWN_OUT, //右键外部按下
		_M_R_UP_IN, //右键内部抬起
		_M_R_UP_OUT, //右键外部抬起
	};

	void SetPosition(int X1, int Y1, int X2, int Y2);
	void GetPosition(int* X1, int* Y1, int* X2, int* Y2);
	void SetShow(bool s);
	bool GetShow();
	void SetActive(bool a);
	bool GetActive();
	
	//UI的消息
	virtual void OnMouseMessage(_MOUSE_MESSAGE mm, int x, int y);
	virtual void OnKeyMessage(bool Down, int Key);
	virtual void OnCharMessage(int Char, bool db, bool print, bool ctrl);
	
	//UI的绘制
	virtual void ActiveRender();
	virtual void UnactiveRender();

	//UI的初始化、结束
	virtual void Init();
	virtual void End();
};

#endif