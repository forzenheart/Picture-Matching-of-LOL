#ifndef _CURSOR_POSITION_GAME_INPUT_H_
#define _CURSOR_POSITION_GAME_INPUT_H_

#include "GameInput.h"

//Cursor Position

class CCPGameInput : public CGameInput
{
public:
	//������ڵ�ǰ�ͻ������򷵻���,��֮���ؼ�
	virtual bool GetCursorPosition(int* pX, int* pY);
};

#endif