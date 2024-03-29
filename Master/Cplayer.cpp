///=============================================================================
//
//	Playerクラス
//
//=============================================================================

//=============================================================================
//	インクルードファイル
//=============================================================================
#include "Cplayer.h"
#include "input.h"
#include "move.h"
#include "CAttraction.h"
#include "debug_font.h"
#include "gamepad.h"
#include "CEnemy.h"
#include "Cwheel.h"
#include "CEnemy_Small.h"
#include "CAttraction_Coaster .h"
#include "CAttraction_Popcorn.h"
#include "gamepad.h"
#include "exp.h"
#include "CTexture.h"
#include "CEnemy.h"
#include "CCamera.h"
#include "common.h"
#include "CCoaster_ON.h"
#include "COrnament.h"
#include "sound.h"
#include "game.h"
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	定数定義
//=============================================================================
#define SPEED (0.2f)
#define COASTER_SPEED (0.4f)
#define MP_ADD (6)//1フレーム辺りのMP回復量
#define PLAYER_SAIZ (0.5f)
#define MPSTOCK_INIT (7)
#define ANGLE (3)
#define CORSTER_START (80)//コースターが来る時間
#define SLANT (45)
bool trigger = false;
//=============================================================================
//	静的変数
//=============================================================================
enum PlayerAnime
{
	PLAYER_WALK,
	PLAYER_IDLE,
	PLAYER_SET,
	PLAYER_DAMAGE,
	PLAYER_JUMP,
	PLAYER_COASTER,
	PLAYER_WHEEL,
	PLAYER_DOWN,
};

static DIJOYSTATE2 js;
static LPDIRECTINPUTDEVICE8 pJoyDevice;
static HRESULT hr;
static int doubleflag;	//キー二つ押しを格納

CPlayer *CPlayer::m_pPlayer[PLAYER_MAX] = {};
int CPlayer::m_PlayerNum = 0;
int CPlayer::m_KO_Count = 0;

bool CPlayer::g_CosterMode = false;

//=============================================================================
//  生成
//=============================================================================
CPlayer::CPlayer() :C3DObj(C3DObj::TYPE_PLAYER)
{


	Initialize();
	m_PlayerIndex = m_PlayerNum;
	m_PlayerNum++;
}

CPlayer *CPlayer::PlayerCreate(void)
{
	//m_pPlayer[m_PlayerNum-1] = new CPlayer;
	//return m_pPlayer[m_PlayerNum - 1];
	CPlayer *m_pPlayer = new CPlayer;
	return m_pPlayer;
}


//=============================================================================
// 破棄
//=============================================================================
CPlayer::~CPlayer()
{

	m_TitleFlag = true;
	//m_PlayerNum--;
}

//=============================================================================
// 初期化処理
//=============================================================================
void CPlayer::Initialize(void)
{
	DWORD i;

	trigger = false;
//	SkinMesh.InitThing(m_pD3DDevice, &Thing, ANIME_MODEL_FILES[MODELL_ANIME_PLAYER].filename);
	char animefile[TEXTURE_FILENAME_MAX] = { };
	strcpy(animefile, C3DObj::Get_AnimeFileName(MODELL_ANIME_PLAYER));
	SkinMesh.InitThing(m_pD3DDevice, &Thing, animefile);

	Thing.Sphere.fRadius = 1.3f;
	Thing.Sphere.vCenter = D3DXVECTOR3(0, 1.2f, 0);
	SkinMesh.InitSphere(m_pD3DDevice, &Thing);
	//モデル情報取得
	//Thing_Anime_model = GetAnimeModel();
	for (i = 0; i < Thing.pAnimController->GetNumAnimationSets(); i++)
	{//AnimSetにアニメーション情報格納
		Thing.pAnimController->GetAnimationSet(i, &pAnimSet[i]);
	}
	//アニメーション情報初期化
	TrackDesc.Weight = 1;
	TrackDesc.Enable = true;
	TrackDesc.Position = 0;//アニメーションタイムリセット
	TrackDesc.Speed = 0.01f;//モーションスピード
	Thing.pAnimController->SetTrackDesc(0, &TrackDesc);//アニメ情報セット
	Thing.pAnimController->SetTrackAnimationSet(0, pAnimSet[PLAYER_WALK]);//初期アニメーションセット
	m_AnimationType = PLAYER_WALK;
	Animation_Change(PLAYER_IDLE, 0.005f);
	bool *standby = C3DObj::GetCosterModeStandby();
	m_Hp = HP_MAX;
	m_Mp = 0;
	m_MpStock = MPSTOCK_INIT;
	m_KO_Count = 0;
	m_Enable = true;
	g_CosterMode = false;
	*standby = false;
	m_DrawCheck = true;
	m_MoveCheck = false;
	m_Direction = DIRE_UP;
	m_DrawCount = 0;
	m_SummonsNum = 0;
	right_trigger = false;
	left_trigger = false;

	D3DXMatrixTranslation(&m_mtxTranslation, 0, 1, -20);
	D3DXMatrixScaling(&m_mtxScaling, PLAYER_SAIZ, PLAYER_SAIZ, PLAYER_SAIZ);
	m_mtxKeepTranslation = m_mtxTranslation;
	D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(200));
	m_mtxWorld = m_mtxScaling * m_mtxRotation * m_mtxTranslation;
	m_mtxInit = m_mtxWorld;
	m_Angle = 270;



	js = { 0 };
	pJoyDevice = *JoyDevice_Get();
	if (pJoyDevice)
	{
		hr = pJoyDevice->Acquire();
	}
	Thing.vPosition = D3DXVECTOR3(m_mtxTranslation._41, m_mtxTranslation._42, m_mtxTranslation._43);

	m_TitleFlag = false;
}

void CPlayer::GameBegin(void)
{
	/*bool *standby = C3DObj::GetCosterModeStandby();
	m_Hp = HP_MAX;
	m_Mp = 0;
	m_MpStock = MPSTOCK_INIT;
	m_KO_Count = 0;
	m_Enable = true;
	g_CosterMode = false;
	*standby = false;
	m_DrawCheck = true;
	m_MoveCheck = false;
	m_Direction = DIRE_UP;
	m_DrawCount = 0;
	m_SummonsNum = 0;

	m_Angle = 270;
	m_mtxWorld = m_mtxInit;
	Animation_Change(PLAYER_WALK, 0.05f);


	js = { 0 };
	pJoyDevice = *JoyDevice_Get();
	if (pJoyDevice)
	{
	hr = pJoyDevice->Acquire();
	}
	Thing.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);

	m_TitleFlag = false;*/
}


void CPlayer::Finalize(void)
{
	bool *standby = C3DObj::GetCosterModeStandby();
	m_Hp = HP_MAX;
	m_Mp = 0;
	m_MpStock = MPSTOCK_INIT;
	m_KO_Count = 0;
	m_Enable = true;
	g_CosterMode = false;
	*standby = false;
	m_DrawCheck = true;
	m_MoveCheck = false;
	right_on = false;
	right_trigger = false;
	left_trigger = false;
	m_Direction = DIRE_UP;
	m_DrawCount = 0;
	m_SummonsNum = 0;

	m_Angle = 270;
	D3DXMatrixTranslation(&m_mtxTranslation, 0, 1, -20);
	D3DXMatrixScaling(&m_mtxScaling, PLAYER_SAIZ, PLAYER_SAIZ, PLAYER_SAIZ);
	m_mtxKeepTranslation = m_mtxTranslation;
	D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(200));
	m_mtxWorld = m_mtxScaling * m_mtxRotation * m_mtxTranslation;
	//m_mtxWorld = m_mtxInit;

	Animation_Change(PLAYER_IDLE, 0.005f);

	int i;
	for (i = 0;i < SUMMONS_MAX;i++)
	{
		m_CoolTime[i] = 0;
	}

	js = { 0 };
	pJoyDevice = *JoyDevice_Get();
	if (pJoyDevice)
	{
		hr = pJoyDevice->Acquire();
	}
	Thing.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);

	//m_TitleFlag = false;
}

//=============================================================================
//  更新
//=============================================================================
void CPlayer::Update(void)
{
	if (!m_TitleFlag)
	{
		if (!Get_Gameend())
		{
			bool *standby = C3DObj::GetCosterModeStandby();
			Player_Camera();

			//コントローラー情報があるときのみ取得
			if (pJoyDevice)
			{
				pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js);
			}
			/////////////////////////////          
			//コースターを呼んでるときは操作できないようにする
			if (!*standby)
			{
				if (g_CosterMode)//コースターの時
				{
					C3DObj *pcoaster = Coaster::Get_Coaster();
					if (pcoaster)
					{
						m_mtxTranslation *= Move(FLONT, COASTER_SPEED);
					}
					else
					{
						g_CosterMode = false;
						ControllerAngleChange(DIRE_UP);
						m_Direction = DIRE_UP;
					}
				}
				else
				{

					/*
					if (Keyboard_IsPress(DIK_1))
					{
						//Fadraw_Set(true, true);
					}*/


					if (pJoyDevice)//ゲームパッドがあったらゲームパッド操作にする
					{
						Player_GamepadMove();
						Player_Move();
						if (js.lRx == 6 || js.lRx == -6)
						{
							right_on = true;
						}
						if (js.lZ >= 1 && !left_trigger)
						{
							left_trigger = true;
						}
						if (js.lZ <= -1 && !right_trigger)
						{
							right_trigger = true;
						}
					}
					else
					{
						Player_Move();
					}

					Ornament_Check();
				}
				
			}
			//十字キーあるいはスティックにより移動
			if (m_KO_Count == 15)//スクリーンショット
			{													   // バックバファの取得
				LPDIRECT3DSURFACE9 pBackBuf;
				m_pD3DDevice->GetRenderTarget(0, &pBackBuf);

				// 繧ｹ繧ｯ繧ｷ繝ｧ蜃ｺ蜉・
				D3DXSaveSurfaceToFile("asset/screenshot.bmp", D3DXIFF_BMP, pBackBuf, NULL, NULL);

				// Get系で取得したサーフェイスはAddRefが呼ばれているので忘れずに解放する
				pBackBuf->Release();
			}
			D3DXMATRIX mtxr;
			D3DXMatrixRotationY(&m_mtxRotation, D3DXToRadian(m_Angle));

			
			Wall_Check();

			//	MP
			Mp_Add();

			Summons_Attraction();


			Player_Damage();
			if (*standby)
			{
				if (m_FrameCount - m_TimeKeep == CORSTER_START)
				{
					CAttraction::Create(CAttraction::AT_COASTER_ON);
				}
				if (m_FrameCount - m_TimeKeep == CORSTER_START + COSTERON_ENDTIME)
				{
					Animation_Change(PLAYER_WALK, 0.01f);
					CAttraction::Create(CAttraction::AT_COASTER);
					m_MpStock -= COASTER_MP;
					m_CoolTime[SUMMONS_COASTER] = (int)COOLTIME_COASTER;
					g_CosterMode = true;
					*standby = false;
				}
			}

			if (Keyboard_IsTrigger(DIK_L))
			{
				Game_End();
			}
			if (Keyboard_IsTrigger(DIK_2))
			{
				AngleChange(DIRE_DOWN);
				m_Direction = DIRE_DOWN;

				m_Hp = 0;
				*standby = true;
				Animation_Change(PLAYER_DOWN, 0.008f);
				Attraction_Delete();
				Game_End();
			}
			if (Keyboard_IsPress(DIK_1))
			{
				Exp_Set(ENEMY_HIKKAI, m_mtxWorld._41 - 4.0f, m_mtxWorld._42 + 1.0f, m_mtxWorld._43, 1.0f, 0.0f);
				Exp_Set(STAR, m_mtxWorld._41 + 2.0f, m_mtxWorld._42 + 1.0f, m_mtxWorld._43, 1.0f, 0.0f);
				Exp_Set(SHINE, m_mtxWorld._41 + 4.0f, m_mtxWorld._42 + 1.0f, m_mtxWorld._43, 1.0f, 0.0f);
				Exp_Set(HIT, m_mtxWorld._41 + 12.0f, m_mtxWorld._42 + 1.0f, m_mtxWorld._43, 1.0f, 0.0f);

				Exp_Set(IMPACT_13, m_mtxWorld._41 - 6.0f, m_mtxWorld._42 + 1.0f, m_mtxWorld._43, 1.0f, 0.0f);
				Exp_Set(SURASYU, m_mtxWorld._41 - 8.0f, m_mtxWorld._42 + 1.0f, m_mtxWorld._43, 1.0f, 0.0f);
				Exp_Set(SMALLSTAR, m_mtxWorld._41 - 10.0f, m_mtxWorld._42 + 1.0f, m_mtxWorld._43, 1.0f, 0.0f);
			}
		}
	}
}
//=============================================================================
// 描画
//=============================================================================

void CPlayer::Draw(void)
{
	if (!m_TitleFlag)
	{
		m_mtxWorld = m_mtxScaling * m_mtxRotation * m_mtxTranslation;
		Thing.vPosition = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	//縲繝ｩ繧､繝・ぅ繝ｳ繧ｰ譛牙柑

		if (!g_CosterMode)//コースターの時は描画しない
		{
			if (!m_DrawCheck)
			{
				if (m_FrameCount % 2 == 0)
				{
					DrawDX_Anime(m_mtxWorld, MODELL_ANIME_PLAYER, &Thing);

					m_DrawCount++;
					if (m_DrawCount >= 20)
					{
						m_DrawCount = 0;
						m_DrawCheck = true;
					}
				}
			}
			else
			{
				DrawDX_Anime(m_mtxWorld, MODELL_ANIME_PLAYER, &Thing);
			}
		}

		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		//	デバッグ
		if (m_SummonsNum == SUMMONS_COFFEE)
		{
			DebugFont_Draw(10, 500, "COFFEE");
		}
		if (m_SummonsNum == SUMMONS_FALL)
		{
			DebugFont_Draw(10, 500, "FALL");
		}
		if (m_SummonsNum == SUMMONS_WHEEL)
		{
			DebugFont_Draw(10, 500, "WHEEL");
		}
		if (m_SummonsNum == SUMMONS_COASTER)
		{
			DebugFont_Draw(10, 500, "COASTER");
		}
		if (m_SummonsNum == SUMMONS_POPCORN)
		{
			DebugFont_Draw(10, 500, "POPCORN");
		}


		for (int i = 0; i < SUMMONS_MAX; i++)
		{
			DebugFont_Draw(10, 530 + (30 * i), "%d", m_CoolTime[i]);
		}
		//コントローラーのスティック取得
		DebugFont_Draw(600, 300, "X = %ld , Y= %ld", js.lX, js.lY);
		//DebugFont_Draw(600, 300, "X = %f , Z= %f", m_mtxWorld._41, m_mtxWorld._43);
		DebugFont_Draw(600, 150, "左X = %ld ", js.lRx);
		//DebugFont_Draw(600, 500, "m_FrameCount - m_TimeKeep = %d\n,", m_FrameCount - m_TimeKeep);
	}
}





C3DObj *CPlayer::Get_Player(void)
{
	int i;
	C3DObj *pplayer;

	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		pplayer = C3DObj::Get(i);
		if (pplayer->Get_3DObjType() == C3DObj::TYPE_PLAYER)
		{
			return pplayer;
		}
	}
	return 0;
}




//=============================================================================
// ダメージ処理
//=============================================================================

void CPlayer::Player_Damage(void)
{
	int i;
	bool *standby = C3DObj::GetCosterModeStandby();
	if (!*standby && m_DrawCheck && !g_CosterMode)
	{
		for (i = 0;i < MAX_GAMEOBJ;i++)
		{
			C3DObj *enemy = CEnemy::Get_Enemy(i);
			if (enemy)
			{
				if (enemy->Get_AttacFlag())
				{
					Thing.vPosition = D3DXVECTOR3(m_mtxTranslation._41, m_mtxTranslation._42, m_mtxTranslation._43);
					THING *thingenemy = enemy->GetAnimeModel();
					if (C3DObj::Collision_AnimeVSAnime(&Thing, thingenemy))
					{
						Exp_Set(SURASYU, m_mtxWorld._41, m_mtxWorld._42 + 1, m_mtxWorld._43, 3.0, 0);
						PlaySound(BIGHIT_SE);
						m_Hp -= enemy->Get_Attck();
						Animation_Change(PLAYER_WALK, 0.05f);
						m_DrawCheck = false;

						if (m_Hp <= 0)
						{
							Animation_Change(PLAYER_DOWN, 0.1f);
							Attraction_Delete();
							Game_End();
						}
						break;
					}
				}
			}
		}
	}
}


//=============================================================================
// 移動処理
//=============================================================================


void CPlayer::Player_Move(void)
{
	//	斜め向き変更

	//	右上
	if (((Keyboard_IsPress(DIK_D)) && (Keyboard_IsTrigger(DIK_W))) || ((Keyboard_IsTrigger(DIK_D)) && (Keyboard_IsPress(DIK_W))) || ((Keyboard_IsTrigger(DIK_D)) && (Keyboard_IsTrigger(DIK_W)||(m_PadDirection == DIRE_UP_RIGHT))))
	{
		if (!doubleflag)
		{
			AngleChange(DIRE_UP_RIGHT);
			m_Direction = DIRE_UP_RIGHT;
			m_MoveCheck = true;
			doubleflag = true;
		}
	}
	//　左上
	if (((Keyboard_IsPress(DIK_A)) && (Keyboard_IsTrigger(DIK_W))) || ((Keyboard_IsTrigger(DIK_A)) && (Keyboard_IsPress(DIK_W))) || ((Keyboard_IsTrigger(DIK_A)) && (Keyboard_IsTrigger(DIK_W) || (m_PadDirection == DIRE_UP_LEFT))))
	{
		if (!doubleflag)
		{
			AngleChange(DIRE_UP_LEFT);
			m_Direction = DIRE_UP_LEFT;
			m_MoveCheck = true;
			doubleflag = true;
		}
	}
	//　右下
	if (((Keyboard_IsPress(DIK_D)) && (Keyboard_IsTrigger(DIK_S))) || ((Keyboard_IsTrigger(DIK_D)) && (Keyboard_IsPress(DIK_S))) || ((Keyboard_IsTrigger(DIK_D)) && (Keyboard_IsTrigger(DIK_S) || (m_PadDirection == DIRE_DOWN_RIGHT))))
	{
		if (!doubleflag)
		{
			AngleChange(DIRE_DOWN_RIGHT);
			m_Direction = DIRE_DOWN_RIGHT;
			m_MoveCheck = true;
			doubleflag = true;
		}
	}
	//　左下
	if (((Keyboard_IsPress(DIK_A)) && (Keyboard_IsTrigger(DIK_S))) || ((Keyboard_IsTrigger(DIK_A)) && (Keyboard_IsPress(DIK_S))) || ((Keyboard_IsTrigger(DIK_A)) && (Keyboard_IsTrigger(DIK_S) || (m_PadDirection == DIRE_DOWN_LEFT))))
	{
		if (!doubleflag)
		{
			AngleChange(DIRE_DOWN_LEFT);
			m_Direction = DIRE_DOWN_LEFT;
			m_MoveCheck = true;
			doubleflag = true;
		}
	}


	//	斜め移動終了
	if ((Keyboard_IsRelease(DIK_D)) && (Keyboard_IsPress(DIK_W)))
	{
		if (doubleflag)
		{
			m_Angle -= SLANT;
			m_Direction = DIRE_UP;
			AngleChange(m_Direction);
			doubleflag = false;
		}
	}
	if ((Keyboard_IsPress(DIK_D)) && (Keyboard_IsRelease(DIK_W)))
	{
		if (doubleflag)
		{
			m_Angle += SLANT;
			m_Direction = DIRE_RIGHT;
			AngleChange(m_Direction);
			doubleflag = false;
		}
	}
	if ((Keyboard_IsRelease(DIK_A)) && (Keyboard_IsPress(DIK_W)))
	{
		if (doubleflag)
		{
			m_Angle += SLANT;
			m_Direction = DIRE_UP;
			AngleChange(m_Direction);
			doubleflag = false;
		}
	}
	if ((Keyboard_IsPress(DIK_A)) && (Keyboard_IsRelease(DIK_W)))
	{
		if (doubleflag)
		{
			m_Angle -= SLANT;
			m_Direction = DIRE_LEFT;
			AngleChange(m_Direction);
			doubleflag = false;
		}
	}
	if ((Keyboard_IsRelease(DIK_D)) && (Keyboard_IsPress(DIK_S)))
	{
		if (doubleflag)
		{
			m_Angle += SLANT;
			m_Direction = DIRE_DOWN;
			AngleChange(m_Direction);
			doubleflag = false;
		}
	}
	if ((Keyboard_IsPress(DIK_D)) && (Keyboard_IsRelease(DIK_S)))
	{
		if (doubleflag)
		{
			m_Angle -= SLANT;
			m_Direction = DIRE_RIGHT;
			AngleChange(m_Direction);
			doubleflag = false;
		}
	}
	if ((Keyboard_IsRelease(DIK_A)) && (Keyboard_IsPress(DIK_S)))
	{
		if (doubleflag)
		{
			m_Angle -= SLANT;
			m_Direction = DIRE_DOWN;
			AngleChange(m_Direction);
			doubleflag = false;
		}
	}
	if ((Keyboard_IsPress(DIK_A)) && (Keyboard_IsRelease(DIK_S)))
	{
		if (doubleflag)
		{
			m_Angle += SLANT;
			m_Direction = DIRE_LEFT;
			AngleChange(m_Direction);
			doubleflag = false;
		}
	}


	//	斜め移動
	//	右上
	if (((Keyboard_IsPress(DIK_D)) && (Keyboard_IsPress(DIK_W)) || (m_PadDirection == DIRE_UP_RIGHT)))
	{
		m_mtxTranslation *= Move(FLONT, SPEED);
		m_mtxTranslation *= Move(RIGHT, SPEED / 2);
		AngleChange(DIRE_UP_RIGHT);
		m_Direction = DIRE_UP_RIGHT;
		doubleflag = true;
	}
	//	左上
	if ((Keyboard_IsPress(DIK_W) && (Keyboard_IsPress(DIK_A)) || (m_PadDirection == DIRE_UP_LEFT)))
	{
		m_mtxTranslation *= Move(FLONT, SPEED);
		m_mtxTranslation *= Move(LEFT, SPEED / 2);
		AngleChange(DIRE_UP_LEFT);
		m_Direction = DIRE_UP_LEFT;
		doubleflag = true;
	}
	//	左下
	if ((Keyboard_IsPress(DIK_S) && (Keyboard_IsPress(DIK_A)) || (m_PadDirection == DIRE_DOWN_LEFT)))
	{
		m_mtxTranslation *= Move(BACK, SPEED);
		m_mtxTranslation *= Move(LEFT, SPEED / 2);
		AngleChange(DIRE_DOWN_LEFT);
		m_Direction = DIRE_DOWN_LEFT;
		doubleflag = true;
	}
	//　右下
	if ((Keyboard_IsPress(DIK_S) && (Keyboard_IsPress(DIK_D)) || (m_PadDirection == DIRE_DOWN_RIGHT)))
	{
		m_mtxTranslation *= Move(BACK, SPEED);
		m_mtxTranslation *= Move(RIGHT, SPEED / 2);
		AngleChange(DIRE_DOWN_RIGHT);
		m_Direction = DIRE_DOWN_RIGHT;
		doubleflag = true;
	}
	if (doubleflag == false)
	{
		//	十字移動　向き変更
		if (Keyboard_IsTrigger(DIK_W) && (m_Direction != DIRE_UP) && (!m_MoveCheck))
		{
			AngleChange(DIRE_UP);
			m_MoveCheck = true;
		}
		if (Keyboard_IsTrigger(DIK_S) && (m_Direction != DIRE_DOWN) && (!m_MoveCheck))
		{
			AngleChange(DIRE_DOWN);
			m_MoveCheck = true;
		}
		if (Keyboard_IsTrigger(DIK_D) && (m_Direction != DIRE_RIGHT) && (!m_MoveCheck))
		{
			AngleChange(DIRE_RIGHT);
			m_MoveCheck = true;
		}
		if (Keyboard_IsTrigger(DIK_A) && (m_Direction != DIRE_LEFT) && (!m_MoveCheck))
		{
			AngleChange(DIRE_LEFT);
			m_MoveCheck = true;
		}

		//	移動終了　待機アニメーション
		if ((Keyboard_IsRelease(DIK_W)) || (Keyboard_IsRelease(DIK_S)) || (Keyboard_IsRelease(DIK_D)) || (Keyboard_IsRelease(DIK_A)))
		{
			m_MoveCheck = false;
			Animation_Change(PLAYER_IDLE, 0.005f);
		}

		//	十字移動
		if (Keyboard_IsPress(DIK_W) || (m_PadDirection == DIRE_UP))
		{
			Animation_Change(PLAYER_WALK, 0.01f);
			if ((Keyboard_IsRelease(DIK_D)) || (Keyboard_IsRelease(DIK_A)))
			{
				AngleChange(DIRE_UP);
			}
			if (!Keyboard_IsPress(DIK_S))
			{
				m_Direction = DIRE_UP;
				AngleChange(DIRE_UP);
				m_mtxTranslation *= Move(FLONT, SPEED);
				m_MoveCheck = true;
			}
		}
		if (Keyboard_IsPress(DIK_S) || (m_PadDirection == DIRE_DOWN))
		{
			Animation_Change(PLAYER_WALK, 0.01f);
			if ((Keyboard_IsRelease(DIK_D)) || (Keyboard_IsRelease(DIK_A)))
			{
				AngleChange(DIRE_DOWN);
			}
			if (!Keyboard_IsPress(DIK_W))
			{
				m_Direction = DIRE_DOWN;
				AngleChange(DIRE_DOWN);
				m_mtxTranslation *= Move(BACK, SPEED);
				m_MoveCheck = true;
			}
		}
		if (Keyboard_IsPress(DIK_D) || (m_PadDirection == DIRE_RIGHT))
		{
			Animation_Change(PLAYER_WALK, 0.01f);
			if ((Keyboard_IsRelease(DIK_W)) || (Keyboard_IsRelease(DIK_S)) || (Keyboard_IsRelease(DIK_A)))
			{
				AngleChange(DIRE_RIGHT);
			}
			if (!Keyboard_IsPress(DIK_A))
			{
				m_Direction = DIRE_RIGHT;
				AngleChange(DIRE_RIGHT);
				m_mtxTranslation *= Move(RIGHT, SPEED);
				m_MoveCheck = true;
			}
		}
		if (Keyboard_IsPress(DIK_A) || (m_PadDirection == DIRE_LEFT))
		{
			Animation_Change(PLAYER_WALK, 0.01f);
			if ((Keyboard_IsRelease(DIK_W)) || (Keyboard_IsRelease(DIK_S)) || (Keyboard_IsRelease(DIK_D)))
			{
				AngleChange(DIRE_LEFT);
			}
			if (!Keyboard_IsPress(DIK_D))
			{
				m_Direction = DIRE_LEFT;
				AngleChange(DIRE_LEFT);
				m_mtxTranslation *= Move(LEFT, SPEED);
				m_MoveCheck = true;
			}
		}
	}
	//doubleflag = false;
}


//=============================================================================
// 移動処理（ゲームパッド）
//=============================================================================

void CPlayer::Player_GamepadMove(void)
{
	//左上
	if (js.lX >= -5 && js.lX <= -3 && js.lY >= -5 && js.lY <= -2 || js.lX == -6 && js.lY == -4)
	{
		/*m_mtxTranslation *= Move(FLONT, SPEED);
		m_mtxTranslation *= Move(LEFT, SPEED / 2);
		ControllerAngleChange(DIRE_UP_LEFT);
		m_Direction = DIRE_UP_LEFT;
		doubleflag = true;*/
		Animation_Change(PLAYER_WALK, 0.01f);
		m_PadDirection = DIRE_UP_LEFT;
	}
	//左下
	if (js.lX >= -5 && js.lX <= -3 && js.lY >= 3 && js.lY <= 5 || js.lX == -4 && js.lY == 6 || js.lX == -6 && js.lY == 4)
	{
		/*m_mtxTranslation *= Move(BACK, SPEED);
		m_mtxTranslation *= Move(LEFT, SPEED / 2);
		ControllerAngleChange(DIRE_DOWN_LEFT);
		m_Direction = DIRE_DOWN_LEFT;
		doubleflag = true;*/
		Animation_Change(PLAYER_WALK, 0.01f);
		m_PadDirection = DIRE_DOWN_LEFT;
	}
	//右上
	if (js.lX >= 3 && js.lX <= 5 && js.lY >= -6 && js.lY <= -4 || js.lX == 6 && js.lY == -4)
	{
		/*m_mtxTranslation *= Move(FLONT, SPEED);
		m_mtxTranslation *= Move(RIGHT, SPEED / 2);
		ControllerAngleChange(DIRE_UP_RIGHT);
		m_Direction = DIRE_UP_RIGHT;
		doubleflag = true;*/
		Animation_Change(PLAYER_WALK, 0.01f);
		m_PadDirection = DIRE_UP_RIGHT;
	}
	//右下
	if (js.lX >= 3 && js.lX <= 5 && js.lY >= 3 && js.lY <= 5 || js.lX == 3 && js.lY == 6 || js.lX == 6 && js.lY == 4)
	{
		/*m_mtxTranslation *= Move(BACK, SPEED);
		m_mtxTranslation *= Move(RIGHT, SPEED / 2);
		ControllerAngleChange(DIRE_DOWN_RIGHT);
		m_Direction = DIRE_DOWN_RIGHT;
		doubleflag = true;*/
		Animation_Change(PLAYER_WALK, 0.01f);
		m_PadDirection = DIRE_DOWN_RIGHT;
	}
	//上
	if (js.lX >= -3 && js.lX <= 2 && js.lY == -6)
	{
		/*Animation_Change(PLAYER_WALK, 0.01f);
		ControllerAngleChange(DIRE_UP);
		m_Direction = DIRE_UP;
		m_mtxTranslation *= Move(FLONT, SPEED);
		m_MoveCheck = true;*/
		Animation_Change(PLAYER_WALK, 0.01f);
		ControllerAngleChange(DIRE_UP);
		doubleflag = false;
		m_PadDirection = DIRE_UP;
	}
	//下
	if (js.lX >= -3 && js.lX <= 2 && js.lY == 6)
	{
		/*Animation_Change(PLAYER_WALK, 0.01f);
		ControllerAngleChange(DIRE_DOWN);
		m_Direction = DIRE_DOWN;
		m_mtxTranslation *= Move(BACK, SPEED);
		m_MoveCheck = true;*/
		Animation_Change(PLAYER_WALK, 0.01f);
		ControllerAngleChange(DIRE_DOWN);
		doubleflag = false;
		m_PadDirection = DIRE_DOWN;
	}
	//右
	if (js.lY >= -3 && js.lY <= 3 && js.lX == 6)
	{
		/*Animation_Change(PLAYER_WALK, 0.01f);
		ControllerAngleChange(DIRE_RIGHT);
		m_Direction = DIRE_RIGHT;
		m_mtxTranslation *= Move(RIGHT, SPEED);
		m_MoveCheck = true;*/
		Animation_Change(PLAYER_WALK, 0.01f);
		ControllerAngleChange(DIRE_RIGHT);
		doubleflag = false;
		m_PadDirection = DIRE_RIGHT;
	}
	//左
	if (js.lY >= -3 && js.lY <= 3 && js.lX == -6)
	{
		/*Animation_Change(PLAYER_WALK, 0.01f);
		ControllerAngleChange(DIRE_LEFT);
		m_Direction = DIRE_LEFT;
		m_mtxTranslation *= Move(LEFT, SPEED);
		m_MoveCheck = true;*/
		Animation_Change(PLAYER_WALK, 0.01f);
		ControllerAngleChange(DIRE_LEFT);
		doubleflag = false;
		m_PadDirection = DIRE_LEFT;
	}
	if (js.lX <= 2 && js.lX >= 0 && js.lY >= -2 && js.lY <= 1)
	{
		/*ControllerAngleChange(DIRE_UP);
		m_Direction = DIRE_UP;*/
		Animation_Change(PLAYER_IDLE, 0.005f);
		m_MoveCheck = false;
		doubleflag = false;
		m_PadDirection = DIRE_N;
	}
}
//=============================================================================
// 方向変更
//=============================================================================

void CPlayer::AngleChange(int index)
{

	/*if (index == DIRE_UP)
	{
		if (m_Direction == DIRE_RIGHT)
		{
			m_Angle -= 90;
		}
		if (m_Direction == DIRE_LEFT)
		{
			m_Angle += 90;
		}
		if (m_Direction == DIRE_DOWN)
		{
			m_Angle += 180;
		}
	}

	if (index == DIRE_DOWN)
	{
		if (m_Direction == DIRE_RIGHT)
		{
			m_Angle += 90;
		}
		if (m_Direction == DIRE_LEFT)
		{
			m_Angle -= 90;
		}
		if (m_Direction == DIRE_UP)
		{
			m_Angle -= 180;
		}
	}

	if (index == DIRE_RIGHT)
	{
		if (m_Direction == DIRE_DOWN)
		{
			m_Angle -= 90;
		}
		if (m_Direction == DIRE_LEFT)
		{
			m_Angle += 180;
		}
		if (m_Direction == DIRE_UP)
		{
			m_Angle += 90;
		}
	}

	if (index == DIRE_LEFT)
	{
		if (m_Direction == DIRE_DOWN)
		{
			m_Angle += 90;
		}
		if (m_Direction == DIRE_RIGHT)
		{
			m_Angle += 180;
		}
		if (m_Direction == DIRE_UP)
		{
			m_Angle -= 90;
		}
	}

	if (index == DIRE_UP_RIGHT)
	{
		if (m_Direction == DIRE_UP)
		{
			m_Angle += SLANT;
		}
		if (m_Direction == DIRE_RIGHT)
		{
			m_Angle -= SLANT;
		}

	}

	if (index == DIRE_UP_LEFT)
	{
		if (m_Direction == DIRE_UP)
		{
			m_Angle -= SLANT;
		}
		if (m_Direction == DIRE_LEFT)
		{
			m_Angle += SLANT;
		}
	}

	if (index == DIRE_DOWN_RIGHT)
	{
		if (m_Direction == DIRE_DOWN)
		{
			m_Angle -= SLANT;
		}
		if (m_Direction == DIRE_RIGHT)
		{
			m_Angle += SLANT;
		}
	}

	if (index == DIRE_DOWN_LEFT)
	{
		if (m_Direction == DIRE_DOWN)
		{
			m_Angle += SLANT;
		}
		if (m_Direction == DIRE_LEFT)
		{
			m_Angle -= SLANT;
		}
	}*/
if (index == DIRE_UP)
{
	if (m_Direction == DIRE_RIGHT)
	{
		m_Angle -= 90;
	}
	if (m_Direction == DIRE_LEFT)
	{
		m_Angle += 90;
	}
	if (m_Direction == DIRE_DOWN)
	{
		m_Angle += 180;
	}
	if (m_Direction == DIRE_DOWN_LEFT)
	{
		m_Angle += 135;
	}
	if (m_Direction == DIRE_DOWN_RIGHT)
	{
		m_Angle += 225;
	}
	if (m_Direction == DIRE_UP_RIGHT)
	{
		m_Angle -= SLANT;
	}
	if (m_Direction == DIRE_UP_LEFT)
	{
		m_Angle += SLANT;
	}
}

if (index == DIRE_DOWN)
{
	if (m_Direction == DIRE_RIGHT)
	{
		m_Angle += 90;
	}
	if (m_Direction == DIRE_LEFT)
	{
		m_Angle -= 90;
	}
	if (m_Direction == DIRE_UP)
	{
		m_Angle -= 180;
	}
	if (m_Direction == DIRE_UP_RIGHT)
	{
		m_Angle -= 225;
	}
	if (m_Direction == DIRE_UP_LEFT)
	{
		m_Angle -= 135;
	}
	if (m_Direction == DIRE_DOWN_RIGHT)
	{
		m_Angle += SLANT;
	}
	if (m_Direction == DIRE_DOWN_LEFT)
	{
		m_Angle -= SLANT;
	}
}

if (index == DIRE_RIGHT)
{
	if (m_Direction == DIRE_DOWN)
	{
		m_Angle -= 90;
	}
	if (m_Direction == DIRE_LEFT)
	{
		m_Angle += 180;
	}
	if (m_Direction == DIRE_UP)
	{
		m_Angle += 90;
	}
	if (m_Direction == DIRE_DOWN_LEFT)
	{
		m_Angle -= 135;
	}
	if (m_Direction == DIRE_UP_LEFT)
	{
		m_Angle -= 225;
	}
	if (m_Direction == DIRE_DOWN_RIGHT)
	{
		m_Angle -= SLANT;
	}
	if (m_Direction == DIRE_UP_RIGHT)
	{
		m_Angle += SLANT;
	}
}
if (index == DIRE_LEFT)
{
	if (m_Direction == DIRE_DOWN)
	{
		m_Angle += 90;
	}
	if (m_Direction == DIRE_RIGHT)
	{
		m_Angle += 180;
	}
	if (m_Direction == DIRE_UP)
	{
		m_Angle -= 90;
	}
	if (m_Direction == DIRE_DOWN_RIGHT)
	{
		m_Angle += 135;
	}
	if (m_Direction == DIRE_UP_RIGHT)
	{
		m_Angle -= 135;
	}
	if (m_Direction == DIRE_DOWN_LEFT)
	{
		m_Angle += SLANT;
	}
	if (m_Direction == DIRE_UP_LEFT)
	{
		m_Angle -= SLANT;
	}
}

if (index == DIRE_UP_RIGHT)
{
	if (m_Direction == DIRE_DOWN)
	{
		m_Angle += 225;
	}
	if (m_Direction == DIRE_LEFT)
	{
		m_Angle += 135;
	}
	if (m_Direction == DIRE_DOWN_LEFT)
	{
		m_Angle += 180;
	}
	if (m_Direction == DIRE_UP_LEFT)
	{
		m_Angle += 90;
	}
	if (m_Direction == DIRE_DOWN_RIGHT)
	{
		m_Angle += 270;
	}
	if (m_Direction == DIRE_UP)
	{
		m_Angle += SLANT;
	}
	if (m_Direction == DIRE_RIGHT)
	{
		m_Angle -= SLANT;
	}
}

if (index == DIRE_UP_LEFT)
{
	if (m_Direction == DIRE_DOWN)
	{
		m_Angle += 135;
	}
	if (m_Direction == DIRE_RIGHT)
	{
		m_Angle -= 135;
	}
	if (m_Direction == DIRE_DOWN_RIGHT)
	{
		m_Angle += 180;
	}
	if (m_Direction == DIRE_DOWN_LEFT)
	{
		m_Angle += 90;
	}
	if (m_Direction == DIRE_UP_RIGHT)
	{
		m_Angle -= 90;
	}
	if (m_Direction == DIRE_UP)
	{
		m_Angle -= SLANT;
	}
	if (m_Direction == DIRE_LEFT)
	{
		m_Angle += SLANT;
	}
}

if (index == DIRE_DOWN_RIGHT)
{
	if (m_Direction == DIRE_UP_RIGHT)
	{
		m_Angle -= 270;
	}
	if (m_Direction == DIRE_DOWN_LEFT)
	{
		m_Angle -= 90;
	}
	if (m_Direction == DIRE_UP_LEFT)
	{
		m_Angle -= 180;
	}
	if (m_Direction == DIRE_LEFT)
	{
		m_Angle -= 135;
	}
	if (m_Direction == DIRE_UP)
	{
		m_Angle -= 225;
	}
	if (m_Direction == DIRE_DOWN)
	{
		m_Angle -= SLANT;
	}
	if (m_Direction == DIRE_RIGHT)
	{
		m_Angle += SLANT;
	}
}

if (index == DIRE_DOWN_LEFT)
{
	if (m_Direction == DIRE_UP_RIGHT)
	{
		m_Angle -= 180;
	}
	if (m_Direction == DIRE_DOWN_RIGHT)
	{
		m_Angle += 90;
	}
	if (m_Direction == DIRE_UP_LEFT)
	{
		m_Angle -= 90;
	}
	if (m_Direction == DIRE_RIGHT)
	{
		m_Angle += 135;
	}
	if (m_Direction == DIRE_UP)
	{
		m_Angle -= 135;
	}
	if (m_Direction == DIRE_DOWN)
	{
		m_Angle += SLANT;
	}
	if (m_Direction == DIRE_LEFT)
	{
		m_Angle -= SLANT;
	}
}
}

//=============================================================================
// 方向変更(ゲームパッド)
//=============================================================================

void CPlayer::ControllerAngleChange(int index)
{
	if (index == DIRE_UP)
	{
		if (m_Direction == DIRE_RIGHT)
		{
			m_Angle -= 90;
		}
		if (m_Direction == DIRE_LEFT)
		{
			m_Angle += 90;
		}
		if (m_Direction == DIRE_DOWN)
		{
			m_Angle += 180;
		}
		if (m_Direction == DIRE_DOWN_LEFT)
		{
			m_Angle += 135;
		}
		if (m_Direction == DIRE_DOWN_RIGHT)
		{
			m_Angle += 225;
		}
		if (m_Direction == DIRE_UP_RIGHT)
		{
			m_Angle -= SLANT;
		}
		if (m_Direction == DIRE_UP_LEFT)
		{
			m_Angle += SLANT;
		}
	}

	if (index == DIRE_DOWN)
	{
		if (m_Direction == DIRE_RIGHT)
		{
			m_Angle += 90;
		}
		if (m_Direction == DIRE_LEFT)
		{
			m_Angle -= 90;
		}
		if (m_Direction == DIRE_UP)
		{
			m_Angle -= 180;
		}
		if (m_Direction == DIRE_UP_RIGHT)
		{
			m_Angle -= 225;
		}
		if (m_Direction == DIRE_UP_LEFT)
		{
			m_Angle -= 135;
		}
		if (m_Direction == DIRE_DOWN_RIGHT)
		{
			m_Angle += SLANT;
		}
		if (m_Direction == DIRE_DOWN_LEFT)
		{
			m_Angle -= SLANT;
		}
	}

	if (index == DIRE_RIGHT)
	{
		if (m_Direction == DIRE_DOWN)
		{
			m_Angle -= 90;
		}
		if (m_Direction == DIRE_LEFT)
		{
			m_Angle += 180;
		}
		if (m_Direction == DIRE_UP)
		{
			m_Angle += 90;
		}
		if (m_Direction == DIRE_DOWN_LEFT)
		{
			m_Angle -= 135;
		}
		if (m_Direction == DIRE_UP_LEFT)
		{
			m_Angle -= 225;
		}
		if (m_Direction == DIRE_DOWN_RIGHT)
		{
			m_Angle -= SLANT;
		}
		if (m_Direction == DIRE_UP_RIGHT)
		{
			m_Angle += SLANT;
		}
	}
	if (index == DIRE_LEFT)
	{
		if (m_Direction == DIRE_DOWN)
		{
			m_Angle += 90;
		}
		if (m_Direction == DIRE_RIGHT)
		{
			m_Angle += 180;
		}
		if (m_Direction == DIRE_UP)
		{
			m_Angle -= 90;
		}
		if (m_Direction == DIRE_DOWN_RIGHT)
		{
			m_Angle += 135;
		}
		if (m_Direction == DIRE_UP_RIGHT)
		{
			m_Angle -= 135;
		}
		if (m_Direction == DIRE_DOWN_LEFT)
		{
			m_Angle += SLANT;
		}
		if (m_Direction == DIRE_UP_LEFT)
		{
			m_Angle -= SLANT;
		}
	}

	if (index == DIRE_UP_RIGHT)
	{
		if (m_Direction == DIRE_DOWN)
		{
			m_Angle += 225;
		}
		if (m_Direction == DIRE_LEFT)
		{
			m_Angle += 135;
		}
		if (m_Direction == DIRE_DOWN_LEFT)
		{
			m_Angle += 180;
		}
		if (m_Direction == DIRE_UP_LEFT)
		{
			m_Angle += 90;
		}
		if (m_Direction == DIRE_DOWN_RIGHT)
		{
			m_Angle += 270;
		}
		if (m_Direction == DIRE_UP)
		{
			m_Angle += SLANT;
		}
		if (m_Direction == DIRE_RIGHT)
		{
			m_Angle -= SLANT;
		}
	}

	if (index == DIRE_UP_LEFT)
	{
		if (m_Direction == DIRE_DOWN)
		{
			m_Angle += 135;
		}
		if (m_Direction == DIRE_RIGHT)
		{
			m_Angle -= 135;
		}
		if (m_Direction == DIRE_DOWN_RIGHT)
		{
			m_Angle += 180;
		}
		if (m_Direction == DIRE_DOWN_LEFT)
		{
			m_Angle += 90;
		}
		if (m_Direction == DIRE_UP_RIGHT)
		{
			m_Angle -= 90;
		}
		if (m_Direction == DIRE_UP)
		{
			m_Angle -= SLANT;
		}
		if (m_Direction == DIRE_LEFT)
		{
			m_Angle += SLANT;
		}
	}

	if (index == DIRE_DOWN_RIGHT)
	{
		if (m_Direction == DIRE_UP_RIGHT)
		{
			m_Angle -= 270;
		}
		if (m_Direction == DIRE_DOWN_LEFT)
		{
			m_Angle -= 90;
		}
		if (m_Direction == DIRE_UP_LEFT)
		{
			m_Angle -= 180;
		}
		if (m_Direction == DIRE_LEFT)
		{
			m_Angle -= 135;
		}
		if (m_Direction == DIRE_UP)
		{
			m_Angle -= 225;
		}
		if (m_Direction == DIRE_DOWN)
		{
			m_Angle -= SLANT;
		}
		if (m_Direction == DIRE_RIGHT)
		{
			m_Angle += SLANT;
		}
	}

	if (index == DIRE_DOWN_LEFT)
	{
		if (m_Direction == DIRE_UP_RIGHT)
		{
			m_Angle -= 180;
		}
		if (m_Direction == DIRE_DOWN_RIGHT)
		{
			m_Angle += 90;
		}
		if (m_Direction == DIRE_UP_LEFT)
		{
			m_Angle -= 90;
		}
		if (m_Direction == DIRE_RIGHT)
		{
			m_Angle += 135;
		}
		if (m_Direction == DIRE_UP)
		{
			m_Angle -= 135;
		}
		if (m_Direction == DIRE_DOWN)
		{
			m_Angle += SLANT;
		}
		if (m_Direction == DIRE_LEFT)
		{
			m_Angle -= SLANT;
		}
	}
}

//=============================================================================
// カメラ処理
//=============================================================================

void CPlayer::Player_Camera(void)
{
	//	繧ｫ繝｡繝ｩ蜷代″螟画峩
	if (CCamera::Get_CameraAngleCheck())
	{
		m_Angle = CCamera::Get_Angle();
	}
	//	繧ｫ繝｡繝ｩ繝ｪ繧ｻ繝・ヨ
	if ((Keyboard_IsRelease(DIK_RIGHT)) || (Keyboard_IsRelease(DIK_LEFT)))
	{
		if (m_Direction % 2 == 0)
		{
			AngleChange(DIRE_UP);
			//ControllerAngleChange(DIRE_UP);
		}
		else
		{
			if (m_Direction == DIRE_RIGHT)
			{
				AngleChange(DIRE_DOWN);
				//ControllerAngleChange(DIRE_DOWN);
			}
			else
			{
				AngleChange(DIRE_DOWN);
				//ControllerAngleChange(DIRE_DOWN);
			}
		}
	/*	if ((m_Direction == DIRE_UP) || (m_Direction == DIRE_UP))
		{
			ControllerAngleChange(DIRE_UP);
		}
		else if ((m_Direction == DIRE_RIGHT) || (m_Direction == DIRE_LEFT))
		{
			ControllerAngleChange(DIRE_DOWN);
		}
		else
		{
			ControllerAngleChange(DIRE_UP);
		}*/
	}
	if (pJoyDevice)
	{
		if (right_on &&js.lRx == 1 || right_on &&js.lRx == 0)
		{
			/*if (m_Direction % 2 == 0)
			{
				AngleChange(DIRE_UP);
			}
			else
			{
				if (m_Direction == DIRE_RIGHT)
				{
					AngleChange(DIRE_DOWN);
				}
				else
				{
					AngleChange(DIRE_DOWN);
				}
			}*/
			if ((m_Direction == DIRE_UP) || (m_Direction == DIRE_DOWN))
			{
				ControllerAngleChange(DIRE_UP);
			}
			else if ((m_Direction == DIRE_RIGHT) || (m_Direction == DIRE_LEFT))
			{
				ControllerAngleChange(DIRE_DOWN);
			}
			else if((m_Direction == DIRE_UP_RIGHT)|| (m_Direction == DIRE_DOWN_LEFT))
			{
				ControllerAngleChange(DIRE_RIGHT);
			}
			else if ((m_Direction == DIRE_UP_LEFT) || (m_Direction == DIRE_DOWN_RIGHT))
			{
				ControllerAngleChange(DIRE_LEFT);
			}
			
			right_on = false;
		}
	}
}



//=============================================================================
// カベ判定
//=============================================================================

void CPlayer::Wall_Check(void)
{
	if ((FIELDSIZE - 2.0)*(FIELDSIZE - 2.0) < (m_mtxTranslation._41*m_mtxTranslation._41) + (m_mtxTranslation._43 * m_mtxTranslation._43))
	{
		m_mtxTranslation = m_mtxKeepTranslation;
	}
	else
	{
		m_mtxKeepTranslation = m_mtxTranslation;
	}
}


//=============================================================================
// アトラクション召喚
//=============================================================================

void CPlayer::Summons_Attraction(void)
{
	Cool_Time();
	bool *standby = C3DObj::GetCosterModeStandby();
	//何もキーが押されてなければボタンを押していい状態にする
	if (!(JoyDevice_IsTrigger(CONTROLLER::R_BUTTON)) && trigger && !(JoyDevice_IsTrigger(CONTROLLER::L_BUTTON)) && trigger && !(JoyDevice_IsTrigger(CONTROLLER::A_BUTTON)) && trigger)
	{
		trigger = false;
	}
	//if((JoyDevice_IsTrigger(CONTROLLER::R_BUTTON)) && )

	if ((JoyDevice_IsTrigger(CONTROLLER::R_BUTTON)) && !trigger || Keyboard_IsTrigger(DIK_RSHIFT) || right_trigger && js.lZ == 0)
	{
		m_SummonsNum++;
		if (m_SummonsNum >= SUMMONS_MAX)
		{
			m_SummonsNum = 0;
		}
		trigger = true;
		right_trigger = false;
	}

	if ((JoyDevice_IsTrigger(CONTROLLER::L_BUTTON)) && !trigger || Keyboard_IsTrigger(DIK_LSHIFT) || left_trigger && js.lZ == 0)
	{
		m_SummonsNum--;
		if (m_SummonsNum < 0)
		{
			m_SummonsNum = SUMMONS_COASTER;
		}
		trigger = true;
		left_trigger = false;
	}
	if (!*standby || !g_CosterMode)//コースターモード中は出せない
	{
		if (Keyboard_IsTrigger(DIK_RETURN) || (JoyDevice_IsTrigger(CONTROLLER::A_BUTTON)) && !trigger || (JoyDevice_IsTrigger(CONTROLLER::B_BUTTON)) && !trigger)
		{
			//	コーヒーカップ
			if (m_SummonsNum == SUMMONS_COFFEE)
			{
				if ((m_MpStock >= COFFEE_MP) && (m_CoolTime[SUMMONS_COFFEE] == 0))
				{
					CAttraction::Create(CAttraction::AT_COFFEE);
					m_MpStock -= COFFEE_MP;
					m_CoolTime[SUMMONS_COFFEE] = (int)COOLTIME_COFFEE;
				}
			}

			//	フリーフォール
			if (m_SummonsNum == SUMMONS_FALL)
			{
				if ((m_MpStock >= FALL_MP) && (m_CoolTime[SUMMONS_FALL] == 0))
				{
					CAttraction::Create(CAttraction::AT_FALL);
					m_MpStock -= FALL_MP;
					m_CoolTime[SUMMONS_FALL] = (int)COOLTIME_FALL;
				}
			}

			//	観覧車
			if (m_SummonsNum == SUMMONS_WHEEL)
			{
				if ((m_MpStock >= WHEEL_MP) && (m_CoolTime[SUMMONS_WHEEL] == 0))
				{
					CAttraction::Create(CAttraction::AT_WHEEL);
					m_MpStock -= WHEEL_MP;
					m_CoolTime[SUMMONS_WHEEL] = (int)COOLTIME_WHEEL;
				}
			}

			//	ジェットコースター
			if (m_SummonsNum == SUMMONS_COASTER)
			{
				if ((m_MpStock >= COASTER_MP) && (m_CoolTime[SUMMONS_COASTER] == 0))
				{
					ControllerAngleChange(DIRE_UP);
					m_Direction = DIRE_UP;
					Animation_Change(PLAYER_COASTER, 0.0065f);
					m_TimeKeep = m_FrameCount;
					*standby = true;
				}
			}

			//	ポップコーン
			if (m_SummonsNum == SUMMONS_POPCORN)
			{
				if ((m_MpStock >= POPCORN_MP) && (m_CoolTime[SUMMONS_POPCORN] == 0))
				{
					CAttraction::Create(CAttraction::AT_POPCORN);
					m_MpStock -= POPCORN_MP;
					m_CoolTime[SUMMONS_POPCORN] = (int)COOLTIME_POPCORN;
				}
			}
		}
	}

}

//=============================================================================
// クールタイム処理
//=============================================================================

void CPlayer::Cool_Time(void)
{
	int i;
	for (i = 0;i < SUMMONS_MAX;i++)
	{
		if (m_CoolTime[i] > 0)
		{
			m_CoolTime[i]--;

			if (m_CoolTime[i] <= 0)
			{
				m_CoolTime[i] = 0;
			}
		}
	}
}


//=============================================================================
// MP増加処理
//=============================================================================

void CPlayer::Mp_Add(void)
{
	if (m_FrameCount % 60 == 0)
	{
		m_Mp += MP_ADD;
		if (m_Mp >= MP_MAX)
		{
			m_MpStock++;
			m_Mp = 0;
		}
	}
}

bool CPlayer::JoyDevice_IsTrigger(int nKey)
{
	return (js.rgbButtons[nKey] & 0x80) ? true : false;
}

void CPlayer::Ornament_Check(void)
{
	int i;
	C3DObj *pornament;
	THING_NORMAL thingorna;
	for (i = 0;i < MAX_GAMEOBJ;i++)
	{
		pornament = COrnament::Get_Ornament(i);
		if (pornament)
		{
			if (pornament->Get_Enable())
			{
				Thing.vPosition = D3DXVECTOR3(m_mtxTranslation._41, m_mtxTranslation._42, m_mtxTranslation._43);
				thingorna = pornament->GetNormalModel();
				if (C3DObj::Collision_AnimeVSNormal(&Thing, &thingorna))
				{
					m_mtxTranslation = m_mtxKeepTranslation;
					break;
				}
			}
		}

	}
}