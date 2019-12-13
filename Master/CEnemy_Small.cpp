//////////////////////////////////////////////////
////
////	エネミー(スモール)クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CEnemy_Small.h"
#include "Cplayer.h"
#include "input.h"
#include "CSkinAnimation.h"
#include "debug_font.h"
#include "CTexture.h"
#include "exp.h"
//=============================================================================
//	定数定義
//=============================================================================

#define SMALL_SPEED (0.05f)
#define SMALL_SIZE (0.8f)
#define SMALL_ATTACK (3)
#define SMALL_HP (1)
#define SMALL_MP (3)
#define SMALL_SCORE (100)
#define FRY_HEIGHT (0.5f)	//	飛ぶ高さ
#define FRY_SPEED (0.05f)	//	飛ぶ速さ
//	アニメーションスピード
#define WALK_SPEED (0.01f)
#define ATTACK_SPEED (0.02f)
//エネミースモールのアニメーション番号定義
enum ANIMATION {
	STOP,
	WALK,
	DASH,
	ATTACK,
	DEATH,

	ANIME_MAX
};
//=============================================================================
//	静的変数
//=============================================================================


//=============================================================================
//	グローバル変数
//=============================================================================

//=============================================================================
//	生成
//=============================================================================

CEnemy_Small::CEnemy_Small(ENEMY_EMITTER *Emitter):CEnemy(TYPE_SMALL),C3DObj(C3DObj::TYPE_ENEMY)
{
	Initialize(Emitter);
}

//=============================================================================
//	破棄
//=============================================================================

CEnemy_Small::~CEnemy_Small()
{

}


//=============================================================================
//	初期化
//=============================================================================

void CEnemy_Small::Initialize(ENEMY_EMITTER *Emitter)
{


	//SkinMesh.InitThing(m_pD3DDevice, &Thing, ANIME_MODEL_FILES[MODELL_ANIME_SMALL].filename);
	char animefile[TEXTURE_FILENAME_MAX] = {  };
	strcpy(animefile, C3DObj::Get_AnimeFileName(MODELL_ANIME_SMALL));
	SkinMesh.InitThing(m_pD3DDevice, &Thing, animefile);
	Thing.Sphere.fRadius = 1.3f;
	Thing.Sphere.vCenter = D3DXVECTOR3(0, 1.2f, 0);
	SkinMesh.InitSphere(m_pD3DDevice, &Thing);

	m_EnemyIndex = Get_EnemyIndex(TYPE_ALL);
	//モデル情報取得
	//アニメーショントラックを得る
	for (DWORD i = 0; i < Thing.pAnimController->GetNumAnimationSets(); i++)
	{//AnimSetにアニメーション情報格納
		Thing.pAnimController->GetAnimationSet(i, &pAnimSet[i]);
	}

	
	//アニメーション情報初期化
	TrackDesc.Weight = 1;
	TrackDesc.Enable = true;
	TrackDesc.Position = 0;//アニメーションタイムリセット
	TrackDesc.Speed = WALK_SPEED;//モーションスピード
	Thing.pAnimController->SetTrackDesc(0, &TrackDesc);//アニメ情報セット
	Thing.pAnimController->SetTrackAnimationSet(0, pAnimSet[WALK]);//初期アニメーションセット
	m_AnimationType = WALK;
	//m_Enable = true;
	m_MoveCheck = false;
	m_DrawCheck = true;
	m_Hp = SMALL_HP;
	m_Attack = SMALL_ATTACK;
	m_Score = SMALL_SCORE;
	m_Mp = SMALL_MP;
	m_Direction = Emitter->InitDirection;
	m_InitDirection = m_Direction;
	m_CreateCount = Emitter->CreateFrame;
	m_InitCreateCount = m_CreateCount;

	D3DXMatrixTranslation(&m_mtxTranslation, Emitter->InitPos.x, Emitter->InitPos.y, Emitter->InitPos.z);
	D3DXMatrixScaling(&m_mtxScaling, 1, 1, 1);
	m_mtxWorld = m_mtxScaling * m_mtxTranslation;
	m_mtxInit = m_mtxWorld;
	m_mtxKeepTranslation = m_mtxTranslation;
	Thing.vPosition = D3DXVECTOR3(m_mtxTranslation._41, m_mtxTranslation._42, m_mtxTranslation._43);
}

void CEnemy_Small::GameBegin(void)
{

}

//=============================================================================
//	終了処理
//=============================================================================
void CEnemy_Small::Finalize(void)
{
	m_MoveCheck = false;
	m_DrawCheck = true;
	
	m_Hp = SMALL_HP;
	m_Attack = SMALL_ATTACK;
	m_Score = SMALL_SCORE;
	m_Mp = SMALL_MP;
	m_Direction = m_InitDirection;
	m_CreateCount = m_InitCreateCount;

	m_mtxWorld = m_mtxInit;
	D3DXMatrixTranslation(&m_mtxTranslation, m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);

	m_Enable = false;
	Thing.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
	m_AttackCheck = false;
	Color_Change(CTexture::TEX_SMALL);
}

//=============================================================================
//	更新処理
//=============================================================================
void CEnemy_Small::Update(void)
{

	if (m_Enable)
	{
		if (!m_DamageFlag)
		{

			if (m_DrawCheck)	//	範囲内にいるか
			{
				if (!PlayerCheck())	//	近くにプレイヤーがいるか
				{
					if ((!Chase_Popcorn()) && (!m_AttackCheck))
					{
						Small_Move();
					}
					else
					{
						Small_Attack();
					}
				}
				else
				{
					Chase_Player();

					Small_Attack();

				}
			}
			else
			{
				Comeback_Move(SMALL_SPEED);

			}

		}
		else
		{
			Enemy_Damage(FRY_HEIGHT);
			Enemy_Flying(FRY_SPEED);



		}

		Ornament_Check();

		m_mtxWorld = m_mtxScaling * m_mtxRotation * m_mtxTranslation;
		m_mtxKeepTranslation = m_mtxTranslation;

		Thing.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
		Draw_Check();

		if (m_Hp <= 0)
		{
			Color_Change(CTexture::TEX_SMALL_END);
			if (!m_DrawCheck)
			{
				CPlayer::Add_KoCount();
				//C3DObj_delete();
				m_Enable = false;
				m_EnemyEnableNum--;
			}
		}
	}
	else
	{
		Enable_Check();
	}
}

	



//=============================================================================
//	描画処理
//=============================================================================
void CEnemy_Small::Draw(void)
{
	
	if (m_Enable)
	{
		if (m_DrawCheck)
		{//当たり判定位置更新
			m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//　ライティング有効
			
			DrawDX_Anime(m_mtxWorld, MODELL_ANIME_SMALL, &Thing);
			m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	//　ライティング有効

			if (m_AttackCheck)
			{
				//DebugFont_Draw(300, 500, "%d",m_FrameCount - m_AttackTime);
			}
		}
	}
	//DebugFont_Draw(10, 470, "%d", m_EnemyEnableNum);
}


//=============================================================================
//	移動処理
//=============================================================================
void CEnemy_Small::Small_Move(void)
{
	if (!m_MoveCheck)
	{
		m_Direction++;
		if (m_Direction >= 9)
		{
			m_Direction = 0;
		}
		
		Animation_Change(WALK, WALK_SPEED);
		D3DXMatrixRotationY(&m_mtxRotation, m_EnemyMove[m_Direction].Angle);
		m_TimeKeep = m_FrameCount;
		m_Movetime = rand() % 3 + 1;
		m_MoveCheck = true;
	}
	else
	{
		Color_Change(CTexture::TEX_SMALL);
		D3DXMATRIX mtxtrans;
		D3DXMatrixTranslation(&mtxtrans, m_EnemyMove[m_Direction].Move.x * SMALL_SPEED, m_EnemyMove[m_Direction].Move.y * SMALL_SPEED, m_EnemyMove[m_Direction].Move.z * SMALL_SPEED);
		m_mtxTranslation *= mtxtrans;

		if (m_FrameCount - m_TimeKeep >= 60 * m_Movetime)
		{
			m_MoveCheck = false;
		}
	}
}

//=============================================================================
//	攻撃処理
//=============================================================================
void CEnemy_Small::Small_Attack(void)
{
	if (!m_AttackCheck)
	{
		m_AttackTime = m_FrameCount;
		m_AttackCheck = true;
		
		
	}
	else
	{
		if (m_FrameCount - m_AttackTime >= 60)
		{
			Color_Change(CTexture::TEX_SMALL_ANOTHER);
		}
		if (m_FrameCount - m_AttackTime >= 120)
		{
			if (m_FrameCount - m_AttackTime == 130)
			{
				//Exp_Set(ENEMY_HIKKAI, m_mtxTranslation._41, m_mtxTranslation._42, m_mtxTranslation._43, 1.0, 0);
			}
			Animation_Change(ATTACK, ATTACK_SPEED);
		}
		if (m_FrameCount - m_AttackTime >= 140)
		{
			Color_Change(CTexture::TEX_SMALL);
			m_AttakFlag = true;
		}
		if (m_FrameCount - m_AttackTime >= 160)
		{
			Animation_Change(WALK, WALK_SPEED);
			m_AttackCheck = false;
			m_AttackTime = 0;
			m_AttakFlag = false;
		}
	
	}
}
