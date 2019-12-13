//////////////////////////////////////////////////
////
////	エネミー(ビッグ)クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CEnemy_Big.h"
#include "Cplayer.h"
#include "input.h"
#include "CSkinAnimation.h"
#include "debug_font.h"
#include "CTexture.h"
//=============================================================================
//	定数定義
//=============================================================================

#define BIG_SIZE (0.8f)
#define BIG_ATTACK (10)
#define BIG_HP (10)
#define BIG_MP (60)
#define BIG_SCORE (10000)
#define FRY_HEIGHT (0.2f)
#define FRY_SPEED (0.05f)
#define WALK_SPEED (0.01f)
#define ATTACK_SPEED (0.01f)
enum ANIMATION {

	WALK,
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

CEnemy_Big::CEnemy_Big(ENEMY_EMITTER *Emitter) :CEnemy(TYPE_BIG), C3DObj(C3DObj::TYPE_ENEMY)
{
	Initialize(Emitter);
}

//=============================================================================
//	破棄
//=============================================================================

CEnemy_Big::~CEnemy_Big()
{
	
}

void CEnemy_Big::Initialize(ENEMY_EMITTER *Emitter)
{
	char animefile[TEXTURE_FILENAME_MAX] = {};
	strcpy(animefile, C3DObj::Get_AnimeFileName(MODELL_ANIME_BIG));
	SkinMesh.InitThing(m_pD3DDevice, &Thing, animefile);
	Thing.Sphere.fRadius = 6.0f;
	Thing.Sphere.vCenter = D3DXVECTOR3(0, 1.2f, 0);
	SkinMesh.InitSphere(m_pD3DDevice, &Thing);

	m_EnemyIndex = Get_EnemyIndex(TYPE_ALL);
	//モデル情報取得
	//Thing_Anime_model = GetAnimeModel();
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
	m_Hp = BIG_HP;
	m_Attack = BIG_ATTACK;
	m_Score = BIG_SCORE;
	m_Mp = BIG_MP;
	m_Direction = Emitter->InitDirection;
	m_InitDirection = m_Direction;

	D3DXMatrixTranslation(&m_mtxTranslation, Emitter->InitPos.x, Emitter->InitPos.y, Emitter->InitPos.z);
	D3DXMatrixScaling(&m_mtxScaling, 2.5f, 2.2f, 2.5f);
	m_mtxWorld = m_mtxScaling * m_mtxTranslation;
	m_mtxInit = m_mtxWorld;

	Thing.vPosition = D3DXVECTOR3(m_mtxTranslation._41, m_mtxTranslation._42, m_mtxTranslation._43);
	m_CreateCount = Emitter->CreateFrame;
	m_InitCreateCount = m_CreateCount;
}

void CEnemy_Big::GameBegin(void)
{

}

void CEnemy_Big::Finalize(void)
{
	m_MoveCheck = false;
	m_DrawCheck = true;
	m_Hp = BIG_HP;
	m_Attack = BIG_ATTACK;
	m_Score = BIG_SCORE;
	m_Mp = BIG_MP;
	m_Direction = m_InitDirection;
	Animation_Change(WALK, WALK_SPEED);
 
	m_mtxWorld = m_mtxInit;
	D3DXMatrixTranslation(&m_mtxTranslation, m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);

	m_Enable = false;
	Thing.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
	m_CreateCount = m_InitCreateCount;
	m_AttackCheck = false;
	Color_Change(CTexture::TEX_BIG);
}


void CEnemy_Big::Update(void)
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
						Big_Move();
					}
					else
					{
						Big_Attack();
					}
				}
				else
				{
					if (!m_AttakFlag)
					{
						Chase_Player();
					}

					Big_Attack();

				}
			}
			else
			{
				Comeback_Move(BIG_SPEED);

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
			Color_Change(CTexture::TEX_BIG_END);
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



void CEnemy_Big::Draw(void)
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
				DebugFont_Draw(300, 500, "%d", m_FrameCount - m_AttackTime);
			}
		}
	}
	
}


void CEnemy_Big::Big_Move(void)
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
		Color_Change(CTexture::TEX_BIG);
		D3DXMATRIX mtxtrans;
		D3DXMatrixTranslation(&mtxtrans, m_EnemyMove[m_Direction].Move.x * BIG_SPEED, m_EnemyMove[m_Direction].Move.y * BIG_SPEED, m_EnemyMove[m_Direction].Move.z * BIG_SPEED);
		m_mtxTranslation *= mtxtrans;

		if (m_FrameCount - m_TimeKeep >= 60 * m_Movetime)
		{
			m_MoveCheck = false;
		}
	}
}

void CEnemy_Big::Big_Attack(void)
{
	if (!m_AttackCheck)
	{
		m_AttackTime = m_FrameCount;
		m_AttackCheck = true;
		Color_Change(CTexture::TEX_BIG_ANOTHER);
		
	}
	else
	{
		if (m_FrameCount - m_AttackTime >= 60)
		{
			if (!m_JumpFlag && !m_AttakFlag)
			{
				//m_AttakFlag = true;
				Animation_Change(ATTACK, ATTACK_SPEED);
				

				m_JumpFlag = true;
			}
		}
		/*if (m_FrameCount - m_AttackTime >= 110)
		{			
			//Aimation_Change(ATTACK, ATTACK_SPEED);
			if (m_JumpFlag)
			{
				m_mtxTranslation._42 += 0.2;
			}
		}
		if (m_FrameCount - m_AttackTime >= 135)
		{
			m_JumpFlag = false;
			Color_Change(CTexture::TEX_BIG);
			if (!m_JumpFlag)
			{
				m_mtxTranslation._42 -= 0.2;
			}
		}
		if (m_FrameCount - m_AttackTime >= 160)
		{
			Animation_Change(WALK, WALK_SPEED);
			m_mtxTranslation._42 = 0.0f;
			m_AttackCheck = false;
			m_AttackTime = 0;
			m_AttakFlag = false;
		}*/

		if (m_FrameCount - m_AttackTime >= 110)
		{
			if (m_JumpFlag)
			{
				m_AttakFlag = true;
				Color_Change(CTexture::TEX_BIG);
				m_mtxTranslation._42 += 0.2f;
				if (m_mtxTranslation._42 >= 8.0)
				{
					
					m_JumpFlag = false;
					
				}
			}
			else
			{
				m_mtxTranslation._42 -= 0.2f;
			//	Thing.Sphere.fRadius = 15.0f;
				if (m_mtxTranslation._42 <= 0.0)
				{
				//	Thing.Sphere.fRadius = 5.0f;
					Animation_Change(WALK, WALK_SPEED);
					m_mtxTranslation._42 = 0.0f;
					m_AttackCheck = false;
					m_AttackTime = 0;
					m_AttakFlag = false;
				}
			}
		}
	}
}




