#ifndef _GAME_OUTPUT_H_
#define _GAME_OUTPUT_H_

#include <windows.h>
#include <map>
#include <string>

class CGameOutput
{
protected:
	HWND m_hWnd; //窗口句柄
	int m_ClientW, m_ClientH; //客户区宽高
	HDC m_MainDC; //主设备
	HDC m_BackDC; //后备设备
	HDC m_MirrorDC; //镜像设备
	HDC m_DarkDC; //暗度设备
	std::map<std::string, HDC> m_Bmp; //位图映射
	std::map<std::string, HFONT> m_Font; //字体映射
	unsigned char m_Dark; //暗度

public:
	CGameOutput(HWND hWnd);
	virtual ~CGameOutput();

	void SetDark(unsigned char Dark);
	unsigned char GetDark();
	void Begin();
	void End();

	//加载相关

	int LoadBmpDirectory( //从一个目录下面加载该目录的所有位图,并用位图的路径作为其ID
		const char* d); //返回加载的位图的数量

	bool LoadBmp( //加载位图
		const char* id, //位图ID
		const char* f); //位图文件名

	bool UnloadBmp(const char* id); //位图ID

	bool LoadFont( //加载字体
		const char* id,//字体ID
		int w = 0, //字体宽
		int h = 0, //字体高
		int t = 400, //字体粗细(0~900)
		bool i = false, //是否为斜体字
		bool u = false, //是否带下划线
		const char* f = "宋体"); //字体外观名

	bool UnloadFont( //卸载字体
		const char* id); //字体ID

	//绘制相关

	bool DrawDot( //绘制点
		int x,
		int y,
		unsigned long c = RGB(0, 0, 0));

	bool DrawLine( //绘制线
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned long c = RGB(0, 0, 0));

	bool DrawRectangle( //绘制矩形
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned long pc = RGB(0, 0, 0), //画笔颜色
		unsigned long bc = RGB(255, 255, 255), //画刷颜色
		bool f = true); //标志(假则用画笔颜色绘制边框,真则使用两种颜色绘制边框填充内部)

	bool DrawEllipse( //绘制圆
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned long pc = RGB(0, 0, 0), //画笔颜色
		unsigned long bc = RGB(255, 255, 255)); //画刷颜色

	bool DrawBmp( //绘制位图
		const char* id,
		int dx,
		int dy,
		int dw,
		int dh,
		int sx,
		int sy,
		int m = 0); //0123分别代表不镜像、上下镜像、左右镜像、上下左右镜像

	bool DrawBmpT( //带透明色绘制位图
		const char* id,
		int dx,
		int dy,
		int dw,
		int dh,
		int sx,
		int sy,
		unsigned long tc = RGB(0, 255, 0), //透明色
		int m = 0); //0123分别代表不镜像、上下镜像、左右镜像、上下左右镜像

	bool DrawTextL( //绘制一行文字
		const char* id,
		int x,
		int y,
		const char* t,
		int l,
		unsigned long c = RGB(0, 0, 0));

	bool DrawTextR( //绘制一个矩形范围的文字
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