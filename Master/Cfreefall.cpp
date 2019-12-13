//////////////////////////////////////////////////
////
////	アトラクション(フリーフォール)クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "Cfreefall.h"
#include "Cplayer.h"
#include "debug_font.h"
#include "BSphere.h"
#include "CEnemy.h"
#include "CTexture.h"
#include "exp.h"
#include "sound.h"
//=============================================================================
//	定数定義
//=============================================================================
#define SPEED (0.2f)
#define SIZE (0.8f)
#define FREEFALL_HP (18)
#define FREEFALL_MP (1)
#define FREEFALL_ATK (3)
#define SCORE (1)
#define FREEFALL_SCALE (1)
#define ATTACK_TIME (5)//円盤が落ちてからの攻撃時間


//=============================================================================
//	静的変数
//=============================================================================

#define FREEFALL_UP		(25.0f)
#define FREEFALL_DOWN	(0.0f)
#define WAIT_TIME (50)//フリーフォールが落ちる時間
THING_NORMAL fall;
//=============================================================================
//	グローバル変数
//=============================================================================

//=============================================================================
//	生成
//=============================================================================
Cfreefall::Cfreefall(D3DXMATRIX mtxWorld) :CAttraction(AT_FALL), C3DObj(TYPE_ATTRACTION)
{
	Initialize(mtxWorld);
}

Cfreefall::~Cfreefall()
{

}

void Cfreefall::Initialize(D3DXMATRIX mtxWorld)
{
	m_AttractionIndex = Get_AttractionIndex(AT_ALL);
	m_Enable = true;
	Bugoki = false;
	attackon = false;
	ugoki = 0.0f;
	CoolTime = 0;
	moveY = 0.0f;
	m_Hp = FREEFALL_HP;
	m_Mp = FREEFALL_MP;
	m_Attack = FREEFALL_ATK;
	m_DrawCount = 0;
	C3DObj *playerget = CPlayer::Get_Player();
	D3DXMATRIX mtx = playerget->Get_mtxWorld();
	D3DXMatrixTranslation(&m_mtxTranslation, mtxWorld._41, 0, mtxWorld._43);//X,Y,Zを渡す
	D3DXMatrixScaling(&m_mtxScaling, FREEFALL_SCALE, FREEFALL_SCALE, FREEFALL_SCALE);
	m_mtxWorld = m_mtxScaling * m_mtxTranslation;
	Thing_Normal_model = GetNormalModel(MODELL_HASIRA);
	fall = GetNormalModel(MODELL_ENBAN);
	Thing_Normal_model.Sphere.vCenter = D3DXVECTOR3(0, 0.0, 0);
	InitSphere(m_pD3DDevice, &Thing_Normal_model, D3DXVECTOR3(0, 0.0, 0), 8.2f);//当たり判定の変更
	InitSphere(m_pD3DDevice, &fall, D3DXVECTOR3(0, 0.0, 0), 1.0f);//当たり判定の変更
	Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
	fall.vPosition = D3DXVECTOR3(m_mtxWorld2._41, m_mtxWorld2._42, m_mtxWorld2._43);
}

void Cfreefall::Update(void)
{
	if (m_Enable)
	{//ここで移動計算を行う
		D3DXMatrixTranslation(&m_mtxTranslation, m_mtxWorld._41, ugoki, m_mtxWorld._43);
		D3DXMatrixScaling(&m_mtxScaling, FREEFALL_SCALE, FREEFALL_SCALE, FREEFALL_SCALE);
		D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(0));
		m_mtxWorld = m_mtxScaling *m_mtxRotation * m_mtxTranslation;
		if (!Bugoki)//上がってるとき	
		{
			if (ugoki <= FREEFALL_UP)
			{
				if (ugoki == 0)
				{
					PlaySound(FREE_WALL_UP_SE);
				}
				ugoki += SPEED;
			}
			if (ugoki >= FREEFALL_UP)
			{
				CoolTime++;
				if (CoolTime >= WAIT_TIME)
				{
					StopSound(FREE_WALL_UP_SE);
					Bugoki = true;
					attackon = true;
					CoolTime = 0;
				}
			}
		}
		if (Bugoki)
		{
			if (ugoki >= 0.0f)
			{
				ugoki -= 0.5f;
			}
			if (ugoki <= 0.0f)
			{
				CoolTime++;
				if (CoolTime == ATTACK_TIME)
				{
					PlaySound(FREEFALL_SE);
					attackon = false;
					Exp_Set(IMPACT_13, m_mtxWorld._41, m_mtxWorld._42 + 1, m_mtxWorld._43, 6.0, 0);
				}
				if (CoolTime >= WAIT_TIME)
				{
					Bugoki = false;
					CoolTime = 0;
				}
			}
		}

		D3DXMatrixTranslation(&m_mtxTranslation, m_mtxWorld._41, 0, m_mtxWorld._43);
		D3DXMatrixScaling(&m_mtxScaling, FREEFALL_SCALE, FREEFALL_SCALE, FREEFALL_SCALE);
		D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(0));
		m_mtxWorld2 = m_mtxScaling * m_mtxRotation * m_mtxTranslation;

		Thing_Normal_model.Sphere.vCenter = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
		Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
		fall.vPosition = D3DXVECTOR3(m_mtxWorld2._41, m_mtxWorld2._42, m_mtxWorld2._43);

		if (attackon)
		{
			fall.vPosition = D3DXVECTOR3(m_mtxWorld2._41, m_mtxWorld2._42, m_mtxWorld2._43);
			OrnamentDamage(Thing_Normal_model);

			EnemyDamage();
		}
		if (m_DrawCheck)
		{
			FreeFallDamage();
		}
	}
}

void Cfreefall::Draw(void)
{
	if (m_Enable)
	{
		D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		
		if (!m_DrawCheck)
		{
			if (m_FrameCount % 2 == 0)
			{
				m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
				DrawDX_Normal(m_mtxWorld, MODELL_ENBAN, &fall);
				DrawDX_NormalAdd(m_mtxWorld2, MODELL_HASIRA, &Thing_Normal_model, position);

				m_DrawCount++;
				if (m_DrawCount >= ATTRACITION_WAIT_TIME)
				{
					m_DrawCount = 0;
					m_DrawCheck = true;
				}
			}
		}
		else
		{
			m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			DrawDX_Normal(m_mtxWorld, MODELL_ENBAN, &fall);
			DrawDX_NormalAdd(m_mtxWorld2, MODELL_HASIRA, &Thing_Normal_model, position);
		}
	}
}

void Cfreefall::Finalize(void)
{
	//Attraction_Finalize(m_AttractionIndex);
	delete this;
}

void Cfreefall::EnemyDamage(void)
{
	int i;
	THING *thingenemy;
	C3DObj *enemy;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		enemy = CEnemy::Get_Enemy(i);
		if (attackon && enemy)
		{
			Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
			thingenemy = enemy->GetAnimeModel();
			if (C3DObj::Collision_AnimeVSNormal(thingenemy, &Thing_Normal_model))
			{
				enemy->DamageFlag_Change();
				enemy->Position_Keep(m_mtxWorld);

			}
		}
	}
}

void Cfreefall::FreeFallDamage(void)
{
	int i;
	C3DObj *enemy;
	THING *thingenemy;
	int attack;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		enemy = CEnemy::Get_Enemy(i);

		if (enemy && m_DrawCheck)
		{
			if (enemy->Get_AttacFlag())
			{
				fall.vPosition = D3DXVECTOR3(m_mtxWorld2._41, m_mtxWorld2._42, m_mtxWorld2._43);
				thingenemy = enemy->GetAnimeModel();
				attack = enemy->Get_Attck();
				if (C3DObj::Collision_AnimeVSNormal(thingenemy, &fall))
				{
					m_Hp -= attack;
					m_DrawCheck = false;
					if (m_Hp <= 0)
					{
						Exp_Set(SMALLSTAR, m_mtxWorld._41, m_mtxWorld._42 + 1, m_mtxWorld._43, 5.0, 0);
						C3DObj_delete();
					}
					break;
				}
			}
		}
	}
}