#ifndef _S2_H_
#define _S2_H_

#include "Scene.h"
#include "Game.h"
#include "UI.h"

class YDJP : public CUI
{
	bool in;
	int qu;
public:
	YDJP();

	void OnMouseMessage(_MOUSE_MESSAGE mm, int x, int y);
	void ActiveRender();
	void UnactiveRender();
};

class Edit1 : public CUI
{
	bool input;
	static const int max = 16;
	char str[32];
	int len;
public:
	Edit1();
	void Init();
	void OnMouseMessage(_MOUSE_MESSAGE mm, int x, int y);
	void OnCharMessage(int Char, bool db, bool print, bool ctrl);
	void ActiveRender();
	void UnactiveRender();
	void SetText(const char* s);
	char* GetText();
};

class s2 : public CScene
{
	CGame* p;
	int dark;
public:
	int x, y; //ÓÎÏ·Êý¾Ý

	void Init();
	void Enter();
	void Run();
	void Quit();
};

#endif