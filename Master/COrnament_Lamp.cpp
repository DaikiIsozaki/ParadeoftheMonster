//////////////////////////////////////////////////
////
////	エネミー(スモール)クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "COrnament_Lamp.h"
#include "debug_font.h"
#include "exp.h"
#include "CEnemy.h"
#include "CTexture.h"
#include "Cplayer.h"
//=============================================================================
//	定数定義
//=============================================================================

#define LAMP_SIZE (1.0f)
#define LAMP_ATTACK (3)
#define LAMP_HP (1)
#define LAMP_SCORE (1)
#define LAMP_ATK (1)

#define FRY_HEIGHT (0.1f)
#define FRY_SPEED (0.07f)
//=============================================================================
//	静的変数
//=============================================================================

//=============================================================================
//	グローバル変数
//=============================================================================

//=============================================================================
//	生成
//=============================================================================

COrnament_Lamp::COrnament_Lamp(ORNAMENT_EMITTER *Emitter) :COrnament(TYPE_LAMP), C3DObj(C3DObj::TYPE_ORNAMENT)
{
	Initialize(Emitter);
}

//=============================================================================
//	破棄
//=============================================================================

COrnament_Lamp::~COrnament_Lamp()
{

}

void COrnament_Lamp::Initialize(ORNAMENT_EMITTER *Emitter)
{
	m_DrawCount = 0;
	m_OrnamentIndex = Get_OrnamentIndex(TYPE_ALL);
	m_Enable = true;
	m_DrawCheck = true;
	m_Hp = LAMP_HP;
	m_Attack = LAMP_ATK;

	Thing_Normal_model = GetNormalModel(MODELL_LAMP);

	m_Direction = Emitter->InitDirection;

	D3DXMatrixTranslation(&m_mtxTranslation, Emitter->InitPos.x, Emitter->InitPos.y, Emitter->InitPos.z);
	D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(m_Direction));
	D3DXMatrixScaling(&m_mtxScaling, Emitter->scale.x, Emitter->scale.y, Emitter->scale.z);
	m_mtxWorld = m_mtxRotation * m_mtxScaling * m_mtxTranslation;
	m_mtxInit = m_mtxWorld;
	Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxTranslation._41, m_mtxTranslation._42, m_mtxTranslation._43);
	InitSphere(m_pD3DDevice, &Thing_Normal_model, D3DXVECTOR3(0.0f, 0.0f, 0.0f),0.7f);//当たり判定の変更
}


void COrnament_Lamp::Finalize(void)
{
	m_DrawCount = 0;
	m_OrnamentIndex = Get_OrnamentIndex(TYPE_ALL);
	m_Enable = true;
	m_DrawCheck = true;
	m_Hp = LAMP_HP;
	m_Attack = LAMP_ATK;
		
	D3DXMatrixTranslation(&m_mtxTranslation, m_mtxInit._41, m_mtxInit._42, m_mtxInit._43);
	m_mtxWorld = m_mtxRotation * m_mtxScaling * m_mtxTranslation;

	Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
}


void COrnament_Lamp::Update(void)
{
	if (m_Enable)
	{
		//エフェクト処理を行う
		if (m_DrawCheck)
		{
			if (m_DamageFlag)
			{
				Ornament_Damage(FRY_HEIGHT);
				Ornament_Flying(FRY_SPEED);
				Damage();
				
			}
		}
		m_mtxWorld = m_mtxRotation * m_mtxScaling * m_mtxTranslation;
		Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);

		Death();
	}	
}



void COrnament_Lamp::Draw(void)
{
	if (m_Enable)
	{
		D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
		if (!m_DrawCheck)
		{
			if (m_FrameCount % 2 == 0)
			{
				DrawDX_NormalAddScale(m_mtxWorld, MODELL_LAMP, &Thing_Normal_model, position, D3DXVECTOR3(1, 10, 1));

				m_DrawCount++;
				if (m_DrawCount >= ORNAMENT_WAIT_TIME)
				{
					m_DrawCount = 0;
					m_DrawCheck = true;
				}
			}
		}
		else
		{
			//DrawDX_NormalAdd(m_mtxWorld, MODELL_LAMP, Thing_Normal_model, position);
		//	DrawDX_NormalAdd(m_mtxWorld, MODELL_LAMP, Thing_Normal_model, position);
			//丸の判定を縦に２倍する
			DrawDX_NormalAddScale(m_mtxWorld, MODELL_LAMP, &Thing_Normal_model, position, D3DXVECTOR3(1, 10, 1));
		}	
		/*
		if (m_DrawCheck)
		{
			PlayerDebug();
		}*/
	}
}


void COrnament_Lamp::Damage(void)
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
					m_DrawCheck = false;
					if (m_Hp <= 0)
					{
						//Exp_Create(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43, 1.0f, 0.0f, CTexture::TEX_EFFECT_HIT1, 14, 1, 3360 / 7, 960 / 2, 7);
						C3DObj_delete();
					}
					break;
				}
			}
		}
	}
}

//プレイヤーと当たっているかデバック
void COrnament_Lamp::PlayerDebug(void)
{
	/*
	for (int i = 0; i < MAX_GAMEOBJ; i++)
	{
		C3DObj *player = CPlayer::Get_Player();

		if (player && m_DrawCheck)
		{
			Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
			THING *thingenemy = player->GetAnimeModel();
			if (C3DObj::Collision_AnimeVSNormal(thingenemy, &Thing_Normal_model))
			{
				DebugFont_Draw(510, 500, "当たってる！！！");
				//m_DrawCheck = false;
				if (m_Hp <= 0)
				{
			//		Exp_Create(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43, 1.0f, 0.0f, CTexture::TEX_EFFECT_HIT1, 14, 1, 3360 / 7, 960 / 2, 7);
			//		C3DObj_delete();
				}
				break;
			}
			else
			{
				DebugFont_Draw(510, 500, "当たってない");
			}
		}
	}*/
}








