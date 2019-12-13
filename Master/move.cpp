//=============================================================================
//	インクルードファイル
//=============================================================================


#include"CCamera.h"
#include "move.h"

//=============================================================================
//	定数定義
//=============================================================================
#define SPEED (0.2f)
//=============================================================================
//	グローバル宣言
//=============================================================================


D3DXMATRIX Move(int direction)
{
	D3DXVECTOR3 right = CCamera::Get_RightVec();
	D3DXMATRIX mtxTranslation;

	right *= SPEED;

	if (direction == FLONT)
	{
		D3DXMatrixTranslation(&mtxTranslation, right.z, right.y, -right.x);
		return mtxTranslation;
	}
	if (direction == BACK)
	{
		D3DXMatrixTranslation(&mtxTranslation, -right.z, right.y, right.x);
		return mtxTranslation;
	}
	if (direction == RIGHT)
	{
		D3DXMatrixTranslation(&mtxTranslation, -right.x, right.y, -right.z);
		return mtxTranslation;
	}
	if (direction == LEFT)
	{
		D3DXMatrixTranslation(&mtxTranslation, right.x, right.y, right.z);
		return mtxTranslation;
	}
	return mtxTranslation;
}

D3DXMATRIX Move(int direction,float speed)
{
	D3DXVECTOR3 right = CCamera::Get_RightVec();
	D3DXMATRIX mtxTranslation;

	right *= speed;

	if (direction == FLONT)
	{
		D3DXMatrixTranslation(&mtxTranslation, right.z, right.y, -right.x);
		return mtxTranslation;
	}
	if (direction == BACK)
	{
		D3DXMatrixTranslation(&mtxTranslation, -right.z, right.y, right.x);
		return mtxTranslation;
	}
	if (direction == RIGHT)
	{
		D3DXMatrixTranslation(&mtxTranslation, -right.x, right.y, -right.z);
		return mtxTranslation;
	}
	if (direction == LEFT)
	{
		D3DXMatrixTranslation(&mtxTranslation, right.x, right.y, right.z);
		return mtxTranslation;
	}
	return mtxTranslation;
}