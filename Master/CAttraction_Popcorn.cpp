//////////////////////////////////////////////////
////
////	アトラクション(ポップコーン)クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CAttraction_Popcorn.h"
#include "Cplayer.h"
#include "CEnemy.h"
#include "debug_font.h"
#include "exp.h"
#include "CTexture.h"
#include "sound.h"
//=============================================================================
//	定数定義
//=============================================================================
#define SPEED (0.05f)
#define POPCORN_SIZE (4)//当たり判定大きさ
#define POPCORN_SCALE (1) //モデルの大きさ
#define POPCORN_HP (12)
#define POPCORN_MP (1)
#define POPCORN_ATK (1)
#define SCORE (1)
#define POPCORN_HEAL (1)
#define POPCORN_EREA (10)

//=============================================================================
//	静的変数
//=============================================================================
//	メンバ変数にする　カウントはm_FrameCountでとる　CEnemySmall参考にして

//=============================================================================
//	グローバル変数
//=============================================================================

//=============================================================================
//	生成
//=============================================================================

Popcorn::Popcorn(D3DXMATRIX mtxWorld) :CAttraction(AT_POPCORN), C3DObj(TYPE_POPCORN)
{
	Initialize(mtxWorld);
}

Popcorn::~Popcorn()
{//範囲内にいたらプレイヤー回復
	
}

void Popcorn::Initialize(D3DXMATRIX mtxWorld)
{
	m_AttractionIndex = Get_AttractionIndex(AT_ALL);

	m_Enable = true;
	m_DrawCheck = true;
	CoolTime = 0;//クールタイム
	B_CoolTime;//クールタイムのブール

	B_CoolTime = true;
	m_Hp = POPCORN_HP;
	m_Mp = POPCORN_MP;
	m_Attack = POPCORN_ATK;
	m_DrawCount = 0;
	C3DObj *playerget = CPlayer::Get_Player();
	D3DXMATRIX mtx = playerget->Get_mtxWorld();
	D3DXMatrixTranslation(&m_mtxTranslation, mtxWorld._41, 0, mtxWorld._43);//X,Y,Zを渡す
	D3DXMatrixScaling(&m_mtxScaling, POPCORN_SCALE, POPCORN_SCALE, POPCORN_SCALE);
	m_mtxWorld = m_mtxScaling * m_mtxTranslation;


	Thing_Normal_model = GetNormalModel(MODELL_POPCORN);
	InitSphere(m_pD3DDevice, &Thing_Normal_model, D3DXVECTOR3(0, 0.0, 0), POPCORN_SIZE);//当たり判定の変更
	Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
}

void Popcorn::Update(void)
{
	if (m_Enable)
	{

		if (PlayerCheck())
		{
			
			if (m_FrameCount % 60 == 0)
			{
				PlaySound(POPCORN_SE);
				Add_Hp();
			}
		}
		//エフェクト処理を行う
		if (m_DrawCheck)
		{
			PopcornDamage();

		}

		
	}
}

void Popcorn::Draw(void)
{
	if (m_Enable)
	{
		D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);

		if (!m_DrawCheck)
		{
			if (m_FrameCount % 2 == 0)
			{
				m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
				DrawDX_NormalAdd(m_mtxWorld, MODELL_POPCORN, &Thing_Normal_model, position);
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
			DrawDX_NormalAdd(m_mtxWorld, MODELL_POPCORN, &Thing_Normal_model, position);
		}
	}
}

void Popcorn::Finalize(void)
{
	//Attraction_Finalize(m_AttractionIndex);
	delete this;
}

bool Popcorn::PlayerCheck(void)
{
	C3DObj *pplayer = CPlayer::Get_Player();
	float l = POPCORN_EREA;//範囲
	D3DXMATRIX playerworld = pplayer->Get_mtxWorld();
	float cc = (playerworld._41 - m_mtxWorld._41) * (playerworld._41 - m_mtxWorld._41) + (playerworld._43 - m_mtxWorld._43) *  (playerworld._43 - m_mtxWorld._43);


	return cc < (l * l);
}

void Popcorn::PopcornDamage(void)
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
				Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
				thingenemy = enemy->GetAnimeModel();
				attack = enemy->Get_Attck();
				if (C3DObj::Collision_AnimeVSNormal(thingenemy, &Thing_Normal_model))
				{
					m_Hp -= attack;
					//Animation_Change(PLAYER_WALK, 0.05);
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

C3DObj* Popcorn::Get_Popcorn(int index)
{
	C3DObj *ppopcorn = C3DObj::Get(index);
	if (ppopcorn)
	{
		if (ppopcorn->Get_3DObjType() == C3DObj::TYPE_POPCORN)
		{
			return ppopcorn;
		}
	}
	return NULL;
}