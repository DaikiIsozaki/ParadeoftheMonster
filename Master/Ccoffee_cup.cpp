//////////////////////////////////////////////////
////
////	アトラクション(コーヒーカップ)クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "Ccoffee_cup.h"
#include "Cplayer.h"
#include "CEnemy.h"
#include "debug_font.h"
#include "exp.h"
#include "CTexture.h"
#include "sound.h"
//=============================================================================
//	定数定義
//=============================================================================
#define SPEED (0.5f)
#define COFFEE_SIZE (4)
#define COFFEE_CUP_HP (10)
#define COFFEE_CUP_MP (1)
#define COFFEE_CUP_ATK (1)
#define SCORE (1)
#define COFFEE_SCALE (2.5f)
#define COFFEE_TABLE_SCALE (3)
//=============================================================================
//	静的変数
//=============================================================================
//	メンバ変数にする　カウントはm_FrameCountでとる　CEnemySmall参考にして

//=============================================================================
//	グローバル変数
//=============================================================================
THING_NORMAL thing_cup2, thing_cup3, thing_cup4;
//=============================================================================
//	生成
//=============================================================================

CCoffeeCup::CCoffeeCup(D3DXMATRIX mtxWorld) :CAttraction(AT_COFFEE), C3DObj(TYPE_ATTRACTION)
{
	Initialize(mtxWorld);
}

CCoffeeCup::~CCoffeeCup()
{

}

void CCoffeeCup::Initialize(D3DXMATRIX mtxWorld)
{
	m_AttractionIndex = Get_AttractionIndex(AT_ALL);

	m_Enable = true;
	m_DrawCount = 0;
	angCup = 0.0f;//カップの回転
	CoolTime = 0;//クールタイム
	attackon = true;
	B_CoolTime = true;
	m_Hp = COFFEE_CUP_HP;
	m_Mp = COFFEE_CUP_MP;
	m_Attack = COFFEE_CUP_ATK;
	//コーヒーカップテーブルのみブレンダーで大きさを変えること
	C3DObj *playerget = CPlayer::Get_Player();
	D3DXMATRIX mtx = playerget->Get_mtxWorld();
	D3DXMatrixTranslation(&m_mtxTranslation, mtxWorld._41, 0, mtxWorld._43);//X,Y,Zを渡す
	D3DXMatrixScaling(&m_mtxScaling, COFFEE_SCALE, COFFEE_SCALE, COFFEE_SCALE);
	D3DXMatrixScaling(&m_mtxScaling2, COFFEE_SCALE / 2, COFFEE_SCALE / 2, COFFEE_SCALE / 2);
	m_mtxWorld = m_mtxScaling * m_mtxTranslation;

	Thing_Normal_model = GetNormalModel(MODELL_CUP_BLUE);
	thing_cup2 = GetNormalModel(MODELL_CUP_YELLOW);
	thing_cup3 = GetNormalModel(MODELL_CUP_GREEN);
	thing_cup4 = GetNormalModel(MODELL_CUP_TABLE);

	D3DXMatrixTranslation(&m_mtxTranslation, m_mtxWorld._41, m_mtxWorld._42+0.5f, m_mtxWorld._43);
	D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(angCup));
	m_mtxWorld =  m_mtxRotation  * m_mtxTranslation;

	D3DXMatrixTranslation(&m_mtxTranslation, -1.9f, -0.5f, -1.0f);
	D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(angCup));
	m_mtxWorld2 = m_mtxScaling2 * m_mtxRotation  * m_mtxTranslation;

	D3DXMatrixTranslation(&m_mtxTranslation, 0.0f, -0.5f, 1.9f);

	D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(angCup));
	m_mtxWorld3 = m_mtxScaling2* m_mtxRotation  * m_mtxTranslation;


	D3DXMatrixTranslation(&m_mtxTranslation, 1.9f, -0.5f, -1.0f);

	D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(angCup));
	m_mtxWorld4 = m_mtxScaling2* m_mtxRotation  * m_mtxTranslation;

	D3DXMatrixMultiply(&m_mtxWorld2, &m_mtxWorld2, &m_mtxWorld);

	D3DXMatrixMultiply(&m_mtxWorld3, &m_mtxWorld3, &m_mtxWorld);

	D3DXMatrixMultiply(&m_mtxWorld4, &m_mtxWorld4, &m_mtxWorld);	

	m_mtxWorld = m_mtxScaling * m_mtxWorld;
	Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld2._41, m_mtxWorld2._42, m_mtxWorld2._43);
	thing_cup2.vPosition = D3DXVECTOR3(m_mtxWorld3._41, m_mtxWorld3._42, m_mtxWorld3._43);
	thing_cup3.vPosition = D3DXVECTOR3(m_mtxWorld4._41, m_mtxWorld4._42, m_mtxWorld4._43);
	thing_cup4.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);

	m_TimeKeep = m_FrameCount;
}

void CCoffeeCup::Update(void)
{

	if (m_Enable)
	{//ここで回転計算を行う
	 //回転
		if (B_CoolTime)
		{
			PlaySound(CUP_SE);
			angCup += 5.0f;

			if (angCup>355.0f)
			{
				B_CoolTime = false;
				angCup = 0.0f;
				attackon = false;
			}
		}

		if (!B_CoolTime)
		{
			CoolTime++;
		}
		//5秒たったら
		if (CoolTime > 60)
		{
			B_CoolTime = true;
			CoolTime = 0;
			attackon = true;
		}

		D3DXMatrixTranslation(&m_mtxTranslation, m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
		D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(angCup));
		m_mtxWorld = m_mtxRotation  * m_mtxTranslation;

		D3DXMatrixTranslation(&m_mtxTranslation, -1.9f, -0.5f, -1.0f);
		D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(angCup));
		m_mtxWorld2 = m_mtxScaling2 * m_mtxRotation  * m_mtxTranslation;

		D3DXMatrixTranslation(&m_mtxTranslation, 0.0f, -0.5f, 1.9f);

		D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(angCup));
		m_mtxWorld3 = m_mtxScaling2* m_mtxRotation  * m_mtxTranslation;


		D3DXMatrixTranslation(&m_mtxTranslation, 1.9f, -0.5f, -1.0f);

		D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(angCup));
		m_mtxWorld4 = m_mtxScaling2* m_mtxRotation  * m_mtxTranslation;

		D3DXMatrixMultiply(&m_mtxWorld2, &m_mtxWorld2, &m_mtxWorld);

		D3DXMatrixMultiply(&m_mtxWorld3, &m_mtxWorld3, &m_mtxWorld);

		D3DXMatrixMultiply(&m_mtxWorld4, &m_mtxWorld4, &m_mtxWorld);

		m_mtxWorld = m_mtxScaling * m_mtxWorld;
		if (attackon)
		{
			Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld2._41, m_mtxWorld2._42, m_mtxWorld2._43);
			thing_cup2.vPosition = D3DXVECTOR3(m_mtxWorld3._41, m_mtxWorld3._42, m_mtxWorld3._43);
			thing_cup3.vPosition = D3DXVECTOR3(m_mtxWorld4._41, m_mtxWorld4._42, m_mtxWorld4._43);
			thing_cup4.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
			OrnamentDamage(thing_cup2);
			OrnamentDamage(thing_cup3);
			OrnamentDamage(thing_cup4);

			EnemyDamage();
			CoffeeCupDamage();
		}
	}
}

void CCoffeeCup::Draw(void)
{
	//DebugFont_Draw(600, 30, "Dodai = %f\n,", angCup);
	//DebugFont_Draw(600, 60, "CoolTime = %d\n,", CoolTime);
	if (m_Enable)
	{
		D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		

		if (!m_DrawCheck)
		{
			if (m_FrameCount % 2 == 0)
			{
				m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
				DrawDX_Normal(m_mtxWorld, MODELL_CUP_TABLE, &thing_cup4);

				DrawDX_NormalAdd(m_mtxWorld2, MODELL_CUP_BLUE, &Thing_Normal_model, position);

				DrawDX_NormalAdd(m_mtxWorld3, MODELL_CUP_YELLOW, &thing_cup2, position);

				DrawDX_NormalAdd(m_mtxWorld4, MODELL_CUP_GREEN, &thing_cup3, position);

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
			DrawDX_Normal(m_mtxWorld, MODELL_CUP_TABLE, &thing_cup4);

			DrawDX_NormalAdd(m_mtxWorld2, MODELL_CUP_BLUE, &Thing_Normal_model, position);

			DrawDX_NormalAdd(m_mtxWorld3, MODELL_CUP_YELLOW, &thing_cup2, position);

			DrawDX_NormalAdd(m_mtxWorld4, MODELL_CUP_GREEN, &thing_cup3, position);
		}

		
	}
}

void CCoffeeCup::Finalize(void)
{
	//Attraction_Finalize(m_AttractionIndex);
	delete this;
}

void CCoffeeCup::CoffeeCup_Create(void)
{

}

void CCoffeeCup::EnemyDamage(void)
{
	if (attackon)
	{
		int i;
		C3DObj *enemy;
		THING *thingenemy;
		for (i = 0; i < MAX_GAMEOBJ; i++)
		{
			enemy = CEnemy::Get_Enemy(i);
			if (attackon && enemy)
			{//コーヒーカップのみ判定
				Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld2._41, m_mtxWorld2._42, m_mtxWorld2._43);
				thingenemy = enemy->GetAnimeModel();
				if (C3DObj::Collision_AnimeVSNormal(thingenemy, &Thing_Normal_model))
				{
					enemy->DamageFlag_Change();
					enemy->Position_Keep(m_mtxWorld);
					m_Hp--;
					break;
				}
				thing_cup2.vPosition = D3DXVECTOR3(m_mtxWorld3._41, m_mtxWorld3._42, m_mtxWorld3._43);
				if (C3DObj::Collision_AnimeVSNormal(thingenemy, &thing_cup2))
				{
					enemy->DamageFlag_Change();
					enemy->Position_Keep(m_mtxWorld);
					m_Hp--;
					break;
				}
				thing_cup3.vPosition = D3DXVECTOR3(m_mtxWorld4._41, m_mtxWorld4._42, m_mtxWorld4._43);
				if (C3DObj::Collision_AnimeVSNormal(thingenemy, &thing_cup3))
				{
					enemy->DamageFlag_Change();
					enemy->Position_Keep(m_mtxWorld);
					m_Hp--;
					break;
				}
			}
		}
	}
}


void CCoffeeCup::CoffeeCupDamage(void)
{
	int i;
	THING *thingenemy;
	C3DObj *enemy;
	int attack;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		enemy = CEnemy::Get_Enemy(i);
		
		if (enemy && m_DrawCheck)
		{
			if (enemy->Get_AttacFlag())
			{
				Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld2._41, m_mtxWorld2._42, m_mtxWorld2._43);
				thing_cup2.vPosition = D3DXVECTOR3(m_mtxWorld3._41, m_mtxWorld3._42, m_mtxWorld3._43);
				thing_cup3.vPosition = D3DXVECTOR3(m_mtxWorld4._41, m_mtxWorld4._42, m_mtxWorld4._43);
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
				if (C3DObj::Collision_AnimeVSNormal(thingenemy, &thing_cup2))
				{
					m_Hp -= attack;
					m_DrawCheck = false;
					if (m_Hp <= 0)
					{
						C3DObj_delete();
					}
					break;
				}
				if (C3DObj::Collision_AnimeVSNormal(thingenemy, &thing_cup3))
				{
					m_Hp -= attack;
					m_DrawCheck = false;
					if (m_Hp <= 0)
					{
						C3DObj_delete();
					}
					break;
				}
			}
		}
	}
}