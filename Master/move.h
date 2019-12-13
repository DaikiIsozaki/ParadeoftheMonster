#ifndef _MOVE_H_
#define	_MOVE_H_
#include <d3dx9.h>


D3DXMATRIX Move(int direction);
D3DXMATRIX Move(int direction, float speed);

enum direction {
	FLONT,
	BACK,
	RIGHT,
	LEFT


};


#endif // 

