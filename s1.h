#ifndef _S1_H_
#define _S1_H_

#include "Scene.h"
#include "Game.h"
#include "UI.h"

class button1 : public CUI
{
	bool in;
public:
	button1();

	void OnMouseMessage(_MOUSE_MESSAGE mm, int x, int y);
	void ActiveRender();
	void UnactiveRender();
};

class button2 : public CUI
{
	bool in;
public:
	button2();

	void OnMouseMessage(_MOUSE_MESSAGE mm, int x, int y);
	void ActiveRender();
	void UnactiveRender();
};

class s1 : public CScene
{
	CGame* p;
public:
	int dark;
	bool change;

	void Init();
	void Enter();
	void Run();
	void Quit();
};

#endif