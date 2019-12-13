//////////////////////////////////////////////////
////
////	アトラクション(ジェットコースター)クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CAttraction_Coaster .h"
#include "input.h"
#include "Cplayer.h"
#include "debug_font.h"
#include "time.h"
#include "move.h"
#include "CEnemy.h"
#include "sound.h"
#include "exp.h"
//=============================================================================
//	定数定義
//=============================================================================
//コースターモデルはここで表示 時間管理も
#define SPEED (0.01f)
#define SIZE (0.8f)
#define J_TIME (480)	//使用時間

//仮の数字
#define COSTER_HP (40)
#define COSTER_MP (1)
#define COSTER_ATK (3)
#define SCORE (1)
#define COSTER_SCALE (1)
//=============================================================================
//	静的変数
//=============================================================================

//=============================================================================
//	グローバル変数
//=============================================================================

//=============================================================================
//	生成
//=============================================================================

Coaster::Coaster() :CAttraction(AT_COASTER), C3DObj(TYPE_COASTER)
{
	Initialize();
}

Coaster::~Coaster()
{

}

void Coaster::Initialize()
{
	m_AttractionIndex = Get_AttractionIndex(AT_ALL);
	PlaySound(COASTER_SE);
	m_Enable = true;

	m_TimeKeep = m_FrameCount;

	u = 0;
	v = -90;
	m_Hp = COSTER_HP;
	m_Mp = COSTER_MP;
	m_Attack = COSTER_ATK;

	C3DObj *playerget = CPlayer::Get_Player();
	D3DXMATRIX mtx = playerget->Get_mtxWorld();
	D3DXMatrixTranslation(&m_mtxTranslation, mtx._41, mtx._42, mtx._43);//X,Y,Zを渡す
	D3DXMatrixScaling(&m_mtxScaling, COSTER_SCALE, COSTER_SCALE, COSTER_SCALE);
	m_mtxWorld = m_mtxScaling * m_mtxTranslation;

	Thing_Normal_model = GetNormalModel(MODELL_COASTER);
	InitSphere(m_pD3DDevice, &Thing_Normal_model, D3DXVECTOR3(0, 0.0, 0), 2.2f);//当たり判定の変更
	Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
}

void Coaster::Update(void)
{
	//有効時間を引く

	C3DObj *playerget = CPlayer::Get_Player();	//	プレイヤー取得
	u = (int)(playerget->Get_Angle());
	if (m_FrameCount - m_TimeKeep <= J_TIME)
	{
		C3DObj *playerget = CPlayer::Get_Player();
		D3DXMATRIX playermatrix = playerget->Get_mtxTranslation();

		m_mtxTranslation = playermatrix;
		D3DXMatrixScaling(&m_mtxScaling, COSTER_SCALE, COSTER_SCALE, COSTER_SCALE);
		D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(u + v));
		m_mtxWorld = m_mtxScaling * m_mtxRotation * m_mtxTranslation;

		//if (m_DrawCheck)
		{
			Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
			EnemyDamage();
			OrnamentDamage(Thing_Normal_model);
		}
	}
	else
	{
		C3DObj_delete();
	}

}

void Coaster::Draw(void)
{
	//DebugFont_Draw(600, 30, "Coaster  = %d\n,", *coaster);
	//DebugFont_Draw(600, 60, "CoolTime = %d\n,", endfream);
	//DebugFont_Draw(600, 0, "U = %f\n,", u);
	if (m_Enable)
	{
		
		DrawDX_Normal(m_mtxWorld, MODELL_COASTER, &Thing_Normal_model);
	}
}

void Coaster::Finalize(void)
{
	//Attraction_Finalize(m_AttractionIndex);
	delete this;
}

void Coaster::Coaster_Create(void)
{

}

C3DObj *Coaster::Get_Coaster(void)
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		C3DObj *Coaster = C3DObj::Get(i);
		if (Coaster)
		{
			if (Coaster->Get_3DObjType() == TYPE_COASTER)
			{
				return Coaster;
			}
		}
	}
	return NULL;
}

void Coaster::EnemyDamage(void)
{
	int i;
	C3DObj *enemy;
	THING *thingenemy;
	int hp;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		enemy = CEnemy::Get_Enemy(i);
		if (enemy)
		{
			if (enemy->Get_DrawCheck())
			{
				Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
				thingenemy = enemy->GetAnimeModel();
				hp = enemy->Get_Hp();
				if (C3DObj::Collision_AnimeVSNormal(thingenemy, &Thing_Normal_model))
				{
					Exp_Set(HIT, thingenemy->vPosition.x, thingenemy->vPosition.y, thingenemy->vPosition.z, 4.0, 0);
					enemy->DamageFlag_Change();
					enemy->Position_Keep(m_mtxWorld);
				}
			}
		}
	}
}