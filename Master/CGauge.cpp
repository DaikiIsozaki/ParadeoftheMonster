//////////////////////////////////////////////////
////
////	アトラクション(建設中)クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CGauge.h"
#include "input.h"
#include "Cplayer.h"
#include "debug_font.h"
#include "time.h"
#include "move.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CAttraction_Standby.h"
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	定数定義
//=============================================================================
#define SPEED (0.05f)
#define SIZE (1.0f)
#define END_TIME (120)	//待機時間	
#define BAR_WIDTH (300)
//=============================================================================
//	静的変数
//=============================================================================


#define FVF_CUBE_VERTEX3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//	構造体のすぐ下に書く

//=============================================================================
//	グローバル変数
//=============================================================================

//=============================================================================
//	生成
//=============================================================================

CGAUGE::CGAUGE(D3DXMATRIX mtxWorld) :CAttraction(AT_GAUGE), C3DObj(TYPE_STADBY)
{
	Initialize(mtxWorld);
	
}

CGAUGE::~CGAUGE()
{

}

void CGAUGE::Initialize(D3DXMATRIX mtxWorld)
{
	 
	g_bb[0] = { D3DXVECTOR3(-3, 0.5,0.0),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(0.0,0.0) };
	g_bb[1] = { D3DXVECTOR3(3, 0.5,0.),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(1.0,0.0) };
	g_bb[2] = { D3DXVECTOR3(-3,-0.5,0.0),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(0.0,1.0) };
	g_bb[3] = { D3DXVECTOR3(3,-0.5,0.0),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(1.0,1.0) };

	m_AttractionIndex = Get_AttractionIndex(AT_ALL);

	m_Enable = true;
	m_TimeKeep = m_FrameCount;
	bar_width = 0.0f;

	C3DObj *standbyget = Standby::Get_Standby();
	//D3DXMATRIX mtx = standbyget->Get_mtxWorld();
	//D3DXMatrixTranslation(&m_mtxTranslation, mtx._41, mtx._42+5.0f, mtx._43);//X,Y,Zを渡す
	
	D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(0));
	m_mtxWorld = m_mtxRotation * m_mtxTranslation;
	D3DXMatrixTranslation(&m_mtxTranslation, mtxWorld._41, mtxWorld._42 + 5.0f, mtxWorld._43);
}

void CGAUGE::Update(void)
{
	if (m_FrameCount - m_TimeKeep <= END_TIME)
	{
		bar_width += 0.022f;
	}
	else
	{
		C3DObj_delete();
	}
	
}

void CGAUGE::Draw(void)
{
	if (m_Enable)
	{
	//	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		WhiteGaugeDraw();
		UpdateGaugeDraw();
		GaugeDraw();
	//	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
}

void CGAUGE::GaugeDraw(void)
{
	D3DXMatrixIdentity(&m_mtxWorld);

	g_bb[0] = { D3DXVECTOR3(-3, 0.5,0.0),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(0.0,0.0) };
	g_bb[1] = { D3DXVECTOR3(3, 0.5,0.),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(1.0,0.0) };
	g_bb[2] = { D3DXVECTOR3(-3,-0.5,0.0),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(0.0,1.0) };
	g_bb[3] = { D3DXVECTOR3(3,-0.5,0.0),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(1.0,1.0) };

	D3DXMATRIX mtxView = CCamera::Get_ViewMtx();

	D3DXMATRIX mtxInvV;
	mtxView._41 = 0.0f;
	mtxView._42 = 0.0f;
	mtxView._43 = 0.0f;
	D3DXMatrixInverse(&mtxInvV, NULL, &mtxView);

	//D3DXMatrixTranslation(&m_mtxTranslation, 0.0f, 5.0f, 0.0f);//平行
	D3DXMatrixScaling(&m_mtxScaling, 1, 1.0f, 1);
	m_mtxWorld = m_mtxScaling * m_mtxTranslation;
	m_mtxWorld = (mtxInvV * m_mtxWorld);
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CGameObj::m_pD3DDevice->SetFVF(FVF_CUBE_VERTEX3D);		//	FVFをデバイスに設定
	CGameObj::m_pD3DDevice->SetTexture(0, CTexture::Texture_GetTexture(CTexture::TEX_GAUGE));
	CGameObj::m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	CGameObj::m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &g_bb, sizeof(BillBoardVertex3D));
}

void CGAUGE::UpdateGaugeDraw(void)
{
	D3DXMatrixIdentity(&m_mtxWorld);
	g_bb[0] = { D3DXVECTOR3(-bar_width, 0.5,0.0),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(0.0,0.0) };
	g_bb[1] = { D3DXVECTOR3(bar_width, 0.5,0.),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(1.0f / bar_width,0.0) };
	g_bb[2] = { D3DXVECTOR3(-bar_width,-0.5,0.0),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(0.0,1.0f / bar_width) };
	g_bb[3] = { D3DXVECTOR3(bar_width,-0.5,0.0),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(1.0f / bar_width,1.0f / bar_width) };
	D3DXMATRIX mtxView = CCamera::Get_ViewMtx();

	D3DXMATRIX mtxInvV;
	mtxView._41 = 0.0f;
	mtxView._42 = 0.0f;
	mtxView._43 = 0.0f;
	D3DXMatrixInverse(&mtxInvV, NULL, &mtxView);

//	D3DXMatrixTranslation(&m_mtxTranslation, 0.0f, 5.0f, 0.0f);//平行
	D3DXMatrixScaling(&m_mtxScaling, 1.0f, 1.0f, 1);
	m_mtxWorld = m_mtxScaling * m_mtxTranslation;
	m_mtxWorld = (mtxInvV * m_mtxWorld);
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CGameObj::m_pD3DDevice->SetFVF(FVF_CUBE_VERTEX3D);		//	FVFをデバイスに設定
	CGameObj::m_pD3DDevice->SetTexture(0, CTexture::Texture_GetTexture(CTexture::TEX_HP2));
	CGameObj::m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	CGameObj::m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &g_bb, sizeof(BillBoardVertex3D));
}

void CGAUGE::WhiteGaugeDraw(void)
{
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMATRIX mtxView = CCamera::Get_ViewMtx();
	g_bb[0] = { D3DXVECTOR3(-3, 0.5,0.0),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(0.0,0.0) };
	g_bb[1] = { D3DXVECTOR3(3, 0.5,0.),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(1.0,0.0) };
	g_bb[2] = { D3DXVECTOR3(-3,-0.5,0.0),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(0.0,1.0) };
	g_bb[3] = { D3DXVECTOR3(3,-0.5,0.0),D3DXVECTOR3(0.0,0.0,0.0),D3DCOLOR(0xffffffff),D3DXVECTOR2(1.0,1.0) };
	D3DXMATRIX mtxInvV;
	mtxView._41 = 0.0f;
	mtxView._42 = 0.0f;
	mtxView._43 = 0.0f;
	D3DXMatrixInverse(&mtxInvV, NULL, &mtxView);

	//D3DXMatrixTranslation(&m_mtxTranslation, 0.0f, 5.0f, 0.0f);//平行
	D3DXMatrixScaling(&m_mtxScaling, 1, 1.0f, 1);
	m_mtxWorld = m_mtxScaling * m_mtxTranslation;
	m_mtxWorld = (mtxInvV * m_mtxWorld);
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CGameObj::m_pD3DDevice->SetFVF(FVF_CUBE_VERTEX3D);		//	FVFをデバイスに設定
	CGameObj::m_pD3DDevice->SetTexture(0, CTexture::Texture_GetTexture(CTexture::TEX_WHITE_BAR));
	CGameObj::m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	CGameObj::m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &g_bb, sizeof(BillBoardVertex3D));
}

void CGAUGE::Finalize(void)
{
	C3DObj_delete();
}

void CGAUGE::CGAUGE_Create(D3DXMATRIX m_mtxWorld)
{
	CGAUGE *m_pAttraction = new CGAUGE(m_mtxWorld);
	
}



C3DObj *CGAUGE::Get_CGAUGE(void)
{
	int i;
	C3DObj *CGAUGE;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		CGAUGE = C3DObj::Get(i);
		if (CGAUGE)
		{
			if (CGAUGE->Get_3DObjType() == AT_GAUGE)
			{
				return CGAUGE;
			}
		}
	}
	return NULL;
}