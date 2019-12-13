//////////////////////////////////////////////////
////
////	ライトクラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CLight.h"
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

//=============================================================================
//	定数定義
//=============================================================================


//=============================================================================
//	静的変数
//=============================================================================

CLight *CLight::m_pLight;


//=============================================================================
//	生成
//=============================================================================
CLight::CLight():CGameObj(CGameObj::TYPE_LIGHT)
{
	Light_Initialize();
}

//=============================================================================
//	破棄
//=============================================================================
CLight::~CLight()
{
	//Light_Finalize();
}



void CLight::Update(void)
{
	
}

void CLight::Draw(void)
{
	Light_Set();
}

//	ライト初期化
void CLight::Light_Initialize(void)
{
	m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);	// 法線ベクトルを1にする
}

CLight* CLight::Light_Create(void)
{
	m_pLight = new CLight;
	return m_pLight;
}

void CLight::Finalize(void)
{
	if (m_pLight != NULL)
	{
		delete m_pLight;
	}
}

//	ライトセット
void CLight::Light_Set(void)
{
	// ライティング
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);	//　ライティング有効
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(77, 67, 65, 255));	// アンビエント

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;	//　平行高原

	D3DXVECTOR3 vecDirLight(0.0f, 0.0f, 1.0f);	//　ライトベクトル
	D3DXVec3Normalize(&vecDirLight, &vecDirLight); //　長さを1にする　正規化

	light.Direction = vecDirLight;
	light.Diffuse.r = 1.0f;	//　ライト色　ディフューズ
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	m_pD3DDevice->SetLight(0, &light);	//　第一引数　ソケット番号
	m_pD3DDevice->LightEnable(0, TRUE);	//　第一引数　ソケット番号

	//g_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);	//	マテリアル設定
	//g_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
	/*
	ライトから
	g_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
	を消す
	キューブのDrowの先頭で呼ぶ

	モデルのDrowの先頭で呼ぶ
	g_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);

	グリッドのDrowの先頭で
	g_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);

	*/
}









