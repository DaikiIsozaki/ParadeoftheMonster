#ifndef _BSPHERE_H_
#define _BSPHERE_H_
#include <windows.h>
#include <d3dx9.h>
#include "CSkinAnimation.h"
//必要なライブラリファイルのロード
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
//ユーザー定義データ型
struct SPHERE
{
	D3DXVECTOR3 vCenter;
	FLOAT fRadius;
};
struct CAPSULE
{
	D3DXVECTOR3 vCenter;
	FLOAT fRadius;
	FLOAT fRadius2;
	FLOAT fLength;
};

struct THING_NORMAL
{
	LPD3DXMESH pMesh;
	LPD3DXMESH pSphereMesh;
	D3DMATERIAL9* pMeshMaterials;
	D3DMATERIAL9* pSphereMeshMaterials;
	LPDIRECT3DTEXTURE9* pMeshTextures;
	DWORD dwNumMaterials;
	D3DXVECTOR3 vPosition;
	D3DXMATRIX mRotation;
	D3DXMATRIX mWorld;
	SPHERE Sphere;
	CAPSULE Capsule;
	FLOAT fYaw;
	FLOAT fPitch;

	THING_NORMAL()
	{
		ZeroMemory(this, sizeof(THING_NORMAL));
		D3DXMatrixIdentity(&mRotation);
		D3DXMatrixIdentity(&mWorld);
	}
};

HRESULT InitSphere(LPDIRECT3DDEVICE9, THING_NORMAL*);
HRESULT InitSphere(LPDIRECT3DDEVICE9 pDevice, THING_NORMAL* pThing, D3DXVECTOR3 vCenter, FLOAT fRadius);
HRESULT InitCapsule(LPDIRECT3DDEVICE9 pDevice, THING_NORMAL* pThing, D3DXVECTOR3 vCenter, FLOAT fRadius, FLOAT length);
#endif