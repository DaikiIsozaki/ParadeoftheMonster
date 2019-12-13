//////////////////////////////////////////////////
////
////	オブジェクト(ゴミ箱)クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================
#include "COrnament_Husen.h"
#include "debug_font.h"
#include "sound.h"
#include "CEnemy.h"
//=============================================================================
//	定数定義
//=============================================================================

#define HUSEN_SIZE (1.0f)
#define HUSEN_ATTACK (3)
#define HUSEN_HP (1)
#define HUSEN_SCORE (1)
#define HUSEN_ATK (1)
//=============================================================================
//	静的変数
//=============================================================================

//=============================================================================
//	グローバル変数
//=============================================================================

//=============================================================================
//	生成
//=============================================================================

COrnament_HUSEN::COrnament_HUSEN(ORNAMENT_EMITTER *Emitter) :COrnament(TYPE_HUSEN), C3DObj(C3DObj::TYPE_ORNAMENT)
{
	Initialize(Emitter);
}

//=============================================================================
//	破棄
//=============================================================================
COrnament_HUSEN::~COrnament_HUSEN()
{

}

void COrnament_HUSEN::Initialize(ORNAMENT_EMITTER *Emitter)
{
	m_OrnamentIndex = Get_OrnamentIndex(TYPE_ALL);
	m_Enable = true;
	m_DrawCheck = true;
	m_Hp = HUSEN_HP;
	m_Attack = HUSEN_ATK;
	m_angle = false;
	updown = 0.0f;
	hit = false;
	Thing_Normal_model = GetNormalModel(MODELL_HUSEN);

	m_Direction = Emitter->InitDirection;

	D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(m_Direction));
	D3DXMatrixTranslation(&m_mtxTranslation, Emitter->InitPos.x, Emitter->InitPos.y, Emitter->InitPos.z);
	D3DXMatrixScaling(&m_mtxScaling, Emitter->scale.x, Emitter->scale.y, Emitter->scale.z);
	m_mtxWorld = m_mtxRotation * m_mtxScaling * m_mtxTranslation;
	m_mtxInit = m_mtxWorld;
	InitSphere(m_pD3DDevice, &Thing_Normal_model, D3DXVECTOR3(0, 0.0, 0), 1.1f);//当たり判定の変更

	Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxTranslation._41, m_mtxTranslation._42, m_mtxTranslation._43);
}


void COrnament_HUSEN::Finalize(void)
{

	m_OrnamentIndex = Get_OrnamentIndex(TYPE_ALL);
	m_Enable = true;
	m_DrawCheck = true;
	m_Hp = HUSEN_HP;
	m_Attack = HUSEN_ATK;


	D3DXMatrixTranslation(&m_mtxTranslation, m_mtxInit._41, m_mtxInit._42, m_mtxInit._43);

	m_mtxWorld = m_mtxRotation * m_mtxScaling * m_mtxTranslation;

	Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);

}


void COrnament_HUSEN::Update(void)
{
	if (m_Enable)
	{
		//エフェクト処理を行う
		if (m_DrawCheck)
		{

			if (m_DamageFlag)
			{
				hit = true;
				PlaySound(HUSEN_SE);
				Ornament_Damage(0.1f);
				Ornament_Flying(0.0f);//飛ばしたいときは0.05fを入れる
				Damage();
			}

			if (!hit)
			{
				if (!m_angle)
				{
					updown += 0.002f;
					if (updown >= 0.08f)
					{
						m_angle = true;
					}
				}
				else
				{
					updown -= 0.002f;
					if (updown <= -0.08f)
					{
						m_angle = false;
					}
				}
				D3DXMatrixTranslation(&m_mtxTranslation, m_mtxWorld._41, m_mtxWorld._42 + updown, m_mtxWorld._43);
			}
			else
			{
				updown += 0.003f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_mtxWorld._41, m_mtxWorld._42 + updown, m_mtxWorld._43);
				Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
			}
			m_mtxWorld = m_mtxRotation * m_mtxScaling * m_mtxTranslation;


			Death();
		}

	}

}



void COrnament_HUSEN::Draw(void)
{
	if (m_Enable)
	{
		D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		DrawDX_NormalAdd(m_mtxWorld, MODELL_HUSEN, &Thing_Normal_model, position);
	}
}


void COrnament_HUSEN::Damage(void)
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
						//C3DObj_delete();
					}
					break;
				}
			}
		}
	}
}