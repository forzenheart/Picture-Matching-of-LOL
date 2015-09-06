#ifndef _UI_H_
#define _UI_H_

//UI(User Interface)
//�û��ӿ�,һ����ָ������Ϸ����Ԫ��֮��Ľ���Ԫ��,��Ҫ
//�������û�ֱ�ӽ��н���,�������а�ť���༭�򡢹�����

class CScene;

class CUI
{
	friend class CScene;
	friend class CGame;

protected:
	CScene* scene; //��������
	int x1, y1, x2, y2; //λ��
	bool show; //�Ƿ�ɼ�(�����Ƿ���Ƴ���)
	bool active; //�Ƿ񼤻�(�����Ƿ������Ϣ)

public:
	CUI(int X1, int Y1, int X2, int Y2, bool s, bool a);
	virtual ~CUI();

	//��Ϣ����
	enum _MOUSE_MESSAGE
	{
		//���
		_M_ENTER, //����
		_M_QUIT, //�˳�
		_M_MOVE_IN, //��UI�ڲ��ƶ�
		_M_MOVE_OUT, //��UI�ⲿ�ƶ�

		_M_L_DOWN_IN, //����ڲ�����
		_M_L_DOWN_OUT, //����ⲿ����
		_M_L_UP_IN, //����ڲ�̧��
		_M_L_UP_OUT, //����ⲿ̧��

		_M_M_DOWN_IN, //�м��ڲ�����
		_M_M_DOWN_OUT, //�м��ⲿ����
		_M_M_UP_IN, //�м��ڲ�̧��
		_M_M_UP_OUT, //�м��ⲿ̧��

		_M_R_DOWN_IN, //�Ҽ��ڲ�����
		_M_R_DOWN_OUT, //�Ҽ��ⲿ����
		_M_R_UP_IN, //�Ҽ��ڲ�̧��
		_M_R_UP_OUT, //�Ҽ��ⲿ̧��
	};

	void SetPosition(int X1, int Y1, int X2, int Y2);
	void GetPosition(int* X1, int* Y1, int* X2, int* Y2);
	void SetShow(bool s);
	bool GetShow();
	void SetActive(bool a);
	bool GetActive();
	
	//UI����Ϣ
	virtual void OnMouseMessage(_MOUSE_MESSAGE mm, int x, int y);
	virtual void OnKeyMessage(bool Down, int Key);
	virtual void OnCharMessage(int Char, bool db, bool print, bool ctrl);
	
	//UI�Ļ���
	virtual void ActiveRender();
	virtual void UnactiveRender();

	//UI�ĳ�ʼ��������
	virtual void Init();
	virtual void End();
};

#endif