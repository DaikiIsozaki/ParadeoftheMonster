//////////////////////////////////////////////////
////
////	オブジェクト(ゴミ箱)クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================
#include "COrnament_MERI.h"
#include "debug_font.h"
#include "CEnemy.h"
#include "sound.h"
#include "exp.h"
//=============================================================================
//	定数定義
//=============================================================================

#define MERI_SIZE (1.0f)
#define MERI_ATTACK (3)
#define MERI_HP (1)
#define MERI_SCORE (1)
#define MERI_ATK (1)
#define FRY_HEIGHT (0.1f)
#define FRY_SPEED (0.07f)
#define ORNAMENT_HITSTOP (100)
//=============================================================================
//	静的変数
//=============================================================================

//=============================================================================
//	グローバル変数
//=============================================================================

//=============================================================================
//	生成
//=============================================================================

COrnament_MERI::COrnament_MERI(ORNAMENT_EMITTER *Emitter) :COrnament(TYPE_MERI), C3DObj(C3DObj::TYPE_ORNAMENT)
{
	Initialize(Emitter);
}

//=============================================================================
//	破棄
//=============================================================================
COrnament_MERI::~COrnament_MERI()
{

}

void COrnament_MERI::Initialize(ORNAMENT_EMITTER *Emitter)
{
	m_OrnamentIndex = Get_OrnamentIndex(TYPE_ALL);
	m_Enable = true;
	m_DrawCheck = true;
	angle = 0.0f;
	m_Hp = MERI_HP;
	m_Attack = MERI_ATK;
	m_OrnamentFlying = false;
	m_OrnamentFlyingDown = false;
	m_FlyingCount = 0;
	m_FlyingMove = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43) - m_PosKeep;

	Thing_Normal_model = GetNormalModel(MODELL_MERI_DODAI);
	uma = GetNormalModel(MODELL_MERI_UMA);

	m_Direction = Emitter->InitDirection;

	D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(m_Direction));
	D3DXMatrixTranslation(&m_mtxTranslation, Emitter->InitPos.x, Emitter->InitPos.y, Emitter->InitPos.z);
	D3DXMatrixScaling(&m_mtxScaling, Emitter->scale.x, Emitter->scale.y, Emitter->scale.z);
	m_mtxWorld = m_mtxRotation * m_mtxScaling * m_mtxTranslation;
	m_mtxInit = m_mtxWorld;

	D3DXMatrixRotationY(&rotation2, D3DXToRadian(m_Direction));
	D3DXMatrixTranslation(&trans2, Emitter->InitPos.x, Emitter->InitPos.y + 1, Emitter->InitPos.z);
	D3DXMatrixScaling(&scale2, Emitter->scale.x, Emitter->scale.y, Emitter->scale.z);
	m_mtxWorld2 = rotation2 * scale2 * trans2;

	InitSphere(m_pD3DDevice, &Thing_Normal_model, D3DXVECTOR3(0, 0.0, 0), 3.1f);//当たり判定の変更

	Thing.vPosition = D3DXVECTOR3(m_mtxTranslation._41, m_mtxTranslation._42, m_mtxTranslation._43);
	uma.vPosition = D3DXVECTOR3(m_mtxWorld2._41, m_mtxWorld2._42, m_mtxWorld2._43);
}


void COrnament_MERI::Finalize(void)
{
	m_OrnamentIndex = Get_OrnamentIndex(TYPE_ALL);
	m_Enable = true;
	m_DrawCheck = true;
	angle = 0.0f;
	m_Hp = MERI_HP;
	m_Attack = MERI_ATK;


	D3DXMatrixTranslation(&m_mtxTranslation, m_mtxInit._41, m_mtxInit._42, m_mtxInit._43);
	m_mtxWorld = m_mtxRotation * m_mtxScaling * m_mtxTranslation;

	D3DXMatrixTranslation(&trans2, m_mtxInit._41, m_mtxInit._42 + 1, m_mtxInit._43);
	m_mtxWorld2 = rotation2 * scale2 * trans2;


	Thing.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
	uma.vPosition = D3DXVECTOR3(m_mtxWorld2._41, m_mtxWorld2._42, m_mtxWorld2._43);
}


void COrnament_MERI::Update(void)
{
	if (m_Enable)
	{
		if (m_DamageFlag)
		{
			Ornament_Damage_Meri(FRY_HEIGHT);
			Ornament_Flying_Meri(FRY_SPEED);
			Damage();
		}
		angle -= 0.5f;
		D3DXMatrixRotationY(&rotation3, D3DXToRadian(angle));
		m_mtxWorld2 = rotation3 * rotation2 * scale2 * trans2;


		uma.vPosition = D3DXVECTOR3(m_mtxWorld2._41, m_mtxWorld2._42, m_mtxWorld2._43);

		m_mtxWorld = m_mtxRotation * m_mtxScaling * m_mtxTranslation;
		Thing_Normal_model.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
		Death_Meri();
	}
}



void COrnament_MERI::Draw(void)
{
	if (m_Enable)
	{
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		DrawDX_Normal(m_mtxWorld2, MODELL_MERI_UMA, &uma);
		DrawDX_Normal(m_mtxWorld, MODELL_MERI_DODAI, &Thing_Normal_model);
	}
}


void COrnament_MERI::Damage(void)
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
						//	C3DObj_delete();
					}
					break;
				}
			}
		}
	}
}

void COrnament_MERI::Ornament_Damage_Meri(float flyinghigh)
{

	if (!m_OrnamentFlying)
	{
		if (m_DrawCheck)
		{
			PlaySound(NORMALHIT_SE);
			m_Hp--;
			Sleep(ORNAMENT_HITSTOP);
			m_FlyingMove = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43) - m_PosKeep;
			m_FlyingMove.y = flyinghigh;
			m_FlyingMove2 = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43) - m_PosKeep;
			m_FlyingMove2.y = flyinghigh;
			if (m_Hp <= 0)
			{
				Exp_Set(SMALLSTAR, m_mtxWorld._41, m_mtxWorld._42 + 1, m_mtxWorld._43, 5.0, 0);
				m_OrnamentFlying = true;
			}
		}

	}
}

void COrnament_MERI::Ornament_Flying_Meri(float speed)
{
	if (m_OrnamentFlying)
	{
		if (m_Hp <= 0)
		{
			if (!m_OrnamentFlyingDown)
			{
				D3DXMatrixTranslation(&m_mtxTranslation, m_mtxWorld._41 + m_FlyingMove.x * speed, m_mtxWorld._42 + m_FlyingMove.y, m_mtxWorld._43 + m_FlyingMove.z * speed);
				D3DXMatrixTranslation(&trans2, m_mtxWorld2._41 + m_FlyingMove2.x * speed, m_mtxWorld2._42 + m_FlyingMove2.y, m_mtxWorld2._43 + m_FlyingMove2.z * speed);
				m_FlyingCount++;
				if (m_FlyingCount >= 60)
				{
					m_OrnamentFlyingDown = true;
				}
			}
			else
			{

				D3DXMatrixTranslation(&m_mtxTranslation, m_mtxWorld._41 + m_FlyingMove.x * speed, m_mtxWorld._42 - m_FlyingMove.y, m_mtxWorld._43 + m_FlyingMove.z * speed);
				D3DXMatrixTranslation(&trans2, m_mtxWorld2._41 + m_FlyingMove2.x * speed, m_mtxWorld2._42 - m_FlyingMove2.y, m_mtxWorld2._43 + m_FlyingMove2.z * speed);
				m_FlyingCount--;
				if (m_FlyingCount < -3)
				{
					m_mtxTranslation._42 = 0.0f;
					trans2._42 = 0.0f;
					m_OrnamentFlying = false;
					m_OrnamentFlyingDown = false;
					m_DamageFlag = false;

				}
			}
		}
		else
		{
			//D3DXMatrixTranslation(&m_mtxTranslation, m_mtxWorld._41 + m_FlyingMove.x * (speed * 2), m_mtxWorld._42 + m_FlyingMove.y, m_mtxWorld._43 + m_FlyingMove.z * (speed * 2));
		}
	}
}

void COrnament_MERI::Death_Meri(void)
{
	if (m_Hp <= 0)
	{
		if (!m_OrnamentFlying)
		{
			Exp_Set(SMALLSTAR, m_mtxWorld._41, m_mtxWorld._42 + 1, m_mtxWorld._43, 5.0, 0);
			//delete this;
			m_Enable = false;
		}
	}
}
