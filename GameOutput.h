#ifndef _GAME_OUTPUT_H_
#define _GAME_OUTPUT_H_

#include <windows.h>
#include <map>
#include <string>

class CGameOutput
{
protected:
	HWND m_hWnd; //���ھ��
	int m_ClientW, m_ClientH; //�ͻ������
	HDC m_MainDC; //���豸
	HDC m_BackDC; //���豸
	HDC m_MirrorDC; //�����豸
	HDC m_DarkDC; //�����豸
	std::map<std::string, HDC> m_Bmp; //λͼӳ��
	std::map<std::string, HFONT> m_Font; //����ӳ��
	unsigned char m_Dark; //����

public:
	CGameOutput(HWND hWnd);
	virtual ~CGameOutput();

	void SetDark(unsigned char Dark);
	unsigned char GetDark();
	void Begin();
	void End();

	//�������

	int LoadBmpDirectory( //��һ��Ŀ¼������ظ�Ŀ¼������λͼ,����λͼ��·����Ϊ��ID
		const char* d); //���ؼ��ص�λͼ������

	bool LoadBmp( //����λͼ
		const char* id, //λͼID
		const char* f); //λͼ�ļ���

	bool UnloadBmp(const char* id); //λͼID

	bool LoadFont( //��������
		const char* id,//����ID
		int w = 0, //�����
		int h = 0, //�����
		int t = 400, //�����ϸ(0~900)
		bool i = false, //�Ƿ�Ϊб����
		bool u = false, //�Ƿ���»���
		const char* f = "����"); //���������

	bool UnloadFont( //ж������
		const char* id); //����ID

	//�������

	bool DrawDot( //���Ƶ�
		int x,
		int y,
		unsigned long c = RGB(0, 0, 0));

	bool DrawLine( //������
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned long c = RGB(0, 0, 0));

	bool DrawRectangle( //���ƾ���
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned long pc = RGB(0, 0, 0), //������ɫ
		unsigned long bc = RGB(255, 255, 255), //��ˢ��ɫ
		bool f = true); //��־(�����û�����ɫ���Ʊ߿�,����ʹ��������ɫ���Ʊ߿�����ڲ�)

	bool DrawEllipse( //����Բ
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned long pc = RGB(0, 0, 0), //������ɫ
		unsigned long bc = RGB(255, 255, 255)); //��ˢ��ɫ

	bool DrawBmp( //����λͼ
		const char* id,
		int dx,
		int dy,
		int dw,
		int dh,
		int sx,
		int sy,
		int m = 0); //0123�ֱ�����������¾������Ҿ����������Ҿ���

	bool DrawBmpT( //��͸��ɫ����λͼ
		const char* id,
		int dx,
		int dy,
		int dw,
		int dh,
		int sx,
		int sy,
		unsigned long tc = RGB(0, 255, 0), //͸��ɫ
		int m = 0); //0123�ֱ�����������¾������Ҿ����������Ҿ���

	bool DrawTextL( //����һ������
		const char* id,
		int x,
		int y,
		const char* t,
		int l,
		unsigned long c = RGB(0, 0, 0));

	bool DrawTextR( //����һ�����η�Χ������
		const char* id,
		int x1,
		int y1,
		int x2,
		int y2,
		const char* t,
		int l,
		unsigned long c = RGB(0, 0, 0));
};

#endif