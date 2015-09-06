#ifndef _CURSOR_POSITION_GAME_INPUT_H_
#define _CURSOR_POSITION_GAME_INPUT_H_

#include "GameInput.h"

//Cursor Position

class CCPGameInput : public CGameInput
{
public:
	//若光标在当前客户区中则返回真,反之返回假
	virtual bool GetCursorPosition(int* pX, int* pY);
};

#endif