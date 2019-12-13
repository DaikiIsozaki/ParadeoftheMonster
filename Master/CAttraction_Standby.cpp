//////////////////////////////////////////////////
////
////	アトラクション(建設中)クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CAttraction_Standby.h"
#include "Ccoffee_cup.h"
#include "CAttraction_Popcorn.h"
#include "Cfreefall.h"
#include "input.h"
#include "Cplayer.h"
#include "debug_font.h"
#include "time.h"
#include "move.h"
#include "CGauge.h"
#include "sound.h"
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	定数定義
//=============================================================================
#define SPEED (0.05f)
#define SIZE (0.8f)
#define END_TIME (120)	//待機時間	

//仮の数字
#define STANDBY_HP (40)
#define STANDBY_MP (1)
#define STANDBY_ATK (3)
#define SCORE (1)
#define STANDBY_SCALE (1)
//=============================================================================
//	静的変数
//=============================================================================

//=============================================================================
//	グローバル変数
//=============================================================================

//=============================================================================
//	生成
//=============================================================================

Standby::Standby(int nType) :CAttraction(AT_STANDBY), C3DObj(TYPE_STADBY)
{
	Initialize(nType);
	
}

Standby::~Standby()
{

}

void Standby::Initialize(int nType)
{
	m_AttractionIndex = Get_AttractionIndex(AT_ALL);

	m_Enable = true;

	m_TimeKeep = m_FrameCount;

	attraction_type = nType;//どのアトラクションを出すか格納
	C3DObj *playerget = CPlayer::Get_Player();
	D3DXMATRIX mtx = playerget->Get_mtxWorld();
	D3DXMatrixTranslation(&m_mtxTranslation, mtx._41, 0, mtx._43);//X,Y,Zを渡す
	D3DXMatrixScaling(&m_mtxScaling, STANDBY_SCALE, STANDBY_SCALE, STANDBY_SCALE);
	D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(90));
	m_mtxWorld = m_mtxRotation * m_mtxScaling * m_mtxTranslation;

	Thing_Normal_model = GetNormalModel(MODELL_STANDBY);
	CGAUGE::CGAUGE_Create(m_mtxWorld);
	PlaySound(KENSETU_SE);
}

void Standby::Update(void)
{
	if (m_FrameCount - m_TimeKeep >= END_TIME)
	{
		if (attraction_type == AT_COFFEE)
		{
			CCoffeeCup *m_pAttraction = new CCoffeeCup(m_mtxWorld);
		}
		if (attraction_type == AT_FALL)
		{
			Cfreefall *m_pAttraction = new Cfreefall(m_mtxWorld);
		}
		if (attraction_type == AT_POPCORN)
		{
			Popcorn *m_pAttraction = new Popcorn(m_mtxWorld);
		}
		StopSound(KENSETU_SE);
		C3DObj_delete();
	}
}

void Standby::Draw(void)
{
	//DebugFont_Draw(600, 30, "Coaster  = %d\n,", *coaster);
	//DebugFont_Draw(600, 60, "CoolTime = %d\n,", endfream);
	//DebugFont_Draw(600, 0, "U = %f\n,", u);
	if (m_Enable)
	{
		Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		DrawDX_Normal(m_mtxWorld, MODELL_STANDBY, &Thing_Normal_model);
	}
}

void Standby::Finalize(void)
{
	//Attraction_Finalize(m_AttractionIndex);
	delete this;
}

void Standby::Standby_Create(void)
{

}



C3DObj *Standby::Get_Standby(void)
{
	int i;
	C3DObj *Standby;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		Standby = C3DObj::Get(i);
		if (Standby)
		{
			if (Standby->Get_3DObjType() == AT_STANDBY)
			{
				return Standby;
			}
		}
	}
	return NULL;
}